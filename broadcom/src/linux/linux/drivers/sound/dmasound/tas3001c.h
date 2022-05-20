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
 * Header file for the i2c/i2s based TA3001C sound chip used
 * on some Apple hardware. Also known as "tumbler".
 *
 *  This file is subject to the terms and conditions of the GNU General Public
 *  License.  See the file COPYING in the main directory of this archive
 *  for more details.
 *
 * Written by Christopher C. Chimelis <chris@debian.org>
 */

#ifndef _tas3001c_h_
#define _tas3001c_h_

/*
 * Macros that correspond to the registers that we write to
 * when setting the various values.
 */
#define TAS_DRC			0x02	/* DRC		*/
#define TAS_VOLUME		0x04	/* Volume	*/
#define TAS_TREBLE		0x05	/* Treble	*/
#define TAS_BASS		0x06	/* Bass		*/
#define TAS_MIXER1		0x07	/* PCM line	*/
#define TAS_MIXER2		0x08	/* Input (Unk)	*/

/*
 * Macros that define various arguments to tas_set_register()
 */
#define TAS_SET_DRC		TAS_DRC, 2
#define TAS_SET_VOLUME		TAS_VOLUME, 6
#define TAS_SET_TREBLE		TAS_TREBLE, 1
#define TAS_SET_BASS		TAS_BASS, 1
#define TAS_SET_MIXER1		TAS_MIXER1, 3
#define TAS_SET_MIXER2		TAS_MIXER2, 3


/*
 * tas_volume_table contains lookup values for the volume settings
 * for tumbler.  This is straight from the programming manual
 * for the chip, however, it's zero-sourced for your shopping pleasure
 * (meaning, you'll have to compute the difference between the desired
 * dB and the index value of the proper setting.
 *
 * This table should've been replaced by the formula:
 * 	dB = 20 log(x)
 * but, since there's no log() or supporting functions like exp(),
 * my implementation of the above won't work. Yeah, I could do it
 * the hard way, but this table is just easier :-)
 *
 * For reference, -70 dB = tas_volume_table[0]
 */

