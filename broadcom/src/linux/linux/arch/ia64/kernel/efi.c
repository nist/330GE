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
 * Extensible Firmware Interface
 *
 * Based on Extensible Firmware Interface Specification version 0.9 April 30, 1999
 *
 * Copyright (C) 1999 VA Linux Systems
 * Copyright (C) 1999 Walt Drummond <drummond@valinux.com>
 * Copyright (C) 1999-2002 Hewlett-Packard Co.
 *	David Mosberger-Tang <davidm@hpl.hp.com>
 *	Stephane Eranian <eranian@hpl.hp.com>
 *
 * All EFI Runtime Services are not implemented yet as EFI only
 * supports physical mode addressing on SoftSDV. This is to be fixed
 * in a future version.  --drummond 1999-07-20
 *
 * Implemented EFI runtime services and virtual mode calls.  --davidm
 *
 * Goutham Rao: <goutham.rao@intel.com>
 *	Skip non-WB memory and ignore empty memory ranges.
 */
#include <linux/config.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/time.h>
#include <linux/proc_fs.h>
#include <linux/efi.h>

#include <asm/io.h>
#include <asm/kregs.h>
#include <asm/pgtable.h>
#include <asm/processor.h>

#define EFI_DEBUG	0

extern efi_status_t efi_call_phys (void *, ...);

struct efi efi;
static efi_runtime_services_t *runtime;

/*
 * efi_dir is allocated here, but the directory isn't created
 * here, as proc_mkdir() doesn't work this early in the bootup
 * process.  Therefore, each module, like efivars, must test for
 *    if (!efi_dir)  efi_dir = proc_mkdir("efi", NULL);
 * prior to creating their own entries under /proc/efi.
 */
#ifdef CONFIG_PROC_FS
struct proc_dir_entry *efi_dir = NULL;
#endif

static unsigned long mem_limit = ~0UL;

static efi_status_t
phys_get_time (efi_time_t *tm, efi_time_cap_t *tc)
{
	return efi_call_phys(__va(runtime->get_time), __pa(tm), __pa(tc));
}

static efi_status_t
phys_set_time (efi_time_t *tm)
{
	return efi_call_phys(__va(runtime->set_time), __pa(tm));
}

static efi_status_t
phys_get_wakeup_time (efi_bool_t *enabled, efi_bool_t *pending, efi_time_t *tm)
{
	return efi_call_phys(__va(runtime->get_wakeup_time), __pa(enabled), __pa(pending),
			     __pa(tm));
}

static efi_status_t
phys_set_wakeup_time (efi_bool_t enabled, efi_time_t *tm)
{
	return efi_call_phys(__va(runtime->set_wakeup_time), enabled, __pa(tm));
}

static efi_status_t
phys_get_variable (efi_char16_t *name, efi_guid_t *vendor, u32 *attr,
		   unsigned long *data_size, void *data)
{
	return efi_call_phys(__va(runtime->get_variable), __pa(name), __pa(vendor), __pa(attr),
			     __pa(data_size), __pa(data));
}

static efi_status_t
phys_get_next_variable (unsigned long *name_size, efi_char16_t *name, efi_guid_t *vendor)
{
	return efi_call_phys(__va(runtime->get_next_variable), __pa(name_size), __pa(name),
			     __pa(vendor));
}

static efi_status_t
phys_set_variable (efi_char16_t *name, efi_guid_t *vendor, u32 attr,
		   unsigned long data_size, void *data)
{
	return efi_call_phys(__va(runtime->set_variable), __pa(name), __pa(vendor), attr,
			     data_size, __pa(data));
}

static efi_status_t
phys_get_next_high_mono_count (u64 *count)
{
	return efi_call_phys(__va(runtime->get_next_high_mono_count), __pa(count));
}

static void
phys_reset_system (int reset_type, efi_status_t status,
		   unsigned long data_size, efi_char16_t *data)
{
	efi_call_phys(__va(runtime->reset_system), status, data_size, __pa(data));
}

