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
 * arch/mips/galileo/misc.c
 *
 * This is a collection of several routines from gzip-1.0.3
 * adapted for Linux.
 *
 * malloc by Hannu Savolainen 1993 and Matthias Urlichs 1994
 * puts by Nick Holloway 1993, better puts by Martin Mares 1995
 * High loaded stuff by Hans Lermen & Werner Almesberger, Feb. 1996
 *
 * Modified by RidgeRun Inc.
 */

#include <linux/vmalloc.h>
#include <asm/io.h>
/*
 * gzip declarations
 */

#define OF(args)  args
#define STATIC static

#define channel 1

#undef memset
#undef memcpy
#define memzero(s, n)     memset ((s), 0, (n))

typedef unsigned char uch;
typedef unsigned short ush;
typedef unsigned long ulg;

#define WSIZE 0x8000		/* Window size must be at least 32k, */
			     /* and a power of two */

static uch *inbuf;		/* input buffer */
static uch window[WSIZE];	/* Sliding window buffer */

static unsigned insize = 0;	/* valid bytes in inbuf */
static unsigned inptr = 0;	/* index of next byte to be processed in inbuf */
static unsigned outcnt = 0;	/* bytes in output buffer */

/* gzip flag byte */
#define ASCII_FLAG   0x01	/* bit 0 set: file probably ASCII text */
#define CONTINUATION 0x02	/* bit 1 set: continuation of multi-part gzip file */
#define EXTRA_FIELD  0x04	/* bit 2 set: extra field present */
#define ORIG_NAME    0x08	/* bit 3 set: original file name present */
#define COMMENT      0x10	/* bit 4 set: file comment present */
#define ENCRYPTED    0x20	/* bit 5 set: file is encrypted */
#define RESERVED     0xC0	/* bit 6,7:   reserved */

void variable_init();
static void puts(const char *);

void int2hex(int i)
{
	int tth, th, h, t, d;

	if (i > 99999) {
		serial_putc(channel, "Error - int2hex outofbounds");
		return;
	}

	tth = (i) / 10000;
	th = (i - (tth * 10000)) / 1000;
	h = (i - ((tth * 10000) + (th * 1000))) / 100;
	t = (i - ((tth * 10000) + (th * 1000) + (h * 100))) / 10;
	d = (i - ((tth * 10000) + (th * 1000) + (h * 100) + (t * 10)));
	serial_putc(channel, tth + '0');
	serial_putc(channel, th + '0');
	serial_putc(channel, h + '0');
	serial_putc(channel, t + '0');
	serial_putc(channel, d + '0');
}

int checksum;
int byte_count;
static unsigned char *input_data;

static int printCnt = 0;

int get_byte()
{
	unsigned char c = (inptr < insize ? inbuf[inptr++] : fill_inbuf());
	byte_count++;
	checksum += c;

	// if (printCnt++ < 150)
	// {
	//   puts("\n");
	//   puts("byte count = ");
	//   int2hex(byte_count & 0xff);
	//   puts(" byte val = ");
	//   int2hex(c);
	//   puts(" checksum = ");
	//   int2hex(checksum & 0xff);
	//   puts("\n");
	// }
	return c;
}

/* Diagnostic functions */
#ifdef DEBUG
#  define Assert(cond,msg) {if(!(cond)) error(msg);}
#  define Trace(x) fprintf x
#  define Tracev(x) {if (verbose) fprintf x ;}
#  define Tracevv(x) {if (verbose>1) fprintf x ;}
#  define Tracec(c,x) {if (verbose && (c)) fprintf x ;}
#  define Tracecv(c,x) {if (verbose>1 && (c)) fprintf x ;}
#else
#  define Assert(cond,msg)
#  define Trace(x)
#  define Tracev(x)
#  define Tracevv(x)
#  define Tracec(c,x)
#  define Tracecv(c,x)
#endif

static int fill_inbuf(void);
static void flush_window(void);
static void error(char *m);
static void gzip_mark(void **);
static void gzip_release(void **);

/*
 * This is set up by the setup-routine at boot-time
 */


#define STACK_SIZE (4096)
long user_stack[STACK_SIZE];
long *stack_start = &user_stack[STACK_SIZE];

