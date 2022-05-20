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

/* OEM Data for 300 series */

const UCHAR SiS300_OEMTVDelay301[8][4] =
{
	{0x08,0x08,0x08,0x08},
	{0x08,0x08,0x08,0x08},
	{0x08,0x08,0x08,0x08},
	{0x2c,0x2c,0x2c,0x2c},
	{0x08,0x08,0x08,0x08},
	{0x08,0x08,0x08,0x08},
	{0x08,0x08,0x08,0x08},
	{0x20,0x20,0x20,0x20}
};

const UCHAR SiS300_OEMTVDelayLVDS[8][4] =
{
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20}
};

const UCHAR SiS300_OEMTVFlicker[8][4] =
{
	{0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00}
};


/* TW: From 630/301B BIOS */
const UCHAR SiS300_OEMLCDDelay2[64][4] =		 /* for 301/301b/302b/301LV/302LV */
{
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20}
};

/* TW: Added for LVDS */
const UCHAR SiS300_OEMLCDDelay3[32][4] = {	/* For LVDS */
	{0x20,0x20,0x20,0x20},  /* --- Expanding panels */
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x04,0x04,0x04,0x04},		/* Clevo 2202 (PanelType 12); Mitac needs 0x20! */
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},  	/* Uniwill N241S2 (PanelType 14)*/
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},  /* ---- NonExpanding panels */
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x04,0x04,0x04,0x04},          /* Gericom 2200C (PanelType 28) */
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20},
	{0x20,0x20,0x20,0x20}
};

const UCHAR SiS300_Phase1[8][6][4] =
{
    {
	{0x21,0xed,0x00,0x08},
	{0x21,0xed,0x8a,0x08},
	{0x21,0xed,0x8a,0x08},
	{0x21,0xed,0x8a,0x08},
	{0x21,0xed,0x8a,0x08},
	{0xff,0xff,0xff,0xff}
    },
    {
        {0x2a,0x05,0xd3,0x00},
	{0x2a,0x05,0xd3,0x00},
	{0x2a,0x05,0xd3,0x00},
	{0x2a,0x05,0xd3,0x00},
	{0x2a,0x05,0xd3,0x00},
	{0xff,0xff,0xff,0xff}
    },
    {
        {0x2a,0x05,0xd3,0x00},
	{0x2a,0x05,0xd3,0x00},
	{0x2a,0x05,0xd3,0x00},
	{0x2a,0x05,0xd3,0x00},
	{0x2a,0x05,0xd3,0x00},
	{0xff,0xff,0xff,0xff}
    },
    {
        {0x2a,0x05,0xd3,0x00},
	{0x2a,0x05,0xd3,0x00},
	{0x2a,0x05,0xd3,0x00},
	{0x2a,0x05,0xd3,0x00},
	{0x2a,0x05,0xd3,0x00},
	{0xff,0xff,0xff,0xff}
    },
    {
        {0x21,0xed,0x00,0x08},
	{0x21,0xed,0x8a,0x08},
	{0x21,0xed,0x8a,0x08},
	{0x21,0xed,0x8a,0x08},
	{0x21,0xed,0x8a,0x08},
	{0xff,0xff,0xff,0xff}
    },
    {
        {0x2a,0x05,0xd3,0x00},
	{0x2a,0x05,0xd3,0x00},
	{0x2a,0x05,0xd3,0x00},
	{0x2a,0x05,0xd3,0x00},
	{0x2a,0x05,0xd3,0x00},
	{0xff,0xff,0xff,0xff}
    },
    {
        {0x2a,0x05,0xd3,0x00},
	{0x2a,0x05,0xd3,0x00},
	{0x2a,0x05,0xd3,0x00},
	{0x2a,0x05,0xd3,0x00},
	{0x2a,0x05,0xd3,0x00},
	{0xff,0xff,0xff,0xff}
    },
    {
        {0x2a,0x05,0xd3,0x00},
	{0x2a,0x05,0xd3,0x00},
	{0x2a,0x05,0xd3,0x00},
	{0x2a,0x05,0xd3,0x00},
	{0x2a,0x05,0xd3,0x00},
	{0xff,0xff,0xff,0xff}
    }
};


