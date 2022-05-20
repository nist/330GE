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
#ifndef _M68K_MACHDEP_H
#define _M68K_MACHDEP_H

struct pt_regs;
struct kbd_repeat;
struct mktime;
struct rtc_time;
struct gendisk;
struct buffer_head;

extern void (*mach_sched_init) (void (*handler)(int, void *, struct pt_regs *));
/* machine dependent keyboard functions */
extern int (*mach_keyb_init) (void);
extern int (*mach_kbdrate) (struct kbd_repeat *);
extern void (*mach_kbd_leds) (unsigned int);
extern int (*mach_kbd_translate)(unsigned char scancode, unsigned char *keycode, char raw_mode);
/* machine dependent irq functions */
extern void (*mach_init_IRQ) (void);
extern void (*(*mach_default_handler)[]) (int, void *, struct pt_regs *);
extern int (*mach_request_irq) (unsigned int irq, void (*handler)(int, void *, struct pt_regs *),
                                unsigned long flags, const char *devname, void *dev_id);
extern void (*mach_free_irq) (unsigned int irq, void *dev_id);
extern void (*mach_get_model) (char *model);
extern int (*mach_get_hardware_list) (char *buffer);
extern int (*mach_get_irq_list) (char *buf);
extern void (*mach_process_int) (int irq, struct pt_regs *fp);
/* machine dependent timer functions */
extern unsigned long (*mach_gettimeoffset)(void);
extern void (*mach_gettod)(int *year, int *mon, int *day, int *hour,
			   int *min, int *sec);
extern int (*mach_hwclk)(int, struct rtc_time*);
extern int (*mach_set_clock_mmss)(unsigned long);
extern void (*mach_reset)( void );
extern void (*mach_halt)( void );
extern void (*mach_power_off)( void );
extern unsigned long (*mach_hd_init) (unsigned long, unsigned long);
extern void (*mach_hd_setup)(char *, int *);
extern long mach_max_dma_address;
extern void (*mach_floppy_setup)(char *, int *);
extern void (*mach_heartbeat) (int);
extern void (*mach_l2_flush) (int);
extern int mach_sysrq_key;
extern int mach_sysrq_shift_state;
extern int mach_sysrq_shift_mask;
extern char *mach_sysrq_xlate;

#endif /* _M68K_MACHDEP_H */
