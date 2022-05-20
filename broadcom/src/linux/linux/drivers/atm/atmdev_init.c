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
/* drivers/atm/atmdev_init.c - ATM device driver initialization */
 
/* Written 1995-2000 by Werner Almesberger, EPFL LRC/ICA */
 

#include <linux/config.h>
#include <linux/init.h>


#ifdef CONFIG_ATM_ZATM
extern int zatm_detect(void);
#endif
#ifdef CONFIG_ATM_NICSTAR
extern int nicstar_detect(void);
#endif
#ifdef CONFIG_ATM_AMBASSADOR
extern int amb_detect(void);
#endif
#ifdef CONFIG_ATM_HORIZON
extern int hrz_detect(void);
#endif
#ifdef CONFIG_ATM_IA
extern int ia_detect(void);
#endif
#ifdef CONFIG_ATM_FORE200E
extern int fore200e_detect(void);
#endif
#ifdef CONFIG_ATM_LANAI
extern int lanai_detect(void);
#endif


/*
 * For historical reasons, atmdev_init returns the number of devices found.
 * Note that some detections may not go via atmdev_init (e.g. eni.c), so this
 * number is meaningless.
 */

int __init atmdev_init(void)
{
	int devs;

	devs = 0;
#ifdef CONFIG_ATM_ZATM
	devs += zatm_detect();
#endif
#ifdef CONFIG_ATM_NICSTAR
	devs += nicstar_detect();
#endif
#ifdef CONFIG_ATM_AMBASSADOR
	devs += amb_detect();
#endif
#ifdef CONFIG_ATM_HORIZON
	devs += hrz_detect();
#endif
#ifdef CONFIG_ATM_IA
	devs += ia_detect();
#endif
#ifdef CONFIG_ATM_FORE200E
	devs += fore200e_detect();
#endif
#ifdef CONFIG_ATM_LANAI
	devs += lanai_detect();
#endif
	return devs;
}
