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
 * BK Id: SCCS/s.find_name.c 1.5 05/17/01 18:14:21 cort
 */
#include <stdio.h>
#include <asm/page.h>
#include <sys/mman.h>
#include <strings.h>
/*
 * Finds a given address in the System.map and prints it out
 * with its neighbors.  -- Cort
 */

int main(int argc, char **argv)
{
	unsigned long addr, cmp, i;
	FILE *f;
	char s[256], last[256];
	
	if ( argc < 2 )
	{
		fprintf(stderr, "Usage: %s <address>\n", argv[0]);
		return -1;
	}

	for ( i = 1 ; argv[i] ; i++ )
	{
		sscanf( argv[i], "%0lx", &addr );
		/* adjust if addr is relative to kernelbase */
		if ( addr < PAGE_OFFSET )
			addr += PAGE_OFFSET;
		
		if ( (f = fopen( "System.map", "r" )) == NULL )
		{
			perror("fopen()\n");
			exit(-1);
		}
		
		while ( !feof(f) )
		{
			fgets(s, 255 , f);
			sscanf( s, "%0lx", &cmp );
			if ( addr < cmp )
				break;
			strcpy( last, s);
		}
		
		printf( "%s%s", last, s );
	}		
	fclose(f);
	return 0;
}
