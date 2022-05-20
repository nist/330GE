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
#include <stdio.h>


void main()
{
	int c;
	int comma=0;
	int count=0;
	while((c=getchar())!=EOF)
	{
		unsigned char x=c;
		if(comma)
			printf(",");
		else
			comma=1;
		if(count==8)
		{
			count=0;
			printf("\n");
		}
		if(count==0)
			printf("\t");
		printf("0x%02X",c);
		count++;
	}
	if(count)
		printf("\n");
	exit(0);
}

		
