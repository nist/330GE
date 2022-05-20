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
 * include/asm-ia64/xor.h
 *
 * Optimized RAID-5 checksumming functions for IA-64.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * You should have received a copy of the GNU General Public License
 * (for example /usr/src/linux/COPYING); if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */


extern void xor_ia64_2(unsigned long, unsigned long *, unsigned long *);
extern void xor_ia64_3(unsigned long, unsigned long *, unsigned long *,
		       unsigned long *);
extern void xor_ia64_4(unsigned long, unsigned long *, unsigned long *,
		       unsigned long *, unsigned long *);
extern void xor_ia64_5(unsigned long, unsigned long *, unsigned long *,
		       unsigned long *, unsigned long *, unsigned long *);

asm ("
	.text

	// Assume L2 memory latency of 6 cycles.

	.proc xor_ia64_2
xor_ia64_2:
	.prologue
	.fframe 0
	{ .mii
	  .save ar.pfs, r31
	  alloc r31 = ar.pfs, 3, 0, 13, 16
	  .save ar.lc, r30
	  mov r30 = ar.lc
	  .save pr, r29
	  mov r29 = pr
	  ;;
	}
	.body
	{ .mii
	  mov r8 = in1
	  mov ar.ec = 6 + 2
	  shr in0 = in0, 3
	  ;;
	}
	{ .mmi
	  adds in0 = -1, in0
	  mov r16 = in1
	  mov r17 = in2
	  ;;
	}
	{ .mii
	  mov ar.lc = in0
	  mov pr.rot = 1 << 16
	  ;;
	}
	.rotr s1[6+1], s2[6+1], d[2]
	.rotp p[6+2]
0:	 { .mmi
(p[0])	  ld8.nta s1[0] = [r16], 8
(p[0])	  ld8.nta s2[0] = [r17], 8
(p[6])	  xor d[0] = s1[6], s2[6]
	}
	{ .mfb
(p[6+1])  st8.nta [r8] = d[1], 8
	  nop.f 0
	  br.ctop.dptk.few 0b
	  ;;
	}
	{ .mii
	  mov ar.lc = r30
	  mov pr = r29, -1
	}
	{ .bbb
	  br.ret.sptk.few rp
	}
	.endp xor_ia64_2

	.proc xor_ia64_3
xor_ia64_3:
	.prologue
	.fframe 0
	{ .mii
	  .save ar.pfs, r31
	  alloc r31 = ar.pfs, 4, 0, 20, 24
	  .save ar.lc, r30
	  mov r30 = ar.lc
	  .save pr, r29
	  mov r29 = pr
	  ;;
	}
	.body
	{ .mii
	  mov r8 = in1
	  mov ar.ec = 6 + 2
	  shr in0 = in0, 3
	  ;;
	}
	{ .mmi
	  adds in0 = -1, in0
	  mov r16 = in1
	  mov r17 = in2
	  ;;
	}
	{ .mii
	  mov r18 = in3
	  mov ar.lc = in0
	  mov pr.rot = 1 << 16
	  ;;
	}
	.rotr s1[6+1], s2[6+1], s3[6+1], d[2]
	.rotp p[6+2]
0:	{ .mmi
(p[0])	  ld8.nta s1[0] = [r16], 8
(p[0])	  ld8.nta s2[0] = [r17], 8
(p[6])	  xor d[0] = s1[6], s2[6]
	  ;;
	}
	{ .mmi
(p[0])	  ld8.nta s3[0] = [r18], 8
(p[6+1])  st8.nta [r8] = d[1], 8
(p[6])	  xor d[0] = d[0], s3[6]
	}
	{ .bbb
	  br.ctop.dptk.few 0b
	  ;;
	}
	{ .mii
	  mov ar.lc = r30
	  mov pr = r29, -1
	}
	{ .bbb
	  br.ret.sptk.few rp
	}
	.endp xor_ia64_3

	.proc xor_ia64_4
xor_ia64_4:
	.prologue
	.fframe 0
	{ .mii
	  .save ar.pfs, r31
	  alloc r31 = ar.pfs, 5, 0, 27, 32
	  .save ar.lc, r30
	  mov r30 = ar.lc
	  .save pr, r29
	  mov r29 = pr
	  ;;
	}
	.body
	{ .mii
	  mov r8 = in1
	  mov ar.ec = 6 + 2
	  shr in0 = in0, 3
	  ;;
	}
	{ .mmi
	  adds in0 = -1, in0
	  mov r16 = in1
	  mov r17 = in2
	  ;;
	}
	{ .mii
	  mov r18 = in3
	  mov ar.lc = in0
	  mov pr.rot = 1 << 16
	}
	{ .mfb
	  mov r19 = in4
	  ;;
	}
	.rotr s1[6+1], s2[6+1], s3[6+1], s4[6+1], d[2]
	.rotp p[6+2]
0:	{ .mmi
(p[0])	  ld8.nta s1[0] = [r16], 8
(p[0])	  ld8.nta s2[0] = [r17], 8
(p[6])	  xor d[0] = s1[6], s2[6]
	}
	{ .mmi
(p[0])	  ld8.nta s3[0] = [r18], 8
(p[0])	  ld8.nta s4[0] = [r19], 8
(p[6])	  xor r20 = s3[6], s4[6]
	  ;;
	}
	{ .mib
(p[6+1])  st8.nta [r8] = d[1], 8
(p[6])	  xor d[0] = d[0], r20
	  br.ctop.dptk.few 0b
	  ;;
	}
	{ .mii
	  mov ar.lc = r30
	  mov pr = r29, -1
	}
	{ .bbb
	  br.ret.sptk.few rp
	}
	.endp xor_ia64_4

	.proc xor_ia64_5
xor_ia64_5:
	.prologue
	.fframe 0
	{ .mii
	  .save ar.pfs, r31
	  alloc r31 = ar.pfs, 6, 0, 34, 40
	  .save ar.lc, r30
	  mov r30 = ar.lc
	  .save pr, r29
	  mov r29 = pr
	  ;;
	}
	.body
	{ .mii
	  mov r8 = in1
	  mov ar.ec = 6 + 2
	  shr in0 = in0, 3
	  ;;
	}
	{ .mmi
	  adds in0 = -1, in0
	  mov r16 = in1
	  mov r17 = in2
	  ;;
	}
	{ .mii
	  mov r18 = in3
	  mov ar.lc = in0
	  mov pr.rot = 1 << 16
	}
	{ .mib
	  mov r19 = in4
	  mov r20 = in5
	  ;;
	}
	.rotr s1[6+1], s2[6+1], s3[6+1], s4[6+1], s5[6+1], d[2]
	.rotp p[6+2]
0:	{ .mmi
(p[0])	  ld8.nta s1[0] = [r16], 8
(p[0])	  ld8.nta s2[0] = [r17], 8
(p[6])	  xor d[0] = s1[6], s2[6]
	}
	{ .mmi
(p[0])	  ld8.nta s3[0] = [r18], 8
(p[0])	  ld8.nta s4[0] = [r19], 8
(p[6])	  xor r21 = s3[6], s4[6]
	  ;;
	}
	{ .mmi
(p[0])	  ld8.nta s5[0] = [r20], 8
(p[6+1])  st8.nta [r8] = d[1], 8
(p[6])	  xor d[0] = d[0], r21
	  ;;
	}
	{ .mfb
(p[6])	  xor d[0] = d[0], s5[6]
	  nop.f 0
	  br.ctop.dptk.few 0b
	  ;;
	}
	{ .mii
	  mov ar.lc = r30
	  mov pr = r29, -1
	}
	{ .bbb
	  br.ret.sptk.few rp
	}
	.endp xor_ia64_5
");

static struct xor_block_template xor_block_ia64 = {
	name: "ia64",
	do_2: xor_ia64_2,
	do_3: xor_ia64_3,
	do_4: xor_ia64_4,
	do_5: xor_ia64_5,
};

#define XOR_TRY_TEMPLATES	xor_speed(&xor_block_ia64)
