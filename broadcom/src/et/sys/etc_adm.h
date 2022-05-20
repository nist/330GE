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
 * ADMtek switch setup functions
 *
 * Copyright 2006, ASUSTek Inc.
 * All Rights Reserved.                
 *                                     
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of ASUSTek Inc.;   
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior      
 * written permission of ASUSTek Inc..                            
 *
 * $Id$
 */

#ifndef _adm_h_
#define _adm_h_

/* forward declarations */
typedef struct adm_info_s adm_info_t;

/* interface prototypes */
extern adm_info_t *adm_attach(sb_t *sbh, char *vars);
extern void adm_detach(adm_info_t *adm);
extern int adm_enable_device(adm_info_t *adm);
extern int adm_config_vlan(adm_info_t *adm);

#endif /* _adm_h_ */
