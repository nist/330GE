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
#ifndef __PRESTO_JOURNAL_H
#define __PRESTO_JOURNAL_H


#include <linux/version.h>

struct journal_prefix {
	int len;
        u32 version;
	int pid;
	int uid;
	int fsuid;
	int fsgid;
	int opcode;
        u32 ngroups;
        u32 groups[0];
};

struct journal_suffix {
	unsigned long prevrec;  /* offset of previous record for dentry */
	int recno;
	int time;
	int len;
};

#endif
