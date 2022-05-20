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
#ifndef __ASM_MIPS_PROM_H
#define __ASM_MIPS_PROM_H

#define PROM_RESET		0
#define PROM_EXEC		1
#define PROM_RESTART		2
#define PROM_REINIT		3
#define PROM_REBOOT		4
#define PROM_AUTOBOOT		5
#define PROM_OPEN		6
#define PROM_READ		7
#define PROM_WRITE		8
#define PROM_IOCTL		9
#define PROM_CLOSE		10
#define PROM_GETCHAR		11
#define PROM_PUTCHAR		12
#define PROM_SHOWCHAR		13	
#define PROM_GETS		14	
#define PROM_PUTS		15	
#define PROM_PRINTF		16	

/* What are these for? */
#define PROM_INITPROTO		17	
#define PROM_PROTOENABLE	18	
#define PROM_PROTODISABLE	19	
#define PROM_GETPKT		20	
#define PROM_PUTPKT		21	

/* More PROM shit.  Probably has to do with VME RMW cycles??? */
#define PROM_ORW_RMW		22	
#define PROM_ORH_RMW		23	
#define PROM_ORB_RMW		24	
#define PROM_ANDW_RMW		25	
#define PROM_ANDH_RMW		26	
#define PROM_ANDB_RMW		27	

/* Cache handling stuff */
#define PROM_FLUSHCACHE		28	
#define PROM_CLEARCACHE		29	

/* Libc alike stuff */
#define PROM_SETJMP		30	
#define PROM_LONGJMP		31	
#define PROM_BEVUTLB		32	
#define PROM_GETENV		33	
#define PROM_SETENV		34	
#define PROM_ATOB		35	
#define PROM_STRCMP		36	
#define PROM_STRLEN		37	
#define PROM_STRCPY		38	
#define PROM_STRCAT		39	

/* Misc stuff */
#define PROM_PARSER		40	
#define PROM_RANGE		41	
#define PROM_ARGVIZE		42	
#define PROM_HELP		43	

/* Entry points for some PROM commands */
#define PROM_DUMPCMD		44	
#define PROM_SETENVCMD		45	
#define PROM_UNSETENVCMD	46	
#define PROM_PRINTENVCMD	47	
#define PROM_BEVEXCEPT		48	
#define PROM_ENABLECMD		49	
#define PROM_DISABLECMD		50	

#define PROM_CLEARNOFAULT	51	
#define PROM_NOTIMPLEMENT	52	

#define PROM_NV_GET		53	
#define PROM_NV_SET		54	

#endif /* __ASM_MIPS_PROM_H */
