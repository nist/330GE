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
/* pptp_gre.h -- encapsulate PPP in PPTP-GRE.
 *               Handle the IP Protocol 47 portion of PPTP.
 *               C. Scott Ananian <cananian@alumni.princeton.edu>
 *
 * $Id: pptp_gre.h,v 1.1.1.1 2007/01/25 12:52:55 jiahao_jhou Exp $
 */

int pptp_gre_bind(struct in_addr inetaddr);
void pptp_gre_copy(u_int16_t call_id, u_int16_t peer_call_id,
		   int pty_fd, int gre_fd);

extern int syncppp;
extern int disable_buffer;

typedef struct pack_track {
  uint32_t seq;       // seq no of this tracked packet
  uint64_t time;      // time when this tracked packet was sent (in usecs)
} pack_track_t;

typedef struct gre_stats {
  /* statistics for GRE receive */

  uint32_t rx_accepted;  // data packet was passed to pppd
  uint32_t rx_lost;      // data packet did not arrive before timeout
  uint32_t rx_underwin;  // data packet was under window (arrived too late
                         // or duplicate packet)
  uint32_t rx_overwin;   // data packet was over window
                         // (too many packets lost?)
  uint32_t rx_buffered;  // data packet arrived earlier than expected,
                         // packet(s) before it were lost or reordered
  uint32_t rx_errors;    // OS error on receive
  uint32_t rx_truncated; // truncated packet
  uint32_t rx_invalid;   // wrong protocol or invalid flags 
  uint32_t rx_acks;      // acknowledgement only

  /* statistics for GRE transmit */

  uint32_t tx_sent;      // data packet write() to GRE socket succeeded
  uint32_t tx_failed;    // data packet write() to GRE socket returned error
  uint32_t tx_short;     // data packet write() to GRE socket underflowed
  uint32_t tx_acks;      // sent packet with just ACK
  uint32_t tx_oversize;  // data packet dropped because it was too large

  /* statistics for packet tracking, for RTT calculation */

  pack_track_t pt;       // last data packet seq/time
  int rtt;               // estimated round-trip time in us

} gre_stats_t;

extern gre_stats_t stats;
