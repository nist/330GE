<head>
<title><#ZVMODELVZ#> Web Manager</title>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<meta http-equiv="Pragma" content="no-cache">
<link rel="stylesheet" type="text/css" href="style.css" media="screen"></link>
<script language="JavaScript" type="text/javascript" src="overlib.js"></script>
<script language="JavaScript" type="text/javascript" src="general.js"></script>

</head>  
<div id="overDiv" style="position:absolute; visibility:hidden; z-index:1000;"></div>    
<body onLoad="load_body();" onunLoad="return unload_body();">
<form method="GET" name="form" action="apply.cgi">
<!-- Table for the conntent page -->	    
<table width="666" border="0" cellpadding="0" cellspacing="0">     	      
    	
<input type="hidden" name="current_page" value="Advanced_WAdvanced_Content.asp">
<input type="hidden" name="next_page" value="Main_Operation2.asp">
<input type="hidden" name="next_host" value="">
<input type="hidden" name="sid_list" value="WLANAuthentication11a;WLANConfig11a;WLANConfig11b;Layer3Forwarding;">
<input type="hidden" name="group_id" value="">
<input type="hidden" name="modified" value="0">
<input type="hidden" name="action_mode" value="">
<input type="hidden" name="first_time" value="">
<input type="hidden" name="action_script" value="">
<!--
<input type="hidden" name="wl_afterburner" value="<% nvram_get_x("WLANConfig11b","wl_afterburner"); %>">
-->
<input type="hidden" name="preferred_lang" value="<% nvram_get_x("","preferred_lang"); %>">
<input type="hidden" name="wl_ssid_pri" value="<% nvram_get_x("WLANConfig11b","wl_ssid2"); %>">
<input type="hidden" name="wl_guest_enable" value="<% nvram_get_x("WLANConfig11b","wl_guest_enable"); %>">
<input type="hidden" name="wl_auth_mode" value="<% nvram_get_x("WLANConfig11b","wl_auth_mode"); %>">
<input type="hidden" name="wan_route_mode" value="<% nvram_get_x("","wan_route_x"); %>">
<input type="hidden" name="wl_guest_ssid_2" value="<% nvram_get_x("WLANConfig11b","wl_guest_ssid_2"); %>">
<input type="hidden" name="wl_crypto" value="<% nvram_get_x("WLANConfig11b","wl_crypto"); %>">
<input type="hidden" name="wan_proto" value="<% nvram_get_x("Layer3Forwarding","wan_proto"); %>">
<input type="hidden" name="wan_nat" value="<% nvram_get_x("Layer3Forwarding","wan_nat_x"); %>">
<!--
<input type="hidden" name="wl_nat" value="<% nvram_get_x("WLANConfig11b","wl_nat"); %>">
-->
<input type="hidden" name="wl_guest_wpa_mode" value="<% nvram_get_x("WLANConfig11b","wl_guest_wpa_mode"); %>">

<input type="hidden" name="wl_guest_wpa_psk_1_org" value="<% nvram_char_to_ascii("WLANConfig11b","wl_guest_wpa_psk_1"); %>">
<input type="hidden" name="wl_guest_phrase_x_1_org" value="<% nvram_char_to_ascii("WLANConfig11b","wl_guest_phrase_x_1"); %>">
<input type="hidden" name="wl_guest_key1_1_org" value="<% nvram_char_to_ascii("WLANConfig11b","wl_guest_key1_1"); %>">
<input type="hidden" name="wl_guest_key2_1_org" value="<% nvram_char_to_ascii("WLANConfig11b","wl_guest_key2_1"); %>">
<input type="hidden" name="wl_guest_key3_1_org" value="<% nvram_char_to_ascii("WLANConfig11b","wl_guest_key3_1"); %>">
<input type="hidden" name="wl_guest_key4_1_org" value="<% nvram_char_to_ascii("WLANConfig11b","wl_guest_key4_1"); %>">

<tr>
<td>
<table width="666" border="1" cellpadding="0" cellspacing="0" bordercolor="E0E0E0">
<tr class="content_header_tr">
<td class="content_header_td_title" colspan="2"><#t1Wireless#> - <#t2Advanced#></td>
</tr>
<input type="hidden" name="wl_gmode" value="<% nvram_get_x("WLANConfig11b","wl_gmode"); %>">
<input type="hidden" name="wl_gmode_protection_x" value="<% nvram_get_x("WLANConfig11b","wl_gmode_protection_x"); %>">
<tr>
<td class="content_desc_td" colspan="2"><#WLANConfig11b_display5_sectiondesc#>
         </td>
