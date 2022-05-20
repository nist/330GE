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
#ifndef _IPT_OWNER_H
#define _IPT_OWNER_H

/* match and invert flags */
#define IPT_OWNER_UID	0x01
#define IPT_OWNER_GID	0x02
#define IPT_OWNER_PID	0x04
#define IPT_OWNER_SID	0x08
#define IPT_OWNER_COMM	0x10

struct ipt_owner_info {
    uid_t uid;
    gid_t gid;
    pid_t pid;
    pid_t sid;
    char comm[16];
    u_int8_t match, invert;	/* flags */
};

#endif /*_IPT_OWNER_H*/