void
efi_gettimeofday (struct timeval *tv)
{
	efi_time_t tm;

	memset(tv, 0, sizeof(tv));
	if ((*efi.get_time)(&tm, 0) != EFI_SUCCESS)
		return;

	tv->tv_sec = mktime(tm.year, tm.month, tm.day, tm.hour, tm.minute, tm.second);
	tv->tv_usec = tm.nanosecond / 1000;
}

static int
is_available_memory (efi_memory_desc_t *md)
{
	if (!(md->attribute & EFI_MEMORY_WB))
		return 0;

	switch (md->type) {
	      case EFI_LOADER_CODE:
	      case EFI_LOADER_DATA:
	      case EFI_BOOT_SERVICES_CODE:
	      case EFI_BOOT_SERVICES_DATA:
	      case EFI_CONVENTIONAL_MEMORY:
		return 1;
	}
	return 0;
}

/*
 * Trim descriptor MD so its starts at address START_ADDR.  If the descriptor covers
 * memory that is normally available to the kernel, issue a warning that some memory
 * is being ignored.
 */
static void
trim_bottom (efi_memory_desc_t *md, u64 start_addr)
{
	u64 num_skipped_pages;

	if (md->phys_addr >= start_addr || !md->num_pages)
		return;

	num_skipped_pages = (start_addr - md->phys_addr) >> EFI_PAGE_SHIFT;
	if (num_skipped_pages > md->num_pages)
		num_skipped_pages = md->num_pages;

	if (is_available_memory(md))
		printk(KERN_NOTICE "efi.%s: ignoring %luKB of memory at 0x%lx due to granule hole "
		       "at 0x%lx\n", __FUNCTION__,
		       (num_skipped_pages << EFI_PAGE_SHIFT) >> 10,
		       md->phys_addr, start_addr - IA64_GRANULE_SIZE);
	/*
	 * NOTE: Don't set md->phys_addr to START_ADDR because that could cause the memory
	 * descriptor list to become unsorted.  In such a case, md->num_pages will be
	 * zero, so the Right Thing will happen.
	 */
	md->phys_addr += num_skipped_pages << EFI_PAGE_SHIFT;
	md->num_pages -= num_skipped_pages;
}

static void
trim_top (efi_memory_desc_t *md, u64 end_addr)
{
	u64 num_dropped_pages, md_end_addr;

	md_end_addr = md->phys_addr + (md->num_pages << EFI_PAGE_SHIFT);

	if (md_end_addr <= end_addr || !md->num_pages)
		return;

	num_dropped_pages = (md_end_addr - end_addr) >> EFI_PAGE_SHIFT;
	if (num_dropped_pages > md->num_pages)
		num_dropped_pages = md->num_pages;

	if (is_available_memory(md))
		printk(KERN_NOTICE "efi.%s: ignoring %luKB of memory at 0x%lx due to granule hole "
		       "at 0x%lx\n", __FUNCTION__,
		       (num_dropped_pages << EFI_PAGE_SHIFT) >> 10,
		       md->phys_addr, end_addr);
	md->num_pages -= num_dropped_pages;
}

/*
 * Walks the EFI memory map and calls CALLBACK once for each EFI memory descriptor that
 * has memory that is available for OS use.
 */