</tr>
<!--
<tr>
	<td class="content_header_td" onMouseOver="return overlib('<#WLANConfig11b_x_RadioPowerEx_itemdesc#>', LEFT);" onMouseOut="return nd();"><#WLANConfig11b_x_RadioPower_itemname#></td>
	<td class="content_input_td">        
    		<div id="sliderDiv" >
		</div>
		<div id="sliderVal" align="right">
			<input type="text"  size="20" style="font-size:12px;" name="power_index_1" readonly="1">
		</div>
		<input type="hidden" maxlength="5" size="5" name="wl_radio_power_x" class="content_input_fd" value="<% nvram_get_x("WLANConfig11b", "wl_radio_power_x"); %>"></td>
		<input type="hidden" maxlength="5" size="5" name="regulation_domain" class="content_input_fd" value="<% nvram_get_x("WLANConfig11b", "regulation_domain"); %>"></td>
	</td>
</tr>
-->
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#WLANConfig11b_x_AfterBurner_itemdesc#>', LEFT);" onMouseOut="return nd();"><#WLANConfig11b_x_AfterBurner_itemname#>
           </td><td class="content_input_td"><select name="wl_afterburner" class="content_input_fd" onChange="return change_common(this, 'WLANConfig11b', 'wl_afterburner')"><option class="content_input_fd" value="off" <% nvram_match_x("WLANConfig11b","wl_afterburner", "off","selected"); %>>Disabled</option><option class="content_input_fd" value="auto" <% nvram_match_x("WLANConfig11b","wl_afterburner", "auto","selected"); %>>Enabled</option></select></td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#WLANConfig11b_x_BlockBCSSID_itemdesc#>', LEFT);" onMouseOut="return nd();"><#WLANConfig11b_x_BlockBCSSID_itemname#>
           </td><td class="content_input_td"><input type="radio" value="1" name="wl_closed" class="content_input_fd" onClick="return change_common_radio(this, 'WLANConfig11b', 'wl_closed', '1')" <% nvram_match_x("WLANConfig11b","wl_closed", "1", "checked"); %>><#RADIO_TEXT_YES#></input><input type="radio" value="0" name="wl_closed" class="content_input_fd" onClick="return change_common_radio(this, 'WLANConfig11b', 'wl_closed', '0')" <% nvram_match_x("WLANConfig11b","wl_closed", "0", "checked"); %>><#RADIO_TEXT_NO#></input></td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#WLANConfig11b_x_IsolateAP_itemdesc#>', LEFT);" onMouseOut="return nd();"><#WLANConfig11b_x_IsolateAP_itemname#>
           </td><td class="content_input_td"><input type="radio" value="1" name="wl_ap_isolate" class="content_input_fd" onClick="return change_common_radio(this, 'WLANConfig11b', 'wl_ap_isolate', '1')" <% nvram_match_x("WLANConfig11b","wl_ap_isolate", "1", "checked"); %>><#RADIO_TEXT_YES#></input><input type="radio" value="0" name="wl_ap_isolate" class="content_input_fd" onClick="return change_common_radio(this, 'WLANConfig11b', 'wl_ap_isolate', '0')" <% nvram_match_x("WLANConfig11b","wl_ap_isolate", "0", "checked"); %>><#RADIO_TEXT_NO#></input></td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#WLANConfig11b_DataRateAll_itemdesc#>', LEFT);" onMouseOut="return nd();"><#WLANConfig11b_DataRateAll_itemname#>
           </td><td class="content_input_td"><select name="wl_rate" class="content_input_fd" onChange="return change_common(this, 'WLANConfig11b', 'wl_rate')"><option class="content_input_fd" value="0" <% nvram_match_x("WLANConfig11b","wl_rate", "0","selected"); %>>Auto</option><option class="content_input_fd" value="1000000" <% nvram_match_x("WLANConfig11b","wl_rate", "1000000","selected"); %>>1</option><option class="content_input_fd" value="2000000" <% nvram_match_x("WLANConfig11b","wl_rate", "2000000","selected"); %>>2</option><option class="content_input_fd" value="5500000" <% nvram_match_x("WLANConfig11b","wl_rate", "5500000","selected"); %>>5.5</option><option class="content_input_fd" value="6000000" <% nvram_match_x("WLANConfig11b","wl_rate", "6000000","selected"); %>>6</option><option class="content_input_fd" value="9000000" <% nvram_match_x("WLANConfig11b","wl_rate", "9000000","selected"); %>>9</option><option class="content_input_fd" value="11000000" <% nvram_match_x("WLANConfig11b","wl_rate", "11000000","selected"); %>>11</option><option class="content_input_fd" value="12000000" <% nvram_match_x("WLANConfig11b","wl_rate", "12000000","selected"); %>>12</option><option class="content_input_fd" value="18000000" <% nvram_match_x("WLANConfig11b","wl_rate", "18000000","selected"); %>>18</option><option class="content_input_fd" value="24000000" <% nvram_match_x("WLANConfig11b","wl_rate", "24000000","selected"); %>>24</option><option class="content_input_fd" value="36000000" <% nvram_match_x("WLANConfig11b","wl_rate", "36000000","selected"); %>>36</option><option class="content_input_fd" value="48000000" <% nvram_match_x("WLANConfig11b","wl_rate", "48000000","selected"); %>>48</option><option class="content_input_fd" value="54000000" <% nvram_match_x("WLANConfig11b","wl_rate", "54000000","selected"); %>>54</option></select></td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#WLANConfig11b_DataRate_itemdesc#>', LEFT);" onMouseOut="return nd();"><#WLANConfig11b_DataRate_itemname#>
           </td><td class="content_input_td"><select name="wl_rateset" class="content_input_fd" onChange="return change_common(this, 'WLANConfig11b', 'wl_rateset')"><option class="content_input_fd" value="default" <% nvram_match_x("WLANConfig11b","wl_rateset", "default","selected"); %>>Default</option><option class="content_input_fd" value="all" <% nvram_match_x("WLANConfig11b","wl_rateset", "all","selected"); %>>All</option><option class="content_input_fd" value="12" <% nvram_match_x("WLANConfig11b","wl_rateset", "12","selected"); %>>1, 2 Mbps</option></select></td>
