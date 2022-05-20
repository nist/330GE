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

/* dvma support routines */

#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/bootmem.h>
#include <linux/list.h>
#include <asm/page.h>
#include <asm/pgtable.h>
#include <asm/sun3mmu.h>
#include <asm/dvma.h>


static unsigned long ptelist[120];

inline unsigned long dvma_page(unsigned long kaddr, unsigned long vaddr)
{
	unsigned long pte;
	unsigned long j;
	pte_t ptep;
	
	j = *(volatile unsigned long *)kaddr;
	*(volatile unsigned long *)kaddr = j;

	ptep = __mk_pte(kaddr, PAGE_KERNEL);
	pte = pte_val(ptep);
//		printk("dvma_remap: addr %lx -> %lx pte %08lx len %x\n", 
//		       kaddr, vaddr, pte, len);
	if(ptelist[(vaddr & 0xff000) >> PAGE_SHIFT] != pte) {
		sun3_put_pte(vaddr, pte);
		ptelist[(vaddr & 0xff000) >> PAGE_SHIFT] = pte;
	}

	return (vaddr + (kaddr & ~PAGE_MASK));

}

int dvma_map_iommu(unsigned long kaddr, unsigned long baddr, 
			      int len)
{

	unsigned long end;
	unsigned long vaddr;

	vaddr = dvma_btov(baddr);

	end = vaddr + len;
	
	while(vaddr < end) {
		dvma_page(kaddr, vaddr);
		kaddr += PAGE_SIZE;
		vaddr += PAGE_SIZE;
	}

	return 0;

}

void sun3_dvma_init(void)
{

	memset(ptelist, 0, sizeof(ptelist));


}
