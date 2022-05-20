<head>
<title><#ZVMODELVZ#> Web Manager</title>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<meta http-equiv="Pragma" content="no-cache">
<link rel="stylesheet" type="text/css" href="style.css" media="screen"></link>
<script language="JavaScript" type="text/javascript" src="overlib.js"></script>
<script language="JavaScript" type="text/javascript" src="general.js"></script>
<script language="JavaScript">
	INDEX=new Array;
	SSID=new Array;
	VLANID=new Array;
	AUTH_MODE=new Array;
	CRYPTO=new Array;
	WPA_PSK=new Array;
	WEP=new Array;
	PHRASE=new Array;
	KEY1=new Array;
	KEY2=new Array;
	KEY3=new Array;
	KEY4=new Array;
	KEY=new Array;
	WPA_GTK_REKEY=new Array;
	ENABLE=new Array;
	var cur;
function gen_mssid_array() {
	for (i=0; i<eval(document.form.mssidnum_x_0.value); i++)
	{
		INDEX.push(document.form.mssid_index_x[i]);
		document.form.ssid_x[i].value=decodeURIComponent(document.form.ssid_x2[i].value);
		SSID.push(document.form.ssid_x[i]);
		VLANID.push(document.form.vlanid_x[i]);
		AUTH_MODE.push(document.form.mssid_auth_mode_x[i]);
		CRYPTO.push(document.form.mssid_crypto_x[i]);
		WPA_PSK.push(document.form.mssid_wpa_psk_x[i]);
		WEP.push(document.form.mssid_wep_x[i]);
		PHRASE.push(document.form.mssid_phrase_x[i]);
		KEY1.push(document.form.mssid_key1_x[i]);
		KEY2.push(document.form.mssid_key2_x[i]);
		KEY3.push(document.form.mssid_key3_x[i]);
		KEY4.push(document.form.mssid_key4_x[i]);
		KEY.push(document.form.mssid_key_x[i]);
		WPA_GTK_REKEY.push(document.form.mssid_wpa_gtk_rekey_x[i]);
		ENABLE.push(document.form.mssid_enable_x[i]);
	}
}

