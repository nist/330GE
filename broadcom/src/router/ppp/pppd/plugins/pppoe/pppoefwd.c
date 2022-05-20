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
#include "pppoe.h"

void fatal (char *fmt, ...)
{
    va_list pvar;

#if defined(__STDC__)
    va_start(pvar, fmt);
#else
    char *fmt;
    va_start(pvar);
    fmt = va_arg(pvar, char *);
#endif

    vprintf( fmt, pvar);
    va_end(pvar);

    exit(1);			/* as promised */
}

void info (char *fmt, ...)
{
    va_list pvar;

#if defined(__STDC__)
    va_start(pvar, fmt);
#else
    char *fmt;
    va_start(pvar);
    fmt = va_arg(pvar, char *);
#endif

    vprintf( fmt, pvar);
    va_end(pvar);

}


int main(int argc, char** argv){
    int ret;
    struct session *ses = (struct session *)malloc(sizeof(struct session));

    if(!ses) return -1;

    ret = relay_init_ses(ses,argv[1],argv[2]);
    
    if( ret < 0 ){
	return -1;
    }

    ses->log_to_fd = 1;
    ses->opt_debug=1;
    while(1)
	ret = session_connect(ses);
    
    
    
    return ret;


}
