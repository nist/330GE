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
 * getopt.c
 */

#include <linux/kernel.h>
#include <linux/string.h>

#include "getopt.h"

/**
 *	smb_getopt - option parser
 *	@caller: name of the caller, for error messages
 *	@options: the options string
 *	@opts: an array of &struct option entries controlling parser operations
 *	@optopt: output; will contain the current option
 *	@optarg: output; will contain the value (if one exists)
 *	@flag: output; may be NULL; should point to a long for or'ing flags
 *	@value: output; may be NULL; will be overwritten with the integer value
 *		of the current argument.
 *
 *	Helper to parse options on the format used by mount ("a=b,c=d,e,f").
 *	Returns opts->val if a matching entry in the 'opts' array is found,
 *	0 when no more tokens are found, -1 if an error is encountered.
 */
int smb_getopt(char *caller, char **options, struct option *opts,
	       char **optopt, char **optarg, unsigned long *flag,
	       unsigned long *value)
{
	char *token;
	char *val;
	int i;

	do {
		if ((token = strsep(options, ",")) == NULL)
			return 0;
	} while (*token == '\0');
	*optopt = token;

	*optarg = NULL;
	if ((val = strchr (token, '=')) != NULL) {
		*val++ = 0;
		if (value)
			*value = simple_strtoul(val, NULL, 0);
		*optarg = val;
	}

	for (i = 0; opts[i].name != NULL; i++) {
		if (!strcmp(opts[i].name, token)) {
			if (!opts[i].flag && (!val || !*val)) {
				printk("%s: the %s option requires an argument\n",
				       caller, token);
				return -1;
			}

			if (flag && opts[i].flag)
				*flag |= opts[i].flag;

			return opts[i].val;
		}
	}
	printk("%s: Unrecognized mount option %s\n", caller, token);
	return -1;
}