function load_security_setting(onload) {
	cur=-1;
	for (i=0; i<INDEX.length; i++)
	{
		if (document.form.mssid_index_x_0.value==INDEX[i].value)
		{
			cur=i;
			break;
		}
	}
	if (cur==-1)
	{
		if (onload==1 || document.form.mssid_index_x_0.value=="0")
		{
			change_auth_mode(0);
//			document.form.mssid_ssid_x_0.value=decodeURIComponent(document.form.wl_ssid2.value);
			document.form.mssid_ssid_x_0.value=decodeURIComponent(document.form.ssid_x2[0].value);
			document.form.mssid_vlanid_x_0.value="1";
			document.form.mssid_auth_mode_x_0.value=document.form.wl_auth_mode_0.value;
			document.form.mssid_crypto_x_0.value=document.form.wl_crypto_0.value;
			document.form.mssid_wpa_psk_x_0.value=document.form.wl_wpa_psk_0.value;
			document.form.mssid_wep_x_0.value=document.form.wl_wep_x_0.value;
			document.form.mssid_phrase_x_0.value=document.form.wl_phrase_x_0.value;
			document.form.mssid_key1_x_0.value=document.form.wl_key1_0.value;
			document.form.mssid_key2_x_0.value=document.form.wl_key2_0.value;
			document.form.mssid_key3_x_0.value=document.form.wl_key3_0.value;
			document.form.mssid_key4_x_0.value=document.form.wl_key4_0.value;
			document.form.mssid_key_x_0.value=document.form.wl_key_0.value;
			document.form.mssid_wpa_gtk_rekey_x_0.value=document.form.wl_wpa_gtk_rekey_0.value;
			inputRCtrl1(document.form.mssid_enable_x_0, 0);
			inputRCtrl2(document.form.mssid_enable_x_0, 0);
		}
		else
		{
			change_auth_mode(1);
			document.form.mssid_ssid_x_0.value="default"+document.form.mssid_index_x_0.value;
			document.form.mssid_vlanid_x_0.value="1";
			document.form.mssid_auth_mode_x_0.value="open";
			document.form.mssid_crypto_x_0.value="tkip";
			document.form.mssid_wpa_psk_x_0.value="";
			document.form.mssid_wep_x_0.value="0";
			document.form.mssid_phrase_x_0.value="";
			document.form.mssid_key1_x_0.value="";
			document.form.mssid_key2_x_0.value="";
			document.form.mssid_key3_x_0.value="";
			document.form.mssid_key4_x_0.value="";
			document.form.mssid_key_x_0.value="1";
			document.form.mssid_wpa_gtk_rekey_x_0.value="0";
			inputRCtrl1(document.form.mssid_enable_x_0, 1);
			inputRCtrl2(document.form.mssid_enable_x_0, 1);
		}
	}
	else
	{
		if (INDEX[cur].value=="0")
		{
			change_auth_mode(0);
//			document.form.mssid_ssid_x_0.value=decodeURIComponent(document.form.wl_ssid2.value);
			document.form.mssid_ssid_x_0.value=decodeURIComponent(document.form.ssid_x2[0].value);
			document.form.mssid_vlanid_x_0.value=VLANID[cur].value;
			document.form.mssid_auth_mode_x_0.value=document.form.wl_auth_mode_0.value;
			document.form.mssid_crypto_x_0.value=document.form.wl_crypto_0.value;
			document.form.mssid_wpa_psk_x_0.value=document.form.wl_wpa_psk_0.value;
			document.form.mssid_wep_x_0.value=document.form.wl_wep_x_0.value;
			document.form.mssid_phrase_x_0.value=document.form.wl_phrase_x_0.value;
			document.form.mssid_key1_x_0.value=document.form.wl_key1_0.value;
			document.form.mssid_key2_x_0.value=document.form.wl_key2_0.value;
			document.form.mssid_key3_x_0.value=document.form.wl_key3_0.value;
			document.form.mssid_key4_x_0.value=document.form.wl_key4_0.value;
			document.form.mssid_key_x_0.value=document.form.wl_key_0.value;
			document.form.mssid_wpa_gtk_rekey_x_0.value=document.form.wl_wpa_gtk_rekey_0.value;
			inputRCtrl1(document.form.mssid_enable_x_0, 0);
			inputRCtrl2(document.form.mssid_enable_x_0, 0);
		}
		else
		{
			change_auth_mode(1);
			document.form.mssid_ssid_x_0.value=SSID[cur].value;
			document.form.mssid_vlanid_x_0.value=VLANID[cur].value;
			document.form.mssid_auth_mode_x_0.value=AUTH_MODE[cur].value;
			document.form.mssid_crypto_x_0.value=CRYPTO[cur].value;
			document.form.mssid_wpa_psk_x_0.value=WPA_PSK[cur].value;
			document.form.mssid_wep_x_0.value=WEP[cur].value;
			document.form.mssid_phrase_x_0.value=PHRASE[cur].value;
			document.form.mssid_key1_x_0.value=KEY1[cur].value;
			document.form.mssid_key2_x_0.value=KEY2[cur].value;
			document.form.mssid_key3_x_0.value=KEY3[cur].value;
			document.form.mssid_key4_x_0.value=KEY4[cur].value;
			document.form.mssid_key_x_0.value=KEY[cur].value;
			document.form.mssid_wpa_gtk_rekey_x_0.value=WPA_GTK_REKEY[cur].value;
			inputRCtrl1(document.form.mssid_enable_x_0, 1);
			if (ENABLE[cur].value=="1")
				inputRCtrl2(document.form.mssid_enable_x_0, 0);
			else
				inputRCtrl2(document.form.mssid_enable_x_0, 1);
		}
	}
	if (document.form.mssid_enable[0].checked == true)
		inputRCtrl1(document.form.vlans_enable, 1);
	else
		inputRCtrl1(document.form.vlans_enable, 0);

	if (document.form.vlans_enable[0].checked == true)
		inputCtrl(document.form.mssid_vlanid_x_0, 1);
	else
		inputCtrl(document.form.mssid_vlanid_x_0, 0);
}

function change_auth_mode(mssid) {
   	free_options(document.form.mssid_auth_mode_x_0);
   	if (mssid == 1)
	{
		vitems = new Array("open", "shared", "psk");
		items = new Array("Open System", "Shared Key", "WPA-PSK/WPA2-PSK");
        }
	else
	{
		vitems = new Array("open", "shared", "psk", "wpa", "wpa2", "radius");
		items = new Array("Open System", "Shared Key", "WPA-PSK/WPA2-PSK", "WPA-Enterprise", "WPA2-Enterprise", "Radius with 802.1x");
        }
	add_options_x3(document.form.mssid_auth_mode_x_0, vitems, items);
}

function add_options_x3(o, varr, arr)
{         	                         
	for (i = 0; i < arr.length; i++)
	{
		tail = o.options.length;
		o.options[tail] = new Option(arr[i]);
		o.options[tail].value = varr[i];
	}
}

function removeMSSIDListEntry(idx)
{
	document.form.MSSIDList_s.selectedIndex=idx;
	document.form.MSSIDList.click();
}

