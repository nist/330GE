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
 *  tst-syslogd.c - tests concurrent threads calling syslog
 *
 *  build with: gcc -Wall tst-syslogd.c -lpthread
 */

#include <stdio.h>
#include <pthread.h>
#include <syslog.h>
#include <unistd.h>

void *log_func(void *arg)
{
	int i;
	int thrid = (int)arg;

	openlog(NULL, LOG_PERROR | LOG_PID, LOG_USER);
	for (i = 0; i < 10; i++) {
		syslog(LOG_DEBUG, "thread %i iter %i\n", thrid, i);
		sleep(thrid); /* this mixes things up a bit */
	}
	closelog();

	return NULL;
}

int main(int argc, char **argv)
{
	pthread_t thr1, thr2, thr3;
	int id1 = 1;
	int id2 = 2;
	int id3 = 3;

	pthread_create(&thr1, NULL, log_func, (void *)id1);
	pthread_create(&thr2, NULL, log_func, (void *)id2);
	pthread_create(&thr3, NULL, log_func, (void *)id3);

	pthread_join(thr1, NULL);
	pthread_join(thr2, NULL);
	pthread_join(thr3, NULL);

	return 0;
}

