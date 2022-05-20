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
/* $Id: cert.c,v 1.1.1.1 2007/01/25 12:51:54 jiahao_jhou Exp $
 *
 * Author       Karsten Keil
 * Copyright    by Karsten Keil      <keil@isdn4linux.de>
 * 
 * This software may be used and distributed according to the terms
 * of the GNU General Public License, incorporated herein by reference.
 *
 * For changes and modifications please read
 * ../../../Documentation/isdn/HiSax.cert
 *
 */
 
#include <linux/kernel.h>

int
certification_check(int output) {

#ifdef CERTIFICATION
#if CERTIFICATION == 0
	if (output) {
		printk(KERN_INFO "HiSax: Approval certification valid\n");
		printk(KERN_INFO "HiSax: Approved with ELSA Microlink PCI cards\n");
		printk(KERN_INFO "HiSax: Approved with Eicon Technology Diva 2.01 PCI cards\n");
		printk(KERN_INFO "HiSax: Approved with Sedlbauer Speedfax + cards\n");
		printk(KERN_INFO "HiSax: Approved with HFC-S PCI A based cards\n");
	}
	return(0);
#endif
#if CERTIFICATION == 1
	if (output) {
		printk(KERN_INFO "HiSax: Approval certification failed because of\n");
		printk(KERN_INFO "HiSax: unauthorized source code changes\n");
	}
	return(1);
#endif
#if CERTIFICATION == 127
	if (output) {
		printk(KERN_INFO "HiSax: Approval certification not possible\n");
		printk(KERN_INFO "HiSax: because \"md5sum\" is not available\n");
	}
	return(2);
#endif
#else
	if (output) {
		printk(KERN_INFO "HiSax: Certification not verified\n");
	}
	return(3);
#endif
}
