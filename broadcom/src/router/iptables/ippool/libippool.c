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
/* support functions for ip_pool modules */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include <libippool/ip_pool_support.h>

void ip_pool_init(void)
{
}

ip_pool_t ip_pool_get_index(char *name)
{
	FILE *fp;
	char buf[256];

	if (isdigit(*name))
		return atoi(name);
	fp = fopen(IPPOOL_CONF, "r");
	if (!fp) exit_error(PARAMETER_PROBLEM,
			"cannot open %s - no pool names", IPPOOL_CONF);
	while (fgets(buf, sizeof(buf), fp)) {
		char *p = strtok(buf, " \t\n");
		ip_pool_t index;

		if (!p || *p == '#') continue;
		index = atoi(p);
		p = strtok(0, " \t\n");
		if (p && 0 == strcmp(p, name)) {
			fclose(fp);
			return index;
		}
	}
	exit_error(PARAMETER_PROBLEM,
		"pool '%s' not found in %s\n", name, IPPOOL_CONF);
}

char *ip_pool_get_name(char *buf, int size, ip_pool_t index, int numeric)
{
	FILE *fp;
	int ok = 0;

	/* make sure we have enough room, at least for a %d */
	if (size < 16)
		exit_error(PARAMETER_PROBLEM,
			"ip_pool_support:get_name buf too small (%d vs. 16)\n",
			size);
	if (numeric)
		goto do_numeric;
	fp = fopen(IPPOOL_CONF, "r");
	if (fp) {
		while (fgets(buf, size, fp)) {
			char *p = strtok(buf, " \t\n");

			if (!p || *p == '#') continue;
			if (index != atoi(p)) continue;
			p = strtok(0, " \t\n");
			if (!p || *p == '#') continue;
			memmove(buf, p, strlen(p)+1);
			ok = 1;
			break;
		}
		fclose(fp);
	}
	if (!ok) {
do_numeric:
		sprintf(buf, "%d", index);
	}
	return buf;
}
