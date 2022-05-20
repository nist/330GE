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

#ifndef _ATARI_SLM_H
#define _ATARI_SLM_H

/* Atari SLM laser printer specific ioctls */

#define	SLMIOGSTAT	0xa100
#define	SLMIOGPSIZE	0xa101
#define	SLMIOGMFEED	0xa102

#define	SLMIORESET	0xa140

#define	SLMIOSPSIZE	0xa181
#define	SLMIOSMFEED	0xa182

/* Status returning structure (SLMIOGSTAT) */
struct SLM_status {
	int		stat;		/* numeric status code */
	char	str[40];	/* status string */
};

/* Paper size structure (SLMIO[GS]PSIZE) */
struct SLM_paper_size {
	int		width;
	int		height;
};

#endif /* _ATARI_SLM_H */
