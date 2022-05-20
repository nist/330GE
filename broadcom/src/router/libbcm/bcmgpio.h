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
 * Copyright 2006, ASUSTek Inc.
 * All Rights Reserved.                
 *                                     
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of ASUSTek Inc.;   
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior      
 * written permission of ASUSTek Inc..                            
 *
 * Header file for GPIO utility functions.
 *
 * $Id: bcmgpio.h,v 1.1.1.1 2007/01/25 12:52:20 jiahao_jhou Exp $
 */
#ifndef __bcmgpio_h__
#define __bcmgpio_h__

#include <bcmtimer.h>


#define BCMGPIO_MAXPINS		32
#define BCMGPIO_MAXINDEX	(BCMGPIO_MAXPINS - 1)
#define BCMGPIO_UNDEFINED	-1

/* GPIO type */
typedef enum bcmgpio_dirn {
	BCMGPIO_DIRN_IN = 0,
	BCMGPIO_DIRN_OUT
} bcmgpio_dirn_t;

/* GPIO strobe information */
typedef struct bcmgpio_strobe {
	int duty_percent;			/* duty cycle of strobe in percent of strobe period */			
	bcm_timer_module_id timer_module;	/* timer module ID obtained by calling bcm_timer_module_init() */
	unsigned long strobe_period_in_ms;	/* strobe period of the GPIO in milliseconds */
	unsigned long num_strobes;		/* total number of strobes */
	int *strobe_done;			/* pointer to memory which is used to signal strobe completion */
} bcmgpio_strobe_t;

/* Functions to implement Buttons and LEDs on the AP, using GPIOs */
int bcmgpio_connect (int gpio_pin, bcmgpio_dirn_t gpio_dirn);
int bcmgpio_disconnect (int gpio_pin);
int bcmgpio_in (unsigned long gpio_mask, unsigned long *value);
int bcmgpio_out (unsigned long gpio_mask, unsigned long value);
int bcmgpio_strobe_start (int gpio_pin, bcmgpio_strobe_t *strobe_info);
int bcmgpio_strobe_stop (int gpio_pin);
int bcmgpio_getpin(char *pin_name, uint def_pin);



#endif	/* __bcmgpio_h__ */