</tr>
<tr>
<td class="content_header_td"><#WLANConfig11b_x_Frag_itemname#>
           </td><td class="content_input_td"><input type="text" maxlength="5" size="5" name="wl_frag" class="content_input_fd" value="<% nvram_get_x("WLANConfig11b", "wl_frag"); %>" onBlur="validate_range(this, 256, 2346)" onChange="page_changed()" onKeyPress="return is_number(this)"></td>
</tr>
<tr>
<td class="content_header_td"><#WLANConfig11b_x_RTS_itemname#>
           </td><td class="content_input_td"><input type="text" maxlength="5" size="5" name="wl_rts" class="content_input_fd" value="<% nvram_get_x("WLANConfig11b", "wl_rts"); %>" onBlur="validate_range(this, 0, 2347)" onChange="page_changed()" onKeyPress="return is_number(this)"></td>
</tr>
<tr>
<td class="content_header_td"><#WLANConfig11b_x_DTIM_itemname#>
           </td><td class="content_input_td"><input type="text" maxlength="5" size="5" name="wl_dtim" class="content_input_fd" value="<% nvram_get_x("WLANConfig11b", "wl_dtim"); %>" onBlur="validate_range(this, 1, 255)" onChange="page_changed()" onKeyPress="return is_number(this)"></td>
</tr>
<tr>
<td class="content_header_td"><#WLANConfig11b_x_Beacon_itemname#>
           </td><td class="content_input_td"><input type="text" maxlength="5" size="5" name="wl_bcn" class="content_input_fd" value="<% nvram_get_x("WLANConfig11b", "wl_bcn"); %>" onBlur="validate_range(this, 1, 65535)" onChange="page_changed()" onKeyPress="return is_number(this)"></td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#WLANConfig11b_x_FrameBurst_itemdesc#>', LEFT);" onMouseOut="return nd();"><#WLANConfig11b_x_FrameBurst_itemname#>
           </td><td class="content_input_td"><select name="wl_frameburst" class="content_input_fd" onChange="return change_common(this, 'WLANConfig11b', 'wl_frameburst')"><option class="content_input_fd" value="off" <% nvram_match_x("WLANConfig11b","wl_frameburst", "off","selected"); %>>Disabled</option><option class="content_input_fd" value="on" <% nvram_match_x("WLANConfig11b","wl_frameburst", "on","selected"); %>>Enabled</option></select></td>
