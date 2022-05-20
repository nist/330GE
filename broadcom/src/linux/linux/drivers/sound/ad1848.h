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

#define AD_F_CS4231     0x0001  /* Returned if a CS4232 (or compatible) detected */
#define AD_F_CS4248     0x0001  /* Returned if a CS4248 (or compatible) detected */

#define         AD1848_SET_XTAL         1
#define         AD1848_MIXER_REROUTE    2

#define AD1848_REROUTE(oldctl, newctl) \
                ad1848_control(AD1848_MIXER_REROUTE, ((oldctl)<<8)|(newctl))
		

int ad1848_init(char *name, int io_base, int irq, int dma_playback,
	int dma_capture, int share_dma, int *osp, struct module *owner);
void ad1848_unload (int io_base, int irq, int dma_playback, int dma_capture, int share_dma);

int ad1848_detect (int io_base, int *flags, int *osp);
int ad1848_control(int cmd, int arg);

void adintr(int irq, void *dev_id, struct pt_regs * dummy);
void attach_ms_sound(struct address_info * hw_config, struct module * owner);

int probe_ms_sound(struct address_info *hw_config);
void unload_ms_sound(struct address_info *hw_info);
