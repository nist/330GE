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
 * Broadcom Home Gateway Reference Design
 * Robo Switch Module header
 *
 * Copyright 2004, ASUSTek Inc.
 * All Rights Reserved.
 * 
 * THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 * $Id: swmod.h,v 1.1.1.1 2007/01/25 12:53:06 jiahao_jhou Exp $
 */

#ifndef _SWMOD_H_
#define _SWMOD_H_
#include <etc53xx.h>

/* robo switch structure */
typedef struct robo_io_s {
   unsigned int   len;    /* length in bytes: 1-4 */
   unsigned int   data;
   unsigned int   write;  /* 1 for write; 0 for read */
   unsigned int   unit;
   unsigned char  page;
   unsigned char  addr;
} robo_io_t;

#define MAXIFNAMSIZ 16 /* IFNAMSIZ is 16 */

typedef struct reg_brcm_s {
    uint    port;
	char	base_name[MAXIFNAMSIZ]; 	
	char 	suffix[MAXIFNAMSIZ];
} reg_brcm_t;

typedef struct reg_mib_stats_s {
	int     port;
    int     count;
	ROBO_MIB_AC_STRUCT 	stats;
} reg_mib_stats_t;

typedef struct port_interface_s {
	int     port;
    int     iface;
} port_interface_t;

typedef struct robo_devtype_s {
	int     devtype;
} robo_devtype_t;

/*
 * Ioctl command codes.
 */

#define BRCM_IOC_MAGIC  'Y'
#define UBSEC_ROBOIF                _IOWR(BRCM_IOC_MAGIC, 1, robo_io_t *)
#define UBSEC_ROBOGETSEM            _IOWR(BRCM_IOC_MAGIC, 2, robo_io_t *)
#define UBSEC_ROBORELSEM            _IOWR(BRCM_IOC_MAGIC, 3, robo_io_t *)
#define UBSEC_ROBO_REGBRCM          _IOWR(BRCM_IOC_MAGIC, 4, reg_brcm_t *)
#define UBSEC_ROBO_UNREGBRCM        _IOWR(BRCM_IOC_MAGIC, 5, reg_brcm_t *)
#define UBSEC_ROBO_MIBACSTATS       _IOWR(BRCM_IOC_MAGIC, 6, reg_mib_stats_t *)
#define UBSEC_ROBO_SETIFACE         _IOWR(BRCM_IOC_MAGIC, 7, port_interface_t *)
#define UBSEC_ROBO_GETPORT          _IOWR(BRCM_IOC_MAGIC, 8, port_interface_t *)
#define UBSEC_ROBO_GETCAM           _IOWR(BRCM_IOC_MAGIC, 9, cam_table_t *)
#define UBSEC_ROBO_GETDEVTYPE       _IOWR(BRCM_IOC_MAGIC, 10, robo_devtype_t *)

#endif  /* _SWMOD_H_ */