function loadSSIDs()
{
//	document.form.mssid_0.value=decodeURIComponent(document.form.wl_ssid2.value);
	document.form.mssid_0.value=SSID[0].value;
	document.form.mssid_1.value=SSID[1].value;
	document.form.mssid_2.value=SSID[2].value;
	document.form.mssid_3.value=SSID[3].value;
}
</script>
</head>  
<div id="overDiv" style="position:absolute; visibility:hidden; z-index:1000;"></div>    
<body onLoad="load_body()" onunLoad="return unload_body();">
<form method="GET" name="form" action="apply.cgi">
<!-- Table for the conntent page -->	    
<table width="666" border="0" cellpadding="0" cellspacing="0">     	      
    	
<input type="hidden" name="current_page" value="Advanced_WirelessMSSID_Content.asp">
<input type="hidden" name="next_page" value="Advanced_WAdvanced_Content.asp">
<input type="hidden" name="next_host" value="">
<input type="hidden" name="sid_list" value="WLANConfig11b;">
<input type="hidden" name="group_id" value="">
<input type="hidden" name="action_mode" value="">
<input type="hidden" name="preferred_lang" value="<% nvram_get_x("","preferred_lang"); %>">

<input type="hidden" name="modify_idx" value="-1">

<input type="hidden" name="exmode" value="<% nvram_get_x("WLANConfig11b","wl_mode_ex"); %>">
<input type="hidden" name="wan_route_mode" value="<% nvram_get_x("","wan_route_x"); %>">
<input type="hidden" name="mssidnum_x_0" value="<% nvram_get_x("WLANConfig11b", "mssidnum_x"); %>" readonly="1">

<input type="hidden" name="mssid_index_x" value="<% nvram_get_x("WLANConfig11b","mssid_index_x0"); %>">
<input type="hidden" name="mssid_index_x" value="<% nvram_get_x("WLANConfig11b","mssid_index_x1"); %>">
<input type="hidden" name="mssid_index_x" value="<% nvram_get_x("WLANConfig11b","mssid_index_x2"); %>">
<input type="hidden" name="mssid_index_x" value="<% nvram_get_x("WLANConfig11b","mssid_index_x3"); %>">

<input type="hidden" name="ssid_x" value="<% nvram_get_x("WLANConfig11b","wl_ssid"); %>">
<input type="hidden" name="ssid_x" value="<% nvram_get_x("WLANConfig11b","mssid_ssid_x1"); %>">
<input type="hidden" name="ssid_x" value="<% nvram_get_x("WLANConfig11b","mssid_ssid_x2"); %>">
<input type="hidden" name="ssid_x" value="<% nvram_get_x("WLANConfig11b","mssid_ssid_x3"); %>">

<input type="hidden" name="ssid_x2" value="<% nvram_get_x("WLANConfig11b","wl_ssid2"); %>">
<input type="hidden" name="ssid_x2" value="<% nvram_get_x("WLANConfig11b","mssid_ssid_2_x1"); %>">
<input type="hidden" name="ssid_x2" value="<% nvram_get_x("WLANConfig11b","mssid_ssid_2_x2"); %>">
<input type="hidden" name="ssid_x2" value="<% nvram_get_x("WLANConfig11b","mssid_ssid_2_x3"); %>">

<input type="hidden" name="vlanid_x" value="<% nvram_get_x("WLANConfig11b","mssid_vlanid_x0"); %>">
<input type="hidden" name="vlanid_x" value="<% nvram_get_x("WLANConfig11b","mssid_vlanid_x1"); %>">
<input type="hidden" name="vlanid_x" value="<% nvram_get_x("WLANConfig11b","mssid_vlanid_x2"); %>">
<input type="hidden" name="vlanid_x" value="<% nvram_get_x("WLANConfig11b","mssid_vlanid_x3"); %>">

<input type="hidden" name="mssid_auth_mode_x" value="<% nvram_get_x("WLANConfig11b","mssid_auth_mode_x0"); %>">
<input type="hidden" name="mssid_auth_mode_x" value="<% nvram_get_x("WLANConfig11b","mssid_auth_mode_x1"); %>">
<input type="hidden" name="mssid_auth_mode_x" value="<% nvram_get_x("WLANConfig11b","mssid_auth_mode_x2"); %>">
<input type="hidden" name="mssid_auth_mode_x" value="<% nvram_get_x("WLANConfig11b","mssid_auth_mode_x3"); %>">

<input type="hidden" name="mssid_crypto_x" value="<% nvram_get_x("WLANConfig11b","mssid_crypto_x0"); %>">
<input type="hidden" name="mssid_crypto_x" value="<% nvram_get_x("WLANConfig11b","mssid_crypto_x1"); %>">
<input type="hidden" name="mssid_crypto_x" value="<% nvram_get_x("WLANConfig11b","mssid_crypto_x2"); %>">
<input type="hidden" name="mssid_crypto_x" value="<% nvram_get_x("WLANConfig11b","mssid_crypto_x3"); %>">

