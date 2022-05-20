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
#ifndef _IP_CT_PPTP_PRIV_H
#define _IP_CT_PPTP_PRIV_H

/* PptpControlMessageType names */
static const char *strMName[] = {
	"UNKNOWN_MESSAGE",
	"START_SESSION_REQUEST",
	"START_SESSION_REPLY",
	"STOP_SESSION_REQUEST",
	"STOP_SESSION_REPLY",
	"ECHO_REQUEST",
	"ECHO_REPLY",
	"OUT_CALL_REQUEST",
	"OUT_CALL_REPLY",
	"IN_CALL_REQUEST",
	"IN_CALL_REPLY",
	"IN_CALL_CONNECT",
	"CALL_CLEAR_REQUEST",
	"CALL_DISCONNECT_NOTIFY",
	"WAN_ERROR_NOTIFY",
	"SET_LINK_INFO"
};

#endif
