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
#ifndef __M68K_FPU_H
#define __M68K_FPU_H

#include <linux/config.h>

/*
 * MAX floating point unit state size (FSAVE/FRESTORE)
 */

#if defined(CONFIG_M68020) || defined(CONFIG_M68030)
#define FPSTATESIZE (216/sizeof(unsigned char))
#elif defined(CONFIG_M68040)
#define FPSTATESIZE (96/sizeof(unsigned char))
#elif defined(CONFIG_M68KFPU_EMU)
#define FPSTATESIZE (28/sizeof(unsigned char))
#elif defined(CONFIG_M68060)
#define FPSTATESIZE (12/sizeof(unsigned char))
#else
#define FPSTATESIZE error no_cpu_type_configured
#endif

#endif /* __M68K_FPU_H */