</tr>
<!--
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#WLANConfig11b_x_RadioEnable_itemdesc#>', LEFT);" onMouseOut="return nd();"><#WLANConfig11b_x_RadioEnable_itemname#>
           </td><td class="content_input_td"><input type="radio" value="1" name="wl_radio_x" class="content_input_fd" onClick="return change_common_radio(this, 'WLANConfig11b', 'wl_radio_x', '1')" <% nvram_match_x("WLANConfig11b","wl_radio_x", "1", "checked"); %>><#RADIO_TEXT_YES#></input><input type="radio" value="0" name="wl_radio_x" class="content_input_fd" onClick="return change_common_radio(this, 'WLANConfig11b', 'wl_radio_x', '0')" <% nvram_match_x("WLANConfig11b","wl_radio_x", "0", "checked"); %>><#RADIO_TEXT_NO#></input></td>
</tr>
-->
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#WLANConfig11b_x_WMM_itemdesc#>', LEFT);" onMouseOut="return nd();"><#WLANConfig11b_x_WMM_itemname#>
           </td><td class="content_input_td"><select name="wl_wme" class="content_input_fd" onChange="return change_common(this, 'WLANConfig11b', 'wl_wme')"><option class="content_input_fd" value="off" <% nvram_match_x("WLANConfig11b","wl_wme", "off","selected"); %>>Disabled</option><option class="content_input_fd" value="on" <% nvram_match_x("WLANConfig11b","wl_wme", "on","selected"); %>>Enabled</option></select></td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#WLANConfig11b_x_NOACK_itemdesc#>', LEFT);" onMouseOut="return nd();"><#WLANConfig11b_x_NOACK_itemname#>
           </td><td class="content_input_td"><select name="wl_wme_no_ack" class="content_input_fd" onChange="return change_common(this, 'WLANConfig11b', 'wl_wme_no_ack')"><option class="content_input_fd" value="off" <% nvram_match_x("WLANConfig11b","wl_wme_no_ack", "off","selected"); %>>Disabled</option><option class="content_input_fd" value="on" <% nvram_match_x("WLANConfig11b","wl_wme_no_ack", "on","selected"); %>>Enabled</option></select></td>
</tr>
<tr>
	<td class="content_header_td" onMouseOver="return overlib('<#WLANConfig11b_x_ExtendMode_itemdesc#>', LEFT);" onMouseOut="return nd();"><#WLANConfig11b_x_ExtendMode_itemname#></td>
	<td class="content_input_td">
		<select name="wl_mode_ex" class="content_input_fd" onChange="return change_common(this, 'WLANConfig11b', 'wl_mode_ex')">
			<option class="content_input_fd" value="ap" <% nvram_match_x("WLANConfig11b","wl_mode_ex", "ap","selected"); %>>AP</option>
			<option class="content_input_fd" value="sta" <% nvram_match_x("WLANConfig11b","wl_mode_ex", "sta","selected"); %>>Adapter</option>
			<option class="content_input_fd" value="re" <% nvram_match_x("WLANConfig11b","wl_mode_ex", "re","selected"); %>>Repeater</option>
		</select>
	</td>
</tr>

<tr class="content_section_header_tr">
<td class="content_section_header_td" colspan="2"><#WLANConfig11b_display6_sectionname#></td>
</tr>
<tr>
<td class="content_desc_td" colspan="2"><#WLANConfig11b_display6_sectiondesc#></td>
</tr>
<tr>
	<td class="content_header_td" onMouseOver="return overlib('<#WLANConfig11b_IndividualURESetting_itemdesc#>', LEFT);" onMouseOut="return nd();"><#WLANConfig11b_IndividualURESetting_itemname#></td>
	<td class="content_input_td">
		<input type="radio" value="1" name="wl_individual_ure" class="content_input_fd" onClick="return change_common_radio(this, 'WLANConfig11b', 'wl_individual_ure', '1')" <% nvram_match_x("WLANConfig11b","wl_individual_ure", "1", "checked"); %>><#RADIO_TEXT_YES#></Input>
		<input type="radio" value="0" name="wl_individual_ure" class="content_input_fd" onClick="return change_common_radio(this, 'WLANConfig11b', 'wl_individual_ure', '0')" <% nvram_match_x("WLANConfig11b","wl_individual_ure", "0", "checked"); %>><#RADIO_TEXT_NO#></Input>
	</td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#WLANConfig11b_DSSID_itemdesc#>', LEFT);" onMouseOut="return nd();"><#WLANConfig11b_GSSID_itemname#>
           </td><td class="content_input_td"><input type="text" maxlength="32" class="content_input_fd" size="32" name="wl_guest_ssid_1" value="<% nvram_get_x("WLANConfig11b","wl_guest_ssid_1"); %>" onChange="page_changed()" onKeyPress="return is_string(this)" onBlur="validate_string4(this)"></td>
