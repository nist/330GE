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

#define DN_DEBUG_BUFFER_BASE 0x82800000
#define DN_DEBUG_BUFFER_SIZE 8*1024*1024

static char *current_dbg_ptr=DN_DEBUG_BUFFER_BASE;

int dn_deb_printf(const char *fmt, ...) {

	va_list args;
	int i;

	if(current_dbg_ptr<(DN_DEBUG_BUFFER_BASE + DN_DEBUG_BUFFER_SIZE)) {
		va_start(args,fmt);
		i=vsprintf(current_dbg_ptr,fmt,args);
		va_end(args);
		current_dbg_ptr+=i;
	
		return i;
	}
	else 
		return 0;
}
