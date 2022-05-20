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
#ifndef _NCP_NO
#define _NCP_NO

/* these define the attribute byte as seen by NCP */
#define aRONLY			(ntohl(0x01000000))
#define aHIDDEN			(__constant_ntohl(0x02000000))
#define aSYSTEM			(__constant_ntohl(0x04000000))
#define aEXECUTE		(ntohl(0x08000000))
#define aDIR			(ntohl(0x10000000))
#define aARCH			(ntohl(0x20000000))
#define aSHARED			(ntohl(0x80000000))
#define aDONTSUBALLOCATE	(ntohl(1L<<(11+8)))
#define aTRANSACTIONAL		(ntohl(1L<<(12+8)))
#define aPURGE			(ntohl(1L<<(16-8)))
#define aRENAMEINHIBIT		(ntohl(1L<<(17-8)))
#define aDELETEINHIBIT		(ntohl(1L<<(18-8)))
#define aDONTCOMPRESS		(nothl(1L<<(27-24)))

#endif /* _NCP_NO */
