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

#ifndef _IEEE1394_CSR_H
#define _IEEE1394_CSR_H

#define CSR_REGISTER_BASE  0xfffff0000000ULL

/* register offsets relative to CSR_REGISTER_BASE */
#define CSR_STATE_CLEAR           0x0
#define CSR_STATE_SET             0x4
#define CSR_NODE_IDS              0x8
#define CSR_RESET_START           0xc
#define CSR_SPLIT_TIMEOUT_HI      0x18
#define CSR_SPLIT_TIMEOUT_LO      0x1c
#define CSR_CYCLE_TIME            0x200
#define CSR_BUS_TIME              0x204
#define CSR_BUSY_TIMEOUT          0x210
#define CSR_BUS_MANAGER_ID        0x21c
#define CSR_BANDWIDTH_AVAILABLE   0x220
#define CSR_CHANNELS_AVAILABLE_HI 0x224
#define CSR_CHANNELS_AVAILABLE_LO 0x228
#define CSR_CONFIG_ROM            0x400
#define CSR_CONFIG_ROM_END        0x800
#define CSR_FCP_COMMAND           0xB00
#define CSR_FCP_RESPONSE          0xD00
#define CSR_FCP_END               0xF00
#define CSR_TOPOLOGY_MAP          0x1000
#define CSR_TOPOLOGY_MAP_END      0x1400
#define CSR_SPEED_MAP             0x2000
#define CSR_SPEED_MAP_END         0x3000


struct csr_control {
        spinlock_t lock;

        quadlet_t state;
        quadlet_t node_ids;
        quadlet_t split_timeout_hi, split_timeout_lo;
        quadlet_t cycle_time;
        quadlet_t bus_time;
        quadlet_t bus_manager_id;
        quadlet_t bandwidth_available;
        quadlet_t channels_available_hi, channels_available_lo;

        const quadlet_t *rom;
        size_t rom_size;

        quadlet_t topology_map[256];
        quadlet_t speed_map[1024];
};


void init_csr(void);
void cleanup_csr(void);

#endif /* _IEEE1394_CSR_H */
