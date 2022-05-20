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
#include <syslog.h>

int do_log(char* msg, int delay)
{
  openlog("testlog", LOG_PID, LOG_DAEMON);
  while(1) {
    syslog(LOG_ERR, "%s: testing one, two, three\n", msg);
    sleep(delay);
  }
  closelog();
  return(0);
};

int main(void)
{
  if (fork()==0)
    do_log("A", 2);
  do_log("B", 3);
}
