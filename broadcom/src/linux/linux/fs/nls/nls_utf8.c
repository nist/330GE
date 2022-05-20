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
 * Module for handling utf8 just like any other charset.
 * By Urban Widmark 2000
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/nls.h>
#include <linux/errno.h>

static unsigned char identity[256];

static int uni2char(wchar_t uni, unsigned char *out, int boundlen)
{
	int n;

	if ( (n = utf8_wctomb(out, uni, boundlen)) == -1) {
		*out = '?';
		return -EINVAL;
	}
	return n;
}

static int char2uni(const unsigned char *rawstring, int boundlen, wchar_t *uni)
{
	int n;

	if ( (n = utf8_mbtowc(uni, rawstring, boundlen)) == -1) {
		*uni = 0x003f;	/* ? */
		n = -EINVAL;
	}
	return n;
}

static struct nls_table table = {
	"utf8",
	uni2char,
	char2uni,
	identity,	/* no conversion */
	identity,
	THIS_MODULE,
};

static int __init init_nls_utf8(void)
{
	int i;
	for (i=0; i<256; i++)
		identity[i] = i;

        return register_nls(&table);
}

static void __exit exit_nls_utf8(void)
{
        unregister_nls(&table);
}

module_init(init_nls_utf8)
module_exit(exit_nls_utf8)
MODULE_LICENSE("Dual BSD/GPL");
