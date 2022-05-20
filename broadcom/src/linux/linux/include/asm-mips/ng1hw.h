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
 * ng1hw.h: Tweaks the newport.h structures and definations to be compatible
 * 	    with IRIX.  Quite ugly, but it works.
 *
 * Copyright (C) 1999 Ulf Carlsson (ulfc@thepuffingroup.com)
 */
#ifndef _SGI_NG1HW_H
#define _SGI_NG1HW_H

#include <video/newport.h>

#define rex3regs	newport_rexregs
#define configregs	newport_cregs
#define float_long	npfreg_t

typedef struct newport_rexregs Rex3regs;
typedef struct newport_cregs Configregs;
typedef union np_dcb DCB_reg;


/* It looks like I can't do a simple tweak with this structure because the IRIX
 * version is just *too* stupid.  Ok, here's a new version of it..
 */

struct rex3chip {
	struct newport_rexregs set;
	unsigned long _unused0[0x16e];
	struct newport_rexregs go;
	unsigned long _unused1[0x22e];
	struct {
		struct newport_cregs set;
		unsigned long _unused2[0x1ef];
		struct newport_cregs go;
	} p1;
};

typedef struct rex3chip rex3Chip;
typedef struct rex3chip Rex3chip;

/* Tweak the defines .. */

#define DM0_OPCODE		NPORT_DMODE0_OPMASK
#define DM0_NOP			NPORT_DMODE0_NOP
#define DM0_READ		NPORT_DMODE0_RD
#define DM0_DRAW		NPORT_DMODE0_DRAW
#define DM0_SCR2SCR		NPORT_DMODE0_S2S

#define DM0_ADRMODE_SHIFT	2
#define DM0_ADRMODE		NPORT_DMODE0_AMMASK
#define DM0_SPAN		NPORT_DMODE0_SPAN
#define DM0_BLOCK		NPORT_DMODE0_BLOCK
#define DM0_ILINE		NPORT_DMODE0_ILINE
#define DM0_FLINE		NPORT_DMODE0_FLINE
#define DM0_ALINE		NPORT_DMODE0_ALINE
#define DM0_TLINE		NPORT_DMODE0_TLINE
#define DM0_BLINE               NPORT_DMODE0_BLINE

#define DM0_DOSETUP		NPORT_DMODE0_DOSETUP
#define DM0_COLORHOST		NPORT_DMODE0_CHOST
#define DM0_ALPHAHOST		NPORT_DMODE0_AHOST
#define DM0_STOPONX		NPORT_DMODE0_STOPX
#define DM0_STOPONY		NPORT_DMODE0_STOPY
#define DM0_STOPONXY		(NPORT_DMODE0_STOPX | NPORT_DMODE0_STOPY)
#define DM0_SKIPFIRST		NPORT_DMODE0_SK1ST
#define DM0_SKIPLAST		NPORT_DMODE0_SKLST
#define DM0_ENZPATTERN		NPORT_DMODE0_ZPENAB
#define DM0_ENLSPATTERN		NPORT_DMODE0_LISPENAB
#define DM0_LSADVLAST		NPORT_DMODE0_LISLST
#define DM0_LENGTH32		NPORT_DMODE0_L32
#define DM0_ZOPAQUE		NPORT_DMODE0_ZOPQ
#define DM0_LSOPAQUE		NPORT_DMODE0_LISOPQ
#define DM0_SHADE		NPORT_DMODE0_SHADE
#define DM0_LRONLY		NPORT_DMODE0_LRONLY
#define DM0_XYOFFSET		NPORT_DMODE0_XYOFF
#define DM0_CICLAMP		NPORT_DMODE0_CLAMP
#define DM0_ENDPTFILTER		NPORT_DMODE0_ENDPF
#define	DM0_YSTRIDE		NPORT_DMODE0_YSTR

#define DM1_PLANES_SHIFT	0
/* The rest of the DM1 planes defines are in newport.h */

