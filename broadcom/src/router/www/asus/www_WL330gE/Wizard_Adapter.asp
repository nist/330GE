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
.style4 {color: #FFFFFF}
-->
</style>
<script language="JavaScript" type="text/javascript" src="ui.js"></script>
<script language="JavaScript" type="text/javascript">
/*
function change_nat_mode()
{if (document.form.wl_nat[0].checked==true)
{document.form.op_mode.value="gw-adapter";
document.form.wan_route_x.value="IP_Routed";
}
else
{document.form.op_mode.value="adapter";
document.form.wan_route_x.value="IP_Bridged";
}
}
*/
</script>
<% nvram_dump("apscan2.log",""); %>
<script language="JavaScript">
var select_idx=-1;
</script>
</head>
<body onLoad="STA_URE_onload();" onUnload="fastHide();">
<iframe name="fframe" src="" width="0" height="0" scrolling="0" frameborder="0"></iframe>
<form method="GET" name="form" action="apply.cgi" target="fframe">
<input type="hidden" name="sid_list" value="WLANConfig11b;Layer3Forwarding;IPConnection;LANHostConfig;">
<input type="hidden" name="current_page" value="Wizard_Adapter.asp">
<input type="hidden" name="next_page" value="SaveRestart.asp">
<input type="hidden" name="next_host" value="">
<input type="hidden" name="group_id" value="">
<input type="hidden" name="action_mode" value="">
<input type="hidden" name="modified" value="0">
<input type="hidden" name="op_mode" value="adapter">
<input type="hidden" name="wl_hotspot" value="0">
<input type="hidden" name="wl_ssid" value="<% nvram_get_x("WLANConfig11b","wl_ssid"); %>">
<input type="hidden" name="wl_ssid2" value="<% nvram_get_x("WLANConfig11b","wl_ssid2"); %>">
<input type="hidden" name="wl_auth_mode" value="<% nvram_get_x("WLANConfig11b","wl_auth_mode"); %>">
<input type="hidden" name="wl_crypto" value="<% nvram_get_x("WLANConfig11b","wl_crypto"); %>">
<input type="hidden" name="wl_wpa_psk_org" value="<% nvram_get_x("WLANConfig11b","wl_wpa_psk"); %>">
<input type="hidden" name="wl_wep_x" value="<% nvram_get_x("WLANConfig11b","wl_wep_x"); %>">
<input type="hidden" name="wl_key" value="1">
<input type="hidden" name="wl_key1" value="<% nvram_get_x("WLANConfig11b","wl_key1"); %>">
<input type="hidden" name="wl_key2" value="<% nvram_get_x("WLANConfig11b","wl_key2"); %>">
<input type="hidden" name="wl_key3" value="<% nvram_get_x("WLANConfig11b","wl_key3"); %>">
<input type="hidden" name="wl_key4" value="<% nvram_get_x("WLANConfig11b","wl_key4"); %>">
<input type="hidden" name="wl_mode_ex" value="sta">
<input type="hidden" name="wl_mode_EX" value="<% nvram_get_x("","wl_mode_EX"); %>">
<input type="hidden" name="wl_wpa_mode" value="<% nvram_get_x("WLANConfig11b","wl_wpa_mode"); %>">
<input type="hidden" name="wl_guest_enable" value="0">
<input type="hidden" name="mssid_enable" value="0">
<input type="hidden" name="mssid_ssid_x0" value="<% nvram_get_x("WLANConfig11b","mssid_ssid_x0"); %>">
<input type="hidden" name="mssid_ssid_2_x0" value="<% nvram_get_x("WLANConfig11b","mssid_ssid_2_x0"); %>">
<input type="hidden" name="wan_nat_X" value="<% nvram_get_x("","wan_nat_X"); %>">
<input type="hidden" name="wan_nat_x" value="0">
<input type="hidden" name="wan_route_x" value="IP_Bridged">
<!--
<input type="hidden" name="wan_proto" value="static">
<input type="hidden" name="lan_proto_x" value="0">
-->
<input type="hidden" name="preferred_lang" value="<% nvram_get_x("","preferred_lang"); %>">
<input type="hidden" name="LANIP" value="<% nvram_get_x("LANHostConfig","lan_ipaddr"); %>">
<input type="hidden" name="LANNM" value="<% nvram_get_x("LANHostConfig","lan_netmask"); %>">
<input type="hidden" name="sta_state" value="<% nvram_get_x("","wl_sta_state2"); %>">
<input type="hidden" name="no_ask_password" value="0">
<input type="hidden" name="PID" value="<% nvram_get_x("","productid"); %>">
<input type="hidden" name="productidodm" value="<% nvram_get_x("", "productid_odm"); %>">
<input type="hidden" name="wf1_ssid" value='<% nvram_get_x("","pf1_ssid"); %>'>
<input type="hidden" name="wf2_ssid" value='<% nvram_get_x("","pf2_ssid"); %>'>
<input type="hidden" name="wf3_ssid" value='<% nvram_get_x("","pf3_ssid"); %>'>
<input type="hidden" name="wf4_ssid" value='<% nvram_get_x("","pf4_ssid"); %>'>
<input type="hidden" name="wf1_key" value='<% nvram_get_x("","pf1_key"); %>'>
<input type="hidden" name="wf2_key" value='<% nvram_get_x("","pf2_key"); %>'>
<input type="hidden" name="wf3_key" value='<% nvram_get_x("","pf3_key"); %>'>
<input type="hidden" name="wf4_key" value='<% nvram_get_x("","pf4_key"); %>'>
<input type="hidden" name="wf1_passwd" value='<% nvram_get_x("","pf1_passwd"); %>'>
<input type="hidden" name="wf2_passwd" value='<% nvram_get_x("","pf2_passwd"); %>'>
<input type="hidden" name="wf3_passwd" value='<% nvram_get_x("","pf3_passwd"); %>'>
<input type="hidden" name="wf4_passwd" value='<% nvram_get_x("","pf4_passwd"); %>'>
<input type="hidden" name="wf1_key_org" value='<% nvram_char_to_ascii("","pf1_key"); %>'>
<input type="hidden" name="wf2_key_org" value='<% nvram_char_to_ascii("","pf2_key"); %>'>
<input type="hidden" name="wf3_key_org" value='<% nvram_char_to_ascii("","pf3_key"); %>'>
<input type="hidden" name="wf4_key_org" value='<% nvram_char_to_ascii("","pf4_key"); %>'>
<input type="hidden" name="wf1_passwd_org" value='<% nvram_char_to_ascii("","pf1_passwd"); %>'>
<input type="hidden" name="wf2_passwd_org" value='<% nvram_char_to_ascii("","pf2_passwd"); %>'>
<input type="hidden" name="wf3_passwd_org" value='<% nvram_char_to_ascii("","pf3_passwd"); %>'>
<input type="hidden" name="wf4_passwd_org" value='<% nvram_char_to_ascii("","pf4_passwd"); %>'>
<input type="hidden" name="add_profile" value='0'>
<input type="hidden" name="language_select" value=''>
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
<td width="184"><a href="Wizard_AP.asp"><div id="tag_over">AP</div></a></td>
<td width="184"><div class="tag_on">Adapter</div></td>
<td width="184"><a href="Wizard_Repeater.asp"><div id="tag_over">Repeater</div></a></td>
</tr>
</table>
<table width="735" height="134" border="0" align="center" cellpadding="0" cellspacing="0" background="graph/bar_bg.gif">
<tr>
<td height="123" colspan="3"><div class="bar" border="0">
<div class="pic" style="background-image: url(graph/adapter_pic.gif); background-repeat: no-repeat;">
<script>
if (document.form.preferred_lang.value=="JP" || document.form.preferred_lang.value=="TR")
document.writeln('<div style=\"margin-left:30px; margin-top:30px; width:80px; font-weight:bold; font-family: Arial; font-size:13px; position:absolute; visibility: visible;\"><#SW_user#></div>');
else if (document.form.preferred_lang.value!="DE")
document.writeln('<div style=\"margin-left:50px; margin-top:30px; width:50px; font-weight:bold; font-family: Arial; font-size:13px; position:absolute; visibility: visible;\"><#SW_user#></div>');
else
document.writeln('<div style=\"margin-left:30px; margin-top:30px; width:50px; font-weight:bold; font-family: Arial; font-size:13px; position:absolute; visibility: visible;\"><#SW_user#></div>');
</script>
<div style="margin-left:246px; margin-top:107px; width:70px; font-weight:bold; font-family: Arial; font-size:13px; position:absolute; visibility: visible;"><#ZVMODELVZ#></div>
<!--
<div style="margin-left:483px; margin-top:107px; width:120px; font-weight:bold; font-family: Arial; font-size:13px; position:absolute; visibility: visible;"><#DEVICE_AP#></div>
-->
<script>
if (document.form.preferred_lang.value=="TR")
document.writeln('<div style=\"margin-left:448px; margin-top:107px; width:120px; font-weight:bold; font-family: Arial; font-size:13px; position:absolute; visibility: visible;\"><#DEVICE_AP#></div>');
else
document.writeln('<div style=\"margin-left:483px; margin-top:107px; width:120px; font-weight:bold; font-family: Arial; font-size:13px; position:absolute; visibility: visible;\"><#DEVICE_AP#></div>');
</script>
<!--
<div style="margin-left:645px; margin-top:8px; width:60px; font-weight:bold; font-family: Arial; font-size:13px; position:absolute; visibility: visible;"><#BM_Internet#></div>
-->
<script>
if (nav)
{if (document.form.preferred_lang.value=="JP")
document.writeln('<div style=\"margin-left:625px; margin-top:8px; width:98px; font-weight:bold; font-family: Arial; font-size:13px; position:absolute; visibility: visible;\"><#BM_Internet#></div>');
else
document.writeln('<div style=\"margin-left:645px; margin-top:8px; width:60px; font-weight:bold; font-family: Arial; font-size:13px; position:absolute; visibility: visible;\"><#BM_Internet#></div>');
}
else
{if (document.form.preferred_lang.value!="JP")
document.writeln('<div style=\"margin-left:645px; margin-top:8px; width:60px; font-weight:bold; font-family: Arial; font-size:13px; position:absolute; visibility: visible;\"><#BM_Internet#></div>');
else
document.writeln('<div style=\"margin-left:625px; margin-top:8px; width:84px; font-weight:bold; font-family: Arial; font-size:13px; position:absolute; visibility: visible;\"><#BM_Internet#></div>');
}
</script>
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
<td class="about_title"><img src="graph/about.gif" width="18" height="16" hspace="0" vspace="0" align="absmiddle">&nbsp;<#SW_About_Adapter#></td>
</tr>
<tr>
<td height="220" valign="top" bordercolor="#FFFFFF"><p class="style2"><#SW_About_Adapter_Message#></p>
<script>
if (nav)
{if (document.form.preferred_lang.value=="JP" || document.form.preferred_lang.value=="TR")
document.write('<br>');
}
else
{if (document.form.preferred_lang.value=="DE" || document.form.preferred_lang.value=="JP" || document.form.preferred_lang.value=="TR")
document.write('<br>');
}
</script>
<img src="graph/plus.gif" width="20" height="20" align="absmiddle"><a href="javascript:Advanced_Setting()"><#SW_Advanced_Setting#></a></td>
</tr>
</table></td>
<td>
<table width="540" cellpadding="0" cellspacing="0" class="table">
<tr background="graph/q_setup_bg.gif">
<td class="q_setup_title" ><img src="graph/q_setup.gif" width="21" height="16" align="absmiddle"> <strong><#SW_Quick_Setup#></strong></td>
</tr>
<tr bordercolor="#FFFFFF">
<script>
if (nav)
document.writeln('<td height=\"228\" valign=\"top\" bordercolor=\"#FFFFFF\">');
else
{if (document.form.preferred_lang.value=="TW" || document.form.preferred_lang.value=="CN")
document.writeln('<td height=\"221\" valign=\"top\" bordercolor=\"#FFFFFF\">');
else
document.writeln('<td height=\"224\" valign=\"top\" bordercolor=\"#FFFFFF\">');
}
</script>
<!--
<br>
<table width="520" height="30"  border="0" align="center" cellpadding="0" cellspacing="0" bgcolor="#D1DEE9" style="border-top:2px solid #999; border-bottom:1px dotted #666; font-size:12px; color:#003366;">
<tr>
<td width="200" align="center" onMouseOver="return overlib('<#Desc_SW_NAT#>', LEFT);" onMouseOut="return nd();"><strong><#SW_NAT#></strong></td>
<td width=20><input type="radio" name="wl_nat" value="1" onClick="change_nat_mode();" <% nvram_match_x("WLANConfig11b", "wl_nat", "1", "checked"); %>></td>
<td width=70><#SW_Radio_Enable#></td>
<td width=20><input type="radio" name="wl_nat" style="line-height:12px; vertical-align: bottom;" value="0" onClick="change_nat_mode();" <% nvram_match_x("WLANConfig11b", "wl_nat", "0", "checked"); %>></td>
<td><#SW_Radio_Disable#></td>
</tr>
</table>
<br>
-->
<table width="520" border="0" align="center" cellspacing="0">
<tr valign="bottom">
<td height="30" colspan="6" style="font-size:13px; font-weight:bold;" ><#SW_Available_Network#></td>
</tr>
<tr>
<td height="2" colspan="6" bgcolor="#999999"></td>
</tr>
<tr style="font-size:11px; font-weight:bold; background-color:#E4E4E4">
<td width="20">&nbsp;</td>
<td><#SW_SSID#></td>
<td width="60" align="center"><#SW_Channel#></td>
<td width="140" align="center"><#SW_Authentication#></td>
<td width="65" align="center"><#SW_RSSI#></td>
<td width="65" align="center"><div align="center">&nbsp;<#t2Status#></div></td>
</tr>
<tr>
<td height="1" colspan="6" bgcolor="#999999"></td>
</tr>
<script>
for(var i = 0; i < ap_count; i++){
document.write("<tr class=\"wl_list\">");
document.write("<td width=\"20\"><input name=\"AP_select\" type=\"radio\" value=\"",decodeURIComponent(AP_status[i][0]) ,"\"</td>");
document.write("<td>");
document.write("<span id=\"ssid", i,"\"></span>");
document.write("</td>");
document.write("<td width=\"60\" align=\"center\">", AP_status[i][1], "</td>");
if (AP_status[i][2]=="0")
document.write("<td width=\"140\" align=\"center\">", "", "</td>");
else if (AP_status[i][2]=="1")
document.write("<td width=\"140\" align=\"center\">", "WEP", "</td>");
else if (AP_status[i][2]=="2")
document.write("<td width=\"140\" align=\"center\">", "WPA-Personal", "</td>");
else if (AP_status[i][2]=="3" || AP_status[i][2]=="8")
document.write("<td width=\"140\" align=\"center\">", "WPA-Personal", "</td>");
else if (AP_status[i][2]=="4")
document.write("<td width=\"140\" align=\"center\">", "WPA2-Personal", "</td>");
else if (AP_status[i][2]=="5" || AP_status[i][2]=="9")
document.write("<td width=\"140\" align=\"center\">", "WPA2-Personal", "</td>");
else if (AP_status[i][2]=="6")
document.write("<td width=\"140\" align=\"center\">", "Unknown", "</td>");
else if (AP_status[i][2]=="7")
document.write("<td width=\"140\" align=\"center\">", "WPA / 802.1X", "</td>");
document.write("<td width=\"65\" align=\"center\"><img src=\"graph/", AP_status[i][3], ".gif\" title=\"", quick_translate_lang(AP_status[i][3], document.form.preferred_lang.value), "\"></td>");
if (AP_status[i][4]!="none")
document.write("<td width=\"65\" align=\"center\"><img src=\"graph/", AP_status[i][4], ".gif\" title=\"", quick_translate_lang(AP_status[i][4], document.form.preferred_lang.value), "\"></td>");
else
document.write("<td width=\"65\" align=\"center\">&nbsp;</td>");
document.write("</tr><tr><td height=\"1\" colspan=\"6\" bgcolor=\"#999999\"></td></tr>");
}
</script>
</table>
<p align="right">
<input class="submit2" style="cursor:pointer; background-color:#DDDDDD" type="button" value="<#CTL_add#>" name="prefer_net"  onClick="show_prefer_w_network();">
<input class="submit2" style="cursor:pointer;" type="button" value="<#PPPConnection_x_WANAction_button1name#>" name="connect"  onClick="showPop();">
<input class="submit2" style="cursor:pointer;" type="button" value="<#SW_refresh#>" name="refresh"  onClick="refreshYA();">
</p>
</td>
</tr>
</table></td>
</tr>
</table>
<div id="popupWindow">
<p>&nbsp;</p><p>&nbsp;</p><p>&nbsp;</p><p>&nbsp;</p><p>&nbsp;</p>
<table width="100%" height="100%"  border="0" cellpadding="0" cellspacing="0">
<tr>
<td width="100%" height="100%" align="center"  valign="top">
<table border="0" align="center" cellpadding="3" cellspacing="1" bgcolor="#999999">
<tr>
<td height="20" align="right"  background="graph/hint_title_bg.gif"><div align="right"><a onclick="hidePop();" href="javascript:;"><img src="graph/windows_close.gif" alt="Close this window!" width="22" height="16" border="0" align="absmiddle"/></a></div></td>
</tr>
<tr>
<td height="120" align="center" bgcolor="#F4F3D5">
<table id="wep_password_re"  width="100%" border="0" cellpadding="4" cellspacing="2">
<tr>
<td width="100"  height="20" align="right"><#BOP_account_pass_item#></td>
<td width="200"  height="20" align="left"><input name="wl_wpa_psk" type="text" class="form_text" size="32" maxlength="63"></td>
</tr>
</table>
<p><input type="button" name="Submit3" style="font-family: Verdana, Arial, Helvetica, sans-serif; font-size: 12px;" value="<#PPPConnection_x_WANAction_button1name#>" onClick="return hidePop_SP();">
<input type="button" name="concel_window" style="font-family: Verdana, Arial, Helvetica, sans-serif; font-size: 12px;" value="<#CTL_cancel#>" onclick="hidePop();"></p></td>
</tr>
</table></td>
</tr>
</table>
</div>
<div id="popupHint">
<p>&nbsp;</p><p>&nbsp;</p><p>&nbsp;</p><p>&nbsp;</p><p>&nbsp;</p>
<table width="100%" height="100%"  border="0" cellpadding="0" cellspacing="0">
<tr>
<td height="100%" align="center" valign="top">
<table width="420" height="150"  border="0" align="center" cellpadding="5" cellspacing="2" bgcolor="#999999" id="popupTable">
<tr>
<td align="right" valign="middle" bgcolor="#3F71A2"  background="graph/hint_title_bg.gif">
<table width="100%" height="100%"  border="0" cellpadding="0" cellspacing="0" >
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
<!--
<p align="center"><input type="button" name="button4" style="font-family: Verdana, Arial, Helvetica, sans-serif; font-size: 12px;" value="<#CTL_ok#>" onClick="change_nat_mode();submit_sta();show_restart();">
-->
<p align="center"><input type="button" name="button4" style="font-family: Verdana, Arial, Helvetica, sans-serif; font-size: 12px;" value="<#CTL_ok#>" onClick="submit_sta();show_restart();">
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
<div id="hint">
<table width="100%" border="0" cellspacing="2" cellpadding="5">
<tr>
<td style="font-family: Verdana, Arial, Helvetica, sans-serif; font-size: 13px;"><script>document.write(quick_translate_lang('JS_hint_autoip', document.form.preferred_lang.value));</script></td>
</tr>
<tr>
<td bgcolor="#FFFFFF"><img name="AUTOIP" id="AUTOIP" src="<% nvram_dump("AUTOIP.log",""); %>" style="border:1px solid #666;"></td>
</tr>
</table>
</div>
<div id="prefer_w_network">
<p>&nbsp;</p><p>&nbsp;</p><p>&nbsp;</p><p>&nbsp;</p><p>&nbsp;</p>
<table width="425" height="155"  border="0" align="center" cellpadding="5" cellspacing="2" bgcolor="#999999" >
<tr>
<td height="17" align="right" valign="middle"  background="graph/hint_title_bg.gif">
<table width="100%" height="100%"  border="0" cellpadding="0" cellspacing="0" >
<tr>
<td align="left" style="font-family: Verdana, Arial, Helvetica, sans-serif; font-size: 12px;"><#SW_ADD_NETWORK_TITLE#></td>
<td align="right"><a onclick="hide_prefer_w_network();"  href="javascript:;"><img src="graph/windows_close.gif" alt="Close this window!" width="22" height="16" border="0"/></a></td>
</tr>
</table></td>
</tr>
<tr>
<td colspan="2" valign="top" bgcolor="#F4F3D5">
<table width="100%" style="font-size:11px; font-family: Verdana, Arial; font-weight:bold;">
<tr>
<td width="173" align="left" valign="middle"><#WLANConfig11b_SSID_itemname#></td>
<td><input name="prefer_SSID" type="text" id="prefer_SSID" size="32" maxlength="32" class="form_text" value=""></td>
</tr>
<tr>
<td width="173" align="left" valign="middle"><#WLANConfig11b_AuthenticationMethod_itemname#></td>
<td>
<select name="prefer_security" id="prefer_security" class="form_text" onChange="Show_security_encry(this.selectedIndex)">
<option value="0" selected>Open System</option>
<option value="1">Shared Key</option>
<option value="2">WPA-Personal</option>
<option value="3">WPA2-Personal</option>
</select>
</td>
</tr>
</table>
<div id="WPA_encry" style="display:none;">
<table width="100%"  style="font-size:11px; font-family: Verdana, Arial; font-weight:bold;">
<tr>
<td width="173" align="left" valign="middle"><#WLANConfig11b_WPAType_itemname#></td>
<td>
<select name="wpa_encryption" id="wpa_encryption" class="form_text" onChange="document.getElementById('wpa_psk').focus();">
<option value="tkip" selected>TKIP</option>
<option value="aes">AES</option>
</select>
</td>
</tr>
<tr>
<td width="173" align="left" valign="middle"><#WLANConfig11b_x_PSKKey_itemname#></td>
<td><input name="wpa_psk" id="wpa_psk" type="text" maxlength="63" size="32" class="form_text" value=""></td>
</tr>
</table>
</div>
<div id="WEP_encry" style="display:none;">
<table width="100%" style="font-size:11px; font-family: Verdana, Arial; font-weight:bold;">
<tr>
<td width="173" align="left" valign="middle"><#WLANConfig11b_WEPType_itemname#></td>
<td>
<select name="select" name="wep_crypto" id="wep_crypto" class="form_text" onChange="Show_wep_entry(this.selectedIndex)">
<option value="0" selected>None</option>
<option value="1">WEP-64bits</option>
<option value="2">WEP-128bits</option>
</select>
</td>
</tr>
</table>
</div>
<div id="WEP_key" style="display:none;">
<table width="100%" style="font-size:11px; font-family: Verdana, Arial; font-weight:bold;">
<tr>
<td width="173" align="left" valign="middle"><#SW_WEP_KEY#></td>
<td><input name="wep_key_str" id="wep_key_str" type="text" maxlength="26" size="26" class="form_text" value=""></td>
</tr>
</table>
</div>
<div id="WEP_key_index" style="display:none;">
<table width="100%" style="font-size:11px; font-family: Verdana, Arial; font-weight:bold;">
<tr>
<td width="173" align="left" valign="middle"><#WLANConfig11b_WEPDefaultKey_itemname#></td>
<td>
<select name="wep_key_index_str" id="wep_key_index_str" class="form_text">
<option value="1" selected>1</option>
<option value="2">2</option>
<option value="3">3</option>
<option value="4">4</option>
</select>
</td>
</tr>
</table>
</div>
<p align="center">
<input type="button" style="font-family: Verdana, Arial, Helvetica, sans-serif; font-size: 12px;" name="button4" value="<#PPPConnection_x_WANAction_button1name#>" onClick="hidePop_SP2_re();">
<input type="button" style="font-family: Verdana, Arial, Helvetica, sans-serif; font-size: 12px;" name="concel_window2" value="<#CTL_cancel#>" onClick="hide_prefer_w_network();">
</p>
</td>
</tr>
</table>
</div>
<script language="javascript" type="text/javascript">
function show_prefer_w_network()
{document.getElementById("prefer_w_network").style.visibility="visible";
document.getElementById('prefer_SSID').focus();
}
function hide_prefer_w_network()
{document.getElementById("prefer_w_network").style.visibility="hidden";
}
function Show_security_encry(x){
if(x == 0 || x == 1)
{document.getElementById('WEP_encry').style.display = 'block';
document.getElementById('WEP_key').style.display = 'block';
document.getElementById('WEP_key_index').style.display = 'block';
document.getElementById('WPA_encry').style.display = 'none';
if(x==1)
{algos=new Array("WEP-64bits", "WEP-128bits");
document.getElementById('wep_crypto').length=algos.length;
for (var i in algos)
{document.getElementById('wep_crypto')[i]=new Option(algos[i], algos[i]);
document.getElementById('wep_crypto')[i].value=eval(i)+1;
}
document.getElementById('wep_crypto')[0].selected=true;
}
else
{algos=new Array("None", "WEP-64bits", "WEP-128bits");
document.getElementById('wep_crypto').length=algos.length;
for (var i in algos)
{document.getElementById('wep_crypto')[i]=new Option(algos[i], algos[i]);
document.getElementById('wep_crypto')[i].value=""+i;
}
document.getElementById('wep_crypto')[0].selected=true;
}
Show_wep_entry(document.getElementById('wep_crypto').selectedIndex);
}
else if(x == 2 || x == 3)
{document.getElementById('WEP_encry').style.display = 'none';
document.getElementById('WEP_key').style.display = 'none';
document.getElementById('WEP_key_index').style.display = 'none';
document.getElementById('WPA_encry').style.display = 'block';
document.getElementById('wpa_psk').focus();
}
}
function Show_wep_entry(x){
if(document.getElementById('wep_crypto').length==3)
{if(x == 0)
{document.getElementById('WEP_key').style.display = 'none';
document.getElementById('WEP_key_index').style.display = 'none';
}
else if(x == 1 || x == 2)
{document.getElementById('WEP_key').style.display = 'block';
document.getElementById('WEP_key_index').style.display = 'block';
document.getElementById('wep_key_str').focus();
}
}
else
{document.getElementById('WEP_key').style.display = 'block';
document.getElementById('WEP_key_index').style.display = 'block';
document.getElementById('wep_key_str').focus();
}
}
</script>
</body>
</html>
