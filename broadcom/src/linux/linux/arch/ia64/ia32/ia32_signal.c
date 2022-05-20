/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */
/*
 * IA32 Architecture-specific signal handling support.
 *
 * Copyright (C) 1999, 2001 Hewlett-Packard Co
 *	David Mosberger-Tang <davidm@hpl.hp.com>
 * Copyright (C) 1999 Arun Sharma <arun.sharma@intel.com>
 * Copyright (C) 2000 VA Linux Co
 * Copyright (C) 2000 Don Dugger <n0ano@valinux.com>
 *
 * Derived from i386 and Alpha versions.
 */

#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/personality.h>
#include <linux/ptrace.h>
#include <linux/sched.h>
#include <linux/signal.h>
#include <linux/smp.h>
#include <linux/smp_lock.h>
#include <linux/stddef.h>
#include <linux/unistd.h>
#include <linux/wait.h>

#include <asm/uaccess.h>
#include <asm/rse.h>
#include <asm/sigcontext.h>
#include <asm/segment.h>
#include <asm/ia32.h>

#include "../kernel/sigframe.h"

#define A(__x)		((unsigned long)(__x))

#define DEBUG_SIG	0
#define _BLOCKABLE	(~(sigmask(SIGKILL) | sigmask(SIGSTOP)))

#define __IA32_NR_sigreturn            119
#define __IA32_NR_rt_sigreturn         173

struct sigframe_ia32
{
       int pretcode;
       int sig;
       struct sigcontext_ia32 sc;
       struct _fpstate_ia32 fpstate;
       unsigned int extramask[_IA32_NSIG_WORDS-1];
       char retcode[8];
};

struct rt_sigframe_ia32
{
       int pretcode;
       int sig;
       int pinfo;
       int puc;
       siginfo_t32 info;
       struct ucontext_ia32 uc;
       struct _fpstate_ia32 fpstate;
       char retcode[8];
};

int
copy_siginfo_from_user32 (siginfo_t *to, siginfo_t32 *from)
{
	unsigned long tmp;
	int err;

	if (!access_ok(VERIFY_READ, from, sizeof(siginfo_t32)))
		return -EFAULT;

	err = __get_user(to->si_signo, &from->si_signo);
	err |= __get_user(to->si_errno, &from->si_errno);
	err |= __get_user(to->si_code, &from->si_code);

	if (from->si_code < 0)
		err |= __copy_from_user(&to->_sifields._pad, &from->_sifields._pad, SI_PAD_SIZE);
	else {
		switch (from->si_code >> 16) {
		      case __SI_CHLD >> 16:
			err |= __get_user(to->si_utime, &from->si_utime);
			err |= __get_user(to->si_stime, &from->si_stime);
			err |= __get_user(to->si_status, &from->si_status);
		      default:
			err |= __get_user(to->si_pid, &from->si_pid);
			err |= __get_user(to->si_uid, &from->si_uid);
			break;
		      case __SI_FAULT >> 16:
			err |= __get_user(tmp, &from->si_addr);
			to->si_addr = (void *) tmp;
			break;
		      case __SI_POLL >> 16:
			err |= __get_user(to->si_band, &from->si_band);
			err |= __get_user(to->si_fd, &from->si_fd);
			break;
			/* case __SI_RT: This is not generated by the kernel as of now.  */
		}
	}
	return err;
}

int
copy_siginfo_to_user32 (siginfo_t32 *to, siginfo_t *from)
{
	int err;

	if (!access_ok(VERIFY_WRITE, to, sizeof(siginfo_t32)))
		return -EFAULT;

	/* If you change siginfo_t structure, please be sure
	   this code is fixed accordingly.
	   It should never copy any pad contained in the structure
	   to avoid security leaks, but must copy the generic
	   3 ints plus the relevant union member.
	   This routine must convert siginfo from 64bit to 32bit as well
	   at the same time.  */
	err = __put_user(from->si_signo, &to->si_signo);
	err |= __put_user(from->si_errno, &to->si_errno);
	err |= __put_user((short)from->si_code, &to->si_code);
	if (from->si_code < 0)
		err |= __copy_to_user(&to->_sifields._pad, &from->_sifields._pad, SI_PAD_SIZE);
	else {
		switch (from->si_code >> 16) {
		case __SI_CHLD >> 16:
			err |= __put_user(from->si_utime, &to->si_utime);
			err |= __put_user(from->si_stime, &to->si_stime);
			err |= __put_user(from->si_status, &to->si_status);
		default:
			err |= __put_user(from->si_pid, &to->si_pid);
			err |= __put_user(from->si_uid, &to->si_uid);
			break;
		case __SI_FAULT >> 16:
			err |= __put_user((long)from->si_addr, &to->si_addr);
			break;
		case __SI_POLL >> 16:
			err |= __put_user(from->si_band, &to->si_band);
			err |= __put_user(from->si_fd, &to->si_fd);
			break;
		/* case __SI_RT: This is not generated by the kernel as of now.  */
		}
	}
	return err;
}

