<html>
<head>
<title><#ZVMODELVZ#></title>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<meta http-equiv="Pragma" content="no-cache">
<script language=JavaScript>
function loadBody()
{}
function redirect()
{window.location.href="/";
/*
if (document.formname.wan_nat_X.value=="1")
window.location.href="Wizard_Gateway.asp";
else
{if (document.formname.wl_mode_EX.value=="ap")
window.location.href="Wizard_AP.asp";
else if (document.formname.wl_mode_EX.value=="sta")
window.location.href="Wizard_Adapter.asp";
else if (document.formname.wl_mode_EX.value=="re")
window.location.href="Wizard_Repeater.asp";
else
window.location.href="index.asp";
}
*/
}
function redirect1()
{top.location="WizardDetect.asp";
}
function buttonOver(o)
{o.style.backgroundColor = "#FFFFCC";
}
function buttonOut(o) {
o.style.backgroundColor = "#C0C0C0";
}
</script>
</head>
<body bgcolor="#FFFFFF" onload="loadBody()">
<form name="formname" method="POST">
<input type="hidden" name="wl_mode_EX" value="<% nvram_get_x("","wl_mode_EX"); %>">
<input type="hidden" name="wan_nat_X" value="<% nvram_get_x("","wan_nat_X"); %>">
<table width="420" border="0" cellpadding="0" cellspacing="0">
<tr bgcolor="#FFFFFF">
<td width="420" height="125">
<table width="420" height="125" border="0" cellpadding="0" cellspacing="0" border="0">
<tbody>
<tr>
<td bgcolor="#FFFFFF" height="50" colspan="2"><b><font size="5" face="Arial" color="#FF0000"><#STS_title#></font></b></td>
</tr>
<tr>
<td height="30"><font face="Arial" color="#000000"><#STS_type#></font></td>
<td><font face="Arial" color="#000000"><input type="text" size="36" name="PPPConnection_x_WANType" value="<% nvram_get_x("Layer3Forwarding","wan_proto_t"); %>" readonly="1"></font></td>
</tr>
<tr>
<td height="30"><font face="Arial" color="#000000"><#STS_status#>:</font></td>
<td><font face="Arial" color="#000000"><input type="text" size="36" name="PPPConnection_x_WANLink" value="<% nvram_get_f("wanstatusshow.log","wan_status_t"); %>" readonly="1"></font></td>
</tr>
<tr>
<td height="30"><font face="Arial" color="#000000"><#STS_reason#></font></td>
<td><font face="Arial" color="#000000">
<input type="text" size="36" name="PPPConnection_x_WANStatus" value="<% nvram_get_f("wanstatusshow.log","wan_reason_t"); %>" readonly="1">
<input type="hidden" size="36" name="PPPConnection_x_WANStatusCode" value="<% nvram_get_f("wanstatusshow.log","wan_reasoncode_t"); %>" readonly="1">
</font></td>
</tr>
<tr>
<td width="420" height="30" bgcolor="#FFFFFF" colspan="2">
</td>
</tr>
<tr>
<td align="right" width="420" height="60" bgcolor="#FFFFFF" colspan="2">
<p><input class=inputSubmit style="cursor:pointer;" onMouseOut=buttonOut(this) onMouseOver=buttonOver(this) type="button" onClick="redirect()" value="<#CTL_login#>" name="action"></p>
</td>
</tr>
</tbody>
</table>
</td>
</tr>
</table>
</form>
</body>
</html>
