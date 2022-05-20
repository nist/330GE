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
#define cons_num	(vc_cons[currcons].d->vc_num)
#define sw		(vc_cons[currcons].d->vc_sw)
#define screenbuf	(vc_cons[currcons].d->vc_screenbuf)
#define screenbuf_size	(vc_cons[currcons].d->vc_screenbuf_size)
#define origin		(vc_cons[currcons].d->vc_origin)
#define scr_top		(vc_cons[currcons].d->vc_scr_top)
#define visible_origin  (vc_cons[currcons].d->vc_visible_origin)
#define scr_end		(vc_cons[currcons].d->vc_scr_end)
#define pos		(vc_cons[currcons].d->vc_pos)
#define top		(vc_cons[currcons].d->vc_top)
#define bottom		(vc_cons[currcons].d->vc_bottom)
#define x		(vc_cons[currcons].d->vc_x)
#define y		(vc_cons[currcons].d->vc_y)
#define vc_state	(vc_cons[currcons].d->vc_state)
#define npar		(vc_cons[currcons].d->vc_npar)
#define par		(vc_cons[currcons].d->vc_par)
#define ques		(vc_cons[currcons].d->vc_ques)
#define attr		(vc_cons[currcons].d->vc_attr)
#define saved_x		(vc_cons[currcons].d->vc_saved_x)
#define saved_y		(vc_cons[currcons].d->vc_saved_y)
#define translate	(vc_cons[currcons].d->vc_translate)
#define G0_charset	(vc_cons[currcons].d->vc_G0_charset)
#define G1_charset	(vc_cons[currcons].d->vc_G1_charset)
#define saved_G0	(vc_cons[currcons].d->vc_saved_G0)
#define saved_G1	(vc_cons[currcons].d->vc_saved_G1)
#define utf		(vc_cons[currcons].d->vc_utf)
#define utf_count	(vc_cons[currcons].d->vc_utf_count)
#define utf_char	(vc_cons[currcons].d->vc_utf_char)
#define video_erase_char (vc_cons[currcons].d->vc_video_erase_char)
#define disp_ctrl	(vc_cons[currcons].d->vc_disp_ctrl)
#define toggle_meta	(vc_cons[currcons].d->vc_toggle_meta)
#define decscnm		(vc_cons[currcons].d->vc_decscnm)
#define decom		(vc_cons[currcons].d->vc_decom)
#define decawm		(vc_cons[currcons].d->vc_decawm)
#define deccm		(vc_cons[currcons].d->vc_deccm)
#define decim		(vc_cons[currcons].d->vc_decim)
#define deccolm		(vc_cons[currcons].d->vc_deccolm)
#define need_wrap	(vc_cons[currcons].d->vc_need_wrap)
#define kmalloced	(vc_cons[currcons].d->vc_kmalloced)
#define report_mouse	(vc_cons[currcons].d->vc_report_mouse)
#define color		(vc_cons[currcons].d->vc_color)
#define s_color		(vc_cons[currcons].d->vc_s_color)
#define def_color	(vc_cons[currcons].d->vc_def_color)
#define foreground	(color & 0x0f)
#define background	(color & 0xf0)
#define charset		(vc_cons[currcons].d->vc_charset)
#define s_charset	(vc_cons[currcons].d->vc_s_charset)
#define	intensity	(vc_cons[currcons].d->vc_intensity)
#define	underline	(vc_cons[currcons].d->vc_underline)
#define	blink		(vc_cons[currcons].d->vc_blink)
#define	reverse		(vc_cons[currcons].d->vc_reverse)
#define	s_intensity	(vc_cons[currcons].d->vc_s_intensity)
#define	s_underline	(vc_cons[currcons].d->vc_s_underline)
#define	s_blink		(vc_cons[currcons].d->vc_s_blink)
#define	s_reverse	(vc_cons[currcons].d->vc_s_reverse)
#define	ulcolor		(vc_cons[currcons].d->vc_ulcolor)
#define	halfcolor	(vc_cons[currcons].d->vc_halfcolor)
#define tab_stop	(vc_cons[currcons].d->vc_tab_stop)
#define palette		(vc_cons[currcons].d->vc_palette)
#define bell_pitch	(vc_cons[currcons].d->vc_bell_pitch)
#define bell_duration	(vc_cons[currcons].d->vc_bell_duration)
#define cursor_type	(vc_cons[currcons].d->vc_cursor_type)
#define display_fg	(vc_cons[currcons].d->vc_display_fg)
#define complement_mask (vc_cons[currcons].d->vc_complement_mask)
#define s_complement_mask (vc_cons[currcons].d->vc_s_complement_mask)
#define hi_font_mask	(vc_cons[currcons].d->vc_hi_font_mask)

#define vcmode		(vt_cons[currcons]->vc_mode)

#define structsize	(sizeof(struct vc_data) + sizeof(struct vt_struct))