static inline void
sigact_set_handler (struct k_sigaction *sa, unsigned int handler, unsigned int restorer)
{
	if (handler + 1 <= 2)
		/* SIG_DFL, SIG_IGN, or SIG_ERR: must sign-extend to 64-bits */
		sa->sa.sa_handler = (__sighandler_t) A((int) handler);
	else
		sa->sa.sa_handler = (__sighandler_t) (((unsigned long) restorer << 32) | handler);
}

asmlinkage long
ia32_rt_sigsuspend (sigset32_t *uset, unsigned int sigsetsize, struct sigscratch *scr)
{
	extern long ia64_do_signal (sigset_t *oldset, struct sigscratch *scr, long in_syscall);
	sigset_t oldset, set;

	scr->scratch_unat = 0;	/* avoid leaking kernel bits to user level */
	memset(&set, 0, sizeof(&set));

	if (sigsetsize > sizeof(sigset_t))
		return -EINVAL;

	if (copy_from_user(&set.sig, &uset->sig, sigsetsize))
		return -EFAULT;

	sigdelsetmask(&set, ~_BLOCKABLE);

	spin_lock_irq(&current->sigmask_lock);
	{
		oldset = current->blocked;
		current->blocked = set;
		recalc_sigpending(current);
	}
	spin_unlock_irq(&current->sigmask_lock);

	/*
	 * The return below usually returns to the signal handler.  We need to pre-set the
	 * correct error code here to ensure that the right values get saved in sigcontext
	 * by ia64_do_signal.
	 */
	scr->pt.r8 = -EINTR;
	while (1) {
		current->state = TASK_INTERRUPTIBLE;
		schedule();
		if (ia64_do_signal(&oldset, scr, 1))
			return -EINTR;
	}
}

asmlinkage long
ia32_sigsuspend (unsigned int mask, struct sigscratch *scr)
{
	return ia32_rt_sigsuspend((sigset32_t *)&mask, sizeof(mask), scr);
}

asmlinkage long
sys32_signal (int sig, unsigned int handler)
{
	struct k_sigaction new_sa, old_sa;
	int ret;

	sigact_set_handler(&new_sa, handler, 0);
	new_sa.sa.sa_flags = SA_ONESHOT | SA_NOMASK;

	ret = do_sigaction(sig, &new_sa, &old_sa);

	return ret ? ret : IA32_SA_HANDLER(&old_sa);
}

asmlinkage long
sys32_rt_sigaction (int sig, struct sigaction32 *act,
		    struct sigaction32 *oact, unsigned int sigsetsize)
{
	struct k_sigaction new_ka, old_ka;
	unsigned int handler, restorer;
	int ret;

	if (sigsetsize != sizeof(sigset32_t))
		return -EINVAL;

	if (act) {
		ret = get_user(handler, &act->sa_handler);
		ret |= get_user(new_ka.sa.sa_flags, &act->sa_flags);
		ret |= get_user(restorer, &act->sa_restorer);
		ret |= copy_from_user(&new_ka.sa.sa_mask, &act->sa_mask, sizeof(sigset32_t));
		if (ret)
			return -EFAULT;

		sigact_set_handler(&new_ka, handler, restorer);
	}

	ret = do_sigaction(sig, act ? &new_ka : NULL, oact ? &old_ka : NULL);

	if (!ret && oact) {
		ret = put_user(IA32_SA_HANDLER(&old_ka), &oact->sa_handler);
		ret |= put_user(old_ka.sa.sa_flags, &oact->sa_flags);
		ret |= put_user(IA32_SA_RESTORER(&old_ka), &oact->sa_restorer);
		ret |= copy_to_user(&oact->sa_mask, &old_ka.sa.sa_mask, sizeof(sigset32_t));
	}
	return ret;
}