<input type="hidden" name="mssid_wpa_psk_x" value="<% nvram_get_x("WLANConfig11b","mssid_wpa_psk_x0"); %>">
<input type="hidden" name="mssid_wpa_psk_x" value="<% nvram_get_x("WLANConfig11b","mssid_wpa_psk_x1"); %>">
<input type="hidden" name="mssid_wpa_psk_x" value="<% nvram_get_x("WLANConfig11b","mssid_wpa_psk_x2"); %>">
<input type="hidden" name="mssid_wpa_psk_x" value="<% nvram_get_x("WLANConfig11b","mssid_wpa_psk_x3"); %>">

<input type="hidden" name="mssid_wep_x" value="<% nvram_get_x("WLANConfig11b","mssid_wep_x0"); %>">
<input type="hidden" name="mssid_wep_x" value="<% nvram_get_x("WLANConfig11b","mssid_wep_x1"); %>">
<input type="hidden" name="mssid_wep_x" value="<% nvram_get_x("WLANConfig11b","mssid_wep_x2"); %>">
<input type="hidden" name="mssid_wep_x" value="<% nvram_get_x("WLANConfig11b","mssid_wep_x3"); %>">

<input type="hidden" name="mssid_phrase_x" value="<% nvram_get_x("WLANConfig11b","mssid_phrase_x0"); %>">
<input type="hidden" name="mssid_phrase_x" value="<% nvram_get_x("WLANConfig11b","mssid_phrase_x1"); %>">
<input type="hidden" name="mssid_phrase_x" value="<% nvram_get_x("WLANConfig11b","mssid_phrase_x2"); %>">
<input type="hidden" name="mssid_phrase_x" value="<% nvram_get_x("WLANConfig11b","mssid_phrase_x3"); %>">

<input type="hidden" name="mssid_key1_x" value="<% nvram_get_x("WLANConfig11b","mssid_key1_x0"); %>">
<input type="hidden" name="mssid_key1_x" value="<% nvram_get_x("WLANConfig11b","mssid_key1_x1"); %>">
<input type="hidden" name="mssid_key1_x" value="<% nvram_get_x("WLANConfig11b","mssid_key1_x2"); %>">
<input type="hidden" name="mssid_key1_x" value="<% nvram_get_x("WLANConfig11b","mssid_key1_x3"); %>">

<input type="hidden" name="mssid_key2_x" value="<% nvram_get_x("WLANConfig11b","mssid_key2_x0"); %>">
<input type="hidden" name="mssid_key2_x" value="<% nvram_get_x("WLANConfig11b","mssid_key2_x1"); %>">
<input type="hidden" name="mssid_key2_x" value="<% nvram_get_x("WLANConfig11b","mssid_key2_x2"); %>">
<input type="hidden" name="mssid_key2_x" value="<% nvram_get_x("WLANConfig11b","mssid_key2_x3"); %>">

<input type="hidden" name="mssid_key3_x" value="<% nvram_get_x("WLANConfig11b","mssid_key3_x0"); %>">
<input type="hidden" name="mssid_key3_x" value="<% nvram_get_x("WLANConfig11b","mssid_key3_x1"); %>">
<input type="hidden" name="mssid_key3_x" value="<% nvram_get_x("WLANConfig11b","mssid_key3_x2"); %>">
<input type="hidden" name="mssid_key3_x" value="<% nvram_get_x("WLANConfig11b","mssid_key3_x3"); %>">

<input type="hidden" name="mssid_key4_x" value="<% nvram_get_x("WLANConfig11b","mssid_key4_x0"); %>">
<input type="hidden" name="mssid_key4_x" value="<% nvram_get_x("WLANConfig11b","mssid_key4_x1"); %>">
<input type="hidden" name="mssid_key4_x" value="<% nvram_get_x("WLANConfig11b","mssid_key4_x2"); %>">
<input type="hidden" name="mssid_key4_x" value="<% nvram_get_x("WLANConfig11b","mssid_key4_x3"); %>">

<input type="hidden" name="mssid_key_x" value="<% nvram_get_x("WLANConfig11b","mssid_key_x0"); %>">
<input type="hidden" name="mssid_key_x" value="<% nvram_get_x("WLANConfig11b","mssid_key_x1"); %>">
<input type="hidden" name="mssid_key_x" value="<% nvram_get_x("WLANConfig11b","mssid_key_x2"); %>">
<input type="hidden" name="mssid_key_x" value="<% nvram_get_x("WLANConfig11b","mssid_key_x3"); %>">

