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
/*
 * Misc utility routines used by kernel or app-level.
 * Contents are wifi-specific, used by any kernel or app-level
 * software that might want wifi things as it grows.
 *
 * Copyright 2006, ASUSTek Inc.
 * All Rights Reserved.                
 *                                     
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of ASUSTek Inc.;   
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior      
 * written permission of ASUSTek Inc..                            
 * $Id: bcmwifi.c,v 1.1.1.1 2007/01/25 12:52:12 jiahao_jhou Exp $
 */

#include <typedefs.h>

#ifdef BCMDRIVER
#include <osl.h>
#include <bcmutils.h>
#define strtoul(nptr, endptr, base) bcm_strtoul((nptr), (endptr), (base))
#define tolower(c) (bcm_isupper((c)) ? ((c) + 'a' - 'A') : (c))
#else
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#endif /* BCMDRIVER */
#include <bcmwifi.h>

/* Chanspec ASCII representation:
 * <channel><band><bandwidth><ctl-sideband>
 *   digit   [AB]      N          [UL]
 *
 * <channel>: channel number of the 10MHz or 20MHz channel,
 *	or control sideband channel of 40MHz channel.
 * <band>: A for 5GHz, B for 2.4GHz
 * <bandwidth>: N for 10MHz, nothing for 20MHz or 40MHz
 *	(ctl-sideband spec implies 40MHz)
 * <ctl-sideband>: U for upper, L for lower
 *
 * <band> may be omitted on input, and will be assumed to be
 * 2.4GHz if channel number <= 14.
 *
 * Examples: ...
 */
/* given a chanspec and a string buffer, format the chanspec as a
 * string, and return the original pointer a. On error, return's NULL
 */
char *
wf_chspec_ntoa(chanspec_t chspec, char *buf)
{
	const char *band, *bw, *sb;
	uint channel;

	bw = "";
	sb = "";
	channel = CHSPEC_CHANNEL(chspec);
	band = (CHSPEC_IS2G(chspec)) ? "b" : "a";
	if (CHSPEC_IS40(chspec)) {
		if (CHSPEC_SB_UPPER(chspec)) {
			sb = "u";
			channel += CH_10MHZ_APART;
		} else {
			sb = "l";
			channel -= CH_10MHZ_APART;
		}
	} else if (CHSPEC_IS10(chspec)) {
		bw = "n";
	}

	sprintf(buf, "%d%s%s%s", channel, band, bw, sb);
	return (buf);
}

/* given a chanspec string, convert to a chanspec.
 * On error, return 0
 */

chanspec_t
wf_chspec_aton(char *a)
{
	char *endp;
	uint channel, band, bw, ctl_sb;
	bool band_set = FALSE, bw_set = FALSE, ctl_sb_set = FALSE;
	int error = 0;

	channel = strtoul(a, &endp, 10);
	if (endp == a)
		return 0;

	if (channel > MAXCHANNEL)
		return 0;

	band = ((channel <= WLC_MAX_2G_CHANNEL) ? WL_CHANSPEC_BAND_2G : WL_CHANSPEC_BAND_5G);
	bw = WL_CHANSPEC_BW_20;
	ctl_sb = WL_CHANSPEC_CTL_SB_NONE;

	a = endp;
	while (*a != 0 && error != -1) {
		switch (tolower((int)*a)) {
			case 'a':
			case 'b':
				if (!band_set) {
					band = (tolower((int)*a) == 'a') ?
					       WL_CHANSPEC_BAND_5G : WL_CHANSPEC_BAND_2G;
					band_set = TRUE;
				} else {
					error = -1;
				}
				break;
			case 'n':
				if (!bw_set) {
					bw = WL_CHANSPEC_BW_10;
					bw_set = TRUE;
				} else {
					error = -1;
				}
				break;
			case 'l':
			case 'u':
				if (!ctl_sb_set && !bw_set) {
					ctl_sb = (tolower((int)*a) == 'l') ?
						WL_CHANSPEC_CTL_SB_LOWER : WL_CHANSPEC_CTL_SB_UPPER;
					ctl_sb_set = TRUE;
					if (ctl_sb == WL_CHANSPEC_CTL_SB_LOWER)
						channel = UPPER_20_SB(channel);
					else
						channel = LOWER_20_SB(channel);
					bw = WL_CHANSPEC_BW_40;
					bw_set = TRUE;
				} else if (bw_set) {
					error = -1;
				} else {
					error = -1;
				}
				break;
			default:
				error = -1;
				break;
		}
		a++;
	}

	if (bw_set && (bw == WL_CHANSPEC_BW_40) && !ctl_sb_set)
		error = -1;

	if (ctl_sb_set && !bw_set)
		error = -1;

	if (!error)
		return ((channel | band | bw | ctl_sb));

	return 0;
}
