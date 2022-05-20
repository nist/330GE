<head>
<title><#ZVMODELVZ#> Web Manager</title>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<meta http-equiv="Pragma" content="no-cache">
<link rel="stylesheet" type="text/css" href="style.css" media="screen">
<script language="JavaScript" type="text/javascript" src="overlib.js"></script>
<script language="JavaScript" type="text/javascript" src="general.js"></script>
<script language=JavaScript>
function checkSNMPpass()
{
	return (checkPass2(this, document.form.Community, document.form.v_Community) &&
		checkPass2(this, document.form.readOnly, document.form.v_readOnly) &&
		checkPass2(this, document.form.v3_auth_passwd, document.form.v_v3_auth_passwd) &&
		checkPass2(this, document.form.v3_priv_passwd, document.form.v_v3_priv_passwd) &&
		validate_string_length(this, document.form.v3_auth_passwd, 8) &&
		validate_string_length(this, document.form.v3_priv_passwd, 8)
	);
}
</script>
</head>

<body bgcolor="#FFFFFF"  onLoad="load_body()">
<div id="overDiv" style="position:absolute; visibility:hidden; z-index:1000;"></div>    
<form method="GET" name="form" action="apply.cgi">

<input type="hidden" name="current_page" value="Advanced_SNMPConfig_Content.asp">
<input type="hidden" name="next_page" value="SaveRestart.asp">
<input type="hidden" name="next_host" value="">
<input type="hidden" name="sid_list" value="SNMP;">
<input type="hidden" name="group_id" value="">
<input type="hidden" name="modified" value="0">
<input type="hidden" name="action_mode" value="">
<input type="hidden" name="preferred_lang" value="<% nvram_get_x("","preferred_lang"); %>">
<input type="hidden" name="preferred_lang2" value="<% nvram_get_x("","preferred_lang"); %>">

<!-- Table for the conntent page -->	    
<table width="660" border="0" cellpadding="1" cellspacing="0">     	      
    	
<tr>
<td>
<table width="666" border="2" cellpadding="0" cellspacing="0" bordercolor="E0E0E0">
<tr class="content_header_tr">
<td class="content_header_td_title" colspan="2"><#t1SYS#> - <#t2SNMP#>
</td>
</tr>
<tr>
	<td class="content_desc_td" colspan="2" width="614">
		<#SNMP_desc#>
	</td>
</tr>
<tr>
	<td class="content_header_td" width="34">
		<#SNMP_sysName#>
	</td>
	<td class="content_input_td" width="310">
		<input type="text" maxlength="255" size="32" name="sysName" value="<% nvram_get_x("SNMP", "sysName"); %>" onChange="page_changed()" onKeyPress="return is_string(this)" onBlur="validate_string(this)">
	</td>
</tr>
<tr>
	<td class="content_header_td" width="34">
		<#SNMP_sysLocation#>
	</td>
	<td class="content_input_td" width="310">
		<input type="text" maxlength="255" size="32" name="sysLocation" value="<% nvram_get_x("SNMP", "sysLocation"); %>" onChange="page_changed()" onKeyPress="return is_string(this)" onBlur="validate_string(this)">
	</td>
</tr>
<tr>
	<td class="content_header_td" width="34">
		<#SNMP_sysContact#>
	</td>
	<td class="content_input_td" width="310">
		<input type="text" maxlength="255" size="32" name="sysContact" value="<% nvram_get_x("SNMP", "sysContact"); %>" onChange="page_changed()" onKeyPress="return is_string(this)" onBlur="validate_string(this)">
	</td>
</tr> 

<tr class="content_section_header_tr">
	<td id="Mode" class="content_section_header_td" colspan="2" width="614">
		<#SNMP_changecommunity#>
	</td>
</tr>

<tr>
	<td class="content_header_td" width="34">
		<#SNMP_community_new#>
	</td>
	<td class="content_input_td" width="310">
		<input type="password" maxlength="16" size="16" name="Community" value="<% nvram_get_x("SNMP", "Community"); %>" onChange="page_changed()" onKeyPress="return is_string(this)" onBlur="validate_string(this)">
	</td>
</tr>
<tr>
	<td class="content_header_td" width="34">
		<#SNMP_community_retry#>
	</td>
	<td class="content_input_td" width="310">
		<input type="password" maxlength="16" size="16" name="v_Community" value="<% nvram_get_x("SNMP", "Community"); %>" onChange="page_changed()" onKeyPress="return is_string(this)" onBlur="validate_string(this)">
	</td>
</tr> 

<tr class="content_section_header_tr">
	<td id="Mode" class="content_section_header_td" colspan="2" width="614">
		<#SNMP_changereadOnly#>
	</td>
</tr>