<input type="hidden" name="mssid_wpa_gtk_rekey_x" value="<% nvram_get_x("WLANConfig11b","mssid_wpa_gtk_rekey_x0"); %>">
<input type="hidden" name="mssid_wpa_gtk_rekey_x" value="<% nvram_get_x("WLANConfig11b","mssid_wpa_gtk_rekey_x1"); %>">
<input type="hidden" name="mssid_wpa_gtk_rekey_x" value="<% nvram_get_x("WLANConfig11b","mssid_wpa_gtk_rekey_x2"); %>">
<input type="hidden" name="mssid_wpa_gtk_rekey_x" value="<% nvram_get_x("WLANConfig11b","mssid_wpa_gtk_rekey_x3"); %>">

<input type="hidden" name="mssid_enable_x" value="<% nvram_get_x("WLANConfig11b","mssid_enable_x0"); %>">
<input type="hidden" name="mssid_enable_x" value="<% nvram_get_x("WLANConfig11b","mssid_enable_x1"); %>">
<input type="hidden" name="mssid_enable_x" value="<% nvram_get_x("WLANConfig11b","mssid_enable_x2"); %>">
<input type="hidden" name="mssid_enable_x" value="<% nvram_get_x("WLANConfig11b","mssid_enable_x3"); %>">

<input type="hidden" name="wl_auth_mode_0" value="<% nvram_get_x("WLANConfig11b","wl_auth_mode"); %>">
<input type="hidden" name="wl_crypto_0" value="<% nvram_get_x("WLANConfig11b","wl_crypto"); %>">
<input type="hidden" name="wl_wpa_psk_0" value="<% nvram_get_x("WLANConfig11b","wl_wpa_psk"); %>">
<input type="hidden" name="wl_wep_x_0" value="<% nvram_get_x("WLANConfig11b","wl_wep_x"); %>">
<input type="hidden" name="wl_phrase_x_0" value="<% nvram_get_x("WLANConfig11b","wl_phrase_x"); %>">
<input type="hidden" name="wl_key1_0" value="<% nvram_get_x("WLANConfig11b","wl_key1"); %>">
<input type="hidden" name="wl_key2_0" value="<% nvram_get_x("WLANConfig11b","wl_key2"); %>">
<input type="hidden" name="wl_key3_0" value="<% nvram_get_x("WLANConfig11b","wl_key3"); %>">
<input type="hidden" name="wl_key4_0" value="<% nvram_get_x("WLANConfig11b","wl_key4"); %>">
<input type="hidden" name="wl_key_0" value="<% nvram_get_x("WLANConfig11b","wl_key"); %>">
<input type="hidden" name="wl_wpa_gtk_rekey_0" value="<% nvram_get_x("WLANConfig11b","wl_wpa_gtk_rekey"); %>">

<input type="hidden" name="mssid_ssid_2_x_0" value="">
<input type="hidden" name="wl_mode_x" value="<% nvram_get_x("WLANConfig11b","wl_mode_x"); %>">

<tr>
<td>
<table width="666" border="1" cellpadding="0" cellspacing="0" bordercolor="E0E0E0">

<tr class="content_header_tr">
	<td class="content_header_td_title" colspan="2"><#t1Wireless#> - <#t2MSSID#></td>
</tr>

<tr>
	<td class="content_desc_td" colspan="2"><#WLANConfig11b_MEnable_sectiondesc#></td>
</tr>

<tr>
	<td class="content_header_td" onMouseOver="return overlib('<#WLANConfig11b_MEnable_itemdesc#>', LEFT);" onMouseOut="return nd();"><#WLANConfig11b_MEnable_itemname#></td>
	<td class="content_input_td">
		<input type="radio" value="1" name="mssid_enable" class="content_input_fd" onClick="return change_common_radio(this, 'WLANConfig11b', 'mssid_enable', '1')" <% nvram_match_x("WLANConfig11b","mssid_enable", "1", "checked"); %>>Yes</input>
		<input type="radio" value="0" name="mssid_enable" class="content_input_fd" onClick="return change_common_radio(this, 'WLANConfig11b', 'mssid_enable', '0')" <% nvram_match_x("WLANConfig11b","mssid_enable", "0", "checked"); %>>No</input>
	</td>
</tr>

<tr>
	<td class="content_header_td" onMouseOver="return overlib('<#WLANConfig11b_VLANS_itemdesc#>', LEFT);" onMouseOut="return nd();"><#WLANConfig11b_VLANS_itemname#></td>
	<td class="content_input_td">
		<input type="radio" value="1" name="vlans_enable" class="content_input_fd" onClick="return change_common_radio(this, 'WLANConfig11b', 'vlans_enable', '1')" <% nvram_match_x("WLANConfig11b","vlans_enable", "1", "checked"); %>>Yes</input>
		<input type="radio" value="0" name="vlans_enable" class="content_input_fd" onClick="return change_common_radio(this, 'WLANConfig11b', 'vlans_enable', '0')" <% nvram_match_x("WLANConfig11b","vlans_enable", "0", "checked"); %>>No</input>
	</td>
