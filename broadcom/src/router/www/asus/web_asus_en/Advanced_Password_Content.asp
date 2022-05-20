<head>
<title><#ZVMODELVZ#> Web Manager</title>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<meta http-equiv="Pragma" content="no-cache">
<link rel="stylesheet" type="text/css" href="style.css" media="screen">
<link href="select_style.css" rel="stylesheet" type="text/css">
<script language="JavaScript" type="text/javascript" src="overlib.js"></script>
<script language="JavaScript" type="text/javascript" src="general.js"></script>
</head>
<body bgcolor="#FFFFFF"  onLoad="load_body()">
<div id="overDiv" style="position:absolute; visibility:hidden; z-index:1000;"></div>
<form method="GET" name="form" action="apply.cgi">
<input type="hidden" name="action_mode" value="">
<input type="hidden" name="preferred_lang2" value="<% nvram_get_x("","preferred_lang"); %>">
<!-- Table for the conntent page -->
<table width="660" border="0" cellpadding="1" cellspacing="0">
<tr>
<td>
<table width="666" border="2" cellpadding="0" cellspacing="0" bordercolor="E0E0E0">
<tr class="content_header_tr">
<td class="content_header_td_title" colspan="2"><#t1SYS#> - <#t2Pass#></td>
<input type="hidden" name="current_page" value="Advanced_Password_Content.asp">
<input type="hidden" name="next_page" value="SaveRestart.asp">
<input type="hidden" name="next_host" value="">
<input type="hidden" name="sid_list" value="General;">
<input type="hidden" name="group_id" value="">
<input type="hidden" name="modified" value="0">
<input type="hidden" name="preferred_lang" value="<% nvram_get_x("LANGUAGE", "preferred_lang");  %>">
</tr>
<tr>
<td class="content_desc_td" colspan="2" width="614">
<p style="margin-top: 7">
<#PASS_desc#>
<p style="margin-bottom: 7">
</td>
</tr>
<tr class="content_section_header_tr">
<td id="Mode" class="content_section_header_td" colspan="2" width="614"><#PASS_changepasswd#></td>
</tr>
<tr>
<td class="content_header_td" width="34"><#PASS_new#></td>
<td class="content_input_td" width="310">
<input type="password" maxlength="16" size="16" name="http_passwd" onBlur="return validate_string(this)" onKeyPress="return is_string(this)" onChange="page_changed()">
</td>
</tr>
<tr>
<td class="content_header_td" width="34"><#PASS_retype#></td>
<td class="content_input_td" width="310">
<input type="password" maxlength="16" size="16" name="v_password" onBlur="return validate_string(this)" onKeyPress="return is_string(this)" onChange="page_changed()">
</td>
</tr>
<tr class="content_section_header_tr">
<td id="Mode" class="content_section_header_td" colspan="2" width="614"><#PASS_changelang#></td>
</tr>
<tr>
<td class="content_desc_td" colspan="2" width="614">
<p style="margin-top: 7">
<#PASS_langdesc#>
<p style="margin-bottom: 7">
</td>
</tr>
<tr>
<td class="content_header_td"><#PASS_LANG#>
<td class="content_input_td">
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
document.form.preferred_lang.value = lang;
document.getElementById('selectMenucontent').style.display = 'none'
isMenuopen = 0;
}
</script>
<!--
<select name="preferred_lang" class="content_input_fd" onChange="return change_common(this, 'LANGUAGE', 'preferred_lang')">
<option class="content_input_fd" value="EN" <% nvram_match_x("LANGUAGE","preferred_lang", "EN","selected"); %>>
English
</option><option class="content_input_fd" value="TW" <% nvram_match_x("LANGUAGE","preferred_lang", "TW","selected"); %>>
<script>
if (document.form.lang_cur.value=="EN" || document.form.lang_cur.value=="DE")
document.write("<#LANG_TW#> (<#LANG_UNI_TW#>)");
else
document.write("<#LANG_UNI_TW#>");
</script>
</option><option class="content_input_fd" value="CN" <% nvram_match_x("LANGUAGE","preferred_lang", "CN","selected"); %>>
<script>
if (document.form.lang_cur.value=="EN" || document.form.lang_cur.value=="DE")
document.write("<#LANG_CN#> (<#LANG_UNI_CN#>)");
else
document.write("<#LANG_UNI_CN#>");
</script>
</option><option class="content_input_fd" value="DE" <% nvram_match_x("LANGUAGE","preferred_lang", "DE","selected"); %>>
Deutsch</option>
</select>
-->
</td>
</tr>
<tr>
<td class="content_input_td_less" height="60" colspan="2">
<p align="right">
<input class="inputSubmit" style="cursor:pointer;" onMouseOut="buttonOut(this)" onMouseOver="buttonOver(this)" type="submit" value="<#CTL_onlysave#>" name="action" onClick="return checkPass(this, document.form.http_passwd, document.form.v_password);">&nbsp;<input class="inputSubmit" style="cursor:pointer;" onMouseOut="buttonOut(this)" onMouseOver="buttonOver(this)" type="reset" value="<#CTL_clear#>" name="reset">
</p>
</td>
</tr>
</table>
</td>
</tr>
</table>
</form>
</body>
