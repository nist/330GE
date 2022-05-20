<html>
<head>
<title><#ZVMODELVZ#></title>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<meta http-equiv="Expires" content="Fri, Jun 12 1981 00:00:00 GMT">
<meta http-equiv="Cache-control" content="no-cache,must-revalidate">
<meta http-equiv="Pragma" content="no-cache">
<input type="hidden" name="preferred_lang" value="<% nvram_get_x("","preferred_lang"); %>">
<link rel="stylesheet" type="text/css" href="style.css" media="screen">
<script language="JavaScript" type="text/javascript" src="overlib.js"></script><script language="JavaScript" type="text/javascript" src="general.js"></script>
</head>
<div id="overDiv" style="position:absolute; visibility:hidden; z-index:1000;"></div>
<body>
<form method="get" name="form" action="apply.cgi">
<input type="hidden" name="wan_route_x" value="<% nvram_get_x("IPConnection","wan_route_x"); %>">
<input type="hidden" name="wan_nat_x" value="<% nvram_get_x("IPConnection","wan_nat_x"); %>">
<input type="hidden" name="wan_nat_X" value="<% nvram_get_x("","wan_nat_X"); %>">
<input type="hidden" name="productid" value="<% nvram_get_f("general.log","productid"); %>">
<input type="hidden" name="productid_odm" value="<% nvram_get_x("", "productid_odm"); %>">
<input type="hidden" name="support_cdma" value="<% nvram_get_x("IPConnection","support_cdma"); %>">
<input type="hidden" name="wl_mode_ex" value="<% nvram_get_x("WLANConfig11b","wl_mode_ex"); %>">
<input type="hidden" name="wl_mode_EX" value="<% nvram_get_x("","wl_mode_EX"); %>">
<input type="hidden" name="preferred_lang" value="<% nvram_get_x("","preferred_lang"); %>">
<table class="content_table" border="0" cellspacing="0" cellpadding="0">
<tr>
<td>
<script>
if (document.form.productid_odm.value.length==0)
document.write("<img src=\"./graph/iBox_title_all.jpg\" width=\"666\" height=\"80\">");
else
document.write("<img src=\"./graph/iBox_title_default.gif\" width=\"666\" height=\"80\">");
</script>
</td>
</tr>
</table>
</form>
</body>
</html>