const UCHAR SiS300_Phase2[8][6][4] =
{
    {
        {0x21,0xed,0x00,0x08},
	{0x21,0xed,0x8a,0x08},
	{0x21,0xed,0x8a,0x08},
	{0x21,0xed,0x8a,0x08},
	{0x21,0xed,0x8a,0x08},
	{0xff,0xff,0xff,0xff}
    },
    {
	{0x2a,0x05,0xd3,0x00},
	{0x2a,0x05,0xd3,0x00},
	{0x2a,0x05,0xd3,0x00},
	{0x2a,0x05,0xd3,0x00},
	{0x2a,0x05,0xd3,0x00},
	{0xff,0xff,0xff,0xff}
    },
    {
        {0x2a,0x05,0xd3,0x00},
	{0x2a,0x05,0xd3,0x00},
	{0x2a,0x05,0xd3,0x00},
	{0x2a,0x05,0xd3,0x00},
	{0x2a,0x05,0xd3,0x00},
	{0xff,0xff,0xff,0xff}
    },
    {
        {0x2a,0x05,0xd3,0x00},
	{0x2a,0x05,0xd3,0x00},
	{0x2a,0x05,0xd3,0x00},
	{0x2a,0x05,0xd3,0x00},
	{0x2a,0x05,0xd3,0x00},
	{0xff,0xff,0xff,0xff}
    },
    {
        {0x21,0xed,0x00,0x08},
	{0x21,0xed,0x8a,0x08},
	{0x21,0xed,0x8a,0x08},
	{0x21,0xed,0x8a,0x08},
	{0x21,0xed,0x8a,0x08},
	{0xff,0xff,0xff,0xff}
    },
    {
        {0x2a,0x05,0xd3,0x00},
	{0x2a,0x05,0xd3,0x00},
	{0x2a,0x05,0xd3,0x00},
	{0x2a,0x05,0xd3,0x00},
	{0x2a,0x05,0xd3,0x00},
	{0xff,0xff,0xff,0xff}
    },
    {
        {0x2a,0x05,0xd3,0x00},
	{0x2a,0x05,0xd3,0x00},
	{0x2a,0x05,0xd3,0x00},
	{0x2a,0x05,0xd3,0x00},
	{0x2a,0x05,0xd3,0x00},
	{0xff,0xff,0xff,0xff}
    },
    {
        {0x2a,0x05,0xd3,0x00},
	{0x2a,0x05,0xd3,0x00},
	{0x2a,0x05,0xd3,0x00},
	{0x2a,0x05,0xd3,0x00},
	{0x2a,0x05,0xd3,0x00},
	{0xff,0xff,0xff,0xff}
    }
};