void
efi_memmap_walk (efi_freemem_callback_t callback, void *arg)
{
	int prev_valid = 0;
	struct range {
		u64 start;
		u64 end;
	} prev, curr;
	void *efi_map_start, *efi_map_end, *p, *q;
	efi_memory_desc_t *md, *check_md;
	u64 efi_desc_size, start, end, granule_addr, first_non_wb_addr = 0;

	efi_map_start = __va(ia64_boot_param->efi_memmap);
	efi_map_end   = efi_map_start + ia64_boot_param->efi_memmap_size;
	efi_desc_size = ia64_boot_param->efi_memdesc_size;

	for (p = efi_map_start; p < efi_map_end; p += efi_desc_size) {
		md = p;

		/* skip over non-WB memory descriptors; that's all we're interested in... */
		if (!(md->attribute & EFI_MEMORY_WB))
			continue;

		if (md->phys_addr + (md->num_pages << EFI_PAGE_SHIFT) > first_non_wb_addr) {
			/*
			 * Search for the next run of contiguous WB memory.  Start search
			 * at first granule boundary covered by md.
			 */
			granule_addr = ((md->phys_addr + IA64_GRANULE_SIZE - 1)
					& -IA64_GRANULE_SIZE);
			first_non_wb_addr = granule_addr;
			for (q = p; q < efi_map_end; q += efi_desc_size) {
				check_md = q;

				if (check_md->attribute & EFI_MEMORY_WB)
					trim_bottom(md, granule_addr);

				if (check_md->phys_addr < granule_addr)
					continue;

				if (!(check_md->attribute & EFI_MEMORY_WB))
					break;	/* hit a non-WB region; stop search */

				if (check_md->phys_addr != first_non_wb_addr)
					break;	/* hit a memory hole; stop search */

				first_non_wb_addr += check_md->num_pages << EFI_PAGE_SHIFT;
			}
			/* round it down to the previous granule-boundary: */
			first_non_wb_addr &= -IA64_GRANULE_SIZE;

			if (!(first_non_wb_addr > granule_addr))
				continue;	/* couldn't find enough contiguous memory */
		}

		/* BUG_ON((md->phys_addr >> IA64_GRANULE_SHIFT) < first_non_wb_addr); */

		trim_top(md, first_non_wb_addr);

		if (is_available_memory(md)) {
			if (md->phys_addr + (md->num_pages << EFI_PAGE_SHIFT) > mem_limit) {
				if (md->phys_addr > mem_limit)
					continue;
				md->num_pages = (mem_limit - md->phys_addr) >> EFI_PAGE_SHIFT;
			}

			if (md->num_pages == 0)
				continue;

			curr.start = PAGE_OFFSET + md->phys_addr;
			curr.end   = curr.start + (md->num_pages << EFI_PAGE_SHIFT);

			if (!prev_valid) {
				prev = curr;
				prev_valid = 1;
			} else {
				if (curr.start < prev.start)
					printk("Oops: EFI memory table not ordered!\n");

				if (prev.end == curr.start) {
					/* merge two consecutive memory ranges */
					prev.end = curr.end;
				} else {
					start = PAGE_ALIGN(prev.start);
					end = prev.end & PAGE_MASK;
					if ((end > start) && (*callback)(start, end, arg) < 0)
						return;
					prev = curr;
				}
			}
		}
	}
	if (prev_valid) {
		start = PAGE_ALIGN(prev.start);
		end = prev.end & PAGE_MASK;
		if (end > start)
			(*callback)(start, end, arg);
	}
}

/*
 * Look for the PAL_CODE region reported by EFI and maps it using an
 * ITR to enable safe PAL calls in virtual mode.  See IA-64 Processor
 * Abstraction Layer chapter 11 in ADAG
 */