</tr>

<tr>
	<td class="content_header_td" onMouseOver="return overlib('<#WLANConfig11b_GAuthenticationMethod_itemdesc#>', LEFT);" onMouseOut="return nd();"><#WLANConfig11b_GAuthenticationMethod_itemname#></td>
	<td class="content_input_td">
		<select name="wl_guest_auth_mode_1" class="content_input_fd" onChange="return change_common(this, 'WLANConfig11b', 'wl_guest_auth_mode_1')">
			<option class="content_input_fd" value="open" <% nvram_match_x("WLANConfig11b","wl_guest_auth_mode_1", "open","selected"); %>>Open System</option>
			<option class="content_input_fd" value="shared" <% nvram_match_x("WLANConfig11b","wl_guest_auth_mode_1", "shared","selected"); %>>Shared Key</option>
			<option class="content_input_fd" value="psk" <% nvram_double_match_x("WLANConfig11b", "wl_guest_auth_mode_1", "psk", "WLANConfig11b", "wl_guest_wpa_mode", "1", "selected"); %>>WPA-Personal</option>
			<option class="content_input_fd" value="psk" <% nvram_double_match_x("WLANConfig11b", "wl_guest_auth_mode_1", "psk", "WLANConfig11b", "wl_guest_wpa_mode", "2", "selected"); %>>WPA2-Personal</option>
			<option class="content_input_fd" value="psk" <% nvram_double_match_x("WLANConfig11b", "wl_guest_auth_mode_1", "psk", "WLANConfig11b", "wl_guest_wpa_mode", "0", "selected"); %>>WPA-Auto-Personal</option>
		</select>
	</td>
</tr>

<tr>
<td class="content_header_td" onMouseOver="return overlib('<#WLANConfig11b_GWPAType_itemdesc#>', LEFT);" onMouseOut="return nd();"><#WLANConfig11b_GWPAType_itemname#>
           </td><td class="content_input_td"><select name="wl_guest_crypto_1" class="content_input_fd" onChange="return change_common(this, 'WLANConfig11b', 'wl_guest_crypto_1')"><option class="content_input_fd" value="tkip" <% nvram_match_x("WLANConfig11b","wl_guest_crypto_1", "tkip","selected"); %>>TKIP</option><option class="content_input_fd" value="aes" <% nvram_match_x("WLANConfig11b","wl_guest_crypto_1", "aes","selected"); %>>AES</option><option class="content_input_fd" value="tkip+aes" <% nvram_match_x("WLANConfig11b","wl_guest_crypto_1", "tkip+aes","selected"); %>>TKIP+AES</option></select></td>
</tr>

<tr>
	<td class="content_header_td" onMouseOver="return overlib('<#WLANConfig11b_Gx_PSKKey_itemdesc#>', LEFT);" onMouseOut="return nd();"><#WLANConfig11b_Gx_PSKKey_itemname#></td>
	<td class="content_input_td">
		<script language="JavaScript" type="text/javascript" src="md5.js"></script>
		<input type="text" maxlength="64" class="content_input_fd" size="32" name="wl_guest_wpa_psk_1" value="<% nvram_get_x("WLANConfig11b","wl_guest_wpa_psk_1"); %>" onKeyUp="return is_wlphrase('WLANConfig11b', 'wl_guest_wpa_psk_1', this)" onBlur="return validate_wlphrase('WLANConfig11b', 'wl_guest_wpa_psk_1', this)">
	</td>
</tr>