static unsigned int tas_volume_table[] = {
	0x00000015,	0x00000016,	0x00000017,	/* -70.0, -69.5, -69.0 */
	0x00000019,	0x0000001a,	0x0000001c,	/* -68.5, -68.0, -67.5 */
	0x0000001d,	0x0000001f,	0x00000021,	/* -67.0, -66.5, -66.0 */
	0x00000023,	0x00000025,	0x00000027,	/* -65.5, -65.0, -64.5 */
	0x00000029,	0x0000002c,	0x0000002e,	/* -64.0, -63.5, -63.0 */
	0x00000031,	0x00000034,	0x00000037,	/* -62.5, -62.0, -61.5 */
	0x0000003a,	0x0000003e,	0x00000042,	/* -61.0, -60.5, -60.0 */
	0x00000045,	0x0000004a,	0x0000004e,	/* -59.5, -59.0, -58.5 */
	0x00000053,	0x00000057,	0x0000005d,	/* -58.0, -57.5, -57.0 */
	0x00000062,	0x00000068,	0x0000006e,	/* -56.5, -56.0, -55.5 */
	0x00000075,	0x0000007b,	0x00000083,	/* -55.0, -54.5, -54.0 */
	0x0000008b,	0x00000093,	0x0000009b,	/* -53.5, -53.0, -52.5 */
	0x000000a5,	0x000000ae,	0x000000b9,	/* -52.0, -51.5, -51.0 */
	0x000000c4,	0x000000cf,	0x000000dc,	/* -50.5, -50.0, -49.5 */
	0x000000e9,	0x000000f6,	0x00000105,	/* -49.0, -48.5, -48.0 */
	0x00000114,	0x00000125,	0x00000136,	/* -47.5, -47.0, -46.5 */
	0x00000148,	0x0000015c,	0x00000171,	/* -46.0, -45.5, -45.0 */
	0x00000186,	0x0000019e,	0x000001b6,	/* -44.5, -44.0, -43.5 */
	0x000001d0,	0x000001eb,	0x00000209,	/* -43.0, -42.5, -42.0 */
	0x00000227,	0x00000248,	0x0000026b,	/* -41.5, -41.0, -40.5 */
	0x0000028f,	0x000002b6,	0x000002df,	/* -40.0, -39.5, -39.0 */
	0x0000030b,	0x00000339,	0x0000036a,	/* -38.5, -38.0, -37.5 */
	0x0000039e,	0x000003d5,	0x0000040f,	/* -37.0, -36.5, -36.0 */
	0x0000044c,	0x0000048d,	0x000004d2,	/* -35.5, -35.0, -34.5 */
	0x0000051c,	0x00000569,	0x000005bb,	/* -34.0, -33.5, -33.0 */
	0x00000612,	0x0000066e,	0x000006d0,	/* -32.5, -32.0, -31.5 */
	0x00000737,	0x000007a5,	0x00000818,	/* -31.0, -30.5, -30.0 */
	0x00000893,	0x00000915,	0x0000099f,	/* -29.5, -29.0, -28.5 */
	0x00000a31,	0x00000acc,	0x00000b6f,	/* -28.0, -27.5, -27.0 */
	0x00000c1d,	0x00000cd5,	0x00000d97,	/* -26.5, -26.0, -25.5 */
	0x00000e65,	0x00000f40,	0x00001027,	/* -25.0, -24.5, -24.0 */
	0x0000111c,	0x00001220,	0x00001333,	/* -23.5, -23.0, -22.5 */
	0x00001456,	0x0000158a,	0x000016d1,	/* -22.0, -21.5, -21.0 */
	0x0000182b,	0x0000199a,	0x00001b1e,	/* -20.5, -20.0, -19.5 */
	0x00001cb9,	0x00001e6d,	0x0000203a,	/* -19.0, -18.5, -18.0 */
	0x00002223,	0x00002429,	0x0000264e,	/* -17.5, -17.0, -16.5 */
	0x00002893,	0x00002afa,	0x00002d86,	/* -16.0, -15.5, -15.0 */
	0x00003039,	0x00003314,	0x0000361b,	/* -14.5, -14.0, -13.5 */
	0x00003950,	0x00003cb5,	0x0000404e,	/* -13.0, -12.5, -12.0 */
	0x0000441d,	0x00004827,	0x00004c6d,	/* -11.5, -11.0, -10.5 */
	0x000050f4,	0x000055c0,	0x00005ad5,	/* -10.0, -09.5, -09.0 */
	0x00006037,	0x000065ea,	0x00006bf4,	/* -08.5, -08.0, -07.5 */
	0x0000725a,	0x00007920,	0x0000804e,	/* -07.0, -06.5, -06.0 */
	0x000087e8,	0x00008ff6,	0x0000987d,	/* -05.5, -05.0, -04.5 */
	0x0000a186,	0x0000ab19,	0x0000b53c,	/* -04.0, -03.5, -03.0 */
	0x0000bff9,	0x0000cb59,	0x0000d766,	/* -02.5, -02.0, -01.5 */
	0x0000e429,	0x0000f1ae,	0x00010000,	/* -01.0, -00.5,  00.0 */
	0x00010f2b,	0x00011f3d,	0x00013042,	/* +00.5, +01.0, +01.5 */
	0x00014249,	0x00015562,	0x0001699c,	/* +02.0, +02.5, +03.0 */
	0x00017f09,	0x000195bc,	0x0001adc6,	/* +03.5, +04.0, +04.5 */
	0x0001c73d,	0x0001e237,	0x0001feca,	/* +05.0, +05.5, +06.0 */
	0x00021d0e,	0x00023d1d,	0x00025f12,	/* +06.5, +07.0, +07.5 */
	0x0002830b,	0x0002a925,	0x0002d182,	/* +08.0, +08.5, +09.0 */
	0x0002fc42,	0x0003298b,	0x00035983,	/* +09.5, +10.0, +10.5 */
	0x00038c53,	0x0003c225,	0x0003fb28,	/* +11.0, +11.5, +12.0 */
	0x0004378b,	0x00047783,	0x0004bb44,	/* +12.5, +13.0, +13.5 */
	0x0005030a,	0x00054f10,	0x00059f98,	/* +14.0, +14.5, +15.0 */
	0x0005f4e5,	0x00064f40,	0x0006aef6,	/* +15.5, +16.0, +16.5 */
	0x00071457,	0x00077fbb,	0x0007f17b	/* +17.0, +17.5, +18.0 */
};