void
efi_map_pal_code (void)
{
	void *efi_map_start, *efi_map_end, *p;
	efi_memory_desc_t *md;
	u64 efi_desc_size;
	int pal_code_count = 0;
	u64 mask, psr;
	u64 vaddr;

	efi_map_start = __va(ia64_boot_param->efi_memmap);
	efi_map_end   = efi_map_start + ia64_boot_param->efi_memmap_size;
	efi_desc_size = ia64_boot_param->efi_memdesc_size;

	for (p = efi_map_start; p < efi_map_end; p += efi_desc_size) {
		md = p;
		if (md->type != EFI_PAL_CODE)
			continue;

		if (++pal_code_count > 1) {
			printk(KERN_ERR "Too many EFI Pal Code memory ranges, dropped @ %lx\n",
			       md->phys_addr);
			continue;
		}
		/*
		 * The only ITLB entry in region 7 that is used is the one installed by
		 * __start().  That entry covers a 64MB range.
		 */
		mask  = ~((1 << KERNEL_TR_PAGE_SHIFT) - 1);
		vaddr = PAGE_OFFSET + md->phys_addr;

		/*
		 * We must check that the PAL mapping won't overlap with the kernel
		 * mapping.
		 *
		 * PAL code is guaranteed to be aligned on a power of 2 between 4k and
		 * 256KB and that only one ITR is needed to map it. This implies that the
		 * PAL code is always aligned on its size, i.e., the closest matching page
		 * size supported by the TLB. Therefore PAL code is guaranteed never to
		 * cross a 64MB unless it is bigger than 64MB (very unlikely!).  So for
		 * now the following test is enough to determine whether or not we need a
		 * dedicated ITR for the PAL code.
		 */
		if ((vaddr & mask) == (KERNEL_START & mask)) {
			printk("%s: no need to install ITR for PAL code\n", __FUNCTION__);
			continue;
		}

		if (md->num_pages << EFI_PAGE_SHIFT > IA64_GRANULE_SIZE)
			panic("Woah!  PAL code size bigger than a granule!");

		mask  = ~((1 << IA64_GRANULE_SHIFT) - 1);
		printk("CPU %d: mapping PAL code [0x%lx-0x%lx) into [0x%lx-0x%lx)\n",
		       smp_processor_id(), md->phys_addr,
		       md->phys_addr + (md->num_pages << EFI_PAGE_SHIFT),
		       vaddr & mask, (vaddr & mask) + IA64_GRANULE_SIZE);

		/*
		 * Cannot write to CRx with PSR.ic=1
		 */
		psr = ia64_clear_ic();
		ia64_itr(0x1, IA64_TR_PALCODE, vaddr & mask,
			 pte_val(mk_pte_phys(md->phys_addr, PAGE_KERNEL)), IA64_GRANULE_SHIFT);
		ia64_set_psr(psr);		/* restore psr */
		ia64_srlz_i();
	}
}

