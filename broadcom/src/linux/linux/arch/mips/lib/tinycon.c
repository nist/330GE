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


#include <linux/tty.h>
#include <asm/bootinfo.h>

static unsigned int size_x;
static unsigned int size_y;
static unsigned short cursor_x;
static unsigned short cursor_y;
static volatile unsigned short *vram_addr;
static int console_needs_init = 1;

extern struct screen_info screen_info;

/* ----------------------------------------------------------------------
 * init_console()
 * ---------------------------------------------------------------------- */

void init_console(void)
{
  size_x = 80;
  size_y = 25;
  cursor_x = 0;
  cursor_y = 0;

  vram_addr = (unsigned short *)0xb00b8000;

  console_needs_init = 0;
}

void
set_size_x(unsigned int x)
{
  size_x = x;
}

void
set_size_y(unsigned int y)
{
  size_y = y;
}

void
set_vram(unsigned short *vram)
{
  vram_addr = vram;
}

void
set_crsr(unsigned int x, unsigned int y)
{
  cursor_x = x;
  cursor_y = y;
}

void
print_char(unsigned int x, unsigned int y, unsigned char c)
{
  volatile unsigned short *caddr;

  caddr = vram_addr + (y * size_x) + x;
  *caddr = (*caddr & 0xff00) | 0x0f00 | (unsigned short) c;
}

static void
scroll(void)
{
  volatile unsigned short *caddr;
  register int i;

  caddr = vram_addr;
  for(i=0; i<size_x * (size_y-1); i++)
    *(caddr++) = *(caddr + size_x);

  /* blank last line */

  caddr = vram_addr + (size_x * (size_y-1));
  for(i=0; i<size_x; i++)
    *(caddr++) = (*caddr & 0xff00) | (unsigned short) ' ';
}

void print_string(const unsigned char *str)
{
  unsigned char c;

  if (console_needs_init)
    init_console();

  while((c = *str++))
    switch(c)
      {
      case '\n':
	cursor_x = 0;
	cursor_y++;
	if(cursor_y == size_y)
	  {
	    scroll();
	    cursor_y = size_y - 1;
	  }
	break;

      default:
	print_char(cursor_x, cursor_y, c);
	cursor_x++;
	if(cursor_x == size_x)
	  {
	    cursor_x = 0;
	    cursor_y++;
	    if(cursor_y == size_y)
	      {
		scroll();
		cursor_y = size_y - 1;
	      }
	  }
	break;
      }
}

/* end of file */