/* tas_treble_table[] is a lookup table that holds the values to drop into
 * the treble setting register on the TAS.  Again, there is a formula for
 * this one, but we use this instead due to lack of real math functions
 * in the kernel.
 */
static char tas_treble_table[] = {
	0x96,		0x95,		0x94,	/* -18.0, -17.5, -17.0 */
	0x93,		0x92,		0x91,	/* -16.5, -16.0, -15.5 */
	0x90,		0x8f,		0x8e,	/* -15.0, -14.5, -14.0 */
	0x8d,		0x8c,		0x8b,	/* -13.5, -13.0, -12.5 */
	0x8a,		0x89,		0x88,	/* -12.0, -11.5, -11.0 */
	0x87,		0x86,		0x85,	/* -10.5, -10.0, -09.5 */
	0x84,		0x83,		0x82,	/* -09.0, -08.5, -08.0 */
	0x81,		0x80,		0x7f,	/* -07.5, -07.0, -06.5 */
	0x7e,		0x7d,		0x7c,	/* -06.0, -05.5, -05.0 */
	0x7b,		0x7a,		0x79,	/* -04.5, -04.0, -03.5 */
	0x78,		0x77,		0x76,	/* -03.0, -02.5, -02.0 */
	0x75,		0x74,		0x73,	/* -01.5, -01.0, -00.5 */
	0x72,		0x71,		0x70,	/*  00.0, +00.5, +01.0 */
	0x6e,		0x6d,		0x6c,	/* +01.5, +02.0, +02.5 */
	0x6b,		0x69,		0x68,	/* +03.0, +03.5, +04.0 */
	0x66,		0x65,		0x63,	/* +04.5, +05.0, +05.5 */
	0x62,		0x60,		0x5e,	/* +06.0, +06.5, +07.0 */
	0x5c,		0x5a,		0x57,	/* +07.5, +08.0, +08.5 */
	0x55,		0x52,		0x4f,	/* +09.0, +09.5, +10.0 */
	0x4c,		0x49,		0x45,	/* +10.5, +11.0, +11.5 */
	0x42,		0x3e,		0x3a,	/* +12.0, +12.5, +13.0 */
	0x36,		0x32,		0x2d,	/* +13.5, +14.0, +14.5 */
	0x28,		0x22,		0x1c,	/* +15.0, +15.5, +16.0 */
	0x16,		0x10,		0x09,	/* +16.5, +17.0, +17.5 */
	0x01					/* +18.0	       */
};

/* tas_bass_table[] is a lookup table that holds the values to drop into
 * the bass setting register on the TAS.  Again, there is a formula for
 * this one, but we use this instead due to lack of real math functions
 * in the kernel.
 */
