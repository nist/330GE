<head>
<title><#ZVMODELVZ#> Web Manager</title>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<meta http-equiv="Pragma" content="no-cache">
<link rel="stylesheet" type="text/css" href="style.css" media="screen">
<script language="JavaScript" type="text/javascript" src="overlib.js"></script>
<script language="JavaScript" type="text/javascript" src="general.js"></script>
<!--
<script language="JavaScript" type="text/javascript" src="quick.js"></script>
-->
</head>
<body bgcolor="#FFFFFF" onLoad="loadMode_SP()">
<div id="overDiv" style="position:absolute; visibility:hidden; z-index:1000;"></div>
<form method="GET" name="form" action="apply.cgi">
<!-- Table for the conntent page -->
<table width="660" border="0" cellpadding="0" cellspacing="0">
<input type="hidden" name="current_page" value="Advanced_OperationMode_Content.asp">
<!--
<input type="hidden" name="next_page" value="Basic_SaveRestart.asp">
-->
<input type="hidden" name="next_page" value="Main_Operation2.asp">
<input type="hidden" name="next_host" value="">
<input type="hidden" name="action_mode" value="">
<input type="hidden" name="sid_list" value="Layer3Forwarding;IPConnection;WLANConfig11b;">
<input type="hidden" name="wan_proto" value="<% nvram_get_x("Layer3Forwarding","wan_proto"); %>">
<input type="hidden" name="wan_route_x" value="<% nvram_get_x("IPConnection","wan_route_x"); %>">
<input type="hidden" name="wan_nat_x" value="<% nvram_get_x("IPConnection","wan_nat_x"); %>">
<input type="hidden" name="preferred_lang" value="<% nvram_get_x("","preferred_lang"); %>">
<input type="hidden" name="wl_mode_ex" value="<% nvram_get_x("WLANConfig11b","wl_mode_ex"); %>">
<input type="hidden" name="wl_guest_enable" value="<% nvram_get_x("WLANConfig11b","wl_guest_enable"); %>">
<input type="hidden" name="mssid_enable" value="<% nvram_get_x("WLANConfig11b","mssid_enable"); %>">
<input type="hidden" name="wl_individual_ure" value="<% nvram_get_x("WLANConfig11b","wl_individual_ure"); %>">
<tr>
<td>
<table width="666" border="2" cellpadding="0" cellspacing="0" bordercolor="E0E0E0">
<tr class="content_header_tr">
<td class="content_header_td_title" colspan="2"><#t1SYS#> - <#t2OP#></td>
</tr>
<tr>
<td class="content_desc_td" colspan="2">
<#OP_desc1#>
</td>
</tr>
<tr>
<td class="content_header_td_30"><input type="radio" name="x_Mode"> <#OP_GW_item#></td>
<td class="content_desc_td">
<#OP_GW_desc1#>
<p></p>
<#OP_GW_desc2#>
</td>
</tr>
<tr>
<td class="content_header_td_30"><input type="radio" name="x_Mode" value="V3"> <#OP_AP_item#></td>
<td class="content_desc_td">
<#OP_AP2_desc1#>
<p></p>
<#OP_AP2_desc2#>
</td>
</tr>
<tr>
<td class="content_input_td_less" colspan="2" height="60">
<p align="right">
<input class="inputSubmit" style="cursor:pointer;" onMouseOut="buttonOut(this)" onMouseOver="buttonOver(this)" type="submit" value="<#CTL_apply#>" name="action" onClick="saveMode_SP(this)">&nbsp;
</p>
</td>
</tr></table>
</td>
</tr>
</table>
</form>
</body>