void __init
efi_init (void)
{
	void *efi_map_start, *efi_map_end;
	efi_config_table_t *config_tables;
	efi_char16_t *c16;
	u64 efi_desc_size;
	char *cp, *end, vendor[100] = "unknown";
	extern char saved_command_line[];
	int i;

	/* it's too early to be able to use the standard kernel command line support... */
	for (cp = saved_command_line; *cp; ) {
		if (memcmp(cp, "mem=", 4) == 0) {
			cp += 4;
			mem_limit = memparse(cp, &end) - 1;
			if (end != cp)
				break;
			cp = end;
		} else {
			while (*cp != ' ' && *cp)
				++cp;
			while (*cp == ' ')
				++cp;
		}
	}
	if (mem_limit != ~0UL)
		printk("Ignoring memory above %luMB\n", mem_limit >> 20);

	efi.systab = __va(ia64_boot_param->efi_systab);

	/*
	 * Verify the EFI Table
	 */
	if (efi.systab == NULL)
		panic("Woah! Can't find EFI system table.\n");
	if (efi.systab->hdr.signature != EFI_SYSTEM_TABLE_SIGNATURE)
		panic("Woah! EFI system table signature incorrect\n");
	if ((efi.systab->hdr.revision ^ EFI_SYSTEM_TABLE_REVISION) >> 16 != 0)
		printk("Warning: EFI system table major version mismatch: "
		       "got %d.%02d, expected %d.%02d\n",
		       efi.systab->hdr.revision >> 16, efi.systab->hdr.revision & 0xffff,
		       EFI_SYSTEM_TABLE_REVISION >> 16, EFI_SYSTEM_TABLE_REVISION & 0xffff);

	config_tables = __va(efi.systab->tables);

	/* Show what we know for posterity */
	c16 = __va(efi.systab->fw_vendor);
	if (c16) {
		for (i = 0;i < sizeof(vendor) && *c16; ++i)
			vendor[i] = *c16++;
		vendor[i] = '\0';
	}

	printk("EFI v%u.%.02u by %s:",
	       efi.systab->hdr.revision >> 16, efi.systab->hdr.revision & 0xffff, vendor);

	for (i = 0; i < efi.systab->nr_tables; i++) {
		if (efi_guidcmp(config_tables[i].guid, MPS_TABLE_GUID) == 0) {
			efi.mps = __va(config_tables[i].table);
			printk(" MPS=0x%lx", config_tables[i].table);
		} else if (efi_guidcmp(config_tables[i].guid, ACPI_20_TABLE_GUID) == 0) {
			efi.acpi20 = __va(config_tables[i].table);
			printk(" ACPI 2.0=0x%lx", config_tables[i].table);
		} else if (efi_guidcmp(config_tables[i].guid, ACPI_TABLE_GUID) == 0) {
			efi.acpi = __va(config_tables[i].table);
			printk(" ACPI=0x%lx", config_tables[i].table);
		} else if (efi_guidcmp(config_tables[i].guid, SMBIOS_TABLE_GUID) == 0) {
			efi.smbios = __va(config_tables[i].table);
			printk(" SMBIOS=0x%lx", config_tables[i].table);
		} else if (efi_guidcmp(config_tables[i].guid, SAL_SYSTEM_TABLE_GUID) == 0) {
			efi.sal_systab = __va(config_tables[i].table);
			printk(" SALsystab=0x%lx", config_tables[i].table);
		} else if (efi_guidcmp(config_tables[i].guid, HCDP_TABLE_GUID) == 0) {
			efi.hcdp = __va(config_tables[i].table);
			printk(" HCDP=0x%lx", config_tables[i].table);
		}
	}
	printk("\n");

	runtime = __va(efi.systab->runtime);
	efi.get_time = phys_get_time;
	efi.set_time = phys_set_time;
	efi.get_wakeup_time = phys_get_wakeup_time;
	efi.set_wakeup_time = phys_set_wakeup_time;
	efi.get_variable = phys_get_variable;
	efi.get_next_variable = phys_get_next_variable;
	efi.set_variable = phys_set_variable;
	efi.get_next_high_mono_count = phys_get_next_high_mono_count;
	efi.reset_system = phys_reset_system;

	efi_map_start = __va(ia64_boot_param->efi_memmap);
	efi_map_end   = efi_map_start + ia64_boot_param->efi_memmap_size;
	efi_desc_size = ia64_boot_param->efi_memdesc_size;

#if EFI_DEBUG
	/* print EFI memory map: */
	{
		efi_memory_desc_t *md;
		void *p;

		for (i = 0, p = efi_map_start; p < efi_map_end; ++i, p += efi_desc_size) {
			md = p;
			printk("mem%02u: type=%u, attr=0x%lx, range=[0x%016lx-0x%016lx) (%luMB)\n",
			       i, md->type, md->attribute, md->phys_addr,
			       md->phys_addr + (md->num_pages << EFI_PAGE_SHIFT),
			       md->num_pages >> (20 - EFI_PAGE_SHIFT));
		}
	}
#endif

	efi_map_pal_code();
	efi_enter_virtual_mode();
}