static char tas_bass_table[] = {
	0x86,		0x82,		0x7f,	/* -18.0, -17.5, -17.0 */
	0x7d,		0x7a,		0x78,	/* -16.5, -16.0, -15.5 */
	0x76,		0x74,		0x72,	/* -15.0, -14.5, -14.0 */
	0x70,		0x6e,		0x6d,   /* -13.5, -13.0, -12.5 */
	0x6b,		0x69,		0x66,	/* -12.0, -11.5, -11.0 */
	0x64,		0x61,		0x5f,	/* -10.5, -10.0, -09.5 */
	0x5d,		0x5c,		0x5a,	/* -09.0, -08.5, -08.0 */
	0x59,		0x58,		0x56,	/* -07.5, -07.0, -06.5 */
	0x55,		0x54,		0x53,	/* -06.0, -05.5, -05.0 */
	0x51,		0x4f,		0x4d,	/* -04.5, -04.0, -03.5 */
	0x4b,		0x49,		0x46,	/* -03.0, -02.5, -02.0 */
	0x44,		0x42,		0x40,	/* -01.5, -01.0, -00.5 */
	0x3e,		0x3c,		0x3b,	/*  00.0, +00.5, +01.0 */
	0x39,		0x38,		0x36,	/* +01.5, +02.0, +02.5 */
	0x35,		0x33,		0x31,	/* +03.0, +03.5, +04.0 */
	0x30,		0x2e,		0x2c,	/* +04.5, +05.0, +05.5 */
	0x2b,		0x29,		0x28,	/* +06.0, +06.5, +07.0 */
	0x26,		0x25,		0x23,	/* +07.5, +08.0, +08.5 */
	0x21,		0x1f,		0x1c,	/* +09.0, +09.5, +10.0 */
	0x19,		0x18,		0x17,	/* +10.5, +11.0, +11.5 */
	0x16,		0x14,		0x13,	/* +12.0, +12.5, +13.0 */
	0x12,		0x10,		0x0f,	/* +13.5, +14.0, +14.5 */
	0x0d,		0x0b,		0x0a,	/* +15.0, +15.5, +16.0 */
	0x08,		0x06,		0x03,	/* +16.5, +17.0, +17.5 */
	0x01					/* +18.0	       */
};

/* tas_input_table[] is a lookup table that holds the values to drop into
 * the setting registers on the TAS for "mixers 1 & 2" (which are the input
 * lines).  Again, there is a formula for these, but we use this instead
 * due to lack of real math functions in the kernel.
 */
