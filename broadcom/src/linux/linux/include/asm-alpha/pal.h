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
#ifndef __ALPHA_PAL_H
#define __ALPHA_PAL_H

/*
 * Common PAL-code
 */
#define PAL_halt	  0
#define PAL_cflush	  1
#define PAL_draina	  2
#define PAL_bpt		128
#define PAL_bugchk	129
#define PAL_chmk	131
#define PAL_callsys	131
#define PAL_imb		134
#define PAL_rduniq	158
#define PAL_wruniq	159
#define PAL_gentrap	170
#define PAL_nphalt	190

/*
 * VMS specific PAL-code
 */
#define PAL_swppal	10
#define PAL_mfpr_vptb	41

/*
 * OSF specific PAL-code
 */
#define PAL_cserve	 9
#define PAL_wripir	13
#define PAL_rdmces	16
#define PAL_wrmces	17
#define PAL_wrfen	43
#define PAL_wrvptptr	45
#define PAL_jtopal	46
#define PAL_swpctx	48
#define PAL_wrval	49
#define PAL_rdval	50
#define PAL_tbi		51
#define PAL_wrent	52
#define PAL_swpipl	53
#define PAL_rdps	54
#define PAL_wrkgp	55
#define PAL_wrusp	56
#define PAL_wrperfmon	57
#define PAL_rdusp	58
#define PAL_whami	60
#define PAL_rtsys	61
#define PAL_rti		63

#endif /* __ALPHA_PAL_H */
