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
 * linux/fs/nls_gb2312.c
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/nls.h>
#include <linux/errno.h>

static struct nls_table *p_nls;

static struct nls_table table = {
	"gb2312",
	NULL,
	NULL,
	NULL,
	NULL,
	THIS_MODULE,
};

static int __init init_nls_gb2312(void)
{
	p_nls = load_nls("cp936");

	if (p_nls) {
		table.uni2char = p_nls->uni2char;
		table.char2uni = p_nls->char2uni;
		table.charset2upper = p_nls->charset2upper;
		table.charset2lower = p_nls->charset2lower;
		return register_nls(&table);
	}

	return -EINVAL;
}

static void __exit exit_nls_gb2312(void)
{
	unregister_nls(&table);
	unload_nls(p_nls);
}

module_init(init_nls_gb2312)
module_exit(exit_nls_gb2312)
MODULE_LICENSE("Dual BSD/GPL");

/*
 * Overrides for Emacs so that we follow Linus's tabbing style.
 * Emacs will notice this stuff at the end of the file and automatically
 * adjust the settings for this buffer only.  This must remain at the end
 * of the file.
 *
---------------------------------------------------------------------------
 * Local variables:
 * c-indent-level: 8
 * c-brace-imaginary-offset: 0
 * c-brace-offset: -8
 * c-argdecl-indent: 8
 * c-label-offset: -8
 * c-continued-statement-offset: 8
 * c-continued-brace-offset: 0
 * End:
 */
