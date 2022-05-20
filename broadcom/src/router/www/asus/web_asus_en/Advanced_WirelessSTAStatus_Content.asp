<html>
<head>
<title><#ZVMODELVZ#> Web Manager</title>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<link rel="stylesheet" type="text/css" href="style.css" media="screen"></link>
<script language="JavaScript" type="text/javascript" src="overlib.js"></script>
<script language="JavaScript" type="text/javascript" src="general.js"></script>
<script language=JavaScript>
function redirect() {
setTimeout("redirect1()", 3000)
}
function redirect1() {
document.form.refresh.click();
}
</script>
</head>
<body onLoad="load_body();redirect();">
<form method="GET" name="form" action="apply.cgi">
<input type="hidden" name="current_page" value="Advanced_WirelessSTAStatus_Content.asp">
<input type="hidden" name="next_page" value="">
<input type="hidden" name="next_host" value="">
<input type="hidden" name="sid_list" value="">
<input type="hidden" name="group_id" value="">
<input type="hidden" name="modified" value="0">
<input type="hidden" name="action_mode" value="">
<input type="hidden" name="first_time" value="">
<input type="hidden" name="action_script" value="">
<input type="hidden" name="preferred_lang" value="<% nvram_get_x("","preferred_lang"); %>">
<input type="hidden" name="ssid2" value="<% nvram_get_x("","wl0_ssid2"); %>">
<table width="330" border="0" cellpadding="0" cellspacing="0">
<tr>
<td>
<table table width="100%" border="1" cellpadding="0" cellspacing="0" bordercolor="E0E0E0">
<tr class="content_list_header_tr">
<td class="content_list_header_td" width="59%"><#WLANSTAStatus_groupitemdesc#></td>
<td width="18%">
<div align="center">
<input class="inputSubmit" style="cursor:pointer; font-weight:normal;" type="submit" onClick="document.form.next_host.value=location.host; onSubmitCtrl(this, ' Refresh ');" name="refresh" value="<#CTL_refresh#>" size="12">
</div>
</td>
<td width="18%">
<div align="center">
<input class="inputSubmit" style="cursor:pointer; font-weight:normal;" type="button" onClick="self.parent.close()" value="<#CTL_close#>" size="12">
</div>
</td>
<td width="5%">
&nbsp;
</td>
</tr>
</table>
</td>
</tr>
<tr>
<td>
<table table width="100%" border="1" cellpadding="0" cellspacing="0" bordercolor="E0E0E0">
<tr>
<td width="30%" align="center" class="content_header_td_ap">Network</td>
<td width="70%" class="content_header_td_ap2">
<input type="text" maxlength="32" class="content_input_fd" size="32" name="ssid" value="" readonly="1">
</td>
</tr>
<tr>
<td width="30%" align="center" class="content_header_td_ap">Status</td>
<td width="70%" class="content_header_td_ap2"><% nvram_get_x("", "wl_sta_state"); %></td>
</tr>
</table>
</td>
</tr>
</table>
</form>
</body>
</html>
