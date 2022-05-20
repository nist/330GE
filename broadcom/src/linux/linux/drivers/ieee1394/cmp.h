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
#ifndef __CMP_H
#define __CMP_H

struct cmp_mpr {
	u32 nplugs:5;
	u32 reserved:3;
	u32 persistent_ext:8;
	u32 non_persistent_ext:8;
	u32 bcast_channel_base:6;
	u32 rate:2;
} __attribute__((packed));

struct cmp_pcr {
	u32 payload:10;
	u32 overhead:4;
	u32 speed:2;
	u32 channel:6;
	u32 reserved:2;
	u32 p2p_count:6;
	u32 bcast_count:1;
	u32 online:1;
} __attribute__((packed));

struct cmp_pcr *cmp_register_opcr(struct hpsb_host *host, int plug,
				  int payload,
				  void (*update)(struct cmp_pcr *plug,
						 void *data),
				  void *data);
void cmp_unregister_opcr(struct hpsb_host *host, struct cmp_pcr *plug);

#endif /* __CMP_H */