extern int linux_compressed_start;
extern int linux_compressed_size;
extern int malloc_start;

static int input_len;

static long bytes_out = 0;
extern int kernel_location_start;
static uch *output_data;
static unsigned long output_ptr = 0;


static void *malloc(int size);
static void free(void *where);
static void error(char *m);
static void gzip_mark(void **);
static void gzip_release(void **);

static unsigned long free_mem_ptr;
static unsigned long free_mem_end_ptr;

#include "../../../../../lib/inflate.c"

static void *malloc(int size)
{
	void *p;

	if (size < 0)
		error("Malloc error\n");
	//      if (free_mem_ptr <= 0) error("Memory error\n");

	free_mem_ptr = (free_mem_ptr + 3) & ~3;	/* Align */

	p = (void *) free_mem_ptr;
	free_mem_ptr += size;

	if (free_mem_ptr >= free_mem_end_ptr)
		error("\nOut of memory\n");

	return p;
}

static void free(void *where)
{				/* Don't care */
}

static void gzip_mark(void **ptr)
{
	*ptr = (void *) free_mem_ptr;
}

static void gzip_release(void **ptr)
{
	free_mem_ptr = (long) *ptr;
}

static void puts(const char *s)
{
	while (*s) {
		if (*s == 10)
			serial_putc(channel, 13);
		serial_putc(channel, *s++);
	}
}

void *memset(void *s, int c, size_t n)
{
	int i;
	char *ss = (char *) s;

	for (i = 0; i < n; i++)
		ss[i] = c;
	return s;
}

void *memcpy(void *__dest, __const void *__src, size_t __n)
{
	int i;
	char *d = (char *) __dest, *s = (char *) __src;

	for (i = 0; i < __n; i++)
		d[i] = s[i];
	return __dest;
}

/* ===========================================================================
 * Fill the input buffer. This is called only when the buffer is empty
 * and at least one byte is really needed.
 */
static int fill_inbuf(void)
{
	if (insize != 0) {
		error("ran out of input data\n");
	}

	inbuf = input_data;
	insize = input_len;
	inptr = 1;
	return inbuf[0];
}

/* ===========================================================================
 * Write the output window window[0..outcnt-1] and update crc and bytes_out.
 * (Used for the decompressed data only.)
 */
static void flush_window(void)
{
	ulg c = crc;		/* temporary variable */
	unsigned n;
	uch *in, *out, ch;

	in = window;
	out = &output_data[output_ptr];
	for (n = 0; n < outcnt; n++) {
		ch = *out++ = *in++;
		c = crc_32_tab[((int) c ^ ch) & 0xff] ^ (c >> 8);
	}
	crc = c;
	bytes_out += (ulg) outcnt;
	output_ptr += (ulg) outcnt;
	outcnt = 0;
}

check_mem()
{
	int i;

	puts("\ncplens = ");
	for (i = 0; i < 10; i++) {
		int2hex(cplens[i]);
		puts(" ");
	}
	puts("\ncplext = ");
	for (i = 0; i < 10; i++) {
		int2hex(cplext[i]);
		puts(" ");
	}
	puts("\nborder = ");
	for (i = 0; i < 10; i++) {
		int2hex(border[i]);
		puts(" ");
	}
	puts("\n");
}

static void error(char *x)
{
	check_mem();
	puts("\n\n");
	puts(x);
	puts("byte_count = ");
	int2hex(byte_count);
	puts("\n");
	puts("\n\n -- System halted");
	while (1);		/* Halt */
}

void variable_init()
{
	byte_count = 0;
	checksum = 0;
	input_data = (unsigned char *) &linux_compressed_start;
	input_len = linux_compressed_size;
	output_data = &kernel_location_start;
	free_mem_ptr = (long) &malloc_start;
	free_mem_end_ptr = (long) ((char *) &malloc_start + 0x400000);
}

int decompress_kernel()
{
	//check_mem();

	variable_init();

	makecrc();
	puts("Uncompressing Linux... \n");
	gunzip();		// ...see inflate.c
	puts("Ok, booting the kernel.\n");
	return 0;
}