</tr>

<tr class="content_section_header_tr">
	<td class="content_section_header_td" colspan="2"><#WLANConfig11b_MSSIDVLAN_groupitemdesc#></td>
</tr>

<tr>
	<td class="content_header_td" onMouseOver="return overlib('<#WLANConfig11b_MSSIDIndex_itemdesc#>', LEFT);" onMouseOut="return nd();"><#WLANConfig11b_MSSIDIndex_itemname#></td>
	<td class="content_input_td">
		<select name="mssid_index_x_0" class="content_input_fd" onChange="return change_common(this, 'WLANConfig11b', 'mssid_index_x_0')">
			<option class="content_input_fd" value="0">Primay SSID</option>
			<option class="content_input_fd" value="1">Multi-SSID1</option>
			<option class="content_input_fd" value="2">Multi-SSID2</option>
			<option class="content_input_fd" value="3">Multi-SSID3</option>
		</select>
	</td>
</tr>

<tr>
	<td class="content_header_td" onMouseOver="return overlib('<#WLANConfig11b_MSEnable_itemdesc#>', LEFT);" onMouseOut="return nd();"><#WLANConfig11b_MSEnable_itemname#></td>
	<td class="content_input_td">
		<input type="radio" value="1" name="mssid_enable_x_0" class="content_input_fd">Yes</input>
		<input type="radio" value="0" name="mssid_enable_x_0" class="content_input_fd">No</input>
	</td>
</tr>

<tr>
	<td class="content_header_td" onMouseOver="return overlib('<#WLANConfig11b_MSSID_itemdesc#>', LEFT);" onMouseOut="return nd();"><#WLANConfig11b_MSSID_itemname#></td>
	<td class="content_input_td"><input type="text" maxlength="32" class="content_input_fd" size="32" name="mssid_ssid_x_0" onKeyPress="return is_string(this)" onBlur="validate_string4(this)"></td>
</tr>

<tr>
	<td class="content_header_td" onMouseOver="return overlib('<#WLANConfig11b_MVLANID_itemdesc#>', LEFT);" onMouseOut="return nd();"><#WLANConfig11b_MVLANID_itemname#></td>
	<td class="content_input_td"><input type="text" maxlength="32" class="content_input_fd" size="32" name="mssid_vlanid_x_0" onBlur="validate_range(this, 1, 4094)" onKeyPress="return is_number(this)"></td>
</tr>

<tr>
	<td class="content_header_td" onMouseOver="return overlib('<#WLANConfig11b_MAuthenticationMethod_itemdesc#>', LEFT);" onMouseOut="return nd();"><#WLANConfig11b_MAuthenticationMethod_itemname#></td>
	<td class="content_input_td">
		<select name="mssid_auth_mode_x_0" class="content_input_fd" onChange="return change_common(this, 'WLANConfig11b', 'mssid_auth_mode_x_0')">
			<option class="content_input_fd" value="open"  >Open System</option>
			<option class="content_input_fd" value="shared">Shared Key</option>
			<option class="content_input_fd" value="psk"   >WPA-PSK/WPA2-PSK</option>
		</select>
	</td>
</tr>

<tr>
	<td class="content_header_td" onMouseOver="return overlib('<#WLANConfig11b_MWPAType_itemdesc#>', LEFT);" onMouseOut="return nd();"><#WLANConfig11b_MWPAType_itemname#></td>
	<td class="content_input_td">
		<select name="mssid_crypto_x_0" class="content_input_fd" onChange="return change_common(this, 'WLANConfig11b', 'mssid_crypto_x_0')">
			<option class="content_input_fd" value="tkip"    >TKIP</option>
			<option class="content_input_fd" value="aes"     >AES</option>
			<option class="content_input_fd" value="tkip+aes">TKIP+AES</option>
		</select>
	</td>
</tr>

<tr>
	<td class="content_header_td" onMouseOver="return overlib('<#WLANConfig11b_Mx_PSKKey_itemdesc#>', LEFT);" onMouseOut="return nd();"><#WLANConfig11b_Mx_PSKKey_itemname#></td>
	<td class="content_input_td">
		<script language="JavaScript" type="text/javascript" src="md5.js"></script>
		<input type="password" maxlength="64" class="content_input_fd" size="32" name="mssid_wpa_psk_x_0" onKeyUp="return is_wlphrase('WLANConfig11b', 'mssid_wpa_psk_x_0', this)" onBlur="return validate_wlphrase('WLANConfig11b', 'mssid_wpa_psk_x_0', this)">
	</td>