const UCHAR SiS300_Filter1[10][17][4] =
{
    {
	{0x00,0xf4,0x10,0x38},
	{0x00,0xf4,0x10,0x38},
	{0xeb,0x04,0x10,0x18},
	{0xf7,0x06,0x19,0x14},
	{0x00,0xf4,0x10,0x38},
	{0xeb,0x04,0x25,0x18},
	{0xeb,0x04,0x25,0x18},
	{0xeb,0x15,0x25,0xf6},
	{0xeb,0x04,0x25,0x18},
	{0xeb,0x04,0x25,0x18},
	{0xeb,0x04,0x25,0x18},
	{0xeb,0x04,0x25,0x18},
	{0xeb,0x04,0x25,0x18},
	{0xeb,0x04,0x25,0x18},
	{0xeb,0x04,0x25,0x18},
	{0xeb,0x04,0x25,0x18},
	{0xff,0xff,0xff,0xff}
    },
    {
	{0x00,0xf4,0x10,0x38},
	{0x00,0xf4,0x10,0x38},
	{0xf1,0xf7,0x10,0x32},
	{0xf3,0x00,0x1d,0x20},
	{0x00,0xf4,0x10,0x38},
	{0xf1,0xf7,0x1f,0x32},
	{0xf1,0xf7,0x1f,0x32},
	{0xfc,0xfb,0x14,0x2a},
	{0xf1,0xf7,0x1f,0x32},
	{0xf1,0xf7,0x1f,0x32},
	{0xf1,0xf7,0x1f,0x32},
	{0xf1,0xf7,0x1f,0x32},
	{0xf1,0xf7,0x1f,0x32},
	{0xf1,0xf7,0x1f,0x32},
	{0xf1,0xf7,0x1f,0x32},
	{0xf1,0xf7,0x1f,0x32},
	{0xff,0xff,0xff,0xff}
    },
    {
	{0x00,0xf4,0x10,0x38},
	{0x00,0xf4,0x10,0x38},
	{0xf1,0xf7,0x10,0x32},
	{0xf3,0x00,0x1d,0x20},
	{0x00,0xf4,0x10,0x38},
	{0xf1,0xf7,0x1f,0x32},
	{0xf1,0xf7,0x1f,0x32},
	{0xfc,0xfb,0x14,0x2a},
	{0xf1,0xf7,0x1f,0x32},
	{0xf1,0xf7,0x1f,0x32},
	{0xf1,0xf7,0x1f,0x32},
	{0xf1,0xf7,0x1f,0x32},
	{0xf1,0xf7,0x1f,0x32},
	{0xf1,0xf7,0x1f,0x32},
	{0xf1,0xf7,0x1f,0x32},
	{0xf1,0xf7,0x1f,0x32},
	{0xff,0xff,0xff,0xff}
    },
    {
	{0x00,0xf4,0x10,0x38},
	{0x00,0xf4,0x10,0x38},
	{0xf1,0xf7,0x10,0x32},
	{0xf3,0x00,0x1d,0x20},
	{0x00,0xf4,0x10,0x38},
	{0xf1,0xf7,0x1f,0x32},
	{0xf1,0xf7,0x1f,0x32},
	{0xfc,0xfb,0x14,0x2a},
	{0xf1,0xf7,0x1f,0x32},
	{0xf1,0xf7,0x1f,0x32},
	{0xf1,0xf7,0x1f,0x32},
	{0xf1,0xf7,0x1f,0x32},
	{0xf1,0xf7,0x1f,0x32},
	{0xf1,0xf7,0x1f,0x32},
	{0xf1,0xf7,0x1f,0x32},
	{0xf1,0xf7,0x1f,0x32},
	{0xff,0xff,0xff,0xff}
    },
    {
	{0x00,0xf4,0x10,0x38},
	{0x00,0xf4,0x10,0x38},
	{0xeb,0x04,0x10,0x18},
	{0xf7,0x06,0x19,0x14},
	{0x00,0xf4,0x10,0x38},
	{0xeb,0x04,0x25,0x18},
	{0xeb,0x04,0x25,0x18},
	{0xeb,0x15,0x25,0xf6},
	{0xeb,0x04,0x25,0x18},
	{0xeb,0x04,0x25,0x18},
	{0xeb,0x04,0x25,0x18},
	{0xeb,0x04,0x25,0x18},
	{0xeb,0x04,0x25,0x18},
	{0xeb,0x04,0x25,0x18},
	{0xeb,0x04,0x25,0x18},
	{0xeb,0x04,0x25,0x18},
	{0xff,0xff,0xff,0xff}
    },
    {
	{0x00,0xf4,0x10,0x38},
	{0x00,0xf4,0x10,0x38},
	{0xf1,0xf7,0x10,0x32},
	{0xf3,0x00,0x1d,0x20},
	{0x00,0xf4,0x10,0x38},
	{0xf1,0xf7,0x1f,0x32},
	{0xf1,0xf7,0x1f,0x32},
	{0xfc,0xfb,0x14,0x2a},
	{0xf1,0xf7,0x1f,0x32},
	{0xf1,0xf7,0x1f,0x32},
	{0xf1,0xf7,0x1f,0x32},
	{0xf1,0xf7,0x1f,0x32},
	{0xf1,0xf7,0x1f,0x32},
	{0xf1,0xf7,0x1f,0x32},
	{0xf1,0xf7,0x1f,0x32},
	{0xf1,0xf7,0x1f,0x32},
	{0xff,0xff,0xff,0xff}
    },
    {
	{0x00,0xf4,0x10,0x38},
	{0x00,0xf4,0x10,0x38},
	{0xf1,0xf7,0x10,0x32},
	{0xf3,0x00,0x1d,0x20},
	{0x00,0xf4,0x10,0x38},
	{0xf1,0xf7,0x1f,0x32},
	{0xf1,0xf7,0x1f,0x32},
	{0xfc,0xfb,0x14,0x2a},
	{0xf1,0xf7,0x1f,0x32},
	{0xf1,0xf7,0x1f,0x32},
	{0xf1,0xf7,0x1f,0x32},
	{0xf1,0xf7,0x1f,0x32},
	{0xf1,0xf7,0x1f,0x32},
	{0xf1,0xf7,0x1f,0x32},
	{0xf1,0xf7,0x1f,0x32},
	{0xf1,0xf7,0x1f,0x32},
	{0xff,0xff,0xff,0xff}
    },
    {
	{0x00,0xf4,0x10,0x38},
	{0x00,0xf4,0x10,0x38},
	{0xf1,0xf7,0x10,0x32},
	{0xf3,0x00,0x1d,0x20},
	{0x00,0xf4,0x10,0x38},
	{0xf1,0xf7,0x1f,0x32},
	{0xf1,0xf7,0x1f,0x32},
	{0xfc,0xfb,0x14,0x2a},
	{0xf1,0xf7,0x1f,0x32},
	{0xf1,0xf7,0x1f,0x32},
	{0xf1,0xf7,0x1f,0x32},
	{0xf1,0xf7,0x1f,0x32},
	{0xf1,0xf7,0x1f,0x32},
	{0xf1,0xf7,0x1f,0x32},
	{0xf1,0xf7,0x1f,0x32},
	{0xf1,0xf7,0x1f,0x32},
	{0xff,0xff,0xff,0xff}
    },
    {
	{0x00,0xf4,0x10,0x38},
	{0x00,0xf4,0x10,0x38},
	{0xeb,0x04,0x10,0x18},
	{0xf7,0x06,0x19,0x14},
	{0x00,0xf4,0x10,0x38},
	{0xeb,0x04,0x25,0x18},
	{0xeb,0x04,0x25,0x18},
	{0xeb,0x15,0x25,0xf6},
	{0xeb,0x04,0x25,0x18},
	{0xeb,0x04,0x25,0x18},
	{0xeb,0x04,0x25,0x18},
	{0xeb,0x04,0x25,0x18},
	{0xeb,0x04,0x25,0x18},
	{0xeb,0x04,0x25,0x18},
	{0xeb,0x04,0x25,0x18},
	{0xeb,0x04,0x25,0x18},
	{0xff,0xff,0xff,0xff}
    },
    {
	{0x00,0xf4,0x10,0x38},
	{0x00,0xf4,0x10,0x38},
	{0xeb,0x04,0x10,0x18},
	{0xf7,0x06,0x19,0x14},
	{0x00,0xf4,0x10,0x38},
	{0xeb,0x04,0x25,0x18},
	{0xeb,0x04,0x25,0x18},
	{0xeb,0x15,0x25,0xf6},
	{0xeb,0x04,0x25,0x18},
	{0xeb,0x04,0x25,0x18},
	{0xeb,0x04,0x25,0x18},
	{0xeb,0x04,0x25,0x18},
	{0xeb,0x04,0x25,0x18},
	{0xeb,0x04,0x25,0x18},
	{0xeb,0x04,0x25,0x18},
	{0xeb,0x04,0x25,0x18},
	{0xff,0xff,0xff,0xff}
    },
};

