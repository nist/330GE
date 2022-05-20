<html>
<head>
<title><#ZVMODELVZ#> Web Manager</title>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<link rel="stylesheet" type="text/css" href="style.css" media="screen"></link>
<script language="JavaScript" type="text/javascript" src="overlib.js"></script>
<script language="JavaScript" type="text/javascript" src="general.js"></script>
<script language=JavaScript>
function copyBSSID(o)
{if (o!=null && o.length!=0)
{BSSID_new="";
for (i=0;i<o.length;i++)
if (o.charAt(i)!=':')
BSSID_new+=o.charAt(i);
top.opener.document.forms[0].wl_wdslist_x_0.value=BSSID_new;
self.parent.close();
}
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
<input type="hidden" name="current_page" value="Advanced_WirelessAPScan2_Content.asp">
<input type="hidden" name="next_page" value="">
<input type="hidden" name="next_host" value="">
<input type="hidden" name="sid_list" value="">
<input type="hidden" name="group_id" value="">
<input type="hidden" name="modified" value="0">
<input type="hidden" name="action_mode" value="">
<input type="hidden" name="first_time" value="">
<input type="hidden" name="action_script" value="">
<input type="hidden" name="preferred_lang" value="<% nvram_get_x("","preferred_lang"); %>">
<table width="560" border="0" cellpadding="0" cellspacing="0">
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
<td width="21%" align="center" class="content_header_td_ap">BSSID</td>
<td width="29%" align="center" class="content_header_td_ap"><#SW_SSID#></td>
<td width="27%" align="center" class="content_header_td_ap"><#SW_Authentication#></td>
<td width="11%" align="center" class="content_header_td_ap"><#SW_Channel#></td>
<td width="11%" align="center" class="content_header_td_ap"><#SW_RSSI#></td>
</tr>
<% nvram_dump("apselect.log",""); %>
</table>
</td>
</tr>
</table>
</form>
</body>
</html>
