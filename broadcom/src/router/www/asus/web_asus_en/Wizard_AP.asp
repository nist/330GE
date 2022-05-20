<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<meta http-equiv="Pragma" content="no-cache">
<title><#ZVMODELVZ#></title>
<style type="text/css">
<!--
body {
margin-top: 0px;
margin-bottom: 0px;
margin-left: 0px;
margin-right: 0px;
}
-->
</style>
<link href="style_wizard.css" rel="stylesheet" type="text/css">
<link href="select_style.css" rel="stylesheet" type="text/css">
<style type="text/css">
<!--
.rt_form{
font-family: Verdana, Arial;
font-size: 10pt;
font-weight:bold;
color: #333333;
}
.style2 {
font-family: Verdana, Arial, Helvetica, sans-serif;
font-size: 12px;
color: #333333;
margin: 3px;
}
.style3 {
color: #990000;
font-weight: bold;
padding-left:40px;
margin-left:40px;
font-size: 14px;
}
-->
</style>
<script language="JavaScript" type="text/javascript" src="ui.js"></script>
</head>
<body onLoad="loadSecurity();">
<iframe name="fframe" src=""; width="0" height="0" scrolling="no" frameborder="0"></iframe>
<form method="GET" name="form" action="apply.cgi" target="fframe">
<input type="hidden" name="sid_list" value="WLANConfig11b;Layer3Forwarding;IPConnection;LANHostConfig;">
<input type="hidden" name="current_page" value="Wizard_AP.asp">
<input type="hidden" name="next_page" value="SaveRestart.asp">
<input type="hidden" name="next_host" value="">
<input type="hidden" name="group_id" value="">
<input type="hidden" name="action_mode" value="">
<input type="hidden" name="modified" value="0">
<input type="hidden" name="op_mode" value="AP">
<input type="hidden" name="wl_hotspot" value="0">
<input type="hidden" name="wl_ssid" value="<% nvram_get_x("WLANConfig11b","wl_ssid"); %>">
<input type="hidden" name="wl_ssid2" value="<% nvram_get_x("WLANConfig11b","wl_ssid2"); %>">
<input type="hidden" name="wl_guest_ssid_2" value="<% nvram_get_x("WLANConfig11b","wl_guest_ssid_2"); %>">
<input type="hidden" name="wl_auth_mode" value="<% nvram_get_x("WLANConfig11b","wl_auth_mode"); %>">
<input type="hidden" name="wl_crypto" value="<% nvram_get_x("WLANConfig11b","wl_crypto"); %>">
<input type="hidden" name="wl_wpa_psk_org" value="<% nvram_get_x("WLANConfig11b","wl_wpa_psk"); %>">
<input type="hidden" name="wl_wpa_psk_org_encoded" value="<% nvram_char_to_ascii("WLANConfig11b","wl_wpa_psk"); %>">
<input type="hidden" name="wl_key1_org" value="<% nvram_char_to_ascii("WLANConfig11b","wl_key1"); %>">
<input type="hidden" name="wl_key2_org" value="<% nvram_char_to_ascii("WLANConfig11b","wl_key2"); %>">
<input type="hidden" name="wl_key3_org" value="<% nvram_char_to_ascii("WLANConfig11b","wl_key3"); %>">
<input type="hidden" name="wl_key4_org" value="<% nvram_char_to_ascii("WLANConfig11b","wl_key4"); %>">
<input type="hidden" name="wl_wep_x" value="<% nvram_get_x("WLANConfig11b","wl_wep_x"); %>">
<input type="hidden" name="wl_key" value="<% nvram_get_x("WLANConfig11b","wl_key"); %>">
<input type="hidden" name="wl_key1" value="<% nvram_get_x("WLANConfig11b","wl_key1"); %>">
<input type="hidden" name="wl_key2" value="<% nvram_get_x("WLANConfig11b","wl_key2"); %>">
<input type="hidden" name="wl_key3" value="<% nvram_get_x("WLANConfig11b","wl_key3"); %>">
<input type="hidden" name="wl_key4" value="<% nvram_get_x("WLANConfig11b","wl_key4"); %>">
<input type="hidden" name="wl_mode_ex" value="ap">
<input type="hidden" name="wl_mode_EX" value="<% nvram_get_x("","wl_mode_EX"); %>">
<input type="hidden" name="wl_wpa_mode" value="<% nvram_get_x("WLANConfig11b","wl_wpa_mode"); %>">
<input type="hidden" name="wl_guest_enable" value="0">
<input type="hidden" name="mssid_enable" value="0">
<input type="hidden" name="mssid_ssid_x0" value="<% nvram_get_x("WLANConfig11b","mssid_ssid_x0"); %>">
<input type="hidden" name="mssid_ssid_2_x0" value="<% nvram_get_x("WLANConfig11b","mssid_ssid_2_x0"); %>">
<input type="hidden" name="wan_nat_x" value="0">
<input type="hidden" name="wan_nat_X" value="<% nvram_get_x("","wan_nat_X"); %>">
<input type="hidden" name="wan_route_x" value="IP_Bridged">
<!--
<input type="hidden" name="wan_proto" value="static">
<input type="hidden" name="lan_proto_x" value="0">
-->
<input type="hidden" name="preferred_lang" value="<% nvram_get_x("","preferred_lang"); %>">
<input type="hidden" name="LANIP" value="<% nvram_get_x("LANHostConfig","lan_ipaddr"); %>">
<input type="hidden" name="LANNM" value="<% nvram_get_x("LANHostConfig","lan_netmask"); %>">
<input type="hidden" name="PID" value="<% nvram_get_x("","productid"); %>">
<input type="hidden" name="productidodm" value="<% nvram_get_x("", "productid_odm"); %>">
<input type="hidden" name="wl_ssid_org" value='<% nvram_get_x("WLANConfig11b","wl_ssid"); %>'>
<input type="hidden" name="wl_guest_ssid_org" value='<% nvram_get_x("WLANConfig11b","wl_guest_ssid_1"); %>'>
<input type="hidden" name="language_select" value=''>
<input type="hidden" name="wl_key_type" value="<% nvram_get_x("WLANConfig11b","wl_key_type"); %>">
<table width="100%"  border="0" align="center" cellpadding="0" cellspacing="0" bgcolor="#3E739D">
<tr>
<td align="center"><table width="741" height="43" border="0" cellpadding="0" cellspacing="0">
<tr>
<td height="43">
<script>
if (document.form.productidodm.value.length==0)
document.write("<img src=\"graph/banner.gif\" width=\"309\" height=\"43\">");
else
document.write("<img src=\"graph/banner2.gif\" width=\"309\" height=\"43\">");
</script>
</td>
<td height="43" class="banner_info" > &#8226; <#SW_Mode#><span style="color:#FF6;">
<script>document.write(is_current_Mode(document.form.wan_nat_X.value,document.form.wl_mode_EX.value));</script></span></td>
<td height="43" align="right" >
<span class="banner_info"> &#8226; <#PASS_LANG#></span>
<td valign="middle">
<div id="selectMenumain" onClick="menutrigger();">
<script>
if (document.form.preferred_lang.value=="EN")
document.write('<img src=\'graph/english_1.gif\'>');
else if (document.form.preferred_lang.value=="TW")
document.write('<img src=\'graph/chinese_1.gif\'>');
else if (document.form.preferred_lang.value=="CN")
document.write('<img src=\'graph/schinese_1.gif\'>');
else if (document.form.preferred_lang.value=="DE")
document.write('<img src=\'graph/german_1.gif\'>');
else if (document.form.preferred_lang.value=="JP")
document.write('<img src=\'graph/japanese_1.gif\'>');
else if (document.form.preferred_lang.value=="TR")
document.write('<img src=\'graph/turkish_1.gif\' style=\'margin-top: 2px;\'>');
</script>
</div>
<div id="selectMenucontent">
<div id="En" onClick="selectit('EN');"><a href="#" class="En"></a></div>
<div id="Tc" onClick="selectit('TW');"><a href="#" class="Tc"></a></div>
<div id="Sc" onClick="selectit('CN');"><a href="#" class="Sc"></a></div>
<div id="De" onClick="selectit('DE');"><a href="#" class="De"></a></div>
<div id="Jp" onClick="selectit('JP');"><a href="#" class="Jp"></a></div>
<div id="Tr" onClick="selectit('TR');"><a href="#" class="Tr"></a></div>
</div>
</td>
<script>
isMenuopen = 0;
document.onmouseup = function hiddenmenu(){
document.getElementById('selectMenucontent').style.display = 'none'
}
function menutrigger(){
if(!isMenuopen){
document.getElementById('selectMenucontent').style.display = 'block';
isMenuopen = 1;
}
else{
document.getElementById('selectMenucontent').style.display = 'none'
isMenuopen = 0;
}
}
function selectit(lang){  
if (lang=="EN")
document.getElementById('selectMenumain').innerHTML = '<img src=\'graph/english_1.gif\'>';
else if (lang=="TW")
document.getElementById('selectMenumain').innerHTML = '<img src=\'graph/chinese_1.gif\'>';
else if (lang=="CN")
document.getElementById('selectMenumain').innerHTML = '<img src=\'graph/schinese_1.gif\'>';
else if (lang=="DE")
document.getElementById('selectMenumain').innerHTML = '<img src=\'graph/german_1.gif\'>';
else if (lang=="JP")
document.getElementById('selectMenumain').innerHTML = '<img src=\'graph/japanese_1.gif\'>';
else if (lang=="TR")
document.getElementById('selectMenumain').innerHTML = '<img src=\'graph/turkish_1.gif\' style=\'margin-top: 2px;\'>';
document.form.language_select.value = lang;
document.getElementById('selectMenucontent').style.display = 'none'
isMenuopen = 0;
if (document.form.preferred_lang.value != lang)
{document.form.action_mode.value = " Select_LANG ";
document.form.target="";
document.form.submit();
return true;
}
}
</script>
</td>
</tr>
</table>
</td>
</tr>
</table>
<br>
<table width="735"  border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="184"><a href="Wizard_Gateway.asp"><div id="tag_over">Gateway</div></a></td>
<td width="184"><div class="tag_on" >AP</div></td>
<td width="184"><a href="Wizard_Adapter.asp"><div id="tag_over">Adapter</div></a></td>
<td width="184"><a href="Wizard_Repeater.asp"><div id="tag_over">Repeater</div></a></td>
</tr>
</table>
<table width="735" height="134" border="0" align="center" cellpadding="0" cellspacing="0" background="graph/bar_bg.gif">
<tr>
<td height="123" colspan="3"><div class="bar" border="0">
<div class="pic" style="background-image: url(graph/AP_pic.gif); background-repeat: no-repeat;">
<!--
<div style="margin-left:55px; margin-top:35px; width:50px; font-weight:bold; font-family: Arial; font-size:13px; position:absolute;"><#SW_user#></div>
-->
<script>
if (document.form.preferred_lang.value!="JP" && document.form.preferred_lang.value!="TR")
document.writeln('<div style=\"margin-left:55px; margin-top:35px; width:50px; font-weight:bold; font-family: Arial; font-size:13px; position:absolute;\"><#SW_user#></div>');
else
document.writeln('<div style=\"margin-left:39px; margin-top:35px; width:80px; font-weight:bold; font-family: Arial; font-size:13px; position:absolute;\"><#SW_user#></div>');
</script>
<div style="margin-left:342px; margin-top:103px; width:70px; font-weight:bold; font-family: Arial; font-size:13px; position:absolute;"><#ZVMODELVZ#></div>
<div style="margin-left:532px; margin-top:104px; width:120px; font-weight:bold; font-family: Arial; font-size:13px; position:absolute;"><#SW_hub#></div>
</div>
</div></td>
</tr>
<tr>
<td width="11" height="11" bgcolor="#FFFFFF"><img src="graph/corner_new.gif" width="11" height="11"></td>
<td width="714" height="11"></td>
<td width="11" height="11" bgcolor="#FFFFFF"><img src="graph/corner_r.gif" width="11" height="11"></td>
</tr>
</table>
<table width="735" border="0" align="center" cellpadding="0" cellspacing="1">
<tr valign="top">
<td width="180">
<table width="180" height="185" class="table" align="center" cellpadding="0" cellspacing="0">
<tr>
<td class="about_title"><img src="graph/about.gif" width="18" height="16" hspace="0" vspace="0" align="absmiddle">&nbsp;<#SW_About_AP#></td>
</tr>
<tr>
<td height="220" bordercolor="#FFFFFF"><p class="style2"><#SW_About_AP_Message#></p>
<script>
if (document.form.preferred_lang.value!="EN" && document.form.preferred_lang.value!="DE" && document.form.preferred_lang.value!="TR")
document.write('<br><br>');
</script>
<br><img src="graph/plus.gif" width="20" height="20" align="absmiddle"> <a href="javascript:Advanced_Setting()"><#SW_Advanced_Setting#></a></td>
</tr>
</table>
</td>
<td>
<table width="540" cellpadding="0" cellspacing="0" class="table">
<tr background="graph/q_setup_bg.gif">
<td class="q_setup_title" ><img src="graph/q_setup.gif" width="21" height="16" align="absmiddle"> <strong><#SW_Quick_Setup#></strong></td>
</tr>
<tr bordercolor="#FFFFFF">
<script>
if (nav)
{if (document.form.preferred_lang.value=="DE" || document.form.preferred_lang.value=="TR")
document.writeln('<td height=\"246\" valign=\"top\" bordercolor=\"#FFFFFF\">');
else
document.writeln('<td height=\"228\" valign=\"top\" bordercolor=\"#FFFFFF\">');
}
else
{if (document.form.preferred_lang.value=="TR")
document.writeln('<td height=\"245\" valign=\"top\" bordercolor=\"#FFFFFF\">');
else if (document.form.preferred_lang.value=="DE")
document.writeln('<td height=\"227\" valign=\"top\" bordercolor=\"#FFFFFF\">');
else if (document.form.preferred_lang.value!="EN" && document.form.preferred_lang.value!="JP")
document.writeln('<td height=\"224\" valign=\"top\" bordercolor=\"#FFFFFF\">');
else
document.writeln('<td height=\"227\" valign=\"top\" bordercolor=\"#FFFFFF\">');
}
</script>
<table width="520" border="0" align="center" cellpadding="4" cellspacing="2" class="rt_form">
<tr>
<td width="165" align="right"><#BOP_wlan_ssid_item#></td>
<td><script>document.write('<input name=\"ssid\" class=\"form_text\" type=\"text\" value=\"\" size=\"32\" maxlength=\"32\">');document.form.ssid.value=decodeURIComponent(document.form.wl_ssid2.value);</script></td>
</tr>
<tr>
<td width="165" align="right"><#SW_wlan_sec_item#>
</td>
<td><select name="SecurityLevel" class="form_text" id="SecurityLevel" onChange="return change_security(this, 'WLANConfig11b', 0)">
<option value="0"><#OPTION_WEP_ENC_NONE#></option>
<option value="1"><#OPTION_WEP_ENC_WEP_64#></option>
<option value="2"><#OPTION_WEP_ENC_WEP_128#></option>
<option value="3"><#OPTION_WPAPSK#></option>
</select>
</td>
</tr>
</table>
<table style="display:none;" width="520" border="0" align="center" cellpadding="4" cellspacing="2" class="rt_form" id="wep_password">
<tr>
<td width="165" align="right"><#BOP_account_pass_item#></td>
<td><script language="JavaScript" type="text/javascript" src="phrase.js"></script><script language="JavaScript" type="text/javascript" src="md5.js"></script><input name="wl_wpa_psk" type="text" class="form_text" size="32" maxlength="63" value="<% nvram_get_x("WLANConfig11b","wl_wpa_psk"); %>" onKeyPress="return is_wlkey(event, this, 'WLANConfig11b');" onKeyUp="return is_wlphrase_q('WLANConfig11b',this);"></td>
</tr>
</table>
<table width="540" border="0" cellpadding="0" cellspacing="0">
<tr>
<td align=right>
<input class="submit2" style="cursor:pointer;"  type="button" value="<#CTL_finish#>" name="action2"  onClick="scrollTo(0, 0);return checkPW();" >
</td>
</tr>
</table>
</td></tr>
</table></td>
</tr>
</table>
<div id="popupHint">
<p>&nbsp;</p><p>&nbsp;</p><p>&nbsp;</p><p>&nbsp;</p><p>&nbsp;</p>
<table width="100%" height="100%"  border="0" cellpadding="0" cellspacing="0">
<tr>
<td height="100%" align="center" valign="top">
<table width="420" height="150"  border="0" align="center" cellpadding="5" cellspacing="2" bgcolor="#999999">
<tr>
<td align="right" valign="middle"  background="graph/hint_title_bg.gif">
<table width="100%" height="100%"  border="0" cellpadding="0" cellspacing="0">
<tr>
<td align="left"><div style = "font-family: Verdana, Arial, Helvetica, sans-serif; font-size: 12px;"><span style="color: #003366";><#SW_Hint#></span></div></td>
<td align="right"><a  onclick="hide_hint();"  href="javascript:;"><img src="graph/windows_close.gif" alt="Close this window!" width="22" height="16" border="0"/></a></td>
</tr>
</table>
</td>
</tr>
<tr>
<td colspan="2" bgcolor="#F4F3D5">
<table width="100%"  border="0" cellspacing="0" cellpadding="0">
<tr>
<td width="60" align="center" valign="top"><img src="graph/Yap.gif">&nbsp;</td>
<td><div style = "font-family: Verdana, Arial, Helvetica, sans-serif; font-size: 12px;">
<span id="hint_area"></span>
<script>document.getElementById("hint_area").innerHTML=confirm_hint(document.form.op_mode.value);</script></div></td>
</tr>
</table>
<p align="center"><input type="button" name="button4" style="font-family: Verdana, Arial, Helvetica, sans-serif; font-size: 12px;" value="<#CTL_ok#>" onClick="submit_gateway();show_restart();">
<input type="button" name="concel_window2" style="font-family: Verdana, Arial, Helvetica, sans-serif; font-size: 12px;" value="<#CTL_cancel#>" onClick="hide_hint();">
</p>
<br></td>
</tr>
</table></td>
</tr>
</table>
</div>
<div id="restart">
<p>&nbsp;</p><p>&nbsp;</p><p>&nbsp;</p><p>&nbsp;</p><p>&nbsp;</p>
<table width="100%" height="100%"  border="0" cellpadding="0" cellspacing="0">
<tr>
<td height="100%" align="center" valign="top">
<table width="400" height="220"  border="0" align="center" cellpadding="5" cellspacing="2" bgcolor="#003366">
<tr>
<td valign="middle" bgcolor="#AAC1D2">
<div style="font-family: Verdana, Arial, Helvetica, sans-serif; font-size: 15px;">
<#SW_Restart_Desc#>
<#SW_Progress_Desc#>
<span id="parea" style="position:relative;">0 %</span>&nbsp;&nbsp;<img src="graph/loading.gif" hspace="4" align="absmiddle"></div>
</td>
</tr>
</table>
</td>
</tr>
</table>
</div>
</form>
</body>
</html>
