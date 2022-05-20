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
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <syslog.h>
#include <signal.h>
#include <string.h>
#include <sys/klog.h>
#include <sys/types.h>
#include <sys/mount.h>
#include <sys/reboot.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <sys/time.h>
#include <sys/utsname.h>
#include <sys/wait.h>
#include <sys/socket.h>

void a_delay(int interval)
{
	unsigned int i, j, k;

	for(i=0;i<interval;i++)
	{
		for(j=0;j<6000;j++) k++;
	}
}

void main(int argc, char *argv[])
{
	char buf[4096];
	unsigned int val, len, interval;
	FILE *fp;
	time_t t1, t2;
	int rand;


	if (argc>1&&strstr(argv[1], "usb"))
	{			
		while(1)
		{
		
		unlink(argv[2]);
		fp = fopen(argv[2], "w");

		if (fp!=NULL)
		{
			len = atoi(argv[3]);
			interval = atoi(argv[4]);
			printf("write %d : %d\n", len, interval);

			while(len!=0)
			{
				//a_delay(interval);

				rand++;
				memset(buf, rand, sizeof(buf));

				if (len>=4096)
				{
					fwrite(buf, 1, sizeof(buf), fp);
					len-=4096;
				}	
				else 
				{
					fwrite(buf, 1, len, fp);
					len = 0;
				}
			}

			fclose(fp);
		}

		printf("after write %d : %d\n", len, interval);
		sleep(interval);
		printf("before %d : %d\n", len, interval);


		fp = fopen(argv[2], "r");
		if (fp!=NULL)
		{
			len = atoi(argv[3]);
			interval = atoi(argv[4]);
			printf("read %d : %d\n", len, interval);
			while(len!=0)
			{
				//printf("r %d", interval);
				//if (interval!=0) sleep(interval);
				//a_delay(interval);

				if (len>=4096)
				{
					fread(buf, 1, sizeof(buf), fp);
					len-=4096;
				}	
				else 
				{
					fread(buf, 1, len, fp);
					len = 0;
				}
			}
			fclose(fp);
		}
		}
	}
	else if (argc>1&&strcmp(argv[1], "gpio")==0)
	{
		if (argc>3) // write
		{
			sscanf(argv[3], "%x", &val);
			fp=fopen(argv[2], "w");
			if (fp!=NULL)
			{
				fwrite(&val, 4, 1, fp);
				fclose(fp);
			}
		}
		else
		{
			fp=fopen(argv[2], "r");
			if (fp!=NULL)
			{
				fread(&val, 4, 1, fp);
				fclose(fp);
				printf("val : %x\n", val);
			}			
		}
	}
	else if (argc>2)
	{	
	
		fp = fopen(argv[1], "w");
		if (fp!=NULL)
		{
			len = atoi(argv[2]);
			time(&t1);
			printf("before write %d : %d\n", len, t1);
			while(len!=0)
			{
				if (len>=4096)
				{
					fwrite(buf, 1, sizeof(buf), fp);
					len-=4096;
				}	
				else 
				{
					fwrite(buf, 1, len, fp);
					len = 0;
				}
			}
			time(&t2);
			//printf("after write %d, rate: %f (MBPS)\n", t2, (double)atoi(argv[1])/difftime(t2-t1));
			fclose(fp);
		}


		fp = fopen(argv[1], "r");
		if (fp!=NULL)
		{
			len = atoi(argv[2]);
			time(&t1);
			printf("before read %d : %d\n", len, t1);
			while(len!=0)
			{
				if (len>=4096)
				{
					fread(buf, 1, sizeof(buf), fp);
					len-=4096;
				}	
				else 
				{
					fwrite(buf, 1, len, fp);
					len = 0;
				}
			}
			time(&t2);
			//printf("after read %d, rate: %f (MBPS)\n", t2, (double)atoi(argv[1])/difftime(t2-t1));
			fclose(fp);
		}
	}	
}
