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
static char *opcode_names[] = {
"NOP", "CLEAR_REMOTE_BUSY", "CONNECT_INDICATION", "CONNECT_CONFIRM", "DATA_INDICATION",
"DISCONNECT_INDICATION", "DUMMY_6", "RESET_CONFIRM", "DUMMY_8",
"IF_F=1_CLEAR_REMOTE_BUSY", "IF_DATA_FLAG=2_STOP_REJ_TIMER", "SEND_DISC_CMD(P=X)",
"SEND_DM_RSP(F=X)", "SEND_FRMR_RSP(F=X)", "RE-SEND_FRMR_RSP(F=0)",
"RE-SEND_FRMR_RSP(F=P)", "SEND_I_CMD(P=1)", "RE-SEND_I_CMD(P=1)",
"RE-SEND_I_CMD(P=1)_OR_SEND_RR", "SEND_I_XXX(X=0)", "RE-SEND_I_XXX(X=0)",
"RE-SEND_I_XXX(X=0)_OR_SEND_RR", "RE-SEND_I_RSP(F=1)", "SEND_REJ_CMD(P=1)",
"SEND_REJ_RSP(F=1)", "SEND_REJ_XXX(X=0)", "SEND_RNR_CMD(F=1)", "SEND_RNR_RSP(F=1)",
"SEND_RNR_XXX(X=0)", "SET_REMOTE_BUSY", "OPTIONAL_SEND_RNR_XXX(X=0)",
"SEND_RR_CMD(P=1)", "SEND_ACKNOWLEDGE_CMD(P=1)", "SEND_RR_RSP(F=1)",
"SEND_ACKNOWLEDGE_RSP(F=1)", "SEND_RR_XXX(X=0)", "SEND_ACKNOWLEDGE_XXX(X=0)",
"SEND_SABME_CMD(P=X)", "SEND_UA_RSP(F=X)", "S_FLAG:=0", "S_FLAG:=1", "START_P_TIMER",
"START_ACK_TIMER", "START_REJ_TIMER", "START_ACK_TIMER_IF_NOT_RUNNING",
"STOP_ACK_TIMER", "STOP_P_TIMER", "STOP_REJ_TIMER", "STOP_ALL_TIMERS",
"STOP_OTHER_TIMERS", "UPDATE_N(R)_RECEIVED", "UPDATE_P_FLAG", "DATA_FLAG:=2",
"DATA_FLAG:=0", "DATA_FLAG:=1", "IF_DATA_FLAG_=0_THEN_DATA_FLAG:=1", "P_FLAG:=0",
"P_FLAG:=P", "REMOTE_BUSY:=0", "RETRY_COUNT:=0", "RETRY_COUNT:=RETRY_COUNT+1",
"V(R):=0", "V(R):=V(R)+1", "V(S):=0", "V(S):=N(R)", "F_FLAG:=P",
"REPORT_STATUS(FRMR_RECEIVED)", "REPORT_STATUS(FRMR_SENT)",
"REPORT_STATUS(REMOTE_BUSY)", "REPORT_STATUS(REMOTE_NOT_BUSY)",
"RESET_INDICATION(LOCAL)", "RESET_INDICATION(REMOTE)"
};