extern asmlinkage long sys_rt_sigprocmask (int how, sigset_t *set, sigset_t *oset,
					   size_t sigsetsize);

asmlinkage long
sys32_rt_sigprocmask (int how, sigset32_t *set, sigset32_t *oset, unsigned int sigsetsize)
{
	mm_segment_t old_fs = get_fs();
	sigset_t s;
	long ret;

	if (sigsetsize > sizeof(s))
		return -EINVAL;

	if (set) {
		memset(&s, 0, sizeof(s));
		if (copy_from_user(&s.sig, set, sigsetsize))
			return -EFAULT;
	}
	set_fs(KERNEL_DS);
	ret = sys_rt_sigprocmask(how, set ? &s : NULL, oset ? &s : NULL, sizeof(s));
	set_fs(old_fs);
	if (ret)
		return ret;
	if (oset) {
		if (copy_to_user(oset, &s.sig, sigsetsize))
			return -EFAULT;
	}
	return 0;
}

asmlinkage long
sys32_sigprocmask (int how, unsigned int *set, unsigned int *oset)
{
	return sys32_rt_sigprocmask(how, (sigset32_t *) set, (sigset32_t *) oset, sizeof(*set));
}

asmlinkage long
sys32_rt_sigtimedwait (sigset32_t *uthese, siginfo_t32 *uinfo, struct timespec32 *uts,
		       unsigned int sigsetsize)
{
	extern asmlinkage long sys_rt_sigtimedwait (const sigset_t *, siginfo_t *,
						    const struct timespec *, size_t);
	extern int copy_siginfo_to_user32 (siginfo_t32 *, siginfo_t *);
	mm_segment_t old_fs = get_fs();
	struct timespec t;
	siginfo_t info;
	sigset_t s;
	int ret;

	if (copy_from_user(&s.sig, uthese, sizeof(sigset32_t)))
		return -EFAULT;
	if (uts) {
		ret = get_user(t.tv_sec, &uts->tv_sec);
		ret |= get_user(t.tv_nsec, &uts->tv_nsec);
		if (ret)
			return -EFAULT;
	}
	set_fs(KERNEL_DS);
	ret = sys_rt_sigtimedwait(&s, &info, &t, sigsetsize);
	set_fs(old_fs);
	if (ret >= 0 && uinfo) {
		if (copy_siginfo_to_user32(uinfo, &info))
			return -EFAULT;
	}
	return ret;
}

asmlinkage long
sys32_rt_sigqueueinfo (int pid, int sig, siginfo_t32 *uinfo)
{
	extern asmlinkage long sys_rt_sigqueueinfo (int, int, siginfo_t *);
	extern int copy_siginfo_from_user32 (siginfo_t *to, siginfo_t32 *from);
	mm_segment_t old_fs = get_fs();
	siginfo_t info;
	int ret;

	if (copy_siginfo_from_user32(&info, uinfo))
		return -EFAULT;
	set_fs(KERNEL_DS);
	ret = sys_rt_sigqueueinfo(pid, sig, &info);
	set_fs(old_fs);
	return ret;
}

asmlinkage long
sys32_sigaction (int sig, struct old_sigaction32 *act, struct old_sigaction32 *oact)
{
	struct k_sigaction new_ka, old_ka;
	unsigned int handler, restorer;
	int ret;

	if (act) {
		old_sigset32_t mask;

		ret = get_user(handler, &act->sa_handler);
		ret |= get_user(new_ka.sa.sa_flags, &act->sa_flags);
		ret |= get_user(restorer, &act->sa_restorer);
		ret |= get_user(mask, &act->sa_mask);
		if (ret)
			return ret;

		sigact_set_handler(&new_ka, handler, restorer);
		siginitset(&new_ka.sa.sa_mask, mask);
	}

	ret = do_sigaction(sig, act ? &new_ka : NULL, oact ? &old_ka : NULL);

	if (!ret && oact) {
		ret = put_user(IA32_SA_HANDLER(&old_ka), &oact->sa_handler);
		ret |= put_user(old_ka.sa.sa_flags, &oact->sa_flags);
		ret |= put_user(IA32_SA_RESTORER(&old_ka), &oact->sa_restorer);
		ret |= put_user(old_ka.sa.sa_mask.sig[0], &oact->sa_mask);
	}

	return ret;
}