static unsigned int tas_input_table[] = {
	0x00014b,	0x00015f,	0x000174,	/* -70.0, -69.5, -69.0 */
	0x00018a,	0x0001a1,	0x0001ba,	/* -68.5, -68.0, -67.5 */
	0x0001d4,	0x0001f0,	0x00020d,	/* -67.0, -66.5, -66.0 */
	0x00022c,	0x00024d,	0x000270,	/* -65.5, -65.0, -64.5 */
	0x000295,	0x0002bc,	0x0002e6,	/* -64.0, -63.5, -63.0 */
	0x000312,	0x000340,	0x000372,	/* -62.5, -62.0, -61.5 */
	0x0003a6,	0x0003dd,	0x000418,	/* -61.0, -60.5, -60.0 */
	0x000456,	0x000498,	0x0004de,	/* -59.5, -59.0, -58.5 */
	0x000528,	0x000576,	0x0005c9,	/* -58.0, -57.5, -57.0 */
	0x000620,	0x00067d,	0x0006e0,	/* -56.5, -56.0, -55.5 */
	0x000748,	0x0007b7,	0x00082c,	/* -55.0, -54.5, -54.0 */
	0x0008a8,	0x00092b,	0x0009b6,	/* -53.5, -53.0, -52.5 */
	0x000a49,	0x000ae5,	0x000b8b,	/* -52.0, -51.5, -51.0 */
	0x000c3a,	0x000cf3,	0x000db8,	/* -50.5, -50.0, -49.5 */
	0x000e88,	0x000f64,	0x00104e,	/* -49.0, -48.5, -48.0 */
	0x001145,	0x00124b,	0x001361,	/* -47.5, -47.0, -46.5 */
	0x001487,	0x0015be,	0x001708,	/* -46.0, -45.5, -45.0 */
	0x001865,	0x0019d8,	0x001b60,	/* -44.5, -44.0, -43.5 */
	0x001cff,	0x001eb7,	0x002089,	/* -43.0, -42.5, -42.0 */
	0x002276,	0x002481,	0x0026ab,	/* -41.5, -41.0, -40.5 */
	0x0028f5,	0x002b63,	0x002df5,	/* -40.0, -39.5, -39.0 */
	0x0030ae,	0x003390,	0x00369e,	/* -38.5, -38.0, -37.5 */
	0x0039db,	0x003d49,	0x0040ea,	/* -37.0, -36.5, -36.0 */
	0x0044c3,	0x0048d6,	0x004d27,	/* -35.5, -35.0, -34.5 */
	0x0051b9,	0x005691,	0x005bb2,	/* -34.0, -33.5, -33.0 */
	0x006121,	0x0066e3,	0x006cfb,	/* -32.5, -32.0, -31.5 */
	0x007370,	0x007a48,	0x008186,	/* -31.0, -30.5, -30.0 */
	0x008933,	0x009154,	0x0099f1,	/* -29.5, -29.0, -28.5 */
	0x00a310,	0x00acba,	0x00b6f6,	/* -28.0, -27.5, -27.0 */
	0x00c1cd,	0x00cd49,	0x00d973,	/* -26.5, -26.0, -25.5 */
	0x00e655,	0x00f3fb,	0x010270,	/* -25.0, -24.5, -24.0 */
	0x0111c0,	0x0121f9,	0x013328,	/* -23.5, -23.0, -22.5 */
	0x01455b,	0x0158a2,	0x016d0e,	/* -22.0, -21.5, -21.0 */
	0x0182af,	0x019999,	0x01b1de,	/* -20.5, -20.0, -19.5 */
	0x01cb94,	0x01e6cf,	0x0203a7,	/* -19.0, -18.5, -18.0 */
	0x022235,	0x024293,	0x0264db,	/* -17.5, -17.0, -16.5 */
	0x02892c,	0x02afa3,	0x02d862,	/* -16.0, -15.5, -15.0 */
	0x03038a,	0x033142,	0x0361af,	/* -14.5, -14.0, -13.5 */
	0x0394fa,	0x03cb50,	0x0404de,	/* -13.0, -12.5, -12.0 */
	0x0441d5,	0x048268,	0x04c6d0,	/* -11.5, -11.0, -10.5 */
	0x050f44,	0x055c04,	0x05ad50,	/* -10.0, -09.5, -09.0 */
	0x06036e,	0x065ea5,	0x06bf44,	/* -08.5, -08.0, -07.5 */
	0x07259d,	0x079207,	0x0804dc,	/* -07.0, -06.5, -06.0 */
	0x087e80,	0x08ff59,	0x0987d5,	/* -05.5, -05.0, -04.5 */
	0x0a1866,	0x0ab189,	0x0b53be,	/* -04.0, -03.5, -03.0 */
	0x0bff91,	0x0cb591,	0x0d765a,	/* -02.5, -02.0, -01.5 */
	0x0e4290,	0x0f1adf,	0x100000,	/* -01.0, -00.5,  00.0 */
	0x10f2b4,	0x11f3c9,	0x13041a,	/* +00.5, +01.0, +01.5 */
	0x14248e,	0x15561a,	0x1699c0,	/* +02.0, +02.5, +03.0 */
	0x17f094,	0x195bb8,	0x1adc61,	/* +03.5, +04.0, +04.5 */
	0x1c73d5,	0x1e236d,	0x1fec98,	/* +05.0, +05.5, +06.0 */
	0x21d0d9,	0x23d1cd,	0x25f125,	/* +06.5, +07.0, +07.5 */
	0x2830af,	0x2a9254,	0x2d1818,	/* +08.0, +08.5, +09.0 */
	0x2fc420,	0x3298b0,	0x35982f,	/* +09.5, +10.0, +10.5 */
	0x38c528,	0x3c224c,	0x3fb278,	/* +11.0, +11.5, +12.0 */
	0x437880,	0x477828,	0x4bb446,	/* +12.5, +13.0, +13.5 */
	0x5030a1,	0x54f106,	0x59f980,	/* +14.0, +14.5, +15.0 */
	0x5f4e52,	0x64f403,	0x6aef5d,	/* +15.5, +16.0, +16.5 */
	0x714575,	0x77fbaa,	0x7f17af	/* +17.0, +17.5, +18.0 */
};

#endif /* _tas3001c_h_ */
