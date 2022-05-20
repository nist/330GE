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
	setup.h	   (c) 1997-8   Grant R. Guenther <grant@torque.net>
		                Under the terms of the GNU General Public License.

        This is a table driven setup function for kernel modules
        using the module.variable=val,... command line notation.

*/

/* Changes:

	1.01	GRG 1998.05.05	Allow negative and defaulted values

*/

#include <linux/ctype.h>
#include <linux/string.h>

struct setup_tab_t {

	char	*tag;	/* variable name */
	int	size;	/* number of elements in array */
	int	*iv;	/* pointer to variable */
};

typedef struct setup_tab_t STT;

/*  t 	  is a table that describes the variables that can be set
	  by gen_setup
    n	  is the number of entries in the table
    ss	  is a string of the form:

		<tag>=[<val>,...]<val>
*/

static void generic_setup( STT t[], int n, char *ss )

{	int	j,k, sgn;

	k = 0;
	for (j=0;j<n;j++) {
		k = strlen(t[j].tag);
		if (strncmp(ss,t[j].tag,k) == 0) break;
	}
	if (j == n) return;

	if (ss[k] == 0) {
		t[j].iv[0] = 1;
		return;
	}

	if (ss[k] != '=') return;
	ss += (k+1);

	k = 0;
	while (ss && (k < t[j].size)) {
		if (!*ss) break;
		sgn = 1;
		if (*ss == '-') { ss++; sgn = -1; }
		if (!*ss) break;
		if (isdigit(*ss))
		  t[j].iv[k] = sgn * simple_strtoul(ss,NULL,0);
		k++; 
		if ((ss = strchr(ss,',')) != NULL) ss++;
	}
}

/* end of setup.h */

