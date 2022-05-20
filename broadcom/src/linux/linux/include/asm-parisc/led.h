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
#ifndef LED_H
#define LED_H

#define	LED7		0x80		/* top (or furthest right) LED */
#define	LED6		0x40
#define	LED5		0x20
#define	LED4		0x10
#define	LED3		0x08
#define	LED2		0x04
#define	LED1		0x02
#define	LED0		0x01		/* bottom (or furthest left) LED */

#define	LED_LAN_TX	LED0		/* for LAN transmit activity */
#define	LED_LAN_RCV	LED1		/* for LAN receive activity */
#define	LED_DISK_IO	LED2		/* for disk activity */
#define	LED_HEARTBEAT	LED3		/* heartbeat */

/* values for pdc_chassis_lcd_info_ret_block.model: */
#define DISPLAY_MODEL_LCD  0		/* KittyHawk LED or LCD */
#define DISPLAY_MODEL_NONE 1		/* no LED or LCD */
#define DISPLAY_MODEL_LASI 2		/* LASI style 8 bit LED */
#define DISPLAY_MODEL_OLD_ASP 0x7F	/* faked: ASP style 8 x 1 bit LED (only very old ASP versions) */

#define LED_CMD_REG_NONE NULL		/* NULL == no addr for the cmd register */

/* led tasklet struct */
extern struct tasklet_struct led_tasklet;

/* register_led_driver() */
int __init register_led_driver( int model, char *cmd_reg, char *data_reg );

/* registers the LED regions for procfs */
void __init register_led_regions(void);

/* writes a string to the LCD display (if possible on this h/w) */
int lcd_print(char *str);

/* main LED initialization function (uses PDC) */ 
int __init led_init(void);

#endif /* LED_H */
