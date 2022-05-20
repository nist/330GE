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
 * HND SiliconBackplane PCI core software interface.
 *
 * $Id: hndpci.h,v 1.1.1.1 2007/01/25 12:52:12 jiahao_jhou Exp $
 * Copyright 2006, ASUSTek Inc.
 * All Rights Reserved.
 * 
 * THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 */

#ifndef _hndpci_h_
#define _hndpci_h_

extern int sbpci_read_config(sb_t *sbh, uint bus, uint dev, uint func, uint off, void *buf,
                             int len);
extern int extpci_read_config(sb_t *sbh, uint bus, uint dev, uint func, uint off, void *buf,
                              int len);
extern int sbpci_write_config(sb_t *sbh, uint bus, uint dev, uint func, uint off, void *buf,
                              int len);
extern int extpci_write_config(sb_t *sbh, uint bus, uint dev, uint func, uint off, void *buf,
                               int len);
extern void sbpci_ban(uint16 core);
extern int sbpci_init(sb_t *sbh);
extern int sbpci_init_pci(sb_t *sbh);
extern void sbpci_init_cores(sb_t *sbh);
extern void sbpci_check(sb_t *sbh);

#endif /* _hndpci_h_ */