#define DM1_DRAWDEPTH_SHIFT	3
#define DM1_DRAWDEPTH_MASK	NPORT_DMODE1_DDMASK
#define DM1_DRAWDEPTH		NPORT_DMODE1_DD24 /* An alias? */
#define DM1_DRAWDEPTH4		NPORT_DMODE1_DD4
#define DM1_DRAWDEPTH8		NPORT_DMODE1_DD8
#define DM1_DRAWDEPTH12		NPORT_DMODE1_DD12
#define DM1_DRAWDEPTH24		NPORT_DMODE1_DD24

#define DM1_DBLSRC		NPORT_DMODE1_DSRC
#define DM1_YFLIP		NPORT_DMODE1_YFLIP
#define DM1_RWPACKED		NPORT_DMODE1_RWPCKD

#define DM1_HOSTDEPTH_SHIFT 	8
#define DM1_HOSTDEPTH_MASK	NPORT_DMODE1_HDMASK
#define DM1_HOSTDEPTH		NPORT_DMODE1_HD32 /* An alias? */
#define DM1_HOSTDEPTH4		NPORT_DMODE1_HD4
#define DM1_HOSTDEPTH8		NPORT_DMODE1_HD8
#define DM1_HOSTDEPTH12		NPORT_DMODE1_HD12
#define DM1_HOSTDEPTH32		NPORT_DMODE1_HD32

#define DM1_RWDOUBLE		NPORT_DMODE1_RWDBL
#define DM1_SWAPENDIAN		NPORT_DMODE1_ESWAP

#define DM1_COLORCOMPARE_SHIFT	12
#define DM1_COLORCOMPARE_MASK	NPORT_DMODE1_CCMASK
#define DM1_COLORCOMPARE	NPORT_DMODE1_CCMASK
#define DM1_COLORCOMPLT		NPORT_DMODE1_CCLT
#define DM1_COLORCOMPEQ		NPORT_DMODE1_CCEQ
#define DM1_COLORCOMPGT		NPORT_DMODE1_CCGT

#define DM1_RGBMODE		NPORT_DMODE1_RGBMD
#define DM1_ENDITHER		NPORT_DMODE1_DENAB
#define DM1_FASTCLEAR		NPORT_DMODE1_FCLR
#define DM1_ENBLEND		NPORT_DMODE1_BENAB

#define DM1_SF_SHIFT		19
#define DM1_SF_MASK   		NPORT_DMODE1_SFMASK
#define DM1_SF			NPORT_DMODE1_SFMASK
#define DM1_SF_ZERO		NPORT_DMODE1_SF0
#define DM1_SF_ONE		NPORT_DMODE1_SF1
#define DM1_SF_DC		NPORT_DMODE1_SFDC
#define DM1_SF_MDC		NPORT_DMODE1_SFMDC
#define DM1_SF_SA		NPORT_DMODE1_SFSA
#define DM1_SF_MSA		NPORT_DMODE1_SFMSA

#define DM1_DF_SHIFT		22	/* dfactor(2:0)	*/
#define DM1_DF_MASK		NPORT_DMODE1_DFMASK
#define DM1_DF			NPORT_DMODE1_DFMASK
#define DM1_DF_ZERO		NPORT_DMODE1_DF0
#define DM1_DF_ONE		NPORT_DMODE1_DF1
#define DM1_DF_SC		NPORT_DMODE1_DFSC
#define DM1_DF_MSC		NPORT_DMODE1_DFMSC
#define DM1_DF_SA		NPORT_DMODE1_DFSA
#define DM1_DF_MSA		NPORT_DMODE1_DFMSA

#define DM1_ENBACKBLEND		NPORT_DMODE1_BBENAB
#define DM1_ENPREFETCH		NPORT_DMODE1_PFENAB
#define DM1_BLENDALPHA		NPORT_DMODE1_ABLEND

