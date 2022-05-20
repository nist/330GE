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
#include <linux/config.h>
#include <linux/module.h>
#include <linux/user.h>
#include <linux/elfcore.h>
#include <linux/sched.h>
#include <linux/in6.h>
#include <linux/interrupt.h>
#include <linux/smp_lock.h>
#include <linux/pm.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/tty.h>
 
#include <asm/semaphore.h>
#include <asm/processor.h>
#include <asm/uaccess.h>
#include <asm/checksum.h>
#include <asm/io.h>
#include <asm/hardirq.h>
#include <asm/delay.h>
#include <asm/irq.h>
#include <asm/pgtable.h>

extern void dump_thread(struct pt_regs *, struct user *);
extern unsigned long get_cmos_time(void);
extern void __Udiv(void);
extern void __ashrdi3(void);
extern void iounmap(void *addr);

/* Platform dependent support */
EXPORT_SYMBOL(dump_thread);
EXPORT_SYMBOL(enable_irq);
EXPORT_SYMBOL(disable_irq);
EXPORT_SYMBOL(kernel_thread);
EXPORT_SYMBOL(get_cmos_time);
EXPORT_SYMBOL(loops_per_usec);

/* String functions */
EXPORT_SYMBOL(memcmp);
EXPORT_SYMBOL(memmove);
EXPORT_SYMBOL(strtok);
EXPORT_SYMBOL(strpbrk);
EXPORT_SYMBOL(strstr);
EXPORT_SYMBOL(strcpy);
EXPORT_SYMBOL(strchr);
EXPORT_SYMBOL(strcmp);
EXPORT_SYMBOL(strlen);
EXPORT_SYMBOL(strncat);
EXPORT_SYMBOL(strncmp);

/* Math functions */
EXPORT_SYMBOL(__Udiv);
EXPORT_SYMBOL(__ashrdi3);

/* Memory functions */
EXPORT_SYMBOL(__ioremap);
EXPORT_SYMBOL(iounmap);

/* Semaphore functions */
EXPORT_SYMBOL(__up);
EXPORT_SYMBOL(__down);

/* Export shadow registers for the CPU I/O pins */
EXPORT_SYMBOL(genconfig_shadow);
EXPORT_SYMBOL(port_pa_data_shadow);
EXPORT_SYMBOL(port_pa_dir_shadow);
EXPORT_SYMBOL(port_pb_data_shadow);
EXPORT_SYMBOL(port_pb_dir_shadow);
EXPORT_SYMBOL(port_pb_config_shadow);
EXPORT_SYMBOL(port_g_data_shadow);

/* Userspace access functions */
EXPORT_SYMBOL(strncpy_from_user);
EXPORT_SYMBOL(__strncpy_from_user);
EXPORT_SYMBOL(__generic_copy_from_user);
EXPORT_SYMBOL(__generic_copy_to_user);
EXPORT_SYMBOL(strnlen_user);
EXPORT_SYMBOL(__copy_user_zeroing);
EXPORT_SYMBOL(__copy_user);

#undef memcpy
#undef memset
extern void * memset(void *, int, __kernel_size_t);
extern void * memcpy(void *, const void *, __kernel_size_t);
EXPORT_SYMBOL_NOVERS(memcpy);
EXPORT_SYMBOL_NOVERS(memset);