static int
setup_sigcontext_ia32 (struct sigcontext_ia32 *sc, struct _fpstate_ia32 *fpstate,
		       struct pt_regs *regs, unsigned long mask)
{
	int  err = 0;
	unsigned long flag;

	err |= __put_user((regs->r16 >> 32) & 0xffff, (unsigned int *)&sc->fs);
	err |= __put_user((regs->r16 >> 48) & 0xffff, (unsigned int *)&sc->gs);
	err |= __put_user((regs->r16 >> 16) & 0xffff, (unsigned int *)&sc->es);
	err |= __put_user(regs->r16 & 0xffff, (unsigned int *)&sc->ds);
	err |= __put_user(regs->r15, &sc->edi);
	err |= __put_user(regs->r14, &sc->esi);
	err |= __put_user(regs->r13, &sc->ebp);
	err |= __put_user(regs->r12, &sc->esp);
	err |= __put_user(regs->r11, &sc->ebx);
	err |= __put_user(regs->r10, &sc->edx);
	err |= __put_user(regs->r9, &sc->ecx);
	err |= __put_user(regs->r8, &sc->eax);
	err |= __put_user(regs->cr_iip, &sc->eip);
	err |= __put_user(regs->r17 & 0xffff, (unsigned int *)&sc->cs);
	/*
	 *  `eflags' is in an ar register for this context
	 */
	asm volatile ("mov %0=ar.eflag ;;" : "=r"(flag));
	err |= __put_user((unsigned int)flag, &sc->eflags);
	err |= __put_user(regs->r12, &sc->esp_at_signal);
	err |= __put_user((regs->r17 >> 16) & 0xffff, (unsigned int *)&sc->ss);

	err |= __put_user(mask, &sc->oldmask);
	return err;
}

static int
restore_sigcontext_ia32 (struct pt_regs *regs, struct sigcontext_ia32 *sc, int *peax)
{
	unsigned int err = 0;

#define COPY(ia64x, ia32x)	err |= __get_user(regs->ia64x, &sc->ia32x)

#define copyseg_gs(tmp)		(regs->r16 |= (unsigned long) tmp << 48)
#define copyseg_fs(tmp)		(regs->r16 |= (unsigned long) tmp << 32)
#define copyseg_cs(tmp)		(regs->r17 |= tmp)
#define copyseg_ss(tmp)		(regs->r17 |= (unsigned long) tmp << 16)
#define copyseg_es(tmp)		(regs->r16 |= (unsigned long) tmp << 16)
#define copyseg_ds(tmp)		(regs->r16 |= tmp)

#define COPY_SEG(seg)					\
	{						\
		unsigned short tmp;			\
		err |= __get_user(tmp, &sc->seg);	\
		copyseg_##seg(tmp);			\
	}
#define COPY_SEG_STRICT(seg)				\
	{						\
		unsigned short tmp;			\
		err |= __get_user(tmp, &sc->seg);	\
		copyseg_##seg(tmp|3);			\
	}

	/* To make COPY_SEGs easier, we zero r16, r17 */
	regs->r16 = 0;
	regs->r17 = 0;

	COPY_SEG(gs);
	COPY_SEG(fs);
	COPY_SEG(es);
	COPY_SEG(ds);
	COPY(r15, edi);
	COPY(r14, esi);
	COPY(r13, ebp);
	COPY(r12, esp);
	COPY(r11, ebx);
	COPY(r10, edx);
	COPY(r9, ecx);
	COPY(cr_iip, eip);
	COPY_SEG_STRICT(cs);
	COPY_SEG_STRICT(ss);
	ia32_load_segment_descriptors(current);
	{
		unsigned int tmpflags;
		unsigned long flag;

		/*
		 *  IA32 `eflags' is not part of `pt_regs', it's in an ar register which
		 *  is part of the thread context.  Fortunately, we are executing in the
		 *  IA32 process's context.
		 */
		err |= __get_user(tmpflags, &sc->eflags);
		asm volatile ("mov %0=ar.eflag ;;" : "=r"(flag));
		flag &= ~0x40DD5;
		flag |= (tmpflags & 0x40DD5);
		asm volatile ("mov ar.eflag=%0 ;;" :: "r"(flag));

		regs->r1 = -1;	/* disable syscall checks, r1 is orig_eax */
	}


	err |= __get_user(*peax, &sc->eax);
	return err;

}