#define DM1_LO_SHIFT		28
#define DM1_LO			NPORT_DMODE1_LOMASK
#define DM1_LO_MASK      	NPORT_DMODE1_LOMASK
#define DM1_LO_ZERO		NPORT_DMODE1_LOZERO
#define DM1_LO_AND		NPORT_DMODE1_LOAND
#define DM1_LO_ANDR		NPORT_DMODE1_LOANDR
#define DM1_LO_SRC		NPORT_DMODE1_LOSRC
#define DM1_LO_ANDI		NPORT_DMODE1_LOANDI
#define DM1_LO_DST		NPORT_DMODE1_LODST
#define DM1_LO_XOR		NPORT_DMODE1_LOXOR
#define DM1_LO_OR		NPORT_DMODE1_LOOR
#define DM1_LO_NOR		NPORT_DMODE1_LONOR
#define DM1_LO_XNOR		NPORT_DMODE1_LOXNOR
#define DM1_LO_NDST		NPORT_DMODE1_LONDST
#define DM1_LO_ORR		NPORT_DMODE1_LOORR
#define DM1_LO_NSRC		NPORT_DMODE1_LONSRC
#define DM1_LO_ORI		NPORT_DMODE1_LOORI
#define DM1_LO_NAND		NPORT_DMODE1_LONAND
#define DM1_LO_ONE		NPORT_DMODE1_LOONE

#define SMASK0			NPORT_CMODE_SM0
#define SMASK1			NPORT_CMODE_SM1
#define SMASK2			NPORT_CMODE_SM2
#define SMASK3			NPORT_CMODE_SM3
#define SMASK4			NPORT_CMODE_SM4
#define ALL_SMASKS		0x1f

#define CM_CIDMATCH_SHIFT       9
#define CM_CIDMATCH_MASK        NPORT_CMODE_CMSK

#define REX3VERSION_MASK	NPORT_STAT_VERS
#define GFXBUSY        		NPORT_STAT_GBUSY
#define BACKBUSY        	NPORT_STAT_BBUSY
#define VRINT           	NPORT_STAT_VRINT
#define VIDEOINT        	NPORT_STAT_VIDINT
#define GFIFO_LEVEL_SHIFT       7
#define GFIFO_LEVEL_MASK        NPORT_STAT_GLMSK
#define BFIFO_LEVEL_SHIFT       13
#define BFIFO_LEVEL_MASK        NPORT_STAT_BLMSK
#define BFIFO_INT        	NPORT_STAT_BFIRQ
#define GFIFO_INT        	NPORT_STAT_GFIRQ

#define GIO32MODE		NPORT_CFG_G32MD
#define BUSWIDTH		NPORT_CFG_BWIDTH
#define EXTREGXCVR		NPORT_CFG_ERCVR
#define BFIFODEPTH_SHIFT	3
#define BFIFODEPTH_MASK		NPORT_CFG_BDMSK
#define BFIFOABOVEINT		NPORT_CFG_BFAINT
#define GFIFODEPTH_SHIFT        8
#define GFIFODEPTH_MASK		NPORT_CFG_GDMSK
#define GFIFOABOVEINT		NPORT_CFG_GFAINT
#define TIMEOUT_SHIFT		14
#define TIMEOUT_MASK		NPORT_CFG_TOMSK
#define VREFRESH_SHIFT		17
#define VREFRESH_MASK		NPORT_CFG_VRMSK
#define FB_TYPE			NPORT_CFG_FBTYP

#define DCB_DATAWIDTH_MASK	(0x3)

#define DCB_CRS_MASK		(0x7 << DCB_CRS_SHIFT)
#define DCB_ADDR_MASK		(0xf << DCB_ADDR_SHIFT)
#define DCB_CSWIDTH_MASK	(0x1f << DCB_CSWIDTH_SHIFT)
#define DCB_CSHOLD_MASK		(0x1f << DCB_CSHOLD_SHIFT)
#define DCB_CSSETUP_MASK	(0x1f << DCB_CSSETUP_SHIFT)

#define DCB_SWAPENDIAN		(1 << 28)

#define REX3WAIT(rex3)  while ((rex3)->p1.set.status & GFXBUSY)
#define BFIFOWAIT(rex3)  while ((rex3)->p1.set.status & BACKBUSY)

#define REX3_GIO_ADDR_0         0x1f0f0000
#define REX3_GIO_ADDR_1         0x1f4f0000
#define REX3_GIO_ADDR_2         0x1f8f0000
#define REX3_GIO_ADDR_3         0x1fcf0000

#define NG1_XSIZE		1280
#define NG1_YSIZE		1024

#endif
