<head>
<title><#ZVMODELVZ#> Web Manager</title>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<meta http-equiv="Pragma" content="no-cache">
<link rel="stylesheet" type="text/css" href="style.css" media="screen"></link>
<script language="JavaScript" type="text/javascript" src="overlib.js"></script>
<script language="JavaScript" type="text/javascript" src="general.js"></script>
<script language="JavaScript">
var subWin=null;
var subWin2=null;
function kill_subWin()
{if (subWin!=null)
subWin.close();
if (subWin2!=null)
subWin2.close();
}
</script>
</head>
<div id="overDiv" style="position:absolute; visibility:hidden; z-index:1000;"></div>
<body onLoad="load_body()" onunLoad="kill_subWin();return unload_body();">
<form method="GET" name="form" action="apply.cgi">
<!-- Table for the conntent page -->
<table width="666" border="0" cellpadding="0" cellspacing="0">
<input type="hidden" name="current_page" value="Advanced_Wireless_Content.asp">
<input type="hidden" name="next_page" value="Advanced_WMode_Content.asp">
<input type="hidden" name="next_host" value="">
<input type="hidden" name="sid_list" value="WLANConfig11b;">
<input type="hidden" name="group_id" value="">
<input type="hidden" name="modified" value="0">
<input type="hidden" name="action_mode" value="">
<input type="hidden" name="preferred_lang" value="<% nvram_get_x("","preferred_lang"); %>">
<input type="hidden" name="exmode" value="<% nvram_get_x("WLANConfig11b","wl_mode_ex"); %>">
<input type="hidden" name="wan_route_mode" value="<% nvram_get_x("","wan_route_x"); %>">
<input type="hidden" name="wl_gmode_protection_x" value="<% nvram_get_x("WLANConfig11b","wl_gmode_protection_x"); %>">
<!--
<input type="hidden" name="mssid_ssid_x0" value="<% nvram_get_x("WLANConfig11b","mssid_ssid_x0"); %>">
<input type="hidden" name="mssid_ssid_2_x0" value="<% nvram_get_x("WLANConfig11b","mssid_ssid_2_x0"); %>">
<input type="hidden" name="mssid_ssid_x1" value="<% nvram_get_x("WLANConfig11b","mssid_ssid_x1"); %>">
<input type="hidden" name="mssid_ssid_x2" value="<% nvram_get_x("WLANConfig11b","mssid_ssid_x2"); %>">
<input type="hidden" name="mssid_ssid_x3" value="<% nvram_get_x("WLANConfig11b","mssid_ssid_x3"); %>">
-->
<input type="hidden" name="wl_ssid_org" value="<% nvram_get_x("WLANConfig11b","wl_ssid"); %>">
<input type="hidden" name="wl_ssid2" value="<% nvram_get_x("WLANConfig11b","wl_ssid2"); %>">
<input type="hidden" name="wl_guest_ssid_2" value="<% nvram_get_x("WLANConfig11b","wl_guest_ssid_2"); %>">
<input type="hidden" name="pf1_ssid2" value="<% nvram_get_x("","pf1_ssid2"); %>">
<input type="hidden" name="pf2_ssid2" value="<% nvram_get_x("","pf2_ssid2"); %>">
<input type="hidden" name="pf3_ssid2" value="<% nvram_get_x("","pf3_ssid2"); %>">
<input type="hidden" name="pf4_ssid2" value="<% nvram_get_x("","pf4_ssid2"); %>">
<input type="hidden" name="wl_wpa_gtk_rekey" value="<% nvram_get_x("WLANConfig11b", "wl_wpa_gtk_rekey"); %>">
<input type="hidden" name="wl_wpa_mode" value="<% nvram_get_x("WLANConfig11b","wl_wpa_mode"); %>">
<input type="hidden" name="wl_wpa_psk_org" value="<% nvram_char_to_ascii("WLANConfig11b","wl_wpa_psk"); %>">
<input type="hidden" name="wl_phrase_x_org" value="<% nvram_char_to_ascii("WLANConfig11b","wl_phrase_x"); %>">
<input type="hidden" name="wl_key1_org" value="<% nvram_char_to_ascii("WLANConfig11b","wl_key1"); %>">
<input type="hidden" name="wl_key2_org" value="<% nvram_char_to_ascii("WLANConfig11b","wl_key2"); %>">
<input type="hidden" name="wl_key3_org" value="<% nvram_char_to_ascii("WLANConfig11b","wl_key3"); %>">
<input type="hidden" name="wl_key4_org" value="<% nvram_char_to_ascii("WLANConfig11b","wl_key4"); %>">
<tr>
<td>
<table width="666" border="1" cellpadding="0" cellspacing="0" bordercolor="E0E0E0">
<tr class="content_header_tr">
<td class="content_header_td_title" colspan="2"><#t1Wireless#> - <#t2IF#>&nbsp;&nbsp;&nbsp;&nbsp;<a href="javascript:document.form.stastatus.click()" class="content_input_link"><#WLANConfig11b_x_STAStatus_linkname#></a></td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#WLANConfig11b_x_Mode11g_itemdesc#>', LEFT);" onMouseOut="return nd();"><#WLANConfig11b_x_Mode11g_itemname#></td>
<td class="content_input_td">
<select name="wl_gmode" class="content_input_fd" onChange="return change_common(this, 'WLANConfig11b', 'wl_gmode')">
<option class="content_input_fd" value="1" <% nvram_match_x("WLANConfig11b","wl_gmode", "1","selected"); %>><#OPTION_AUTO#></option>
<option class="content_input_fd" value="4" <% nvram_match_x("WLANConfig11b","wl_gmode", "4","selected"); %>>54G Only</option>
<option class="content_input_fd" value="0" <% nvram_match_x("WLANConfig11b","wl_gmode", "0","selected"); %>>802.11B Only</option>
</select>
<input type="checkbox" style="margin-left:30" name="wl_gmode_check" value="" onClick="return change_common_radio(this, 'WLANConfig11b', 'wl_gmode_check', '1')">54g Protection</input>
</td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#WLANConfig11b_x_ConnectToSelected_itemdesc#>', LEFT);" onMouseOut="return nd();"><#WLANConfig11b_x_ConnectToSelected_itemname#></td>
<td class="content_input_td">
<input type="radio" value="1" name="wl_connect_selected" class="content_input_fd" onClick="return change_common_radio(this, 'WLANConfig11b', 'wl_connect_selected', '1')" <% nvram_match_x("WLANConfig11b","wl_connect_selected", "1", "checked"); %>><#RADIO_TEXT_YES#></Input>
<input type="radio" value="0" name="wl_connect_selected" class="content_input_fd" onClick="return change_common_radio(this, 'WLANConfig11b', 'wl_connect_selected', '0')" <% nvram_match_x("WLANConfig11b","wl_connect_selected", "0", "checked"); %>><#RADIO_TEXT_NO#></Input>
</td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#WLANConfig11b_x_APIndex_itemdesc#>', LEFT);" onMouseOut="return nd();"><#WLANConfig11b_x_APIndex_itemname#></td>
<td class="content_input_td">
<select name="wl_ap_ssid" class="content_input_fd" onChange="return change_common(this, 'WLANConfig11b', 'wl_ap_ssid')">
<% select_profile("WLANConfig11b"); %>
</select>
</td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#WLANConfig11b_APOrder_itemdesc#>', LEFT);" onMouseOut="return nd();"><#WLANConfig11b_APOrder_itemname#></td>
<td class="content_input_td">
<select name="wl_ap_order" class="content_input_fd" onChange="return change_common(this, 'WLANConfig11b', 'wl_ap_order')">
<option class="content_input_fd" value="0" <% nvram_match_x("WLANConfig11b","wl_ap_order", "0","selected"); %>>Index</option>
<option class="content_input_fd" value="1" <% nvram_match_x("WLANConfig11b","wl_ap_order", "1","selected"); %>>RSSI</option>
</select>
</td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#WLANConfig11b_x_AutoConnectToNoneDefault_itemdesc#>', LEFT);" onMouseOut="return nd();"><#WLANConfig11b_x_AutoConnectToNoneDefault_itemname#></td>
<td class="content_input_td">
<input type="radio" value="1" name="wl_connect_none_default" class="content_input_fd" onClick="return change_common_radio(this, 'WLANConfig11b', 'wl_connect_none_default', '1')" <% nvram_match_x("WLANConfig11b","wl_connect_none_default", "1", "checked"); %>><#RADIO_TEXT_YES#></Input>
<input type="radio" value="0" name="wl_connect_none_default" class="content_input_fd" onClick="return change_common_radio(this, 'WLANConfig11b', 'wl_connect_none_default', '0')" <% nvram_match_x("WLANConfig11b","wl_connect_none_default", "0", "checked"); %>><#RADIO_TEXT_NO#></Input>
</td>
</tr>
<tr class="content_section_header_tr">
<td class="content_section_header_td" colspan="2"><#WLANConfig11b_x_WirelessProfile_sectionname#></td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#WLANConfig11b_SSID_itemdesc#>', LEFT);" onMouseOut="return nd();"><#WLANConfig11b_SSID_itemname#>
<div id="hidden0" style="display:none">
<input type="button" style="cursor:pointer;" class="content_input_fd_ro" onClick="subWin2=window.open('Advanced_WirelessSTAStatus_Content.asp','SS','width=346,height=138')" value="" name="stastatus">
</div>
</td>
<td class="content_input_td">
<input type="text" maxlength="32" class="content_input_fd" size="30" name="wl_ssid" value="<% nvram_get_x("WLANConfig11b","wl_ssid"); %>" onKeyPress="return is_string(this)" onBlur="validate_string4(this)">
<input type="button" style="cursor:pointer;" class="content_input_fd_ro" onClick="subWin=window.open('Advanced_WirelessAPScanPre_Content.asp','AS','width=632,height=480')" value="<#CTL_apscan#>" name="apscan" size="4">
</td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#WLANConfig11b_AuthenticationMethod_itemdesc#>', LEFT);" onMouseOut="return nd();"><#WLANConfig11b_AuthenticationMethod_itemname#></td>
<td class="content_input_td">
<select name="wl_auth_mode" class="content_input_fd" onChange="return change_common(this, 'WLANConfig11b', 'wl_auth_mode')">
<option class="content_input_fd" value="open" <% nvram_match_x("WLANConfig11b","wl_auth_mode", "open","selected"); %>>Open System</option>
<option class="content_input_fd" value="shared" <% nvram_match_x("WLANConfig11b","wl_auth_mode", "shared","selected"); %>>Shared Key</option>
<option class="content_input_fd" value="psk" <% nvram_double_match_x("WLANConfig11b", "wl_auth_mode", "psk", "WLANConfig11b", "wl_wpa_mode", "1", "selected"); %>>WPA-Personal</option>
<option class="content_input_fd" value="psk" <% nvram_double_match_x("WLANConfig11b", "wl_auth_mode", "psk", "WLANConfig11b", "wl_wpa_mode", "2", "selected"); %>>WPA2-Personal</option>
</select>
</td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#WLANConfig11b_WPAType_itemdesc#>', LEFT);" onMouseOut="return nd();"><#WLANConfig11b_WPAType_itemname#></td>
<td class="content_input_td">
<select name="wl_crypto" class="content_input_fd" onChange="return change_common(this, 'WLANConfig11b', 'wl_crypto')">
<option class="content_input_fd" value="tkip" <% nvram_match_x("WLANConfig11b","wl_crypto", "tkip","selected"); %>>TKIP</option>
<option class="content_input_fd" value="aes" <% nvram_match_x("WLANConfig11b","wl_crypto", "aes","selected"); %>>AES</option>
</select>
</td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#WLANConfig11b_x_PSKKey_itemdesc#>', LEFT);" onMouseOut="return nd();"><#WLANConfig11b_x_PSKKey_itemname#>
</td><td class="content_input_td"><script language="JavaScript" type="text/javascript" src="md5.js"></script><input type="text" maxlength="63" class="content_input_fd" size="32" name="wl_wpa_psk" value="<% nvram_get_x("WLANConfig11b","wl_wpa_psk"); %>" onKeyUp="return is_wlphrase('WLANConfig11b', 'wl_wpa_psk', this)" onBlur="return validate_wlphrase('WLANConfig11b', 'wl_wpa_psk', this)"></td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#WLANConfig11b_WEPType_itemdesc#>', LEFT);" onMouseOut="return nd();"><#WLANConfig11b_WEPType_itemname#>
</td><td class="content_input_td"><select name="wl_wep_x" class="content_input_fd" onChange="return change_common(this, 'WLANConfig11b', 'wl_wep_x')"><option class="content_input_fd" value="0" <% nvram_match_x("WLANConfig11b","wl_wep_x", "0","selected"); %>>None</option><option class="content_input_fd" value="1" <% nvram_match_x("WLANConfig11b","wl_wep_x", "1","selected"); %>>WEP-64bits</option><option class="content_input_fd" value="2" <% nvram_match_x("WLANConfig11b","wl_wep_x", "2","selected"); %>>WEP-128bits</option></select></td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#WLANConfig11b_WEPKeyType_itemdesc#>', LEFT);" onMouseOut="return nd();"><#WLANConfig11b_WEPKeyType_itemname#></td>
<td class="content_input_td">
<select name="wl_key_type" class="content_input_fd" onChange="return change_common(this, 'WLANConfig11b', 'wl_key_type')">
<option class="content_input_fd" value="0" <% nvram_match_x("WLANConfig11b","wl_key_type", "0","selected"); %>>HEX</option>
<option class="content_input_fd" value="1" <% nvram_match_x("WLANConfig11b","wl_key_type", "1","selected"); %>>ASCII</option>
</select>
</td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#WLANConfig11b_x_Phrase_itemdesc#>', LEFT);" onMouseOut="return nd();"><#WLANConfig11b_x_Phrase_itemname#>
</td><td class="content_input_td"><script language="JavaScript" type="text/javascript" src="md5.js"></script><input type="text" maxlength="64" class="content_input_fd" size="32" name="wl_phrase_x" value="<% nvram_get_x("WLANConfig11b","wl_phrase_x"); %>" onKeyUp="return is_wlphrase('WLANConfig11b', 'wl_phrase_x', this)" onChange="return is_wlphrase('WLANConfig11b', 'wl_phrase_x', this)" onBlur="return validate_wlphrase('WLANConfig11b', 'wl_phrase_x', this)"></td>
</tr>
<tr>
<td class="content_header_td"><#WLANConfig11b_WEPKey1_itemname#>
</td><td class="content_input_td"><input type="text" maxlength="32" class="content_input_fd" size="32" name="wl_key1" value="<% nvram_get_x("WLANConfig11b","wl_key1"); %>" onBlur="return validate_wlkey(this, 'WLANConfig11b')" onKeyPress="return is_wlkey(this, 'WLANConfig11b')" onKeyUp="return change_wlkey(this, 'WLANConfig11b')"></td>
</tr>
<tr>
<td class="content_header_td"><#WLANConfig11b_WEPKey2_itemname#>
</td><td class="content_input_td"><input type="text" maxlength="32" class="content_input_fd" size="32" name="wl_key2" value="<% nvram_get_x("WLANConfig11b","wl_key2"); %>" onBlur="return validate_wlkey(this, 'WLANConfig11b')" onKeyPress="return is_wlkey(this, 'WLANConfig11b')" onKeyUp="return change_wlkey(this, 'WLANConfig11b')"></td>
</tr>
<tr>
<td class="content_header_td"><#WLANConfig11b_WEPKey3_itemname#>
</td><td class="content_input_td"><input type="text" maxlength="32" class="content_input_fd" size="32" name="wl_key3" value="<% nvram_get_x("WLANConfig11b","wl_key3"); %>" onBlur="return validate_wlkey(this, 'WLANConfig11b')" onKeyPress="return is_wlkey(this, 'WLANConfig11b')" onKeyUp="return change_wlkey(this, 'WLANConfig11b')"></td>
</tr>
<tr>
<td class="content_header_td"><#WLANConfig11b_WEPKey4_itemname#>
</td><td class="content_input_td"><input type="text" maxlength="32" class="content_input_fd" size="32" name="wl_key4" value="<% nvram_get_x("WLANConfig11b","wl_key4"); %>" onBlur="return validate_wlkey(this, 'WLANConfig11b')" onKeyPress="return is_wlkey(this, 'WLANConfig11b')" onKeyUp="return change_wlkey(this, 'WLANConfig11b')"></td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#WLANConfig11b_WEPDefaultKey_itemdesc#>', LEFT);" onMouseOut="return nd();"><#WLANConfig11b_WEPDefaultKey_itemname#>
</td><td class="content_input_td"><select name="wl_key" class="content_input_fd" onChange="return change_common(this, 'WLANConfig11b', 'wl_key')"><option class="content_input_fd" value="1" <% nvram_match_x("WLANConfig11b","wl_key", "1","selected"); %>>Key1</option><option class="content_input_fd" value="2" <% nvram_match_x("WLANConfig11b","wl_key", "2","selected"); %>>Key2</option><option class="content_input_fd" value="3" <% nvram_match_x("WLANConfig11b","wl_key", "3","selected"); %>>Key3</option><option class="content_input_fd" value="4" <% nvram_match_x("WLANConfig11b","wl_key", "4","selected"); %>>Key4</option></select></td>
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
<table width="666" border="1" cellpadding="0" cellspacing="0" bordercolor="E0E0E0">
<tr class="content_list_header_tr">
<td class="content_list_header_td" width="43%" id="ProfileList"><#WLANConfig11b_ProfileList_groupitemdesc#></td>
<td width="13%">
<div align="center">
<input class="inputSubmit" style="cursor:pointer; font-weight:normal;" type="submit" onMouseOut="buttonOut(this)" onMouseOver="buttonOver(this)" onClick="return markGroup(this, 'ProfileList', 4, ' MoveUp ');" name="ProfileList" value="<#CTL_moveup#>" size="12">
</div>
</td>
<td width="13%">
<div align="center">
<input class="inputSubmit" style="cursor:pointer; font-weight:normal;" type="submit" onMouseOut="buttonOut(this)" onMouseOver="buttonOver(this)" onClick="return markGroup(this, 'ProfileList', 4, ' MoveDown ');" name="ProfileList" value="<#CTL_movedown#>" size="12">
</div>
</td>
<td width="13%">
<div align="center">
<input class="inputSubmit" style="cursor:pointer; font-weight:normal;" type="submit" onMouseOut="buttonOut(this)" onMouseOver="buttonOver(this)" onClick="return markGroup(this, 'ProfileList', 4, ' DelProfile ');" name="ProfileList" value="<#CTL_del#>" size="12">
</div>
</td>
<td width="13%">
<div align="center">
<input class="inputSubmit" style="cursor:pointer; font-weight:normal;" type="submit" onMouseOut="buttonOut(this)" onMouseOver="buttonOver(this)" onClick="return markGroup(this, 'ProfileList', 4, ' SaveProfile ');" name="ProfileList" value="<#CTL_store#>" size="12">
</div>
</td>
<td width="5%">&nbsp
</td>
</tr>
</table>
</td>
</tr>
<tr>
<td>
<table width="666" border="0" cellpadding="0" cellspacing="0" bordercolor="E0E0E0"></table>
</td>
</tr>
<tr>
<td>
<table table width="666" border="1" cellpadding="0" cellspacing="0" bordercolor="E0E0E0">
<tr>
<td width="13%" align="center" class="content_header_td_ap"><#WLANConfig11b_Index_td#></td>
<td width="42%" align="center" class="content_header_td_ap"><#SW_SSID#></td>
<td width="36%" align="center" class="content_header_td_ap"><#WLANConfig11b_AuthenticationMethod_td#></td>
<td width="9%" align="center" class="content_header_td_ap"><#WLANConfig11b_Select_td#></td>
</tr>
<% nvram_dump("profile.log",""); %>
</table>
</td>
</tr>
<tr>
<td>
<table width="666" border="1" cellpadding="0" cellspacing="0" bordercolor="B0B0B0">
<tr bgcolor="#CCCCCC"><td colspan="3"><font face="arial" size="2"><b>&nbsp</b></font></td></tr>
<tr bgcolor="#FFFFFF">
<td id ="Confirm" height="25" width="34%">
<div align="center"><font face="Arial"> <input class=inputSubmit style="cursor:pointer;" onMouseOut=buttonOut(this) onMouseOver="buttonOver(this)" onClick="return onSubmitCtrl(this, ' Restore ');" type="submit" value=" <#CTL_restore#> " name="action"></font></div>
</td>
<td height="25" width="33%">
<div align="center"><font face="Arial"> <input class=inputSubmit style="cursor:pointer;" onMouseOut=buttonOut(this) onMouseOver="buttonOver(this)" onClick="return onSubmitCtrl(this, ' Finish ');" type="submit" value=" <#CTL_finish#> " name="action"></font></div>
</td>
<td height="25" width="33%">
<div align="center"><font face="Arial"> <input class=inputSubmit style="cursor:pointer;" onMouseOut=buttonOut(this) onMouseOver="buttonOver(this)" onClick="return onSubmitCtrl(this, ' Apply ');" type="submit" value=" <#CTL_apply#> " name="action"></font></div>
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
