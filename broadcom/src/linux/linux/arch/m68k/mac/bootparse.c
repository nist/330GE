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
#include <linux/string.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <asm/irq.h>
#include <asm/setup.h>
#include <asm/bootinfo.h>
#include <asm/macintosh.h>

/*
 *	Booter vars
 */
 
int boothowto;
int _boothowto;
 
/*
 *	Called early to parse the environment (passed to us from the booter)
 *	into a bootinfo struct. Will die as soon as we have our own booter
 */

#define atol(x)	simple_strtoul(x,NULL,0)
 
void parse_booter(char *env)
{
 	char *name;
 	char *value;
 	while(*env)
 	{
 		name=env;
 		value=name;
 		while(*value!='='&&*value)
 			value++;
 		if(*value=='=')
 			*value++=0;
 		env=value;
 		while(*env)
 			env++;
 		env++;
 		if(strcmp(name,"BOOTTIME")==0)
 			mac_bi_data.boottime=atol(value);
 		if(strcmp(name,"GMTBIAS")==0)
 			mac_bi_data.gmtbias=atol(value);
 		if(strcmp(name,"BOOTERVER")==0)
 			mac_bi_data.bootver=atol(value);
 		if(strcmp(name,"MACOS_VIDEO")==0)
 			mac_bi_data.videological=atol(value);
 		if(strcmp(name,"MACOS_SCC")==0)
 			mac_bi_data.sccbase=atol(value);
 		if(strcmp(name,"MACHINEID")==0)
 			mac_bi_data.id=atol(value);
 		if(strcmp(name,"MEMSIZE")==0)
 			mac_bi_data.memsize=atol(value);
 		if(strcmp(name,"SERIAL_MODEM_FLAGS")==0)
 			mac_bi_data.serialmf=atol(value);
 		if(strcmp(name,"SERIAL_MODEM_HSKICLK")==0)
 			mac_bi_data.serialhsk=atol(value);
 		if(strcmp(name,"SERIAL_MODEM_GPICLK")==0)
 			mac_bi_data.serialgpi=atol(value);
 		if(strcmp(name,"SERIAL_PRINT_FLAGS")==0)
 			mac_bi_data.printmf=atol(value);
 		if(strcmp(name,"SERIAL_PRINT_HSKICLK")==0)
 			mac_bi_data.printhsk=atol(value);
 		if(strcmp(name,"SERIAL_PRINT_GPICLK")==0)
 			mac_bi_data.printgpi=atol(value);
 		if(strcmp(name,"PROCESSOR")==0)
 			mac_bi_data.cpuid=atol(value);
 		if(strcmp(name,"ROMBASE")==0)
 			mac_bi_data.rombase=atol(value);
 		if(strcmp(name,"TIMEDBRA")==0)
 			mac_bi_data.timedbra=atol(value);
 		if(strcmp(name,"ADBDELAY")==0)
 			mac_bi_data.adbdelay=atol(value);
 	}
 }
 

void print_booter(char *env)
{
 	char *name;
 	char *value;
 	while(*env)
 	{
 		name=env;
 		value=name;
 		while(*value!='='&&*value)
 			value++;
 		if(*value=='=')
 			*value++=0;
 		env=value;
 		while(*env)
 			env++;
 		env++;
 		printk("%s=%s\n", name,value);
 	}
 }
 

