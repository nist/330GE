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
#undef A_NCR53c7xx_msg_abort
#undef A_NCR53c7xx_msg_reject
#undef A_NCR53c7xx_sink
#undef A_NCR53c7xx_zero
#undef A_NOP_insn
#undef A_addr_reconnect_dsa_head
#undef A_addr_scratch
#undef A_addr_temp
#undef A_dmode_memory_to_memory
#undef A_dmode_memory_to_ncr
#undef A_dmode_ncr_to_memory
#undef A_dsa_check_reselect
#undef A_dsa_cmdout
#undef A_dsa_cmnd
#undef A_dsa_datain
#undef A_dsa_dataout
#undef A_dsa_end
#undef A_dsa_fields_start
#undef A_dsa_msgin
#undef A_dsa_msgout
#undef A_dsa_msgout_other
#undef A_dsa_next
#undef A_dsa_restore_pointers
#undef A_dsa_save_data_pointer
#undef A_dsa_select
#undef A_dsa_status
#undef A_dsa_temp_addr_array_value
#undef A_dsa_temp_addr_dsa_value
#undef A_dsa_temp_addr_new_value
#undef A_dsa_temp_addr_next
#undef A_dsa_temp_addr_residual
#undef A_dsa_temp_addr_saved_pointer
#undef A_dsa_temp_addr_saved_residual
#undef A_dsa_temp_lun
#undef A_dsa_temp_next
#undef A_dsa_temp_sync
#undef A_dsa_temp_target
#undef A_int_debug_break
#undef A_int_debug_panic
#undef A_int_err_check_condition
#undef A_int_err_no_phase
#undef A_int_err_selected
#undef A_int_err_unexpected_phase
#undef A_int_err_unexpected_reselect
#undef A_int_msg_1
#undef A_int_msg_sdtr
#undef A_int_msg_wdtr
#undef A_int_norm_aborted
#undef A_int_norm_command_complete
#undef A_int_norm_disconnected
#undef A_int_norm_reselect_complete
#undef A_int_norm_reset
#undef A_int_norm_select_complete
#undef A_int_test_1
#undef A_int_test_2
#undef A_int_test_3
#undef A_msg_buf
#undef A_reconnect_dsa_head
#undef A_reselected_identify
#undef A_reselected_tag
#undef A_schedule
#undef A_test_dest
#undef A_test_src
#undef Ent_accept_message
#undef Ent_cmdout_cmdout
#undef Ent_command_complete
#undef Ent_command_complete_msgin
#undef Ent_data_transfer
#undef Ent_datain_to_jump
#undef Ent_debug_break
#undef Ent_dsa_code_begin
#undef Ent_dsa_code_check_reselect
#undef Ent_dsa_code_fix_jump
#undef Ent_dsa_code_restore_pointers
#undef Ent_dsa_code_save_data_pointer
#undef Ent_dsa_code_template
#undef Ent_dsa_code_template_end
#undef Ent_dsa_schedule
#undef Ent_dsa_zero
#undef Ent_end_data_transfer
#undef Ent_initiator_abort
#undef Ent_msg_in
#undef Ent_msg_in_restart
#undef Ent_other_in
#undef Ent_other_out
#undef Ent_other_transfer
#undef Ent_reject_message
#undef Ent_reselected_check_next
#undef Ent_reselected_ok
#undef Ent_respond_message
#undef Ent_select
#undef Ent_select_msgout
#undef Ent_target_abort
#undef Ent_test_1
#undef Ent_test_2
#undef Ent_test_2_msgout
#undef Ent_wait_reselect