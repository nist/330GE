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
 * usema/usemaclone-related stuff.
 *
 * `Inspired' by IRIX's sys/usioctl.h
 *
 * Mike.
 */

/* ioctls */
#define UIOC	('u' << 16 | 's' << 8)

#define UIOCATTACHSEMA	(UIOC|2)	/* attach to sema */
#define UIOCBLOCK	(UIOC|3)	/* block sync "intr"? */
#define UIOCABLOCK	(UIOC|4)	/* block async */
#define UIOCNOIBLOCK	(UIOC|5)	/* IRIX: block sync intr
					   Linux: block sync nointr */
#define UIOCUNBLOCK	(UIOC|6)	/* unblock sync */
#define UIOCAUNBLOCK	(UIOC|7)	/* unblock async */
#define UIOCINIT	(UIOC|8)	/* init sema (async) */

typedef struct usattach_s {
  dev_t	us_dev;		/* attach dev */
  void	*us_handle;	/* userland semaphore handle */
} usattach_t;