</tr>

<tr>
	<td class="content_header_td" onMouseOver="return overlib('<#WLANConfig11b_MWEPType_itemdesc#>', LEFT);" onMouseOut="return nd();"><#WLANConfig11b_MWEPType_itemname#></td>
	<td class="content_input_td">
		<select name="mssid_wep_x_0" class="content_input_fd" onChange="return change_common(this, 'WLANConfig11b', 'mssid_wep_x_0')">
			<option class="content_input_fd" value="0">None</option>
			<option class="content_input_fd" value="1">WEP-64bits</option>
			<option class="content_input_fd" value="2">WEP-128bits</option>
		</select>
	</td>
</tr>

<tr>
	<td class="content_header_td" onMouseOver="return overlib('<#WLANConfig11b_Mx_Phrase_itemdesc#>', LEFT);" onMouseOut="return nd();"><#WLANConfig11b_Mx_Phrase_itemname#></td>
	<td class="content_input_td">
		<script language="JavaScript" type="text/javascript" src="md5.js"></script>
		<input type="password" maxlength="64" class="content_input_fd" size="32" name="mssid_phrase_x_0" onKeyUp="return is_wlphrase('WLANConfig11b', 'mssid_phrase_x_0', this)" onChange="return is_wlphrase('WLANConfig11b', 'mssid_phrase_x_0', this)" onBlur="return validate_wlphrase('WLANConfig11b', 'mssid_phrase_x_0', this)">
	</td>
</tr>

<tr>
	<td class="content_header_td"><#WLANConfig11b_MWEPKey1_itemname#></td>
	<td class="content_input_td">
		<input type="pssword" maxlength="32" class="content_input_fd" size="32" name="mssid_key1_x_0" onBlur="return validate_wlkey(this, 'WLANConfig11b')" onKeyPress="return is_wlkey(this, 'WLANConfig11b')" onKeyUp="return change_wlkey(this, 'WLANConfig11b')">
	</td>
</tr>

<tr>
	<td class="content_header_td"><#WLANConfig11b_MWEPKey2_itemname#></td>
	<td class="content_input_td">
		<input type="pssword" maxlength="32" class="content_input_fd" size="32" name="mssid_key2_x_0" onBlur="return validate_wlkey(this, 'WLANConfig11b')" onKeyPress="return is_wlkey(this, 'WLANConfig11b')" onKeyUp="return change_wlkey(this, 'WLANConfig11b')">
	</td>
</tr>

<tr>
	<td class="content_header_td"><#WLANConfig11b_MWEPKey3_itemname#></td>
	<td class="content_input_td">
		<input type="pssword" maxlength="32" class="content_input_fd" size="32" name="mssid_key3_x_0" onBlur="return validate_wlkey(this, 'WLANConfig11b')" onKeyPress="return is_wlkey(this, 'WLANConfig11b')" onKeyUp="return change_wlkey(this, 'WLANConfig11b')">
	</td>
</tr>

<tr>
	<td class="content_header_td"><#WLANConfig11b_MWEPKey4_itemname#></td>
	<td class="content_input_td">
		<input type="pssword" maxlength="32" class="content_input_fd" size="32" name="mssid_key4_x_0" onBlur="return validate_wlkey(this, 'WLANConfig11b')" onKeyPress="return is_wlkey(this, 'WLANConfig11b')" onKeyUp="return change_wlkey(this, 'WLANConfig11b')">
	</td>
</tr>

<tr>
	<td class="content_header_td" onMouseOver="return overlib('<#WLANConfig11b_MWEPDefaultKey_itemdesc#>', LEFT);" onMouseOut="return nd();"><#WLANConfig11b_MWEPDefaultKey_itemname#></td>
	<td class="content_input_td">
		<select name="mssid_key_x_0" class="content_input_fd" onChange="return change_common(this, 'WLANConfig11b', 'mssid_key_x_0')">
			<option class="content_input_fd" value="1">Key1</option>
			<option class="content_input_fd" value="2">Key2</option>
			<option class="content_input_fd" value="3">Key3</option>
			<option class="content_input_fd" value="4">Key4</option>
		</select>
	</td>
</tr>

<tr>
	<td class="content_header_td" onMouseOver="return overlib('<#WLANConfig11b_Mx_Rekey_itemdesc#>', LEFT);" onMouseOut="return nd();"><#WLANConfig11b_Mx_Rekey_itemname#></td>
	<td class="content_input_td">
		<input type="text" maxlength="5" size="5" name="mssid_wpa_gtk_rekey_x_0" class="content_input_fd" onBlur="validate_range(this, 0, 86400)" onKeyPress="return is_number(this)">
	</td>
