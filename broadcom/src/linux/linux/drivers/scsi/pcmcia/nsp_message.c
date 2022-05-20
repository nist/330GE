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
/*==========================================================================
  NinjaSCSI-3 message handler
      By: YOKOTA Hiroshi <yokota@netlab.is.tsukuba.ac.jp>

   This software may be used and distributed according to the terms of
   the GNU General Public License.
 */

/* $Id: nsp_message.c,v 1.1.1.1 2007/01/25 12:51:59 jiahao_jhou Exp $ */

static void nsp_message_in(Scsi_Cmnd *SCpnt, nsp_hw_data *data)
{
	unsigned int  base = SCpnt->host->io_port;
	unsigned char data_reg, control_reg;
	int           ret, len;

	ret = 16;
	len = 0;

	DEBUG(0, " msgin loop\n");
	do {
		/* read data */
		data_reg = nsp_index_read(base, SCSIDATAIN);

		/* assert ACK */
		control_reg = nsp_index_read(base, SCSIBUSCTRL);
		control_reg |= SCSI_ACK;
		nsp_index_write(base, SCSIBUSCTRL, control_reg);
		nsp_negate_signal(SCpnt, BUSMON_REQ, "msgin<REQ>");

		data->MsgBuffer[len] = data_reg; len++;

		/* deassert ACK */
		control_reg =  nsp_index_read(base, SCSIBUSCTRL);
		control_reg &= ~SCSI_ACK;
		nsp_index_write(base, SCSIBUSCTRL, control_reg);

		/* catch a next signal */
		ret = nsp_expect_signal(SCpnt, BUSPHASE_MESSAGE_IN, BUSMON_REQ);
	} while (ret > 0 && MSGBUF_SIZE > len);

	data->MsgLen = len;

}

static void nsp_message_out(Scsi_Cmnd *SCpnt, nsp_hw_data *data)
{
	int ret = 1;
	int len = data->MsgLen;


	DEBUG(0, " msgout loop\n");
	do {
		if (nsp_xfer(SCpnt, data, BUSPHASE_MESSAGE_OUT)) {
			printk(KERN_DEBUG " " __FUNCTION__ " msgout: xfer short\n");
		}

		/* catch a next signal */
		ret = nsp_expect_signal(SCpnt, BUSPHASE_MESSAGE_OUT, BUSMON_REQ);
	} while (ret > 0 && len-- > 0);

}

/* end */
