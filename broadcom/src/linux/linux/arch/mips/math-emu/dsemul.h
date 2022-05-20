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
typedef long gpreg_t;
typedef void *vaddr_t;

#define REG_TO_VA (vaddr_t)
#define VA_TO_REG (gpreg_t)

int mips_dsemul(struct pt_regs *regs, mips_instruction ir, gpreg_t cpc);
int do_dsemulret(struct pt_regs *xcp);

/* Instruction which will always cause an address error */
#define AdELOAD 0x8c000001	/* lw $0,1($0) */
/* Instruction which will plainly cause a CP1 exception when FPU is disabled */
#define CP1UNDEF 0x44400001    /* cfc1 $0,$0 undef  */

/* Instruction inserted following the badinst to further tag the sequence */
#define BD_COOKIE 0x0000bd36 /* tne $0,$0 with baggage */

/* Setup which instruction to use for trampoline */
#ifdef STANDALONE_EMULATOR
#define BADINST CP1UNDEF
#else
#define BADINST AdELOAD
#endif