<tr>
<td class="content_header_td" onMouseOver="return overlib('<#WLANConfig11b_GWEPType_itemdesc#>', LEFT);" onMouseOut="return nd();"><#WLANConfig11b_GWEPType_itemname#>
           </td><td class="content_input_td"><select name="wl_guest_wep_x_1" class="content_input_fd" onChange="return change_common(this, 'WLANConfig11b', 'wl_guest_wep_x_1')"><option class="content_input_fd" value="0" <% nvram_match_x("WLANConfig11b","wl_guest_wep_x_1", "0","selected"); %>>None</option><option class="content_input_fd" value="1" <% nvram_match_x("WLANConfig11b","wl_guest_wep_x_1", "1","selected"); %>>WEP-64bits</option><option class="content_input_fd" value="2" <% nvram_match_x("WLANConfig11b","wl_guest_wep_x_1", "2","selected"); %>>WEP-128bits</option></select></td>
</tr>
<tr>
	<td class="content_header_td" onMouseOver="return overlib('<#WLANConfig11b_WEPKeyType_itemdesc#>', LEFT);" onMouseOut="return nd();"><#WLANConfig11b_WEPKeyType_itemname#></td>
	<td class="content_input_td">
		<select name="wl_guest_key_type_1" class="content_input_fd" onChange="return change_common(this, 'WLANConfig11b', 'wl_guest_key_type_1')">
			<option class="content_input_fd" value="0" <% nvram_match_x("WLANConfig11b","wl_guest_key_type_1", "0","selected"); %>>HEX</option>
			<option class="content_input_fd" value="1" <% nvram_match_x("WLANConfig11b","wl_guest_key_type_1", "1","selected"); %>>ASCII</option>
		</select>
	</td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#WLANConfig11b_Gx_Phrase_itemdesc#>', LEFT);" onMouseOut="return nd();"><#WLANConfig11b_Gx_Phrase_itemname#>
           </td><td class="content_input_td"><script language="JavaScript" type="text/javascript" src="md5.js"></script><input type="text" maxlength="64" class="content_input_fd" size="32" name="wl_guest_phrase_x_1" value="<% nvram_get_x("WLANConfig11b","wl_guest_phrase_x_1"); %>" onKeyUp="return is_wlphrase('WLANConfig11b', 'wl_guest_phrase_x_1', this)" onChange="return is_wlphrase('WLANConfig11b', 'wl_guest_phrase_x_1', this)" onBlur="return validate_wlphrase('WLANConfig11b', 'wl_guest_phrase_x_1', this)"></td>
</tr>
<tr>
<td class="content_header_td"><#WLANConfig11b_GWEPKey1_itemname#>
           </td><td class="content_input_td"><input type="text" maxlength="32" class="content_input_fd" size="32" name="wl_guest_key1_1" value="<% nvram_get_x("WLANConfig11b","wl_guest_key1_1"); %>" onBlur="return validate_wlkey(this, 'WLANConfig11b')" onKeyPress="return is_wlkey(this, 'WLANConfig11b')" onKeyUp="return change_wlkey(this, 'WLANConfig11b')"></td>
</tr>
<tr>
<td class="content_header_td"><#WLANConfig11b_GWEPKey2_itemname#>
           </td><td class="content_input_td"><input type="text" maxlength="32" class="content_input_fd" size="32" name="wl_guest_key2_1" value="<% nvram_get_x("WLANConfig11b","wl_guest_key2_1"); %>" onBlur="return validate_wlkey(this, 'WLANConfig11b')" onKeyPress="return is_wlkey(this, 'WLANConfig11b')" onKeyUp="return change_wlkey(this, 'WLANConfig11b')"></td>
</tr>
<tr>
<td class="content_header_td"><#WLANConfig11b_GWEPKey3_itemname#>
           </td><td class="content_input_td"><input type="text" maxlength="32" class="content_input_fd" size="32" name="wl_guest_key3_1" value="<% nvram_get_x("WLANConfig11b","wl_guest_key3_1"); %>" onBlur="return validate_wlkey(this, 'WLANConfig11b')" onKeyPress="return is_wlkey(this, 'WLANConfig11b')" onKeyUp="return change_wlkey(this, 'WLANConfig11b')"></td>