void
efi_enter_virtual_mode (void)
{
	void *efi_map_start, *efi_map_end, *p;
	efi_memory_desc_t *md;
	efi_status_t status;
	u64 efi_desc_size;

	efi_map_start = __va(ia64_boot_param->efi_memmap);
	efi_map_end   = efi_map_start + ia64_boot_param->efi_memmap_size;
	efi_desc_size = ia64_boot_param->efi_memdesc_size;

	for (p = efi_map_start; p < efi_map_end; p += efi_desc_size) {
		md = p;
		if (md->attribute & EFI_MEMORY_RUNTIME) {
			/*
			 * Some descriptors have multiple bits set, so the order of
			 * the tests is relevant.
			 */
			if (md->attribute & EFI_MEMORY_WB) {
				md->virt_addr = (u64) __va(md->phys_addr);
			} else if (md->attribute & EFI_MEMORY_UC) {
				md->virt_addr = (u64) ioremap(md->phys_addr, 0);
			} else if (md->attribute & EFI_MEMORY_WC) {
				printk("EFI_MEMORY_WC mapping\n");
				md->virt_addr = (u64) ioremap(md->phys_addr, 0);
			} else if (md->attribute & EFI_MEMORY_WT) {
				printk("EFI_MEMORY_WT mapping\n");
				md->virt_addr = (u64) ioremap(md->phys_addr, 0);
			}
		}
	}

	status = efi_call_phys(__va(runtime->set_virtual_address_map),
			       ia64_boot_param->efi_memmap_size,
			       efi_desc_size, ia64_boot_param->efi_memdesc_version,
			       ia64_boot_param->efi_memmap);
	if (status != EFI_SUCCESS) {
		printk("Warning: unable to switch EFI into virtual mode (status=%lu)\n", status);
		return;
	}

	/*
	 * Now that EFI is in virtual mode, we arrange for EFI functions to be
	 * called directly:
	 */
	efi.get_time = __va(runtime->get_time);
	efi.set_time = __va(runtime->set_time);
	efi.get_wakeup_time = __va(runtime->get_wakeup_time);
	efi.set_wakeup_time = __va(runtime->set_wakeup_time);
	efi.get_variable = __va(runtime->get_variable);
	efi.get_next_variable = __va(runtime->get_next_variable);
	efi.set_variable = __va(runtime->set_variable);
	efi.get_next_high_mono_count = __va(runtime->get_next_high_mono_count);
	efi.reset_system = __va(runtime->reset_system);
}

/*
 * Walk the EFI memory map looking for the I/O port range.  There can only be one entry of
 * this type, other I/O port ranges should be described via ACPI.
 */
u64
efi_get_iobase (void)
{
	void *efi_map_start, *efi_map_end, *p;
	efi_memory_desc_t *md;
	u64 efi_desc_size;

	efi_map_start = __va(ia64_boot_param->efi_memmap);
	efi_map_end   = efi_map_start + ia64_boot_param->efi_memmap_size;
	efi_desc_size = ia64_boot_param->efi_memdesc_size;

	for (p = efi_map_start; p < efi_map_end; p += efi_desc_size) {
		md = p;
		if (md->type == EFI_MEMORY_MAPPED_IO_PORT_SPACE) {
			/* paranoia attribute checking */
			if (md->attribute == (EFI_MEMORY_UC | EFI_MEMORY_RUNTIME))
				return md->phys_addr;
		}
	}
	return 0;
}

u32
efi_mem_type (unsigned long phys_addr)
{
	void *efi_map_start, *efi_map_end, *p;
	efi_memory_desc_t *md;
	u64 efi_desc_size;

	efi_map_start = __va(ia64_boot_param->efi_memmap);
	efi_map_end   = efi_map_start + ia64_boot_param->efi_memmap_size;
	efi_desc_size = ia64_boot_param->efi_memdesc_size;

	for (p = efi_map_start; p < efi_map_end; p += efi_desc_size) {
		md = p;

		if ((md->phys_addr <= phys_addr) && (phys_addr <=
		    (md->phys_addr + (md->num_pages << EFI_PAGE_SHIFT) - 1)))
			 return md->type;
	}
	return 0;
}

u64
efi_mem_attributes (unsigned long phys_addr)
{
	void *efi_map_start, *efi_map_end, *p;
	efi_memory_desc_t *md;
	u64 efi_desc_size;

	efi_map_start = __va(ia64_boot_param->efi_memmap);
	efi_map_end   = efi_map_start + ia64_boot_param->efi_memmap_size;
	efi_desc_size = ia64_boot_param->efi_memdesc_size;

	for (p = efi_map_start; p < efi_map_end; p += efi_desc_size) {
		md = p;

		if ((md->phys_addr <= phys_addr) && (phys_addr <=
		    (md->phys_addr + (md->num_pages << EFI_PAGE_SHIFT) - 1)))
			return md->attribute;
	}
	return 0;
}

static void __exit
efivars_exit (void)
{
#ifdef CONFIG_PROC_FS
 	remove_proc_entry(efi_dir->name, NULL);
#endif
}