/*
 * Determine which stack to use..
 */
static inline void *
get_sigframe (struct k_sigaction *ka, struct pt_regs * regs, size_t frame_size)
{
	unsigned long esp;

	/* Default to using normal stack (truncate off sign-extension of bit 31: */
	esp = (unsigned int) regs->r12;

	/* This is the X/Open sanctioned signal stack switching.  */
	if (ka->sa.sa_flags & SA_ONSTACK) {
		if (!on_sig_stack(esp))
			esp = current->sas_ss_sp + current->sas_ss_size;
	}
	/* Legacy stack switching not supported */

	return (void *)((esp - frame_size) & -8ul);
}

static int
setup_frame_ia32 (int sig, struct k_sigaction *ka, sigset_t *set, struct pt_regs * regs)
{
	struct exec_domain *ed = current->exec_domain;
	struct sigframe_ia32 *frame;
	int err = 0;

	frame = get_sigframe(ka, regs, sizeof(*frame));

	if (!access_ok(VERIFY_WRITE, frame, sizeof(*frame)))
		goto give_sigsegv;

	err |= __put_user((ed && ed->signal_invmap
			   && sig < 32 ? (int)(ed->signal_invmap[sig]) : sig), &frame->sig);

	err |= setup_sigcontext_ia32(&frame->sc, &frame->fpstate, regs, set->sig[0]);

	if (_IA32_NSIG_WORDS > 1)
		err |= __copy_to_user(frame->extramask, (char *) &set->sig + 4,
				      sizeof(frame->extramask));

	/* Set up to return from userspace.  If provided, use a stub
	   already in userspace.  */
	if (ka->sa.sa_flags & SA_RESTORER) {
		unsigned int restorer = IA32_SA_RESTORER(ka);
		err |= __put_user(restorer, &frame->pretcode);
	} else {
		err |= __put_user((long)frame->retcode, &frame->pretcode);
		/* This is popl %eax ; movl $,%eax ; int $0x80 */
		err |= __put_user(0xb858, (short *)(frame->retcode+0));
		err |= __put_user(__IA32_NR_sigreturn & 0xffff, (short *)(frame->retcode+2));
		err |= __put_user(__IA32_NR_sigreturn >> 16, (short *)(frame->retcode+4));
		err |= __put_user(0x80cd, (short *)(frame->retcode+6));
	}

	if (err)
		goto give_sigsegv;

	/* Set up registers for signal handler */
	regs->r12 = (unsigned long) frame;
	regs->cr_iip = IA32_SA_HANDLER(ka);

	set_fs(USER_DS);
	regs->r16 = (__USER_DS << 16) |  (__USER_DS); /* ES == DS, GS, FS are zero */
	regs->r17 = (__USER_DS << 16) | __USER_CS;



	return 1;

  give_sigsegv:
	if (sig == SIGSEGV)
		ka->sa.sa_handler = SIG_DFL;
	force_sig(SIGSEGV, current);
	return 0;
}