<tr>
	<td class="content_header_td" width="34">
		<#SNMP_readOnly_new#>
	</td>
	<td class="content_input_td" width="310">
		<input type="password" maxlength="16" size="16" name="readOnly" value="<% nvram_get_x("SNMP", "readOnly"); %>" onChange="page_changed()" onKeyPress="return is_string(this)" onBlur="validate_string(this)">
	</td>
</tr>
<tr>
	<td class="content_header_td" width="34">
		<#SNMP_readOnly_retry#>
	</td>
	<td class="content_input_td" width="310">
		<input type="password" maxlength="16" size="16" name="v_readOnly" value="<% nvram_get_x("SNMP", "readOnly"); %>" onChange="page_changed()" onKeyPress="return is_string(this)" onBlur="validate_string(this)">
	</td>
</tr>

<tr class="content_section_header_tr">
	<td id="Mode" class="content_section_header_td" colspan="2" width="614">
		<#SNMP_Auth#>
	</td>
</tr>

<tr>
	<td class="content_header_td" width="34">
		<#SNMP_Auth_Type#>
	</td>
	<td class="content_input_td" width="310">
		<select name="v3_auth_type" class="content_input_fd" onChange="return change_common(this, 'SNMP', 'v3_auth_type')">
			<option class="content_input_fd" value="MD5" <% nvram_match_x("SNMP","v3_auth_type", "MD5","selected"); %>>MD5</option>
			<option class="content_input_fd" value="SHA" <% nvram_match_x("SNMP","v3_auth_type", "SHA","selected"); %>>SHA</option>
		</select>
	</td>
</tr>
<tr>
	<td class="content_header_td" width="34">
		<#SNMP_Auth_Passwd_new#>
	</td>
	<td class="content_input_td" width="310">
		<input type="password" maxlength="64" size="32" name="v3_auth_passwd" value="<% nvram_get_x("SNMP", "v3_auth_passwd"); %>" onChange="page_changed()" onKeyPress="return is_string(this)" onBlur="validate_string0(this);validate_string_length(this, 8);">
	</td>
</tr>
<tr>
	<td class="content_header_td" width="34">
		<#SNMP_Auth_Passwd_retry#>
	</td>
	<td class="content_input_td" width="310">
		<input type="password" maxlength="64" size="32" name="v_v3_auth_passwd" value="<% nvram_get_x("SNMP", "v3_auth_passwd"); %>" onChange="page_changed()" onKeyPress="return is_string(this)" onBlur="validate_string0(this);validate_string_length(this, 8);">
	</td>
</tr>

<tr class="content_section_header_tr">
	<td id="Mode" class="content_section_header_td" colspan="2" width="614">
		<#SNMP_Priv#>
	</td>
</tr>

<tr>
	<td class="content_header_td" width="34">
		<#SNMP_Priv_Type#>
	</td>
	<td class="content_input_td" width="310">
		<select name="v3_priv_type" class="content_input_fd" onChange="return change_common(this, 'SNMP', 'v3_priv_type')">
			<option class="content_input_fd" value="NONE" <% nvram_match_x("SNMP","v3_priv_type", "NONE","selected"); %>>NONE</option>
			<option class="content_input_fd" value="DES" <% nvram_match_x("SNMP","v3_priv_type", "DES","selected"); %>>DES</option>
			<option class="content_input_fd" value="AES" <% nvram_match_x("SNMP","v3_priv_type", "AES","selected"); %>>AES</option>
		</select>
	</td>
</tr>
<tr>
	<td class="content_header_td" width="34">
		<#SNMP_Priv_Passwd_new#>
	</td>
	<td class="content_input_td" width="310">
		<input type="password" maxlength="64" size="32" name="v3_priv_passwd" value="<% nvram_get_x("SNMP", "v3_priv_passwd"); %>" onChange="page_changed()" onKeyPress="return is_string(this)" onBlur="validate_string0(this);validate_string_length(this, 8);">
	</td>
</tr>
<tr>
	<td class="content_header_td" width="34">
		<#SNMP_Priv_Passwd_retry#>
	</td>
	<td class="content_input_td" width="310">
		<input type="password" maxlength="64" size="32" name="v_v3_priv_passwd" value="<% nvram_get_x("SNMP", "v3_priv_passwd"); %>" onChange="page_changed()" onKeyPress="return is_string(this)" onBlur="validate_string0this);validate_string_length(this, 8);">
	</td>
</tr>

<tr>
	<td class="content_input_td_less" height="60" colspan="2">
	<p align="right">	
		<input class="inputSubmit" style="cursor:pointer;" onMouseOut="buttonOut(this)" onMouseOver="buttonOver(this)" type="submit" value="<#CTL_onlysave#>" name="action" onClick="return checkSNMPpass();">&nbsp;
		<input class="inputSubmit" style="cursor:pointer;" onMouseOut="buttonOut(this)" onMouseOver="buttonOver(this)" type="reset" value="<#CTL_clear#>" name="reset">
	</p>
	</td>
</tr>
</table>
</td>
</tr>

</table>
</form>
</body>
