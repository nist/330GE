<html>
<head>
<title><#ZVMODELVZ#> Web Manager</title>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<link rel="stylesheet" type="text/css" href="style.css" media="screen"></link>
<script language="JavaScript" type="text/javascript" src="overlib.js"></script>
<script language="JavaScript" type="text/javascript" src="general.js"></script>
<script language=JavaScript>
function setSSID(o1, o2, o3)
{flag_close=1;
if (o2=="0")
{top.opener.document.change = 1;
top.opener.window.top.pageChanged = 1;
top.opener.document.forms[0].wl_ssid.value = decodeURIComponent(o1);
top.opener.document.forms[0].wl_auth_mode.value = "open";
top.opener.document.forms[0].wl_auth_mode.onchange();
top.opener.document.forms[0].wl_wep_x.value = "0";
top.opener.document.forms[0].wl_wep_x.onchange();
}
else if (o2=="128")
{top.opener.document.change = 1;
top.opener.window.top.pageChanged = 1;
top.opener.document.forms[0].wl_ssid.value = decodeURIComponent(o1);
top.opener.document.forms[0].wl_auth_mode.value = "open";
top.opener.document.forms[0].wl_auth_mode.onchange();
top.opener.document.forms[0].wl_wep_x.value = "1";
top.opener.document.forms[0].wl_wep_x.onchange();
}
else
{if (o2=="1" || o2=="64")
{if (top.opener.parent.titleFrame.document.form.wl_mode_ex.value != 'ap')
{flag_close=0;
alert(translate_lang('JS_nowpa',document.form.preferred_lang.value));
}
else
{top.opener.document.change = 1;
top.opener.window.top.pageChanged = 1;
top.opener.document.forms[0].wl_ssid.value = decodeURIComponent(o1);
top.opener.document.forms[0].wl_auth_mode.value = "wpa";
top.opener.document.forms[0].wl_auth_mode.onchange();
}
}
else if (o2=="2" || o2=="32")
{top.opener.document.change = 1;
top.opener.window.top.pageChanged = 1;
top.opener.document.forms[0].wl_ssid.value = decodeURIComponent(o1);
if (o2=="2")
top.opener.document.forms[0].wl_auth_mode.options[2].selected = 1;
else
top.opener.document.forms[0].wl_auth_mode.options[3].selected = 1;
top.opener.document.forms[0].wl_auth_mode.onchange();
}
else if (o2=="4")
;
else if (o2=="8")
{flag_close=0;
alert(translate_lang('JS_no8021X',document.form.preferred_lang.value));
}
else
;
if (o3=="1")
;
else if (o3=="2")
;
else if (o3=="4")
;
else if (o3=="8")
{top.opener.document.forms[0].wl_crypto.value = "tkip";
top.opener.document.forms[0].wl_crypto.onchange();
}
else if (o3=="16")
{top.opener.document.forms[0].wl_crypto.value = "aes";
top.opener.document.forms[0].wl_crypto.onchange();
}
else if (o3=="24")
{if (document.form.modeEX.value=="re")
top.opener.document.forms[0].wl_crypto.value = "tkip";
else
top.opener.document.forms[0].wl_crypto.value = "aes";
top.opener.document.forms[0].wl_crypto.onchange();
}
else
;
}
if (flag_close==1)
self.parent.close();
}
function loadRSSIDs()
{if (document.form.apinfo_n.value=="0")
return;
for(i=0;i<eval(document.form.apinfo_n.value);i++)
{if (i==0 || document.form.RSSIDE[i]!=null)
{if (i==0 && document.form.apinfo_n.value=="1")
document.form.RSSID.value=decodeURIComponent(document.form.RSSIDE.value);
else
document.form.RSSID[i].value=decodeURIComponent(document.form.RSSIDE[i].value);
}
}
}
</script>
</head>
<body onLoad="loadRSSIDs()">
<form method="GET" name="form" action="apply.cgi">
<input type="hidden" name="current_page" value="Advanced_WirelessAPScan_Content.asp">
<input type="hidden" name="next_page" value="">
<input type="hidden" name="next_host" value="">
<input type="hidden" name="sid_list" value="">
<input type="hidden" name="group_id" value="">
<input type="hidden" name="modified" value="0">
<input type="hidden" name="action_mode" value="">
<input type="hidden" name="first_time" value="">
<input type="hidden" name="action_script" value="">
<input type="hidden" name="preferred_lang" value="<% nvram_get_x("","preferred_lang"); %>">
<input type="hidden" name="modeEX" value="<% nvram_get_x("","wl_mode_ex"); %>">
<table width="614" border="0" cellpadding="0" cellspacing="0">
<tr>
<td>
<table table width="100%" border="1" cellpadding="0" cellspacing="0" bordercolor="E0E0E0">
<tr class="content_list_header_tr">
<td class="content_list_header_td" width="65%"><#WLANAPScan_groupitemdesc#></td>
<td width="15%">
<div align="center">
<input class="inputSubmit" style="cursor:pointer; font-weight:normal;" type="submit" onClick="document.form.next_host.value=location.host; onSubmitCtrl(this, ' Refresh ');" value="<#CTL_refresh#>" size="12">
</div>
</td>
<td width="15%">
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
<td width="17%" align="center" class="content_header_td_ap">BSSID</td>
<td width="38%" align="center" class="content_header_td_ap"><#SW_SSID#></td>
<td width="25%" align="center" class="content_header_td_ap"><#SW_Authentication#></td>
<td width="10%" align="center" class="content_header_td_ap"><#SW_Channel#></td>
<td width="10%" align="center" class="content_header_td_ap"><#SW_RSSI#></td>
</tr>
<% nvram_dump("apscan.log",""); %>
</table>
</td>
</tr>
</table>
</form>
</body>
</html>