const UCHAR SiS300_Filter2[10][9][7] =
{
    {
	{0xFF,0x03,0x02,0xF6,0xFC,0x27,0x46},
	{0x01,0x02,0xFE,0xF7,0x03,0x27,0x3C},
	{0xFF,0x03,0x02,0xF6,0xFC,0x27,0x46},
	{0x01,0x02,0xFE,0xF7,0x03,0x27,0x3C},
	{0xFF,0x03,0x02,0xF6,0xFC,0x27,0x46},
	{0xFF,0x03,0x02,0xF6,0xFC,0x27,0x46},
	{0x01,0x02,0xFE,0xF7,0x03,0x27,0x3C},
	{0x01,0x01,0xFC,0xF8,0x08,0x26,0x38},
	{0xFF,0xFF,0xFC,0x00,0x0F,0x22,0x28}
    },
    {
	{0xFF,0x03,0x02,0xF6,0xFC,0x27,0x46},
	{0x01,0x02,0xFE,0xF7,0x03,0x27,0x3C},
	{0xFF,0x03,0x02,0xF6,0xFC,0x27,0x46},
	{0x01,0x02,0xFE,0xF7,0x03,0x27,0x3C},
	{0xFF,0x03,0x02,0xF6,0xFC,0x27,0x46},
	{0xFF,0x03,0x02,0xF6,0xFC,0x27,0x46},
	{0x01,0x02,0xFE,0xF7,0x03,0x27,0x3C},
	{0x01,0x01,0xFC,0xF8,0x08,0x26,0x38},
	{0xFF,0xFF,0xFC,0x00,0x0F,0x22,0x28}
    },
    {
	{0xFF,0x03,0x02,0xF6,0xFC,0x27,0x46},
	{0x01,0x02,0xFE,0xF7,0x03,0x27,0x3C},
	{0xFF,0x03,0x02,0xF6,0xFC,0x27,0x46},
	{0x01,0x02,0xFE,0xF7,0x03,0x27,0x3C},
	{0xFF,0x03,0x02,0xF6,0xFC,0x27,0x46},
	{0xFF,0x03,0x02,0xF6,0xFC,0x27,0x46},
	{0x01,0x02,0xFE,0xF7,0x03,0x27,0x3C},
	{0x01,0x01,0xFC,0xF8,0x08,0x26,0x38},
	{0xFF,0xFF,0xFC,0x00,0x0F,0x22,0x28}
    },
    {
	{0xFF,0x03,0x02,0xF6,0xFC,0x27,0x46},
	{0x01,0x02,0xFE,0xF7,0x03,0x27,0x3C},
	{0xFF,0x03,0x02,0xF6,0xFC,0x27,0x46},
	{0x01,0x02,0xFE,0xF7,0x03,0x27,0x3C},
	{0xFF,0x03,0x02,0xF6,0xFC,0x27,0x46},
	{0xFF,0x03,0x02,0xF6,0xFC,0x27,0x46},
	{0x01,0x02,0xFE,0xF7,0x03,0x27,0x3C},
	{0x01,0x01,0xFC,0xF8,0x08,0x26,0x38},
	{0xFF,0xFF,0xFC,0x00,0x0F,0x22,0x28}
    },
    {
	{0xFF,0x03,0x02,0xF6,0xFC,0x27,0x46},
	{0x01,0x02,0xFE,0xF7,0x03,0x27,0x3C},
	{0xFF,0x03,0x02,0xF6,0xFC,0x27,0x46},
	{0x01,0x02,0xFE,0xF7,0x03,0x27,0x3C},
	{0xFF,0x03,0x02,0xF6,0xFC,0x27,0x46},
	{0xFF,0x03,0x02,0xF6,0xFC,0x27,0x46},
	{0x01,0x02,0xFE,0xF7,0x03,0x27,0x3C},
	{0x01,0x01,0xFC,0xF8,0x08,0x26,0x38},
	{0xFF,0xFF,0xFC,0x00,0x0F,0x22,0x28}
    },
    {
	{0xFF,0x03,0x02,0xF6,0xFC,0x27,0x46},
	{0x01,0x02,0xFE,0xF7,0x03,0x27,0x3C},
	{0xFF,0x03,0x02,0xF6,0xFC,0x27,0x46},
	{0x01,0x02,0xFE,0xF7,0x03,0x27,0x3C},
	{0xFF,0x03,0x02,0xF6,0xFC,0x27,0x46},
	{0xFF,0x03,0x02,0xF6,0xFC,0x27,0x46},
	{0x01,0x02,0xFE,0xF7,0x03,0x27,0x3C},
	{0x01,0x01,0xFC,0xF8,0x08,0x26,0x38},
	{0xFF,0xFF,0xFC,0x00,0x0F,0x22,0x28}
    },
    {
	{0xFF,0x03,0x02,0xF6,0xFC,0x27,0x46},
	{0x01,0x02,0xFE,0xF7,0x03,0x27,0x3C},
	{0xFF,0x03,0x02,0xF6,0xFC,0x27,0x46},
	{0x01,0x02,0xFE,0xF7,0x03,0x27,0x3C},
	{0xFF,0x03,0x02,0xF6,0xFC,0x27,0x46},
	{0xFF,0x03,0x02,0xF6,0xFC,0x27,0x46},
	{0x01,0x02,0xFE,0xF7,0x03,0x27,0x3C},
	{0x01,0x01,0xFC,0xF8,0x08,0x26,0x38},
	{0xFF,0xFF,0xFC,0x00,0x0F,0x22,0x28}
    },
    {
	{0xFF,0x03,0x02,0xF6,0xFC,0x27,0x46},
	{0x01,0x02,0xFE,0xF7,0x03,0x27,0x3C},
	{0xFF,0x03,0x02,0xF6,0xFC,0x27,0x46},
	{0x01,0x02,0xFE,0xF7,0x03,0x27,0x3C},
	{0xFF,0x03,0x02,0xF6,0xFC,0x27,0x46},
	{0xFF,0x03,0x02,0xF6,0xFC,0x27,0x46},
	{0x01,0x02,0xFE,0xF7,0x03,0x27,0x3C},
	{0x01,0x01,0xFC,0xF8,0x08,0x26,0x38},
	{0xFF,0xFF,0xFC,0x00,0x0F,0x22,0x28}
    },
    {
	{0xFF,0x03,0x02,0xF6,0xFC,0x27,0x46},
	{0x01,0x02,0xFE,0xF7,0x03,0x27,0x3C},
	{0xFF,0x03,0x02,0xF6,0xFC,0x27,0x46},
	{0x01,0x02,0xFE,0xF7,0x03,0x27,0x3C},
	{0xFF,0x03,0x02,0xF6,0xFC,0x27,0x46},
	{0xFF,0x03,0x02,0xF6,0xFC,0x27,0x46},
	{0x01,0x02,0xFE,0xF7,0x03,0x27,0x3C},
	{0x01,0x01,0xFC,0xF8,0x08,0x26,0x38},
	{0xFF,0xFF,0xFC,0x00,0x0F,0x22,0x28}
    },
    {
	{0xFF,0x03,0x02,0xF6,0xFC,0x27,0x46},
	{0x01,0x02,0xFE,0xF7,0x03,0x27,0x3C},
	{0xFF,0x03,0x02,0xF6,0xFC,0x27,0x46},
	{0x01,0x02,0xFE,0xF7,0x03,0x27,0x3C},
	{0xFF,0x03,0x02,0xF6,0xFC,0x27,0x46},
	{0xFF,0x03,0x02,0xF6,0xFC,0x27,0x46},
	{0x01,0x02,0xFE,0xF7,0x03,0x27,0x3C},
	{0x01,0x01,0xFC,0xF8,0x08,0x26,0x38},
	{0xFF,0xFF,0xFC,0x00,0x0F,0x22,0x28}
    }
};
