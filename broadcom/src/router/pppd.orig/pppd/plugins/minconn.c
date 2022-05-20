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
 * minconn.c - pppd plugin to implement a `minconnect' option.
 *
 * Copyright 1999 Paul Mackerras.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms.  The name of the author
 * may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */
#include <stddef.h>
#include <time.h>
#include "pppd.h"

char pppd_version[] = VERSION;

static int minconnect = 0;

static option_t my_options[] = {
	{ "minconnect", o_int, &minconnect,
	  "Set minimum connect time before idle timeout applies" },
	{ NULL }
};

static int my_get_idle(struct ppp_idle *idle)
{
	time_t t;

	if (idle == NULL)
		return minconnect? minconnect: idle_time_limit;
	t = idle->xmit_idle;
	if (idle->recv_idle < t)
		t = idle->recv_idle;
	return idle_time_limit - t;
}

void plugin_init(void)
{
	info("plugin_init");
	add_options(my_options);
	idle_time_hook = my_get_idle;
}