static int
setup_rt_frame_ia32 (int sig, struct k_sigaction *ka, siginfo_t *info,
		     sigset_t *set, struct pt_regs * regs)
{
	struct exec_domain *ed = current->exec_domain;
	struct rt_sigframe_ia32 *frame;
	int err = 0;

	frame = get_sigframe(ka, regs, sizeof(*frame));

	if (!access_ok(VERIFY_WRITE, frame, sizeof(*frame)))
		goto give_sigsegv;

	err |= __put_user((ed && ed->signal_invmap
			   && sig < 32 ? ed->signal_invmap[sig] : sig),
			  &frame->sig);
	err |= __put_user((long)&frame->info, &frame->pinfo);
	err |= __put_user((long)&frame->uc, &frame->puc);
	err |= copy_siginfo_to_user32(&frame->info, info);

	/* Create the ucontext.  */
	err |= __put_user(0, &frame->uc.uc_flags);
	err |= __put_user(0, &frame->uc.uc_link);
	err |= __put_user(current->sas_ss_sp, &frame->uc.uc_stack.ss_sp);
	err |= __put_user(sas_ss_flags(regs->r12), &frame->uc.uc_stack.ss_flags);
	err |= __put_user(current->sas_ss_size, &frame->uc.uc_stack.ss_size);
	err |= setup_sigcontext_ia32(&frame->uc.uc_mcontext, &frame->fpstate, regs, set->sig[0]);
	err |= __copy_to_user(&frame->uc.uc_sigmask, set, sizeof(*set));
	if (err)
		goto give_sigsegv;

	/* Set up to return from userspace.  If provided, use a stub
	   already in userspace.  */
	if (ka->sa.sa_flags & SA_RESTORER) {
		unsigned int restorer = IA32_SA_RESTORER(ka);
		err |= __put_user(restorer, &frame->pretcode);
	} else {
		err |= __put_user((long)frame->retcode, &frame->pretcode);
		/* This is movl $,%eax ; int $0x80 */
		err |= __put_user(0xb8, (char *)(frame->retcode+0));
		err |= __put_user(__IA32_NR_rt_sigreturn, (int *)(frame->retcode+1));
		err |= __put_user(0x80cd, (short *)(frame->retcode+5));
	}

	if (err)
		goto give_sigsegv;

	/* Set up registers for signal handler */
	regs->r12 = (unsigned long) frame;
	regs->cr_iip = IA32_SA_HANDLER(ka);

	set_fs(USER_DS);

	regs->r16 = (__USER_DS << 16) |  (__USER_DS); /* ES == DS, GS, FS are zero */
	regs->r17 = (__USER_DS << 16) | __USER_CS;



	return 1;

give_sigsegv:
	if (sig == SIGSEGV)
		ka->sa.sa_handler = SIG_DFL;
	force_sig(SIGSEGV, current);
	return 0;
}

int
ia32_setup_frame1 (int sig, struct k_sigaction *ka, siginfo_t *info,
		   sigset_t *set, struct pt_regs *regs)
{
       /* Set up the stack frame */
       if (ka->sa.sa_flags & SA_SIGINFO)
               return setup_rt_frame_ia32(sig, ka, info, set, regs);
       else
               return setup_frame_ia32(sig, ka, set, regs);
}

asmlinkage long
sys32_sigreturn (int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7,
		 unsigned long stack)
{
	struct pt_regs *regs = (struct pt_regs *) &stack;
	unsigned long esp = (unsigned int) regs->r12;
	struct sigframe_ia32 *frame = (struct sigframe_ia32 *)(esp - 8);
	sigset_t set;
	int eax;

	if (verify_area(VERIFY_READ, frame, sizeof(*frame)))
		goto badframe;

	if (__get_user(set.sig[0], &frame->sc.oldmask)
	    || (_IA32_NSIG_WORDS > 1 && __copy_from_user((char *) &set.sig + 4, &frame->extramask,
							 sizeof(frame->extramask))))
		goto badframe;

	sigdelsetmask(&set, ~_BLOCKABLE);
	spin_lock_irq(&current->sigmask_lock);
	current->blocked = (sigset_t) set;
	recalc_sigpending(current);
	spin_unlock_irq(&current->sigmask_lock);

	if (restore_sigcontext_ia32(regs, &frame->sc, &eax))
		goto badframe;
	return eax;

  badframe:
	force_sig(SIGSEGV, current);
	return 0;
}

asmlinkage long
sys32_rt_sigreturn (int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7,
		    unsigned long stack)
{
	struct pt_regs *regs = (struct pt_regs *) &stack;
	unsigned long esp = (unsigned int) regs->r12;
	struct rt_sigframe_ia32 *frame = (struct rt_sigframe_ia32 *)(esp - 4);
	sigset_t set;
	stack_t st;
	int eax;

	if (verify_area(VERIFY_READ, frame, sizeof(*frame)))
		goto badframe;
	if (__copy_from_user(&set, &frame->uc.uc_sigmask, sizeof(set)))
		goto badframe;

	sigdelsetmask(&set, ~_BLOCKABLE);
	spin_lock_irq(&current->sigmask_lock);
	current->blocked =  set;
	recalc_sigpending(current);
	spin_unlock_irq(&current->sigmask_lock);

	if (restore_sigcontext_ia32(regs, &frame->uc.uc_mcontext, &eax))
		goto badframe;

	if (__copy_from_user(&st, &frame->uc.uc_stack, sizeof(st)))
		goto badframe;
	/* It is more difficult to avoid calling this function than to
	   call it and ignore errors.  */
	do_sigaltstack(&st, NULL, esp);

	return eax;

  badframe:
	force_sig(SIGSEGV, current);
	return 0;
}