</tr>
<tr>
<td class="content_header_td"><#WLANConfig11b_GWEPKey4_itemname#>
           </td><td class="content_input_td"><input type="text" maxlength="32" class="content_input_fd" size="32" name="wl_guest_key4_1" value="<% nvram_get_x("WLANConfig11b","wl_guest_key4_1"); %>" onBlur="return validate_wlkey(this, 'WLANConfig11b')" onKeyPress="return is_wlkey(this, 'WLANConfig11b')" onKeyUp="return change_wlkey(this, 'WLANConfig11b')"></td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#WLANConfig11b_GWEPDefaultKey_itemdesc#>', LEFT);" onMouseOut="return nd();"><#WLANConfig11b_GWEPDefaultKey_itemname#>
           </td><td class="content_input_td"><select name="wl_guest_key_1" class="content_input_fd" onChange="return change_common(this, 'WLANConfig11b', 'wl_guest_key_1')"><option class="content_input_fd" value="1" <% nvram_match_x("WLANConfig11b","wl_guest_key_1", "1","selected"); %>>Key1</option><option class="content_input_fd" value="2" <% nvram_match_x("WLANConfig11b","wl_guest_key_1", "2","selected"); %>>Key2</option><option class="content_input_fd" value="3" <% nvram_match_x("WLANConfig11b","wl_guest_key_1", "3","selected"); %>>Key3</option><option class="content_input_fd" value="4" <% nvram_match_x("WLANConfig11b","wl_guest_key_1", "4","selected"); %>>Key4</option></select></td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#WLANConfig11b_Gx_Rekey_itemdesc#>', LEFT);" onMouseOut="return nd();"><#WLANConfig11b_Gx_Rekey_itemname#>
           </td><td class="content_input_td"><input type="text" maxlength="5" size="5" name="wl_guest_wpa_gtk_rekey_1" class="content_input_fd" value="<% nvram_get_x("WLANConfig11b", "wl_guest_wpa_gtk_rekey_1"); %>" onBlur="validate_range(this, 0, 86400)" onKeyPress="return is_number(this)"></td>
</tr>
</table>
</td>
</tr>

<tr>
<td>
<table width="666" border="2" cellpadding="0" cellspacing="0" bordercolor="E0E0E0"></table>
</td>
</tr>
<tr>
<td>		
<table width="666" border="1" cellpadding="0" cellspacing="0" bordercolor="B0B0B0">
<tr bgcolor="#CCCCCC"><td colspan="3"><font face="arial" size="2"><b>&nbsp</b></font></td></tr>
<tr bgcolor="#FFFFFF">  
   <td id ="Confirm" height="25" width="34%">  
   <div align="center"><font face="Arial"> <input class=inputSubmit style="cursor:pointer;" style="cursor:pointer;" style="cursor:pointer;" onMouseOut=buttonOut(this) onMouseOver="buttonOver(this)" onClick="return onSubmitCtrl(this, ' Restore ')" type="submit" value=" <#CTL_restore#> " name="action"></font></div> 
   </td>  
   <td height="25" width="33%">  
   <div align="center"><font face="Arial"> <input class=inputSubmit style="cursor:pointer;" style="cursor:pointer;" style="cursor:pointer;" onMouseOut=buttonOut(this) onMouseOver="buttonOver(this)" onClick="return onSubmitCtrl(this, ' Finish ')" type="submit" value=" <#CTL_finish#> " name="action"></font></div> 
   </td>
   <td height="25" width="33%">  
   <div align="center"><font face="Arial"> <input class=inputSubmit style="cursor:pointer;" style="cursor:pointer;" style="cursor:pointer;" onMouseOut=buttonOut(this) onMouseOver="buttonOver(this)" onClick="return onSubmitCtrl(this, ' Apply ')" type="submit" value=" <#CTL_apply#> " name="action"></font></div> 
   </td>    
</tr>
</table>
</td>
</tr>

<tr>
<td>
<table width="666" border="1" cellpadding="0" cellspacing="0" bordercolor="B0B0B0">
<tr>
    <td colspan="2" width="616" height="25" bgcolor="#FFBB00"></td> 
</tr>                   
<tr bgcolor="#FFFFFF">
    <td class="content_header_td_15" align="left"><#CTL_restore#>: </td>
    <td class="content_input_td_padding" align="left"><#CTL_desc_restore#></td>
</tr>
<tr bgcolor="#FFFFFF">
    <td class="content_header_td_15" align="left"><#CTL_finish#>: </td>
    <td class="content_input_td_padding" align="left"><#CTL_desc_finish#></td>
</tr>
<tr bgcolor="#FFFFFF">
    <td class="content_header_td_15" align="left"><#CTL_apply#>: </td>
    <td class="content_input_td_padding" align="left"><#CTL_desc_apply#></td>
</tr>
</table>
</td>
</tr>

</table>
</form>
</body>
