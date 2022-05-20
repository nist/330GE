<html>
<head>
<title><#ZVMODELVZ#></title>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<meta http-equiv="Pragma" content="no-cache">
</head>
<script language="JavaScript" type="text/javascript">
function redirect()
{top.location="/";
}
</script>
<body bgcolor="#FFFFFF" onLoad="setTimeout('redirect()', 19000);">
<form method="GET" name="form" action="apply.cgi">
<input type="hidden" name="wl_mode_EX" value="<% nvram_get_x("","wl_mode_ex"); %>">
<input type="hidden" name="wan_nat_X" value="<% nvram_get_x("","wan_nat_x"); %>">
<table width="666" border="0" cellpadding="2" cellspacing="0">
<tr bgcolor="#CCCCCC">
<td width="666" height="125">
<table width="662" height="125" border="0" cellpadding="2" cellspacing="0">
<tbody>
<tr>
<td bgcolor="#FF0000" height="50" width="652"><b><font size="5" face="Arial" color="#FFFFFF"><#FIRM_ok_title#></font></b></td>
</tr>
<tr>
<td width="652" height="25" bgcolor="#FFFFFF" colspan="2"><font face="Arial"><#FIRM_ok_desc#></font></td>
</tr>
<tr>
<td width="652" height="25" bgcolor="#FFFFFF" colspan="2"><font face="Arial">
<font color="#FF0000"><b><#FIRM_note#></b></font></font></td>
</tr>
</tbody>
</table>
</td>
</tr>
</table>
</form>
</body>
</html>
