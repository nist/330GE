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
#ifndef _ASM_X8664_PROTO_H
#define _ASM_X8664_PROTO_H 1

/* misc architecture specific prototypes */

struct cpuinfo_x86; 

extern void get_cpu_vendor(struct cpuinfo_x86*);
extern void start_kernel(void);
extern void pda_init(int); 

extern void mcheck_init(struct cpuinfo_x86 *c);
extern void init_memory_mapping(void);

extern void system_call(void); 
extern void ia32_cstar_target(void); 
extern void calibrate_delay(void);
extern void cpu_idle(void);
extern void sys_ni_syscall(void);
extern void config_acpi_tables(void);
extern void ia32_syscall(void);
extern void iommu_hole_init(void);

extern void do_softirq_thunk(void);

extern int setup_early_printk(char *); 
extern void early_printk(const char *fmt, ...) __attribute__((format(printf,1,2)));

extern int k8_scan_nodes(unsigned long start, unsigned long end);

extern int numa_initmem_init(unsigned long start_pfn, unsigned long end_pfn);
extern unsigned long numa_free_all_bootmem(void);

extern void reserve_bootmem_generic(unsigned long phys, unsigned len);
extern void free_bootmem_generic(unsigned long phys, unsigned len);

extern unsigned long start_pfn, end_pfn; 

extern void show_stack(unsigned long * rsp);

#define round_up(x,y) (((x) + (y) - 1) & ~((y)-1))
#define round_down(x,y) ((x) & ~((y)-1))

#endif
