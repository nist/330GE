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
#ifndef _LINUX_BINFMTS_H
#define _LINUX_BINFMTS_H

#include <linux/ptrace.h>
#include <linux/capability.h>

/*
 * MAX_ARG_PAGES defines the number of pages allocated for arguments
 * and envelope for the new program. 32 should suffice, this gives
 * a maximum env+arg of 128kB w/4KB pages!
 */
#define MAX_ARG_PAGES 32

/* sizeof(linux_binprm->buf) */
#define BINPRM_BUF_SIZE 128

#ifdef __KERNEL__

/*
 * This structure is used to hold the arguments that are used when loading binaries.
 */
struct linux_binprm{
	char buf[BINPRM_BUF_SIZE];
	struct page *page[MAX_ARG_PAGES];
	unsigned long p; /* current top of mem */
	int sh_bang;
	struct file * file;
	int e_uid, e_gid;
	kernel_cap_t cap_inheritable, cap_permitted, cap_effective;
	int argc, envc;
	char * filename;	/* Name of binary */
	unsigned long loader, exec;
};

/*
 * This structure defines the functions that are used to load the binary formats that
 * linux accepts.
 */
struct linux_binfmt {
	struct linux_binfmt * next;
	struct module *module;
	int (*load_binary)(struct linux_binprm *, struct  pt_regs * regs);
	int (*load_shlib)(struct file *);
	int (*core_dump)(long signr, struct pt_regs * regs, struct file * file);
	unsigned long min_coredump;	/* minimal dump size */
};

extern int register_binfmt(struct linux_binfmt *);
extern int unregister_binfmt(struct linux_binfmt *);

extern int prepare_binprm(struct linux_binprm *);
extern void remove_arg_zero(struct linux_binprm *);
extern int search_binary_handler(struct linux_binprm *,struct pt_regs *);
extern int flush_old_exec(struct linux_binprm * bprm);
extern int setup_arg_pages(struct linux_binprm * bprm);
extern int copy_strings(int argc,char ** argv,struct linux_binprm *bprm); 
extern int copy_strings_kernel(int argc,char ** argv,struct linux_binprm *bprm);
extern void compute_creds(struct linux_binprm *binprm);
extern int do_coredump(long signr, struct pt_regs * regs);
extern void set_binfmt(struct linux_binfmt *new);



#endif /* __KERNEL__ */
#endif /* _LINUX_BINFMTS_H */
