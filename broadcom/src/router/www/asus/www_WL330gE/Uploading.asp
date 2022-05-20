<html>
<head>
<title><#ZVMODELVZ#></title>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<meta http-equiv="Pragma" content="no-cache">
<script language="javascript">
var count=0;
var time=16;
function rebootClock() {
ptext = ""+Math.round(count*100/time).toString()+" \%";
count++;
if (count<=time)
document.getElementById("parea").innerHTML = ptext;
else
{document.getElementById("parea").innerHTML = "100 \%";
top.location="/";
}
}
function change_time() {
if (document.form.mode_ex.value!="ap")
time=18;
}
</script>
</head>
<body onLoad="change_time()" bgcolor="#FFFFFF">
<form method="GET" name="form" action="apply.cgi">
<input type="hidden" name="mode_ex" value="<% nvram_get_x("","wl_mode_ex"); %>">
<input type="hidden" name="nat_x" value="<% nvram_get_x("","wan_nat_x"); %>">
<table width="666" border="0" cellpadding="2" cellspacing="0">
<tr bgcolor="#CCCCCC">
<td width="666" height="125">
<table width="662" height="125" cellpadding="2" cellspacing="0" border="0">
<tbody>
<tr>
<td bgcolor="#FF0000" height="50" width="652"><b><font size="5" face="Arial" color="#FFFFFF"><#SET_ok_title#></font></b></td>
</tr>
<tr>
<td width="652" height="25" bgcolor="#FFFFFF" colspan="2"><font face="Arial"><#SET_ok_desc#></font></td>
</tr>
<tr>
<td width="652" height="25" bgcolor="#FFFFFF" colspan="2"><font face="Arial"><#FIRM_progress_desc#><img src=width=0 height=0 onerror=setInterval('rebootClock()',1000)><span id="parea" style="position:relative;">0 %</span></font></td>
</tr>
<tr>
<td width="652" height="25" bgcolor="#FFFFFF" colspan="2"><font face="Arial">
<font color="#FF0000"><b><#SET_note#></b></font></font></td>
</tr>
</tbody>
</table>
</td>
</tr>
</table>
</form>
</body>
</html>
