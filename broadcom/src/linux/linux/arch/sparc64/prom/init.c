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
/* $Id: init.c,v 1.1.1.1 2007/01/25 12:51:51 jiahao_jhou Exp $
 * init.c:  Initialize internal variables used by the PROM
 *          library functions.
 *
 * Copyright (C) 1995 David S. Miller (davem@caip.rutgers.edu)
 * Copyright (C) 1996,1997 Jakub Jelinek (jj@sunsite.mff.cuni.cz)
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/string.h>
#include <linux/ctype.h>

#include <asm/openprom.h>
#include <asm/oplib.h>

enum prom_major_version prom_vers;
unsigned int prom_rev, prom_prev;

/* The root node of the prom device tree. */
int prom_root_node;
int prom_stdin, prom_stdout;
int prom_chosen_node;

/* You must call prom_init() before you attempt to use any of the
 * routines in the prom library.  It returns 0 on success, 1 on
 * failure.  It gets passed the pointer to the PROM vector.
 */

extern void prom_meminit(void);
extern void prom_cif_init(void *, void *);

void __init prom_init(void *cif_handler, void *cif_stack)
{
	char buffer[80], *p;
	int ints[3];
	int node;
	int i = 0;

	prom_vers = PROM_P1275;

	prom_cif_init(cif_handler, cif_stack);

	prom_root_node = prom_getsibling(0);
	if((prom_root_node == 0) || (prom_root_node == -1))
		prom_halt();

	prom_chosen_node = prom_finddevice("/chosen");
	if (!prom_chosen_node || prom_chosen_node == -1)
		prom_halt();

	prom_stdin = prom_getint (prom_chosen_node, "stdin");
	prom_stdout = prom_getint (prom_chosen_node, "stdout");

	node = prom_finddevice("/openprom");
	if (!node || node == -1)
		prom_halt();

	prom_getstring (node, "version", buffer, sizeof (buffer));

	prom_printf ("\n");

	if (strncmp (buffer, "OBP ", 4))
		goto strange_version;

	/* Version field is expected to be 'OBP xx.yy.zz date...' */

	p = buffer + 4;
	while (p && isdigit(*p) && i < 3) {
		ints[i++] = simple_strtoul(p, NULL, 0);
		if ((p = strchr(p, '.')) != NULL)
			p++;
	}
	if (i != 3)
		goto strange_version;

	prom_rev = ints[1];
	prom_prev = (ints[0] << 16) | (ints[1] << 8) | ints[2];

	printk ("PROMLIB: Sun IEEE Boot Prom %s\n", buffer + 4);

	prom_meminit();

	/* Initialization successful. */
	return;

strange_version:
	prom_printf ("Strange OBP version `%s'.\n", buffer);
	prom_halt ();
}