</tr>

</table>
</td>
</tr>

<tr>
<td>
<table width="666" border="0" cellpadding="0" cellspacing="0" bordercolor="E0E0E0"></table>
</td>
</tr>

<tr>
<td>
<table width="666" border="1" cellpadding="0" cellspacing="0" bordercolor="E0E0E0">

<tr class="content_list_header_tr">
	<td class="content_list_header_td" width="82%" id="MSSIDList"><#WLANConfig11b_MSSIDList_groupitemdesc#></td>
	<td width="13%">
		<div align="center">
		<input class="inputSubmit" style="cursor:pointer; font-weight:normal;" type="submit" onMouseOut="buttonOut(this)" onMouseOver="buttonOver(this)" onClick="return markGroup(this, 'MSSIDList', 4, ' Save ');" name="MSSIDList" value="<#CTL_store#>" size="12">
		</div>
	</td>
	<td class="content_list_header_td width="5%">&nbsp</td>
</tr>

</table>
</td>
</tr>

<tr>
<td>
<table width="666" border="0" cellpadding="0" cellspacing="0" bordercolor="E0E0E0"></table>
</td>
</tr>

<tr>
<td>
<table table width="666" border="1" cellpadding="0" cellspacing="0" bordercolor="E0E0E0">
<tr>
<td width="13%" align="center" class="content_header_td_ap">Index</td>
<td width="42%" align="center" class="content_header_td_ap">SSID</td>
<td width="27%" align="center" class="content_header_td_ap">Security</td>
<td width="9%" align="center" class="content_header_td_ap">VLAN ID</td>
<td width="9%" align="center" class="content_header_td_ap">State</td>
</tr>
<% nvram_dump("mssid.log",""); %>
</table>
</td>
</tr>

<tr>
<td colspan="2">
<div id="hidden0" style="display:none">
<table class="content_list_value_table" border="0" cellspacing="0" cellpadding="0">
<tr>
	<td>
		<select size="4" name="MSSIDList_s" multiple="true" style="font-family: fixedsys; font-size: 8pt">
			<% nvram_get_table_x("WLANConfig11b","MSSIDList"); %>
		</select>
	</td>
</tr>
</table>
</div>
</td>
</tr>

<tr>
<td>		
<table width="666" border="1" cellpadding="0" cellspacing="0" bordercolor="B0B0B0">
<tr bgcolor="#CCCCCC">
	<td colspan="3"><font face="arial" size="2"><b>&nbsp</b></font></td>
</tr>
<tr bgcolor="#FFFFFF">

	<td id ="Confirm" height="25" width="34%"><div align="center"><font face="Arial">
			<input class=inputSubmit style="cursor:pointer;" onMouseOut=buttonOut(this) onMouseOver="buttonOver(this)" onClick="return onSubmitCtrl(this, ' Restore ');" type="submit" value=" <#CTL_restore#> " name="action">
	</font></div></td>

	<td height="25" width="33%"><div align="center"><font face="Arial">
			<input class=inputSubmit style="cursor:pointer;" onMouseOut=buttonOut(this) onMouseOver="buttonOver(this)" onClick="return onSubmitCtrl(this, ' Finish ');" type="submit" value=" <#CTL_finish#> " name="action">
	</font></div></td>

	<td height="25" width="33%"><div align="center"><font face="Arial">
			<input class=inputSubmit style="cursor:pointer;" onMouseOut=buttonOut(this) onMouseOver="buttonOver(this)" onClick="return onSubmitCtrl(this, ' Apply ');" type="submit" value=" <#CTL_apply#> " name="action">
	</font></div></td>    
</tr>
</table>
</td>
</tr>

<tr>
<td>
<table width="666" border="1" cellpadding="0" cellspacing="0" bordercolor="B0B0B0">
<tr>
	<td colspan="2" width="616" height="25" bgcolor="#FFBB00"></td> 
</tr>                   
<tr bgcolor="#FFFFFF">
	<td class="content_header_td_15" align="left"><#CTL_restore#>: </td>
	<td class="content_input_td_padding" align="left"><#CTL_desc_restore#></td>
</tr>
<tr bgcolor="#FFFFFF">
	<td class="content_header_td_15" align="left"><#CTL_finish#>: </td>
	<td class="content_input_td_padding" align="left"><#CTL_desc_finish#></td>
</tr>
<tr bgcolor="#FFFFFF">
	<td class="content_header_td_15" align="left"><#CTL_apply#>: </td>
	<td class="content_input_td_padding" align="left"><#CTL_desc_apply#></td>
</tr>
</table>
</td>
</tr>

</table>
</form>
</body>
