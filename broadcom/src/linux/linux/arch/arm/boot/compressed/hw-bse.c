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
 * Bright Star Engineering Inc.
 *
 * code for readng parameters from the
 * parameter blocks of the boot block
 * flash memory
 *
 */

static int strcmp(const char *s1, const char *s2)
{
  while (*s1 != '\0' && *s1 == *s2)
    {
      s1++;
      s2++;
    }

  return (*(unsigned char *) s1) - (*(unsigned char *) s2);
}

struct pblk_t {
  char type;
  unsigned short size;
};

static char *bse_getflashparam(char *name) {
  unsigned int esize;
  char *q,*r;
  unsigned char *p,*e;
  struct pblk_t *thepb = (struct pblk_t *) 0x00004000;
  struct pblk_t *altpb = (struct pblk_t *) 0x00006000;  
  if (thepb->type&1) {
    if (altpb->type&1) {
      /* no valid param block */ 
      return (char*)0;
    } else {
      /* altpb is valid */
      struct pblk_t *tmp;
      tmp = thepb;
      thepb = altpb;
      altpb = tmp;
    }
  }
  p = (char*)thepb + sizeof(struct pblk_t);
  e = p + thepb->size; 
  while (p < e) {
    q = p;
    esize = *p;
    if (esize == 0xFF) break;
    if (esize == 0) break;
    if (esize > 127) {
      esize = (esize&0x7F)<<8 | p[1];
      q++;
    }
    q++;
    r=q;
    if (*r && ((name == 0) || (!strcmp(name,r)))) {
      while (*q++) ;
      return q;
    }
    p+=esize;
  }
  return (char*)0;
}

void bse_setup(void) {
  /* extract the linux cmdline from flash */
  char *name=bse_getflashparam("linuxboot");
  char *x = (char *)0xc0000100;
  if (name) { 
    while (*name) *x++=*name++;
  }
  *x=0;
}
