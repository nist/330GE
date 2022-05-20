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
#include <nvcommons5.h>

char * strsep_with_validation(char **ptr,char *sep,char * retchar)	
{ 
	char * strptr ;	
	char *cptr;

	if (*ptr == NULL)
		return retchar;	

	strptr  =strsep(ptr,sep);	
	if (strptr != NULL) {

		/* change eny last \n to NULL */	
		cptr = strchr(strptr,'\r');;
		if (cptr != NULL)
			*cptr =' '; 


		if (*strptr == 0x00)		
			return retchar;	
	
	}else if (strptr == NULL)
		return retchar;
	
	return	strptr;		
}
				
