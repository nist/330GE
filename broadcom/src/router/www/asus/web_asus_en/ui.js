var hint_word;
var wl_mode_current;

var nav;
var IP_RNAGE_STR_GLOBAL="";
var LAN_NETMASK_GLOBAL="";

NS = (document.layers) ? 1 : 0;
IE = (document.all) ? 1: 0;

var slowHide_ID_start = 0;
var slowHide_ID_mid = 0;
var slowShow_ID_start = 0;
var slowShow_ID_mid = 0;

if (IE)
{
	nav = false;
} 
else
{
	nav = true;
}

var seconds = 18; // the seconds of 330gE restart time

function matchSubnet(ip1, sb1, ip2, sb2)
{
   nsb1 = intoa(sb1.value);
   nsb2 = intoa(sb2.value);
   if(nsb1<nsb2) nsb=nsb1;
   else nsb=nsb2;

   if ((intoa(ip1)&nsb)==(intoa(ip2)&nsb)) return 1;
   else return 0;
}

function Advanced_Setting()
{
	window.location.href='index.asp';
}

function show_restart(){
	document.getElementById("restart").style.visibility="visible";
	document.getElementById("popupHint").style.visibility="hidden";

	if(document.form.op_mode.value=="gw-repeater")
	{
		document.getElementById("SecurityLevel").style.visibility="hidden";
	}
	
	if (document.form.wl_mode_ex.value=="ap")
	{
		if (document.form.wan_nat_x.value=="1")
			seconds=15;
		else
			seconds=14;
	}
	showLoading(seconds);
}

function showLoading(x)
{	
	if(x!=-1)
	{
		ptext = ""+Math.round((seconds-x)*100/seconds).toString()+" \%";
		document.getElementById("parea").innerHTML = ptext;
		x--;
		setTimeout("showLoading('"+x+"')", 1000);
	}
	else
	{
		document.getElementById("parea").innerHTML = "100 \%";
		window.location.href=document.form.current_page.value;
	}
}

function writeSSID()
{
	for(var i = 0; i < ap_count; i++){
		E_str=""+"ssid"+i;
		if(nav)
//			document.getElementById(E_str).innerHTML = AP_status[i][0];
			document.getElementById(E_str).innerHTML = decodeURIComponent(AP_status[i][0]);
		else
//			document.getElementById(E_str).innerText = AP_status[i][0];
			document.getElementById(E_str).innerText = decodeURIComponent(AP_status[i][0]);
	}
}

function writeSSID_SP()
{
	if (document.form.old_wl_hotspot.value=="1")
		writeSSID();
}

function slowshowHint(){
	if (document.form.sta_state.value!="authorized")
		return;
	document.getElementById("hint").style.filter = "alpha(opacity=0)";
	document.getElementById("hint").style.opacity = 0;
	document.getElementById("hint").style.visibility = "visible";
	if(document.form.op_mode.value=="gw-repeater")
	{
//		document.getElementById("selectMenumain").style.visibility="hidden";
		document.getElementById("SecurityLevel").style.visibility="hidden";
	}
	slowShow_ID_start = setTimeout("slowShow(0,'hint')", 1000);

	slowHide_ID_start = setTimeout("slowHide(100,'hint')", 8000);    
}

function slowshowHint_SP()
{
	if (document.form.old_wl_hotspot.value=="1")
		slowshowHint();
}
 
function slowShow(x,divName){
	document.getElementById(divName).style.filter ="alpha(opacity="+ x +")";
	document.getElementById(divName).style.opacity = x*0.01;       
	x += 5;
	slowShow_ID_mid = setTimeout("slowShow("+ x +",'"+divName+"')",100);
	if(x >= 100){
		clearTimeout(slowShow_ID_start);
		slowShow_ID_start = 0;
		clearTimeout(slowShow_ID_mid);
		slowShow_ID_mid = 0; 
	}
}
 
function slowHide(x,divName){
	document.getElementById(divName).style.filter = "alpha(opacity="+ x +")";
	document.getElementById(divName).style.opacity = x*0.01; 
	x -= 5;
	slowHide_ID_mid = setTimeout("slowHide("+ x +",'"+divName+"')",100);

	if(x < 0){
		clearTimeout(slowHide_ID_start);
		slowHide_ID_start = 0;
		clearTimeout(slowHide_ID_mid);
		slowHide_ID_mid = 0;
		document.getElementById(divName).style.visibility = "hidden";
		if(document.form.op_mode.value=="gw-repeater")
		{
//			document.getElementById("selectMenumain").style.visibility="visible";
			document.getElementById("SecurityLevel").style.visibility="visible";
		}
	}
}

function fastHide(){
	document.getElementById("hint").style.visibility = "hidden";
	if(document.form.op_mode.value=="gw-repeater")
	{
//		document.getElementById("selectMenumain").style.visibility="visible";
		document.getElementById("SecurityLevel").style.visibility="visible";
	}
	document.getElementById("hint").style.filter = "alpha(opacity=0)";
	document.getElementById("hint").style.opacity = 0; 

	clearTimeout(slowHide_ID_start);
	slowHide_ID_start = 0;
	clearTimeout(slowHide_ID_mid);
	slowHide_ID_mid = 0;
}

function fastHide_SP()
{
	if (document.form.old_wl_hotspot.value=="1")	
		fastHide();
}

function validate_char(ch)
{
	if (ch>=32&&ch<=126)
		return true;
	else
		return false;
}

function validate_ssid(o)
{		
	for(i=0;i<o.value.length;i++)
	{
		c = o.value.charCodeAt(i);
		if (!validate_char(c))
		{
//			alert(quick_translate_lang('JS_validstr', document.form.preferred_lang.value) + ' \"' + o.value.charAt(i) + '\" !');

			if (document.form.preferred_lang.value!="DE")
				alert(quick_translate_lang('JS_validstr', document.form.preferred_lang.value) + ' \"' + o.value.charAt(i) + '\" !');
			else
				alert(quick_translate_lang('JS_validstr3', document.form.preferred_lang.value) + ' \"' + o.value.charAt(i) + '\"' + quick_translate_lang('JS_validstr4',document.form.preferred_lang.value) + '!');

  			o.value="";
  			o.focus();
			return false;
		}
	}

  	if (o.value == "")
  	{
  		alert(quick_translate_lang('JS_fieldblank', document.form.preferred_lang.value));
		o.value = "default";
  		o.focus();
		return false;
	}

	return true;
}

function showPop(){
	fastHide();
	scrollTo(0, 0);
	count=ap_count;
	document.form.no_ask_password.value="0";
	if(document.form.op_mode.value!="AP" && document.form.op_mode.value!="router")
		document.form.add_profile.value="0";
	document.form.wl_wpa_psk.value="";
	if (count>0)
	{
		if(count>1)
		for(var i = 0; i < count; i++)
		{
			if(document.form.AP_select[i].checked==true)
			{
				if (AP_status[i][2]=="0" || (AP_status[i][4]=="authenticated" && (decodeURIComponent(AP_status[i][0])==document.form.wf1_ssid.value || decodeURIComponent(AP_status[i][0])==document.form.wf2_ssid.value || decodeURIComponent(AP_status[i][0])==document.form.wf3_ssid.value || decodeURIComponent(AP_status[i][0])==document.form.wf4_ssid.value)))
				{
					if (AP_status[i][2]!="0")	// use settings in profile list
						document.form.no_ask_password.value="1";

					select_idx=i;
					document.getElementById("popupWindow").style.visibility="hidden";
					show_hint();
				}
				else if (AP_status[i][2]=="6" || AP_status[i][2]=="7")
				{
					alert(quick_translate_lang('JS_unsupported', document.form.preferred_lang.value));
					select_idx=-1;
					document.getElementById("popupWindow").style.visibility="hidden";
				}
				else
				{
					select_idx=i;
					document.getElementById("popupWindow").style.visibility="visible";
					document.form.wl_wpa_psk.focus();
				}

				if (document.form.no_ask_password.value=="0")
				{
					if (decodeURIComponent(AP_status[i][0])==document.form.wf1_ssid.value)
						document.form.wl_wpa_psk.value=document.form.wf1_passwd.value;
					else if (decodeURIComponent(AP_status[i][0])==document.form.wf2_ssid.value)
						document.form.wl_wpa_psk.value=document.form.wf2_passwd.value;
					else if (decodeURIComponent(AP_status[i][0])==document.form.wf3_ssid.value)
						document.form.wl_wpa_psk.value=document.form.wf3_passwd.value;
					else if (decodeURIComponent(AP_status[i][0])==document.form.wf4_ssid.value)
						document.form.wl_wpa_psk.value=document.form.wf4_passwd.value;
				}

				return;
			}
		}
		else
		{
			if(document.form.AP_select.checked==true)
			{
				if (AP_status[0][2]=="0" || (AP_status[0][4]=="authenticated" &&(decodeURIComponent(AP_status[0][0])==document.form.wf1_ssid.value || decodeURIComponent(AP_status[0][0])==document.form.wf2_ssid.value || decodeURIComponent(AP_status[0][0])==document.form.wf3_ssid.value || decodeURIComponent(AP_status[0][0])==document.form.wf4_ssid.value)))
				{
					if (AP_status[0][2]!="0")	// use settings in profile list
						document.form.no_ask_password.value="1";

					select_idx=0;
					document.getElementById("popupWindow").style.visibility="hidden";
					show_hint();
				}
				else if (AP_status[0][2]=="6" || AP_status[0][2]=="7")
				{
					alert(quick_translate_lang('JS_unsupported', document.form.preferred_lang.value));
					select_idx=-1;
					document.getElementById("popupWindow").style.visibility="hidden";
				}
				else
				{
					select_idx=0;
					document.getElementById("popupWindow").style.visibility="visible";
				}

				if (document.form.no_ask_password.value=="0")
				{
					if (decodeURIComponent(AP_status[0][0])==document.form.wf1_ssid.value)
						document.form.wl_wpa_psk.value=document.form.wf1_passwd.value;
					else if (decodeURIComponent(AP_status[0][0])==document.form.wf2_ssid.value)
						document.form.wl_wpa_psk.value=document.form.wf2_passwd.value;
					else if (decodeURIComponent(AP_status[0][0])==document.form.wf3_ssid.value)
						document.form.wl_wpa_psk.value=document.form.wf3_passwd.value;
					else if (decodeURIComponent(AP_status[0][0])==document.form.wf4_ssid.value)
						document.form.wl_wpa_psk.value=document.form.wf4_passwd.value;
				}

				return;
			}
		}
	}
}

function showPop_re(){
	if(document.form.op_mode.value!="gw-repeater")
		return checkPW();

	fastHide();
	scrollTo(0, 0);
	count=ap_count;
	document.form.no_ask_password.value="0";
	document.form.add_profile.value="0";
	document.form.wl_wpa_psk_ure.value="";
	if (count>0)
	{
		if(count>1)
		for(var i = 0; i < count; i++)
		{
			if(document.form.AP_select[i].checked==true)
			{
				if (AP_status[i][2]=="0" || AP_status[i][4]=="authenticated")
				{
					if (AP_status[i][2]!="0")	// use settings in profile list
						document.form.no_ask_password.value="1";

					select_idx=i;
					document.getElementById("popupWindow").style.visibility="hidden";
					show_hint();
				}
				else if (AP_status[i][2]=="6" || AP_status[i][2]=="7")
				{
					alert(quick_translate_lang('JS_unsupported', document.form.preferred_lang.value));
					select_idx=-1;
					document.getElementById("popupWindow").style.visibility="hidden";
				}
				else
				{
					select_idx=i;
					document.getElementById("popupWindow").style.visibility="visible";
					document.getElementById("selectMenumain").style.visibility="hidden";
					document.getElementById("SecurityLevel").style.visibility="hidden";
					document.form.wl_wpa_psk_ure.focus();
				}

				if (document.form.no_ask_password.value=="0")
				{
					if (decodeURIComponent(AP_status[i][0])==document.form.wf1_ssid.value)
						document.form.wl_wpa_psk_ure.value=document.form.wf1_passwd.value;
					else if (decodeURIComponent(AP_status[i][0])==document.form.wf2_ssid.value)
						document.form.wl_wpa_psk_ure.value=document.form.wf2_passwd.value;
					else if (decodeURIComponent(AP_status[i][0])==document.form.wf3_ssid.value)
						document.form.wl_wpa_psk_ure.value=document.form.wf3_passwd.value;
					else if (decodeURIComponent(AP_status[i][0])==document.form.wf4_ssid.value)
						document.form.wl_wpa_psk_ure.value=document.form.wf4_passwd.value;
				}

				return;
			}
		}
		else
		{
			if(document.form.AP_select.checked==true)
			{
				if (AP_status[0][2]=="0" || AP_status[0][4]=="authenticated")
				{
					if (AP_status[0][2]!="0")	// use settings in profile list
						document.form.no_ask_password.value="1";

					select_idx=0;
					document.getElementById("popupWindow").style.visibility="hidden";
					show_hint();
				}
				else if (AP_status[0][2]=="6" || AP_status[0][2]=="7")
				{
					alert(quick_translate_lang('JS_unsupported', document.form.preferred_lang.value));
					select_idx=-1;
					document.getElementById("popupWindow").style.visibility="hidden";
				}
				else
				{
					select_idx=0;
					document.getElementById("popupWindow").style.visibility="visible";
					document.getElementById("selectMenumain").style.visibility="hidden";
					document.getElementById("SecurityLevel").style.visibility="hidden";
				}

				if (document.form.no_ask_password.value=="0")
				{
					if (decodeURIComponent(AP_status[0][0])==document.form.wf1_ssid.value)
						document.form.wl_wpa_psk_ure.value=document.form.wf1_passwd.value;
					else if (decodeURIComponent(AP_status[0][0])==document.form.wf2_ssid.value)
						document.form.wl_wpa_psk_ure.value=document.form.wf2_passwd.value;
					else if (decodeURIComponent(AP_status[0][0])==document.form.wf3_ssid.value)
						document.form.wl_wpa_psk_ure.value=document.form.wf3_passwd.value;
					else if (decodeURIComponent(AP_status[0][0])==document.form.wf4_ssid.value)
						document.form.wl_wpa_psk_ure.value=document.form.wf4_passwd.value;
				}

				return;
			}
		}
	}
}

function hidePop(){
	document.getElementById("popupWindow").style.visibility="hidden";
	if(document.form.op_mode.value=="gw-repeater")
	{
		document.getElementById("selectMenumain").style.visibility="visible";
		document.getElementById("SecurityLevel").style.visibility="visible";
	}
}

function hidePop_SP(){
	if (validate_wlphrase_client('WLANConfig11b', document.form.wl_wpa_psk))
	{
		hidePop();
		show_hint();
		return true;
	}
	else
	{
		return false;
	}
}

function hidePop_SP_re(){
	if (validate_wlphrase_client('WLANConfig11b', document.form.wl_wpa_psk_ure))
	{
		hidePop();
		show_hint();
		return true;
	}
	else
	{
		return false;
	}
}

function hidePop_SP2_re(){
	if (validate_wlphrase_client_SP() && validate_ssid(document.getElementById('prefer_SSID')))
	{
		document.form.add_profile.value="1";
		hide_prefer_w_network();
		show_hint();
		return true;
	}
	else
	{
		return false;
	}
}

function is_current_Mode(X, EX){
	mode_current="";
	if( X == 1 && EX == "ap")
	{
		mode_current = "Gateway";
	}
	else if( X == 1 && EX == "sta")
	{
		mode_current = "Gateway-Adapter";
	}
	else if( X == 1 && EX == "re")
	{
		mode_current = "Gateway(Hotspot)";
	}
	else if(X == 0 && EX == "ap")
	{
		mode_current = "AP";
	}
	else if(X == 0 && EX == "sta")
	{
		mode_current = "Adapter";
	}
	else if(X == 0 && EX == "re")
	{
		mode_current = "Repeater";
	}
	return mode_current;
}

function STA_URE_onload()
{
	document.form.wf1_key.value = decodeURIComponent(document.form.wf1_key_org.value);
	document.form.wf2_key.value = decodeURIComponent(document.form.wf2_key_org.value);
	document.form.wf3_key.value = decodeURIComponent(document.form.wf3_key_org.value);
	document.form.wf4_key.value = decodeURIComponent(document.form.wf4_key_org.value);
	document.form.wf1_passwd.value = decodeURIComponent(document.form.wf1_passwd_org.value);
	document.form.wf2_passwd.value = decodeURIComponent(document.form.wf2_passwd_org.value);
	document.form.wf3_passwd.value = decodeURIComponent(document.form.wf3_passwd_org.value);
	document.form.wf4_passwd.value = decodeURIComponent(document.form.wf4_passwd_org.value);

	writeSSID();
	isAuthenticated();
	slowshowHint();
	Show_security_encry(document.getElementById('prefer_security').selectedIndex);
}

function STA_URE_onload_SP()
{
	document.form.wf1_key.value = decodeURIComponent(document.form.wf1_key_org.value);
	document.form.wf2_key.value = decodeURIComponent(document.form.wf2_key_org.value);
	document.form.wf3_key.value = decodeURIComponent(document.form.wf3_key_org.value);
	document.form.wf4_key.value = decodeURIComponent(document.form.wf4_key_org.value);
	document.form.wf1_passwd.value = decodeURIComponent(document.form.wf1_passwd_org.value);
	document.form.wf2_passwd.value = decodeURIComponent(document.form.wf2_passwd_org.value);
	document.form.wf3_passwd.value = decodeURIComponent(document.form.wf3_passwd_org.value);
	document.form.wf4_passwd.value = decodeURIComponent(document.form.wf4_passwd_org.value);

	writeSSID_SP();
	isAuthenticated_SP();
//	slowshowHint_SP();
	WAN_IP_CHECK();
}

function refreshYA(){
	fastHide();
	location.reload();
}

function ShowPass(y){
	if (y==0 || y==4)
		document.getElementById("wep_password").style.display = 'none';
	else
		document.getElementById("wep_password").style.display = 'block';
}

function checkPW(){
	if (!validate_ssid(document.form.ssid))
		return false;

	if(	document.form.SecurityLevel.selectedIndex == 0 ||
		document.form.SecurityLevel.selectedIndex == 4
	)
	{
		show_hint();
//		confirm_hint(document.form.op_mode.value);
	}
	else if(document.form.SecurityLevel.selectedIndex == 1 ||
		document.form.SecurityLevel.selectedIndex == 2 ||
		document.form.SecurityLevel.selectedIndex == 3
	)
	{
		if (validate_wlphrase_q('WLANConfig11b', document.form.wl_wpa_psk))
		{
			show_hint();
//			confirm_hint(document.form.op_mode.value);
		}
	}
}//end of function checkPW()

function confirm_hint(op_mode){
	wl_mode_current=is_current_Mode(document.form.wan_nat_X.value,document.form.wl_mode_EX.value);
	IP_RNAGE_STR_GLOBAL=lip_range(document.form.LANIP.value, document.form.LANNM.value, document.form.preferred_lang.value);
	LAN_NETMASK_GLOBAL=document.form.LANNM.value;

	//  AP -> Gateway
	if(op_mode == "router" && wl_mode_current == "AP")
	{
		hint_word = quick_translate_lang('JS_hint_ap_gw',document.form.preferred_lang.value);
	}
	//  Adapter  -> Gateway
	else if(op_mode == "router" && wl_mode_current == "Adapter")
	{
		hint_word = quick_translate_lang('JS_hint_sta_gw',document.form.preferred_lang.value);
	}
	//  Repeater -> Gateway
	else if(op_mode == "router" && wl_mode_current == "Repeater")
	{
		hint_word = quick_translate_lang('JS_hint_re_gw',document.form.preferred_lang.value);
	}
	//  Gateway-Adapter -> Gateway
	else if(op_mode == "router" && wl_mode_current == "Gateway-Adapter")
	{
		hint_word = quick_translate_lang('JS_hint_gwsta_gw',document.form.preferred_lang.value);
	}
	//  Gateway(Hotspot) -> Gateway
	else if(op_mode == "router" && wl_mode_current == "Gateway(Hotspot)")
	{
		hint_word = quick_translate_lang('JS_hint_gwre_gw',document.form.preferred_lang.value);
	}


	//  AP -> Gateway(Hotspot)
	else if(op_mode == "gw-repeater" && wl_mode_current == "AP")
	{
		hint_word = quick_translate_lang('JS_hint_ap_gwre',document.form.preferred_lang.value);
	}
	//  Adapter  -> Gateway(Hotspot)
	else if(op_mode == "gw-repeater" && wl_mode_current == "Adapter")
	{
		hint_word = quick_translate_lang('JS_hint_sta_gwre',document.form.preferred_lang.value);
	}
	//  Repeater -> Gateway(Hotspot)
	else if(op_mode == "gw-repeater" && wl_mode_current == "Repeater")
	{
		hint_word = quick_translate_lang('JS_hint_re_gwre',document.form.preferred_lang.value);
	}
	//  Gateway -> Gateway(Hotspot)
	else if(op_mode == "gw-repeater" && wl_mode_current == "Gateway")
	{
		hint_word = quick_translate_lang('JS_hint_gw_gwre',document.form.preferred_lang.value);
	}
	//  Gateway-Adapter -> Gateway(Hotspot)
	else if(op_mode == "gw-repeater" && wl_mode_current == "Gateway-Adapter")
	{
		hint_word = quick_translate_lang('JS_hint_gwsta_gwre',document.form.preferred_lang.value);
	}

	//  AP -> Gateway-Adapter
	else if(op_mode == "gw-adapter" && wl_mode_current == "AP")
	{
		hint_word = quick_translate_lang('JS_hint_ap_gwsta',document.form.preferred_lang.value);
	}
	//  Adapter  -> Gateway-Adapter
	else if(op_mode == "gw-adapter" && wl_mode_current == "Adapter")
	{
		hint_word = quick_translate_lang('JS_hint_sta_gwsta',document.form.preferred_lang.value);
	}
	//  Repeater -> Gateway-Adapter
	else if(op_mode == "gw-adapter" && wl_mode_current == "Repeater")
	{
		hint_word = quick_translate_lang('JS_hint_re_gwsta',document.form.preferred_lang.value);
	}
	//  Gateway -> Gateway-Adapter
	else if(op_mode == "gw-adapter" && wl_mode_current == "Gateway")
	{
		hint_word = quick_translate_lang('JS_hint_gw_gwsta',document.form.preferred_lang.value);
	}
	//  Gateway(Hotspot) -> Gateway-Adapter
	else if(op_mode == "gw-adapter" && wl_mode_current == "Gateway-Adapter")
	{
		hint_word = quick_translate_lang('JS_hint_gwre_gwsta',document.form.preferred_lang.value);
	}

	//  Gateway -> AP
	else if(op_mode == "AP" && wl_mode_current == "Gateway")
	{
		hint_word = quick_translate_lang('JS_hint_gw_ap',document.form.preferred_lang.value);
	}
	//  Adapter -> AP	
	else if(op_mode == "AP" && wl_mode_current == "Adapter")
	{
		hint_word = quick_translate_lang('JS_hint_sta_ap',document.form.preferred_lang.value);
	}
	//  Repeater -> AP	
	else if(op_mode == "AP" && wl_mode_current == "Repeater")
	{
		hint_word = quick_translate_lang('JS_hint_re_ap',document.form.preferred_lang.value);
	}
	//  Gateway-Adapter -> AP	
	else if(op_mode == "AP" && wl_mode_current == "Gateway-Adapter")
	{
		hint_word = quick_translate_lang('JS_hint_gwsta_ap',document.form.preferred_lang.value);
	}
	//  Gateway(Hotspot) -> AP	
	else if(op_mode == "AP" && wl_mode_current == "Gateway(Hotspot)")
	{
		hint_word = quick_translate_lang('JS_hint_gwre_ap',document.form.preferred_lang.value);
	}


	//  Gateway -> Adapter	
	else if(op_mode == "adapter" && wl_mode_current == "Gateway")
	{
		hint_word = quick_translate_lang('JS_hint_gw_sta',document.form.preferred_lang.value);
	}			
	//  AP -> Adapter		
	else if(op_mode == "adapter" && wl_mode_current == "AP")
	{
		hint_word = quick_translate_lang('JS_hint_ap_sta',document.form.preferred_lang.value);
	}				
	//  Repeater -> Adapter		
	else if(op_mode == "adapter" && wl_mode_current == "Repeater")
	{
		hint_word = quick_translate_lang('JS_hint_re_sta',document.form.preferred_lang.value);
	}
	//  Gateway-Adapter -> Adapter		
	else if(op_mode == "adapter" && wl_mode_current == "Gateway-Adapter")
	{
		hint_word = quick_translate_lang('JS_hint_gwsta_sta',document.form.preferred_lang.value);
	}
	//  Gateway(Hotspot) -> Adapter		
	else if(op_mode == "adapter" && wl_mode_current == "Gateway(Hotspot)")
	{
		hint_word = quick_translate_lang('JS_hint_gwre_sta',document.form.preferred_lang.value);
	}


	//  Gateway -> Repeater	
	else if(op_mode == "repeater" && wl_mode_current == "Gateway")
	{
		hint_word = quick_translate_lang('JS_hint_gw_re',document.form.preferred_lang.value);
	}			
	//  AP -> Repeater		
	else if(op_mode == "repeater" && wl_mode_current == "AP")
	{
		hint_word = quick_translate_lang('JS_hint_ap_re',document.form.preferred_lang.value);
	}				
	//  Adapter -> Repeater		
	else if(op_mode == "repeater" && wl_mode_current == "Adapter")
	{
		hint_word = quick_translate_lang('JS_hint_sta_re',document.form.preferred_lang.value);
	}
	//  Gateway-Adapter -> Repeater		
	else if(op_mode == "repeater" && wl_mode_current == "Gateway-Adapter")
	{
		hint_word = quick_translate_lang('JS_hint_gwsta_re',document.form.preferred_lang.value);
	}
	//  Gateway(Hotspot) -> Repeater		
	else if(op_mode == "repeater" && wl_mode_current == "Gateway(Hotspot)")
	{
		hint_word = quick_translate_lang('JS_hint_gwre_re',document.form.preferred_lang.value);
	}


	else 
	{
		hint_word = quick_translate_lang('JS_hint_others',document.form.preferred_lang.value);
	}

	return hint_word+quick_translate_lang('JS_LAN_IP_hint',document.form.preferred_lang.value);
}	

function isAuthenticated()
{
	count=ap_count;
	if (count>0)
	{
		if (count>1)
		{
			for(var i = 0; i < count; i++)
			{
				if(AP_status[i][4] == "authenticated")
				{
					document.form.AP_select[i].checked = true;
					return;
				}
			}
			for(var i = 0; i < count; i++)
			{
				if(AP_status[i][4] != "none")
				{
					document.form.AP_select[i].checked = true;
					return;
				}
			}
			document.form.AP_select[0].checked = true;
		}
		else if (count==1)
			document.form.AP_select.checked = true;
	}
}//end of function isAuthenticated=================

function isAuthenticated_SP()
{
	if (document.form.old_wl_hotspot.value=="1")
		isAuthenticated();
}

function show_hint()
{
	document.getElementById("popupHint").style.visibility="visible";
	document.getElementById("selectMenumain").style.visibility="hidden";
	if (document.form.current_page.value=="Wizard_Gateway.asp" || document.form.current_page.value=="Wizard_AP.asp")
		document.getElementById("SecurityLevel").style.visibility="hidden";  
}//end of function show_hint

function hide_hint()
{
	if(document.form.op_mode.value!="AP" && document.form.op_mode.value!="router")
		document.form.add_profile.value="0";
	document.getElementById("popupHint").style.visibility="hidden";
	document.getElementById("selectMenumain").style.visibility="visible"; 
	if (document.form.current_page.value=="Wizard_Gateway.asp" || document.form.current_page.value=="Wizard_AP.asp")
		document.getElementById("SecurityLevel").style.visibility="visible";
}

function add_option_x(o, i, s, f)
{
	tail = o.options.length;

	o.options[tail] = new Option(s);
	o.options[tail].value = i;

	if (f==1)
	{
		o.options[tail].selected = f;
	}
}

function change_security(o, s, l)
{
	if (o.value == "0")
	{
		pflag = 0;
		wflag = 0;

		authTypeValue = "open"; // Open or Shared
		wepTypeValue = "0";	// None
		cryptoValue = "tkip";

		document.getElementById("wep_password").style.display ="none";
	}
	else if (o.value == "1")
	{
		pflag = 1;
		wflag = 1;

		authTypeValue = "open";	// Shared
		wepTypeValue = "1";     // WEP-128 bits
		cryptoValue = "tkip";
	}
	else if (o.value == "2")
	{
		pflag = 1;
	    	wflag = 1;

		authTypeValue = "open";	// Shared
		wepTypeValue = "2";     // WEP-128 bits
		cryptoValue = "tkip";
	}
	else if (o.value == "3")
	{
		pflag = 1;
		wflag = 0;

		authTypeValue = "psk";	// WPA-PSK only
		wepTypeValue= "0";	// TKIP
		cryptoValue = "tkip";
	}
	else if (o.value == "4")
	{
		pflag = 0;
		wflag = 0;
		authTypeValue = "";
    		wepTypeValue = "";
		cryptoValue = "";
	}
	if (o.options.length == 5 && o.value != "4")
	{
		if (confirm(quick_translate_lang('JS_wpa',document.form.preferred_lang.value)))
		{
			o.options[4].value = null;
			o.options[4] = null;

			document.form.wl_auth_mode.value = authTypeValue;
			document.form.wl_crypto.value = cryptoValue;
			document.form.wl_wep_x.value.value = wepTypeValue;
		}
		else
		{
			pflag = 0;
			wflag = 0;
			o.value = "4";
			authTypeValue="";
			cryptoValue="";
			wepTypeValue="";
		}
	}

	ShowPass(o.selectedIndex);

	if (authTypeValue!="")
	{
		document.form.wl_auth_mode.value = authTypeValue;
		document.form.wl_crypto.value = cryptoValue;
		document.form.wl_wep_x.value = wepTypeValue;
	}

	if (l==1)
	{
		if (o.value=="1" || o.value=="2")
		{
			if (document.form.wl_key.value=="1")
				document.form.wl_wpa_psk.value=document.form.wl_key1.value;
			else if (document.form.wl_key.value=="2")
				document.form.wl_wpa_psk.value=document.form.wl_key2.value;
			else if (document.form.wl_key.value=="3")
				document.form.wl_wpa_psk.value=document.form.wl_key3.value;
			else if (document.form.wl_key.value=="4")
				document.form.wl_wpa_psk.value=document.form.wl_key4.value;
		}
	}
	else
	{
		if (o.value=="1")
		{
			is_wlphrase("WLANConfig11b", 'wl_phrase_x', document.form.wl_wpa_psk);
		}
		else if (o.value=="2")
		{
			is_wlphrase("WLANConfig11b", 'wl_phrase_x', document.form.wl_wpa_psk);
		}
		if (o.value=="3")
			document.form.wl_wpa_psk.value=document.form.wl_wpa_psk_org.value;
	}

	if (l==0 && o.value != "0")
	{
		document.form.wl_wpa_psk.focus();
	}
}

function load_URE_Security()
{
	document.form.wl_guest_wpa_psk_1_org.value = decodeURIComponent(document.form.wl_guest_wpa_psk_1_org_encoded.value);
	document.form.wl_guest_key1_1_org.value = decodeURIComponent(document.form.wl_guest_key1_1_org_encoded.value);
	document.form.wl_guest_key2_1_org.value = decodeURIComponent(document.form.wl_guest_key2_1_org_encoded.value);
	document.form.wl_guest_key3_1_org.value = decodeURIComponent(document.form.wl_guest_key3_1_org_encoded.value);
	document.form.wl_guest_key4_1_org.value = decodeURIComponent(document.form.wl_guest_key4_1_org_encoded.value);

	inputCtrl(document.form.wl_guest_wpa_psk_1_org_encoded, 0);
	inputCtrl(document.form.wl_guest_key1_1_org_encoded, 0);
	inputCtrl(document.form.wl_guest_key2_1_org_encoded, 0);
	inputCtrl(document.form.wl_guest_key3_1_org_encoded, 0);
	inputCtrl(document.form.wl_guest_key4_1_org_encoded, 0);

	document.form.wl_auth_mode.value=document.form.wl_guest_auth_mode_1_org.value;
	document.form.wl_wpa_mode.value=document.form.wl_guest_wpa_mode_org.value;
	document.form.wl_crypto.value=document.form.wl_guest_crypto_1_org.value;
	document.form.wl_wpa_psk.value=document.form.wl_guest_wpa_psk_1_org.value;
	document.form.wl_wep_x.value=document.form.wl_guest_wep_x_1_org.value;
	document.form.wl_key1.value=document.form.wl_guest_key1_1_org.value;
	document.form.wl_key2.value=document.form.wl_guest_key2_1_org.value;
	document.form.wl_key3.value=document.form.wl_guest_key3_1_org.value;
	document.form.wl_key4.value=document.form.wl_guest_key4_1_org.value;
	document.form.wl_key.value=document.form.wl_guest_key_1_org.value;
}

function loadSecurity()
{
	document.form.wl_wpa_psk.value = decodeURIComponent(document.form.wl_wpa_psk_org_encoded.value);
	document.form.wl_wpa_psk_org.value = decodeURIComponent(document.form.wl_wpa_psk_org_encoded.value);
	document.form.wl_key1.value = decodeURIComponent(document.form.wl_key1_org.value);
	document.form.wl_key2.value = decodeURIComponent(document.form.wl_key2_org.value);
	document.form.wl_key3.value = decodeURIComponent(document.form.wl_key3_org.value);
	document.form.wl_key4.value = decodeURIComponent(document.form.wl_key4_org.value);

	if(document.form.op_mode.value=="gw-repeater")
		load_URE_Security();

	authType=document.form.wl_auth_mode.value;
	cryptoType=document.form.wl_crypto.value;
	wepType=document.form.wl_wep_x.value;

	if (authType == "open" && wepType == "0")
	{
		document.form.SecurityLevel.value = "0";
		document.getElementById("wep_password").style.display ="none";
	}
	else if (authType == "open" && wepType == "1")
	{
		document.form.SecurityLevel.value = "1";
	}
	else if (authType == "open" && wepType == "2")
	{
		document.form.SecurityLevel.value = "2";
	}
	else if (authType == "psk" && cryptoType == "tkip")
	{
		document.form.SecurityLevel.value = "3";
	}
	else 
	{
		add_option_x(document.form.SecurityLevel, 4, "Extra", 1);
		document.form.SecurityLevel.value = "4";
		document.getElementById("wep_password").style.display ="none";
	}

	change_security(document.form.SecurityLevel, "WLANConfig11b", 1);
}

function validate_wlphrase(s, v, o)
{
	if (v == "wl_wpa_psk")
	{
		if(document.form.wl_auth_mode.value == "psk")
		{
			if (o.value.length < 8)
			{
				alert(quick_translate_lang('JS_passzero',document.form.preferred_lang.value));
				if (v == "wl_wpa_psk")
					document.form.wl_wpa_psk.value = "12345678";
				else if (v == "wl_guest_wpa_psk_1")
					document.form.wl_guest_wpa_psk_1.value = "12345678";
				else if (v == "mssid_wpa_psk_x_0")
					document.form.mssid_wpa_psk_x_0.value = "12345678";
				o.focus();
				return false;
			}
		}
	}

	return true;
}

function validate_wlphrase_client(s, o)
{
	if (AP_status[select_idx][2]=="1")
		return(validate_wlkey_client(o, 'WLANConfig11b'))
	else
		return(validate_psk_client(s, 'wl_wpa_psk', o));
}

function validate_wlphrase_client_SP()
{
	if (	(document.getElementById('prefer_security').selectedIndex==0 && document.getElementById('wep_crypto').selectedIndex!=0) ||
		document.getElementById('prefer_security').selectedIndex==1
	)
		return(validate_wlkey_client_SP())
	else if (document.getElementById('prefer_security').selectedIndex==2 ||document.getElementById('prefer_security').selectedIndex==3)
		return(validate_psk_client_SP());
	else
		return true;
}

function validate_wlkey_client(o, s)
{
	if ((o.value.length == 10 || o.value.length == 26) && isvalid_wepkey(o.value))
	{
		return true;
	}
	else if ((o.value.length == 5 || o.value.length == 13) && isvalid_wepkey_ascii(o.value))
	{
		return true;
	}
	else
	{
		alert(quick_translate_lang('JS_wepkey',document.form.preferred_lang.value));
		o.value = "0000000000";
		o.focus();
		return false;
	}
}

function validate_wlkey_client_SP()
{
	o=document.getElementById('wep_key_str');

	if (isvalid_wepkey(document.getElementById('wep_key_str').value))
	{
		if (	(document.getElementById('wep_crypto').value=='1' && document.getElementById('wep_key_str').value.length == 10) ||
			(document.getElementById('wep_crypto').value=='2' && document.getElementById('wep_key_str').value.length == 26)
		)
			return true;
	}
	if (isvalid_wepkey_ascii(document.getElementById('wep_key_str').value))
	{
		if (	(document.getElementById('wep_crypto').value=='1' && document.getElementById('wep_key_str').value.length == 5) ||
			(document.getElementById('wep_crypto').value=='2' && document.getElementById('wep_key_str').value.length == 13)
		)
			return true;
	}
	if (document.getElementById('wep_crypto').value=='1')
		alert(quick_translate_lang('JS_wepkey1',document.form.preferred_lang.value));
	else
		alert(quick_translate_lang('JS_wepkey2',document.form.preferred_lang.value));
	if (document.getElementById('wep_crypto').value=='1')
		o.value = "0000000000";
	else
		o.value = "00000000000000000000000000";
	o.focus();
	return false;
}

function validate_psk_client(s, v, o)
{
	if (v == "wl_wpa_psk")
	{
		if (o.value.length < 8)
		{
			alert(quick_translate_lang('JS_passzero',document.form.preferred_lang.value));
			if (v == "wl_wpa_psk")
			{
				if(document.form.op_mode.value=="gw-repeater")
					document.form.wl_wpa_psk_ure.value = "12345678";
				else
					document.form.wl_wpa_psk.value = "12345678";
			}
			else if (v == "wl_guest_wpa_psk_1")
				document.form.wl_guest_wpa_psk_1.value = "12345678";
			else if (v == "mssid_wpa_psk_x_0")
				document.form.mssid_wpa_psk_x_0.value = "12345678";
			o.focus();
			return false;
		}
	}

	return true;
}

function validate_psk_client_SP()
{
	o=document.getElementById('wpa_psk');

	if (o.value.length < 8)
	{
		alert(quick_translate_lang('JS_passzero',document.form.preferred_lang.value));
		o.value = "12345678";
		o.focus();
		return false;
	}

	return true;
}

function validate_wlphrase_q(s, o)
{
	if (document.form.SecurityLevel.value=="1" || document.form.SecurityLevel.value=="2")
		return(validate_wlkey(o, 'WLANConfig11b'))
	else if (document.form.SecurityLevel.value=="3")
		return(validate_wlphrase(s, 'wl_wpa_psk', o));
	else
		return false;
}

function isvalid_wepkey(v)
{
	for (i = 0; i < v.length; ++i) {
		var c = v.charAt(i);
		var c1 = (c >= '0') && (c <= '9');  /* 0 ~ 9 */
		var c2 = (c >= 'A') && (c <= 'F');  /* A ~ F */
		var c3 = (c >= 'a') && (c <= 'f');  /* a ~ f */
		if (!c1 && !c2 && !c3) {
			return false;
		}
	}
	return true;
}

function isvalid_wepkey_ascii(v)
{
	for (i = 0; i < v.length; ++i) {
		var c = v.charAt(i);
		if (c>'~' || c<' ') {
			return false;
		}
	}
	return true;
}

function validate_wlkey(o, s)
{
	wep = document.form.wl_wep_x.value;

	if (wep == "1")
	{
		if(isvalid_wepkey(o.value) && o.value.length == 10)
		{
			document.form.wl_key_type.value="0";
			return true;
		}
		else if(isvalid_wepkey_ascii(o.value) && o.value.length == 5)
		{
			document.form.wl_key_type.value="1";
			return true;
		}
	}
	else if (wep == "2")
	{
		if(isvalid_wepkey(o.value) && o.value.length == 26)
		{
			document.form.wl_key_type.value="0";
			return true;
		}
		else if(isvalid_wepkey_ascii(o.value) && o.value.length == 13)
		{
			document.form.wl_key_type.value="1";
			return true;
		}
	}

	if (wep == "1")
		alert(quick_translate_lang('JS_wepkey1',document.form.preferred_lang.value));
	else
		alert(quick_translate_lang('JS_wepkey2',document.form.preferred_lang.value));

	if (wep == "1")
		o.value = "0000000000";
	else if (wep == "2")
		o.value = "00000000000000000000000000";
	o.focus();

	return false;
}

function valid_WPAPSK(o)
{
	if (o.value.length>=64)
	{
		o.value = o.value.substring(0, 63);
		alert(quick_translate_lang('JS_wpapass',document.form.preferred_lang.value));
		o.focus();
		return false;
	}

	return true;
}

function is_wlphrase(s, v, o)
{
	if (v=='wl_wpa_psk')
		return(valid_WPAPSK(o));

	pseed = new Array;
	wep_key = new Array(5);
    	wepType = document.form.wl_wep_x.value;
    	wepKey1 = document.form.wl_key1;
    	wepKey2 = document.form.wl_key2;
    	wepKey3 = document.form.wl_key3;
    	wepKey4 = document.form.wl_key4;

	phrase = o.value;

	if(wepType == "1")
	{
		for (i=0; i<phrase.length; i++)
		{
			pseed[i%4] ^= phrase.charCodeAt(i);
		}

        	randNumber = pseed[0] | (pseed[1]<<8) | (pseed[2]<<16) | (pseed[3]<<24);

        	for (j=0; j<5; j++)
		{
			randNumber = ((randNumber * 0x343fd) % 0x1000000);
			randNumber = ((randNumber + 0x269ec3) % 0x1000000);
			wep_key[j] = ((randNumber>>16) & 0xff);
		}
		wepKey1.value = binl2hex_c(wep_key);

		o.value = wepKey1.value;
	}
	else if (wepType == "2")
	{
		password = "";

		if (phrase.length>0)
		{
			for (i=0; i<64; i++)
			{
				ch = phrase.charAt(i%phrase.length);
				password = password + ch;
			}
		}

		password = calcMD5(password);

		if (wepType == "2")
		{
			wepKey1.value = password.substr(0, 26);
		}
		else
		{
			wepKey1.value = password.substr(0, 32);
		}

		o.value = wepKey1.value;
	}

	return true;
}

function change_wlkey(o, s)
{
	wep = document.form.wl_wep_x.value;

	if (wep == "1")
	{
		if(o.value.length > 10)
			o.value = o.value.substring(0, 10);
	}
	else if (wep == "2")
	{
		if(o.value.length > 26)
			o.value = o.value.substring(0, 26);
	}
	else
		return false;
}

function is_wlphrase_client(s, o)
{
	if (AP_status[select_idx][2]=="1")
		return change_wlkey(o, s);
	else
		return(is_wlphrase(s, 'wl_wpa_psk', o));
}

function is_wlphrase_q(s, o)
{
	if (document.form.SecurityLevel.value=="1" || document.form.SecurityLevel.value=="2")
    		return change_wlkey(o, s);
	else if (document.form.SecurityLevel.value=="3")
		return(is_wlphrase(s, 'wl_wpa_psk', o));
	else
		return false;
}

function is_wlkey(event, o, s) 
{
	if (document.form.SecurityLevel.value=="1" || document.form.SecurityLevel.value=="2")
 	{
		var keyPressed = event.keyCode ? event.keyCode : event.which ? event.which : event.charCode;
		if (nav)
		{
			if (	(keyPressed==8)||	// backspace
				(keyPressed==35)||	// end
				(keyPressed==36)||	// home
				(keyPressed==37)||	// <-
				(keyPressed==39)||	// ->
				(keyPressed==46)	// delete
			)
				keyPressed=0;
		}

 		if (keyPressed == 0) return true;

 		wep = document.form.wl_wep_x.value;
 		
		if (wep == "1")
		{
			if(o.value != "" && o.value.length > 10) return false;
		}
		else if (wep == "2")
		{
			if(o.value != "" && o.value.length > 26) return false;
		}
		else
			return false;

		return true;
	}
	else
/*
	if (document.form.SecurityLevel.value=="1" || document.form.SecurityLevel.value=="2")
 	{
		var keyPressed = event.keyCode ? event.keyCode : event.which ? event.which : event.charCode;
		if (nav)
		{
			if (	(keyPressed==8)||	// backspace
				(keyPressed==35)||	// end
				(keyPressed==36)||	// home
				(keyPressed==37)||	// <-
				(keyPressed==39)||	// ->
				(keyPressed==46)	// delete
			)
				keyPressed=0;
		}

 		if (keyPressed == 0) return true;

 		wep = document.form.wl_wep_x.value;

 		if ((keyPressed>47 && keyPressed<58)||(keyPressed>64 && keyPressed<71)||(keyPressed>96 && keyPressed<103))
 		{
     			if (wep == "1")
     			{
     	  			if(o.value != "" && o.value.length > 10) return false;
     			}
     			else if (wep == "2")
     			{
     	  			if(o.value != "" && o.value.length > 26) return false;
     			}
     			else return false;
 		}
 		else
 			return false;
		return true;
	}
	else
*/
		return true;
}

function quick_translate_lang(o,v)
{
	translated_string="";
	PID_STR=document.form.PID.value;

        if(v=='TW')
        {
		if(o=='JS_wpa')
                	translated_string="您已選擇另一種無線驗證方式及加密功能，您確定要作覆蓋嗎？";
                else if(o=='JS_GW_AP')
                	translated_string='在"家用閘道器-AP"模式下, 您只能透過無線網路連接區域網路！';
		else if(o=='JS_passzero')
			translated_string="WPA預先共用金鑰不得少於8個字元！如果您將本欄位保留空白，系統將會指定12345678作為您的通關密語。";
		else if(o=='JS_wepkey')
			translated_string="請輸入完整的WEP金鑰。\n您可以輸入 10 或 26 個十六進位字元 (0~9, a~f, A~F)， 5 或 13 個ASCII字元。";
		else if(o=='JS_wepkey1')
			translated_string="請輸入完整的WEP金鑰。\n您可以輸入 10 個十六進位字元 (0~9, a~f, A~F)， 或 5 個ASCII字元。";
		else if(o=='JS_wepkey2')
			translated_string="請輸入完整的WEP金鑰。\n您可以輸入 26 個十六進位字元 (0~9, a~f, A~F)， 或 13 個ASCII字元。";
		else if(o=='JS_wpapass')
			translated_string="WPA預先共用金鑰不得超出63個字元！";
		else if(o=='JS_LANG_EN')
			translated_string="英文";
		else if(o=='JS_LANG_TW')
			translated_string="繁體中文";
		else if(o=='JS_LANG_CN')
			translated_string="簡體中文";
		else if(o=='JS_LANG_KR')
			translated_string="韓文";
		else if(o=='JS_LANG_DE')
			translated_string="德文";

		else if(o=='JS_hint_ap_gw')
			translated_string="您正準備由AP模式切換到Gateway模式。<br><br>按下確定後，請將您電腦的IP位址更改為「自動取得IP位址」，始可以無線方式連接設定頁面。<br><br>您確定要切換成Gateway模式？<br><br>";
		else if(o=='JS_hint_sta_gw')
			translated_string="您正準備由Adapter模式切換到Gateway模式。<br><br>按下確定後，請移除您電腦與" + PID_STR + "之間的乙太網路纜線。將" + PID_STR + "（藉由乙太網路纜線）接上您的ADSL/Cable Modem。並將您電腦的IP位址更改為「自動取得IP位址」，以（藉由無線）連結網際網路或進入設定頁面。<br><br>您確定要切換成Gateway模式？";
		else if(o=='JS_hint_re_gw')
			translated_string="您正準備由Repeater模式切換到Gateway模式。<br><br>按下確定後，請將" + PID_STR + "（藉由乙太網路纜線）重新接上您的區域網路。並將您電腦的IP位址更改為「自動取得IP位址」，以（藉由無線）連結網際網路或進入設定頁面。<br><br>您確定要切換成Gateway模式？";
		else if(o=='JS_hint_gwsta_gw')
			translated_string="您確定要切換成Gateway模式？";
		else if(o=='JS_hint_gwre_gw')
			translated_string="您正準備由Gateway(Hotspot)模式切換到Gateway模式。<br><br>按下確定後，請將" + PID_STR + "（藉由乙太網路纜線）重新接上您的區域網路。並將您電腦的IP位址更改為「自動取得IP位址」，以（藉由無線）連結網際網路或進入設定頁面。<br><br>您確定要切換成Gateway模式？";

		else if(o=='JS_hint_ap_gwre')
			translated_string="您正準備由AP模式切換到Gateway(Hotspot)模式。<br><br>按下確定後，請將您的IP位址更改為「自動取得IP位址」，以（藉由無線或乙太網路纜線）連結網際網路或進入設定頁面。<br><br>您確定要切換成Gateway(Hotspot)模式？";
		else if(o=='JS_hint_sta_gwre')
			translated_string="您正準備由Adapter模式切換到Gateway(Hotspot)模式。<br><br>按下確定後，請將您的IP位址更改為「自動取得IP位址」，以（藉由無線或乙太網路纜線）連結網際網路或進入設定頁面。<br><br>您確定要切換成Gateway(Hotspot)模式？";
		else if(o=='JS_hint_re_gwre')
			translated_string="您正準備由Repeater模式切換到Gateway(Hotspot)模式。<br><br>按下確定後，請將您的IP位址更改為「自動取得IP位址」，以（藉由無線或乙太網路纜線）連結網際網路或進入設定頁面。<br><br>您確定要切換成Gateway(Hotspot)模式？";
		else if(o=='JS_hint_gw_gwre')
			translated_string="您正準備由Gateway模式切換到Gateway(Hotspot)模式。<br><br>按下確定後，您可以（藉由無線或乙太網路纜線）連結網際網路或進入設定頁面。<br><br>您確定要切換成Gateway(Hotspot)模式？";
		else if(o=='JS_hint_gwsta_gwre')
			translated_string="您確定要切換成Gateway(Hotspot)模式？";

		else if(o=='JS_hint_ap_gwsta')
			translated_string="您正準備由AP模式切換到Gateway-Adapter模式。<br><br>按下確定後，請將您的IP位址更改為「自動取得IP位址」，以（藉由乙太網路纜線）連結網際網路或進入設定頁面。<br><br>您確定要切換成Gateway-Adapter模式？";
		else if(o=='JS_hint_sta_gwsta')
			translated_string="您正準備由Adapter模式切換到Gateway-Adapter模式。<br><br>按下確定後，請將您的IP位址更改為「自動取得IP位址」，以（藉由乙太網路纜線）連結網際網路或進入設定頁面。<br><br>您確定要切換成Gateway-Adapter模式？";
		else if(o=='JS_hint_re_gwsta')
			translated_string="您正準備由Repeater模式切換到Gateway-Adapter模式。<br><br>按下確定後，請將您的IP位址更改為「自動取得IP位址」，以（藉由乙太網路纜線）連結網際網路或進入設定頁面。<br><br>您確定要切換成Gateway-Adapter模式？";
		else if(o=='JS_hint_gw_gwsta')
			translated_string="您正準備由Gateway模式切換到Gateway-Adapter模式。<br><br>按下確定後，您可以（藉由乙太網路纜線）連結網際網路或進入設定頁面。<br><br>您確定要切換成Gateway-Adapter模式？";
		else if(o=='JS_hint_gwre_gwsta')
			translated_string="您確定要切換成Gateway-Adapter模式？";

		else if(o=='JS_hint_gw_ap')
			translated_string="您正準備由Gateway模式切換到AP模式。<br><br>按下確定後，若要進入設定頁面，請先將您電腦的IP位址更改為" + IP_RNAGE_STR_GLOBAL + "﹙子網路遮罩" + LAN_NETMASK_GLOBAL + "﹚。<br><br>您確定要切換成AP模式？";
		else if(o=='JS_hint_sta_ap')
			translated_string="您正準備由Adapter模式切換到AP模式。<br><br>按下確定後，請移除您電腦與" + PID_STR + "之間的乙太網路纜線，並將" + PID_STR + "（藉由乙太網路纜線）接上您的區域網路。若要（藉由無線）進入設定頁面，請先將您電腦的IP位址更改為" + IP_RNAGE_STR_GLOBAL + "﹙子網路遮罩" + LAN_NETMASK_GLOBAL + "﹚。<br><br>您確定要切換成AP模式？";
		else if(o=='JS_hint_re_ap')
			translated_string="您正準備由Repeater模式切換到AP模式。<br><br>按下確定後，請將" + PID_STR + "（藉由乙太網路纜線）重新接上您的區域網路。若要（藉由無線）進入設定頁面，請先將您電腦的IP位址更改為" + IP_RNAGE_STR_GLOBAL + "﹙子網路遮罩" + LAN_NETMASK_GLOBAL + "﹚。<br><br>您確定要切換成AP模式？";
		else if(o=='JS_hint_gwsta_ap')
			translated_string="您確定要切換成AP模式？";
		else if(o=='JS_hint_gwre_ap')
			translated_string="您正準備由Gateway(Hotspot)模式切換到AP模式。<br><br>按下確定後，請將" + PID_STR + "（藉由乙太網路纜線）重新接上您的區域網路。若要（藉由無線）進入設定頁面，請先將您電腦的IP位址更改為" + IP_RNAGE_STR_GLOBAL + "﹙子網路遮罩" + LAN_NETMASK_GLOBAL + "﹚。<br><br>您確定要切換成AP模式？";

		else if(o=='JS_hint_gw_sta')
			translated_string="您正準備由Gateway模式切換到Adapter模式，<br><br>按下確定後，請以乙太網路纜線連接您的電腦與" + PID_STR + "，並手動將電腦IP位址改為" + IP_RNAGE_STR_GLOBAL + "﹙子網路遮罩" + LAN_NETMASK_GLOBAL + "﹚，始可（藉由乙太網路纜線）重新進入設定頁面。<br><br>您確定要切換成Adapter模式？";
		else if(o=='JS_hint_ap_sta')
			translated_string="您正準備由AP模式切換到Adapter模式。<br><br>按下確定後，請以乙太網路纜線連接您的電腦與" + PID_STR + "，並手動將電腦IP位址改為" + IP_RNAGE_STR_GLOBAL + "﹙子網路遮罩" + LAN_NETMASK_GLOBAL + "﹚，始可（藉由乙太網路纜線）重新進入設定頁面。<br><br>您確定要切換成Adapter模式？";
		else if(o=='JS_hint_re_sta')
			translated_string="您正準備由Repeater模式切換到Adapter模式。<br><br>按下確定後，請以乙太網路纜線連接您的電腦與" + PID_STR + "，並手動將電腦IP位址改為" + IP_RNAGE_STR_GLOBAL + "﹙子網路遮罩" + LAN_NETMASK_GLOBAL + "﹚，始可（藉由乙太網路纜線）重新進入設定頁面。<br><br>您確定要切換成Adapter模式？";
		else if(o=='JS_hint_gwsta_sta')
			translated_string="您確定要切換成Adapter模式？";
		else if(o=='JS_hint_gwre_sta')
			translated_string="您正準備由Gateway(Hotspot)模式切換到Adapter模式。<br><br>按下確定後，請以乙太網路纜線連接您的電腦與" + PID_STR + "，並手動將電腦IP位址改為" + IP_RNAGE_STR_GLOBAL + "﹙子網路遮罩" + LAN_NETMASK_GLOBAL + "﹚，始可（藉由乙太網路纜線）重新進入設定頁面。<br><br>您確定要切換成Adapter模式？";

		else if(o=='JS_hint_gw_re')
			translated_string="您正準備由Gateway模式切換到Repeater模式。<br><br>按下確定後，請將您的IP位址更改為「自動取得IP位址」以藉由無線或乙太網路纜線連結網際網路，若要再次進入設定頁面，請將先電腦IP位址設為" + IP_RNAGE_STR_GLOBAL + "﹙子網路遮罩" + LAN_NETMASK_GLOBAL + "﹚。<br><br>您確定要切換成Repeater模式？";
		else if(o=='JS_hint_ap_re')
			translated_string="您正準備由AP模式切換到Repeater模式。<br><br>按下確定後，請將您的IP位址更改為「自動取得IP位址」以藉由無線或乙太網路纜線連結網際網路，若要再次進入設定頁面，請將先電腦IP位址設為" + IP_RNAGE_STR_GLOBAL + "﹙子網路遮罩" + LAN_NETMASK_GLOBAL + "﹚。<br><br>您確定要切換成Repeater模式？";
		else if(o=='JS_hint_sta_re')
			translated_string="您正準備由Adapter模式切換到Repeater模式。<br><br>按下確定後，請將您的IP位址更改為「自動取得IP位址」以藉由無線或乙太網路纜線連結網際網路，若要再次進入設定頁面，請將先電腦IP位址設為" + IP_RNAGE_STR_GLOBAL + "﹙子網路遮罩" + LAN_NETMASK_GLOBAL + "﹚。<br><br>您確定要切換成Repeater模式？";
		else if(o=='JS_hint_gwsta_re')
			translated_string="您確定要切換成Repeater模式？";
		else if(o=='JS_hint_gwre_re')
			translated_string="您正準備由Gateway(Hotspot)模式切換到Repeater模式。<br><br>按下確定後，請將您的IP位址更改為「自動取得IP位址」以藉由無線或乙太網路纜線連結網際網路，若要再次進入設定頁面，請將先電腦IP位址設為" + IP_RNAGE_STR_GLOBAL + "﹙子網路遮罩" + LAN_NETMASK_GLOBAL + "﹚。<br><br>您確定要切換成Repeater模式？";
			
		else if(o=='JS_hint_others')
			translated_string="您確定要儲存設定？";

		else if(o=='JS_hint_autoip')
			translated_string="若要連接網際網路或無線區域網路，請先將您電腦的IP設定更改為「<strong>自動取得IP位址</strong>」。";

		else if(o=='JS_unsupported')
			translated_string="不支援此類型的AP！";
		else if(o=='JS_validstr')
			translated_string="本字串無法包含";
		else if(o=='authenticated')
			translated_string="已連線";
		else if(o=='connecting')
			translated_string="連線中";
		else if(o=='default')
			translated_string="慣用的無線網路";
		else if(o=='5')
			translated_string="非常好";
		else if(o=='4')
			translated_string="良好";
		else if(o=='3')
			translated_string="好";
		else if(o=='2')
			translated_string="尚可";
		else if(o=='1')
			translated_string="差";
		else if(o=='LAN_WAN_match')
			translated_string='目前的LAN端IP子網路與WAN端IP子網路相衝突！請至"進階設定"更改您的LAN端IP子網路設定！';
		else if(o=='JS_fieldblank')
		        translated_string="此欄位不可以空白！";
		else if(o=='JS_LAN_IP_hint')
		        translated_string="<br><br><br><br><i>註：" + PID_STR + "的 LAN端 IP位址將會是 <b>" + document.form.LANIP.value + "</b> </i>。";
        }
        else if(v=='CN')
        {
		if(o=='JS_wpa')
                	translated_string="您已选择另一种无线认证方式及加密功能，您确定要覆盖吗？";
                else if(o=='JS_GW_AP')
                	translated_string='在"家用网关-AP"模式下, 您只能透过无线网路连接局域网！';
		else if(o=='JS_passzero')
		        translated_string="WPA预先共享金钥不得少于8个字符！如果您将本栏目保留空白，系统将会指定12345678作为您的密码。";
		else if(o=='JS_wepkey')
		        translated_string="请输入完整的WEP密钥。\n您可以输入 10 或 26 个十六进制字符 (0~9, a~f, A~F)， 5 或 13 个ASCII字符。";
		else if(o=='JS_wepkey1')
		        translated_string="请输入完整的WEP密钥。\n您可以输入 10 个十六进制字符 (0~9, a~f, A~F)， 或 5 个ASCII字符。";
		else if(o=='JS_wepkey2')
		        translated_string="请输入完整的WEP密钥。\n您可以输入 26 个十六进制字符 (0~9, a~f, A~F)， 或 13 个ASCII字符。";
		else if(o=='JS_wpapass')
		        translated_string="WPA预先共享金钥不得多于63个字符！";
		else if(o=='JS_LANG_EN')
			translated_string="英文";
		else if(o=='JS_LANG_TW')
			translated_string="繁体中文";
		else if(o=='JS_LANG_CN')
			translated_string="简体中文";
		else if(o=='JS_LANG_KR')
			translated_string="韩文";
		else if(o=='JS_LANG_DE')
			translated_string="德文";

		else if(o=='JS_hint_ap_gw')
			translated_string="您正准备由AP模式切换到Gateway模式。<br><br>按下确定后，请将您电脑的IP地址更改为「自动获得IP地址」，始可以无线方式连接设定页面。<br><br>您确定要切换成Gateway模式？";
		else if(o=='JS_hint_sta_gw')
			translated_string="您正准备由Adapter模式切换到Gateway模式。<br><br>按下确定后，请移除您电脑与" + PID_STR + "之间的以太网络线。将" + PID_STR + "（藉由以太网络线）连上您的ADSL/Cable Modem。并将您电脑的IP地址更改为「自动获得IP地址」，始可以无线连接进入设定页面。<br><br>您确定要切换成Gateway模式？";
		else if(o=='JS_hint_re_gw')
			translated_string="您正准备由Repeater模式切换到Gateway模式。<br><br>按下确定后，请将" + PID_STR + "（藉由以太网络线）重新接上您的局域网。并将您电脑的IP地址更改为「自动获得IP地址」，始可以无线连接连结因特网或进入设定页面。<br><br>您确定要切换成Gateway模式？";
		else if(o=='JS_hint_gwsta_gw')
			translated_string="您确定要切换成Gateway模式？";
		else if(o=='JS_hint_gwre_gw')
			translated_string="您正准备由Gateway(Hotspot)模式切换到Gateway模式。<br><br>按下确定后，请将" + PID_STR + "（藉由以太网络线）重新接上您的局域网。并将您电脑的IP地址更改为「自动获得IP地址」，始可以无线连接连结因特网或进入设定页面。<br><br>您确定要切换成Gateway模式？";

		else if(o=='JS_hint_ap_gwre')
			translated_string="您正准备由AP模式切换到Gateway(Hotspot)模式。<br><br>按下确定后，请将您电脑的IP地址更改为「自动获得IP地址」，以（藉由无线连接或以太网络线）连结因特网或进入接设定页面。<br><br>您确定要切换成Gateway(Hotspot)模式？";
		else if(o=='JS_hint_sta_gwre')
			translated_string="您正准备由Adapter模式切换到Gateway(Hotspot)模式。<br><br>按下确定后，请将您电脑的IP地址更改为「自动获得IP地址」，以（藉由无线连接或以太网络线）连结因特网或进入接设定页面。<br><br>您确定要切换成Gateway(Hotspot)模式？";
		else if(o=='JS_hint_re_gwre')
			translated_string="您正准备由Repeater模式切换到Gateway(Hotspot)模式。<br><br>按下确定后，请将您电脑的IP地址更改为「自动获得IP地址」，以（藉由无线连接或以太网络线）连结因特网或进入接设定页面。<br><br>您确定要切换成Gateway(Hotspot)模式？";
		else if(o=='JS_hint_gw_gwre')
			translated_string="您正准备由Gateway模式切换到Gateway(Hotspot)模式。<br><br>按下确定后，您可以（藉由无线连接或以太网络线）连结因特网或进入接设定页面。<br><br>您确定要切换成Gateway(Hotspot)模式？";
		else if(o=='JS_hint_gwsta_gwre')
			translated_string="您确定要切换成Gateway(Hotspot)模式？";

		else if(o=='JS_hint_ap_gwsta')
			translated_string="您正准备由AP模式切换到Gateway-Adapter模式。<br><br>按下确定后，请将您电脑的IP地址更改为「自动获得IP地址」，以（藉由以太网络线）连结因特网或进入接设定页面。<br><br>您确定要切换成Gateway-Adapter模式？";
		else if(o=='JS_hint_sta_gwsta')
			translated_string="您正准备由Adapter模式切换到Gateway-Adapter模式。<br><br>按下确定后，请将您电脑的IP地址更改为「自动获得IP地址」，以（藉由以太网络线）连结因特网或进入接设定页面。<br><br>您确定要切换成Gateway-Adapter模式？";
		else if(o=='JS_hint_re_gwsta')
			translated_string="您正准备由Repeater模式切换到Gateway-Adapter模式。<br><br>按下确定后，请将您电脑的IP地址更改为「自动获得IP地址」，以（藉由以太网络线）连结因特网或进入接设定页面。<br><br>您确定要切换成Gateway-Adapter模式？";
		else if(o=='JS_hint_gw_gwsta')
			translated_string="您正准备由Gateway模式切换到Gateway-Adapter模式。<br><br>按下确定后，您可以（藉由以太网络线）连结因特网或进入接设定页面。<br><br>您确定要切换成Gateway-Adapter模式？";
		else if(o=='JS_hint_gwre_gwsta')
			translated_string="您确定要切换成Gateway-Adapter模式？";

		else if(o=='JS_hint_gw_ap')
			translated_string="您正准备由Gateway模式切换到AP模式。<br><br>按下确定后，若要进入设定页面，请先将您电脑的IP地址更改为" + IP_RNAGE_STR_GLOBAL + "（网络掩码" + LAN_NETMASK_GLOBAL + "）。<br><br>您确定要切换成AP模式？";
		else if(o=='JS_hint_sta_ap')
			translated_string="您正准备由Adapter模式切换到AP模式。<br><br>按下确定后，请移除您电脑与" + PID_STR + "之间的以太网络线。将" + PID_STR + "（藉由以太网络线）接上您的局域网。若要（藉由无线连接）进入设定页面，请先将您电脑的IP地址更改为" + IP_RNAGE_STR_GLOBAL + "（网络掩码" + LAN_NETMASK_GLOBAL + "）。<br><br>您确定要切换成AP模式？";
		else if(o=='JS_hint_re_ap')
			translated_string="您正准备由Repeater模式切换到AP模式。<br><br>按下确定后，请将" + PID_STR + "（藉由以太网络线）重新接上您的局域网。若要（藉由无线连接）进入设定页面，请先将您电脑的IP地址更改为" + IP_RNAGE_STR_GLOBAL + "（网络掩码" + LAN_NETMASK_GLOBAL + "）。<br><br>您确定要切换成AP模式？";
		else if(o=='JS_hint_gwsta_ap')
			translated_string="您确定要切换成AP模式？";
		else if(o=='JS_hint_gwre_ap')
			translated_string="您正准备由Gateway(Hotspot)模式切换到AP模式。<br><br>按下确定后，请将" + PID_STR + "（藉由以太网络线）重新接上您的局域网。若要（藉由无线连接）进入设定页面，请先将您电脑的IP地址更改为" + IP_RNAGE_STR_GLOBAL + "（网络掩码" + LAN_NETMASK_GLOBAL + "）。<br><br>您确定要切换成AP模式？";

		else if(o=='JS_hint_gw_sta')
			translated_string="您正准备由Gateway模式切换到Adapter模式，<br><br>按下确定后，请以以太网络线连接您的电脑与" + PID_STR + "，并手动将电脑IP地址改为" + IP_RNAGE_STR_GLOBAL + "（网络掩码" + LAN_NETMASK_GLOBAL + "），始可（藉由以太网络线）重新进入设定页面。<br><br>您确定要切换成Adapter模式？";
		else if(o=='JS_hint_ap_sta')
			translated_string="您正准备由AP模式切换到Adapter模式。<br><br>按下确定后，请以以太网络线连接您的电脑与" + PID_STR + "，并手动将电脑IP地址改为" + IP_RNAGE_STR_GLOBAL + "（网络掩码" + LAN_NETMASK_GLOBAL + "），始可（藉由以太网络线）重新进入设定页面。<br>br>您确定要切换成Adapter模式？";
		else if(o=='JS_hint_re_sta')
			translated_string="您正准备由Repeater模式切换到Adapter模式。<br><br>按下确定后，请以以太网络线连接您的电脑与" + PID_STR + "，并手动将电脑IP地址改为" + IP_RNAGE_STR_GLOBAL + "（网络掩码" + LAN_NETMASK_GLOBAL + "），始可（藉由以太网络线）重新进入设定页面。<br><br>您确定要切换成Adapter模式？";
		else if(o=='JS_hint_gwsta_sta')
			translated_string="您确定要切换成Adapter模式？";
		else if(o=='JS_hint_gwre_sta')
			translated_string="您正准备由Gateway(Hotspot)模式切换到Adapter模式。<br><br>按下确定后，请以以太网络线连接您的电脑与" + PID_STR + "，并手动将电脑IP地址改为" + IP_RNAGE_STR_GLOBAL + "（网络掩码" + LAN_NETMASK_GLOBAL + "），始可（藉由以太网络线）重新进入设定页面。<br><br>您确定要切换成Adapter模式？";

		else if(o=='JS_hint_gw_re')
			translated_string="您正准备由Gateway模式切换到Repeater模式。<br><br>按下确定后，请将您的IP地址更改为「自动获得IP地址」以藉由无线连接或以太网络线连结因特网，若要进入设定页面，请先将电脑IP地址设为" + IP_RNAGE_STR_GLOBAL + "（网络掩码" + LAN_NETMASK_GLOBAL + "）。<br><br>您确定要切换成Repeater模式？";
		else if(o=='JS_hint_ap_re')
			translated_string="您正准备由AP模式切换到Repeater模式。<br><br>按下确定后，请将您的IP地址更改为「自动获得IP地址」以藉由无线连接或以太网络线连结因特网，若要进入设定页面，请先将电脑IP地址设为" + IP_RNAGE_STR_GLOBAL + "（网络掩码" + LAN_NETMASK_GLOBAL + "）。<br><br>您确定要切换成Repeater模式？";
		else if(o=='JS_hint_sta_re')
			translated_string="您正准备由Adapter模式切换到Repeater模式。<br><br>按下确定后，请将您的IP地址更改为「自动获得IP地址」以藉由无线连接或以太网络线连结因特网，若要进入设定页面，请先将电脑IP地址设为" + IP_RNAGE_STR_GLOBAL + "（网络掩码" + LAN_NETMASK_GLOBAL + "）。<br><br>您确定要切换成Repeater模式？";
		else if(o=='JS_hint_gwsta_re')
			translated_string="您确定要切换成Repeater模式？";
		else if(o=='JS_hint_gwre_re')
			translated_string="您正准备由Gateway(Hotspot)模式切换到Repeater模式。<br><br>按下确定后，请将您的IP地址更改为「自动获得IP地址」以藉由无线连接或以太网络线连结因特网，若要进入设定页面，请先将电脑IP地址设为" + IP_RNAGE_STR_GLOBAL + "（网络掩码" + LAN_NETMASK_GLOBAL + "）。<br><br>您确定要切换成Repeater模式？";
			
		else if(o=='JS_hint_others')
			translated_string="您确定要储存设定？";

		else if(o=='JS_hint_autoip')
			translated_string="若要连接因特网或无线局域网，请先将您电脑的IP设定更改为「<strong>自动获得IP地址</strong>」。";

		else if(o=='JS_unsupported')
			translated_string="不支持此类型的AP！";
		else if(o=='JS_validstr')
		        translated_string="本字符串无法包含";
		else if(o=='authenticated')
			translated_string="已连接";
		else if(o=='connecting')
			translated_string="连接中";
		else if(o=='default')
			translated_string="惯用的无线网络";
		else if(o=='5')
			translated_string="非常好";
		else if(o=='4')
			translated_string="良好";
		else if(o=='3')
			translated_string="好";
		else if(o=='2')
			translated_string="尚可";
		else if(o=='1')
			translated_string="差";
		else if(o=='LAN_WAN_match')
			translated_string='目前的局域网（LAN）端IP子网路与广域网（WAN）端IP子网路相冲突！请至"高级设置"更改您的局域网端IP子网路设置！';
		else if(o=='JS_fieldblank')
		        translated_string="此栏目不可以空白！";
		else if(o=='JS_LAN_IP_hint')
		        translated_string="<br><br><br><br><i>注：" + PID_STR + "的局域网（LAN）端 IP地址将会是 <b>" + document.form.LANIP.value + "</b> </i>。";
        }
        else if(v=='DE')
        {
                if(o=='JS_wpa')
                	translated_string="Sie haben eine zusätzliche Wireless-Authentifikation und einen Verschlüsselungsmodus ausgewählt. Wollen Sie dies wirklich überschreiben?";
                else if(o=='JS_GW_AP')
                	translated_string='Sie können sich übr Wireless-Zugriff nur im "Heim-Gateway-AP"-Modus mit dem LAN verbinden!';
		else if(o=='JS_passzero')
		        translated_string="Gleiche gemeinsam benutzte WPA-Schlüssel sollten mehr als 7 Zeichen besitzen!!! Wenn Sie dieses Feld leer lassen wird das System 12345678 als Ihr Passwort festlegen.";
		else if(o=='JS_wepkey')
		        translated_string="Bitte geben Sie den vollständigen WEP-Schlüssel ein.\nSie können 10 oder 26 hexadezimale Zeichen (0-9, a-f, A-F) eingeben, 5 oder 13 ASCII Zeichen einzugeben.";
		else if(o=='JS_wepkey1')
		        translated_string="Bitte geben Sie den vollständigen WEP-Schlüssel ein.\nSie können 10 hexadezimale Zeichen (0-9, a-f, A-F) eingeben, oder 5 ASCII Zeichen einzugeben.";
		else if(o=='JS_wepkey2')
		        translated_string="Bitte geben Sie den vollständigen WEP-Schlüssel ein.\nSie können 26 hexadezimale Zeichen (0-9, a-f, A-F) eingeben, oder 13 ASCII Zeichen einzugeben.";
		else if(o=='JS_wpapass')
		        translated_string="Gleiche gemeinsam benutzte WPA-Schlüssel sollten weniger als 64 Zeichen haben!!!";
		else if(o=='JS_LANG_EN')
			translated_string="Englisch";
		else if(o=='JS_LANG_TW')
			translated_string="Chinesisch traditionell";
		else if(o=='JS_LANG_CN')
			translated_string="Chinesisch einfach";
		else if(o=='JS_LANG_KR')
			translated_string="Koreanisch";
		else if(o=='JS_LANG_DE')
			translated_string="Deutsch";

		else if(o=='JS_hint_ap_gw')
			translated_string="Sie wollen vom AP-Modus zum Gateway-Modus umschalten.<br><br>Nachdem Sie auf 'OK' geklickt haben setzen Sie bitte Ihre IP-Konfiguration auf 'IP-Adresse automatisch beziehen', um sich mit dem Internet zu verbinden oder die Schnelleinstellung zu starten (über Wireless-Zugriff).<br><br>Wollen Sie in den Gateway-Modus umschalten?";
		else if(o=='JS_hint_sta_gw')
			translated_string="Sie wollen vom Adaptermodus zum Gateway-Modus umschalten.<br><br>Nachdem Sie auf 'OK' geklickt haben verbinden Sie das eine Ende des Ethernet-Kabels mit dem Ethernet-Port des " + PID_STR + " und das andere Ende mit dem Ethernet-Port Ihres lokalen Netzwerks. Setzen Sie dann Ihre IP-Konfiguration auf 'IP-Adresse automatisch beziehen', um sich mit dem Internet zu verbinden oder die Schnelleinstellung aufzurufen (über Wireless-Zugriff) wollen.<br><br>Wollen Sie zum Gateway-Modus umschalten?";
		else if(o=='JS_hint_re_gw')
			translated_string="Sie wollen vom Repeater-Modus zum Gateway-Modus umschalten.<br><br>Nachdem Sie auf 'OK' geklickt haben verbinden Sie bitte ein Ende des Netzwerkkabels mit dem Ethernet-Port des " + PID_STR + " und das andere Ende mit dem Ethernet-Port Ihres lokalen Netzwerks. Setzen Sie dann Ihre IP-Konfiguration auf 'IP-Adresse automatisch beziehen', um sich mit dem Internet zu verbinden oder die Schnelleinstellung aufzurufen (über Wireless-Zugriff) wollen.<br><br>Wollen Sie in den Gateway-Modus umschalten?";
		else if(o=='JS_hint_gwsta_gw')
			translated_string="Wollen Sie in den Gateway-Modus umschalten?";
		else if(o=='JS_hint_gwre_gw')
			translated_string="Sie wollen vom Gateway (Hotspot) -Modus zum Gateway-Modus umschalten.<br><br>Nachdem Sie auf 'OK' geklickt haben verbinden Sie bitte ein Ende des Netzwerkkabels mit dem Ethernet-Port des " + PID_STR + " und das andere Ende mit dem Ethernet-Port Ihres lokalen Netzwerks. Setzen Sie dann Ihre IP-Konfiguration auf 'IP-Adresse automatisch beziehen', um sich mit dem Internet zu verbinden oder die Schnelleinstellung aufzurufen (über Wireless-Zugriff).<br><br>Wollen Sie in den Gateway-Modus umschalten?";

		else if(o=='JS_hint_ap_gwre')
			translated_string="Sie wollen vom AP-Modus zum Gateway (Hotspot) -Modus umschalten.<br><br>Nachdem Sie auf 'OK' geklickt haben setzen Sie bitte Ihre IP-Konfiguration auf 'IP-Adresse automatisch beziehen', um sich mit dem Internet zu verbinden oder die Schnelleinstellung (über das Netzwerkkabel oder Wireless-Zugriff).<br><br>Wollen Sie in den Gateway (Hotspot) -Modus umschalten?";
		else if(o=='JS_hint_sta_gwre')
			translated_string="Sie wollen vom Adaptermodus zum Gateway (Hotspot) -Modus umschalten.<br><br>Nachdem Sie auf 'OK' geklickt haben setzen Sie Ihre IP-Konfiguration auf 'IP-Adresse automatisch beziehen', um sich mit dem Internet zu verbinden oder die Schnelleinstellung aufzurufen (über Netzwerkkabel oder Wireless-Zugriff) wollen.<br><br>Wollen Sie zum Gateway (Hotspot) -Modus umschalten?";
		else if(o=='JS_hint_re_gwre')
			translated_string="Sie wollen vom Repeater-Modus zum Gateway (Hotspot) -Modus umschalten.<br><br>Nachdem Sie auf 'OK' geklickt haben setzen Sie Ihre IP-Konfiguration auf 'IP-Adresse automatisch beziehen', um sich mit dem Internet zu verbinden oder die Schnelleinstellung aufzurufen (über Netzwerkkabel oder Wireless-Zugriff) wollen.<br><br>Wollen Sie in den Gateway (Hotspot) -Modus umschalten?";
		else if(o=='JS_hint_gw_gwre')
			translated_string="Sie wollen vom Gateway-Modus in den Gateway (Hotspot) -Modus umschalten.<br><br>Nachdem Sie auf 'OK' geklickt haben können Sie sich mit dem Internet verbinden oder die Schnelleinstellung aufrufen (über Netzwerkkabel oder Wireless-Zugriff).<br><br>Wollen Sie in den Gateway (Hotspot) -Modus umschalten?";
		else if(o=='JS_hint_gwsta_gwre')
			translated_string="Wollen Sie in den Gateway (Hotspot) -Modus umschalten?";

		else if(o=='JS_hint_ap_gwsta')
			translated_string="Sie können jetzt vom AP Modus in den Gateway-Adapter Modus wechseln. <br><br>Nachdem Sie 'OK'gedrückt haben, setzten Sie bitte ihre IP Konfiguration auf 'automatisches beziehen einer IP Adresse' um eine Verbindung zum Internet herzustellen oder nutzen Sie Quick Setup (über ein Ethernet Kabel).<br><br>Möchten Sie in den Gateway-Adapter Modus wechseln?";
		else if(o=='JS_hint_sta_gwsta')
			translated_string="Sie können jetzt vom Adapter Modus in den Gateway-Adapter Modus wechseln. <br><br>Nachdem Sie 'OK'gedrückt haben, setzten Sie bitte ihre IP Konfiguration auf 'automatisches beziehen einer IP Adresse' um eine Verbindung zum Internet herzustellen oder nutzen Sie Quick Setup (über ein Ethernet Kabel).<br><br>Möchten Sie in den Gateway-Adapter Modus wechseln?";
		else if(o=='JS_hint_re_gwsta')
			translated_string="Sie können jetzt vom Repeater Modus in den Gateway-Adapter Modus wechseln. <br><br>Nachdem Sie 'OK'gedrückt haben, setzten Sie bitte ihre IP Konfiguration auf 'automatisches beziehen einer IP Adresse' um eine Verbindung zum Internet herzustellen oder nutzen Sie Quick Setup (über ein Ethernet Kabel).<br><br>Möchten Sie in den Gateway-Adapter Modus wechseln?";
		else if(o=='JS_hint_gw_gwsta')
			translated_string="Sie können jetzt vom Gateway Modus in den Gateway-Adapter Modus wechseln. <br><br>Nachdem Sie 'OK' gedrückt haben, können Sie eine Verbindung mit dem Internet herstellen oder nutzen Sie Quick Setup (über ein Ethernet Kabel).<br><br>Möchten Sie in den Gateway-Adapter Modus wechseln?";
		else if(o=='JS_hint_gwre_gwsta')
			translated_string="Möchten Sie in den Gateway-Adapter Modus wechseln?";

		else if(o=='JS_hint_gw_ap')
			translated_string="Sie wollen vom Gateway-Modus in den AP-Modus umschalten.<br><br>Nachdem Sie auf 'OK' geklickt haben und Sie die Schnelleinstellung aufrufen (über Wireless-Zugriff) wollen, setzen Sie Ihre PC-IP-Adresse auf "  + IP_RNAGE_STR_GLOBAL + " (die Subnetzmaske ist " + LAN_NETMASK_GLOBAL + ").<br><br>Wolle Sie in den AP-Modus umschalten?";
		else if(o=='JS_hint_sta_ap')
			translated_string="Sie wollen vom Adaptermodus zum AP-Modus umschalten.<br><br>Nachdem Sie auf 'OK' geklickt haben verbinden Sie das eine Ende des Ethernet-Kabels mit dem Ethernet-Port des " + PID_STR +" und das andere Ende mit dem Ethernet-Port Ihres lokalen Netzwerks. Wenn Sie die Schnelleinstellung aufrufen (über Wireless-Zugriff) wollen setzen Sie Ihre PC-IP-Adresse auf " + IP_RNAGE_STR_GLOBAL + " (die Subnetzmaske ist " + LAN_NETMASK_GLOBAL + ").<br><br>Wollen Sie in den AP-Modus umschalten?";
		else if(o=='JS_hint_re_ap')
			translated_string="Sie wollen vom Repeater-Modus zum AP-Modus umschalten.<br><br>Nachdem Sie auf 'OK' geklickt haben verbinden Sie bitte ein Ende des Netzwerkkabels mit dem Ethernet-Port des " + PID_STR +" und das andere Ende mit dem Ethernet-Port Ihres lokalen Netzwerks. Wenn Sie die Schnelleinstellung aufrufen (über Wireless-Zugriff) wollen setzen Sie bitte Ihre PC-IP-Adresse auf " + IP_RNAGE_STR_GLOBAL + " (die Subnetzmaske ist " + LAN_NETMASK_GLOBAL + ").<br><br>Wollen Sie in den AP-Modus umschalten?";
		else if(o=='JS_hint_gwsta_ap')
			translated_string="Wollen Sie in den AP-Modus umschalten?";
		else if(o=='JS_hint_gwre_ap')
			translated_string="Sie wollen vom Gateway (Hotspot) -Modus zum AP-Modus umschalten.<br><br>Nachdem Sie auf 'OK' geklickt haben verbinden Sie bitte ein Ende des Netzwerkkabels mit dem Ethernet-Port des " + PID_STR + " und das andere Ende mit dem Ethernet-Port Ihres lokalen Netzwerks. Wenn Sie die Schnelleinstellung erneut ausfrufen (über Wireless-Zugriff) wollen setzen Sie bitte Ihre PC-IP-Adresse auf " + IP_RNAGE_STR_GLOBAL + " (die Subnetzmaske ist " + LAN_NETMASK_GLOBAL + ").<br><br>Wollen Sie in den AP-Modus umschalten?";

		else if(o=='JS_hint_gw_sta')
			translated_string="Sie wollen vom Gateway-Modus zum Adapter-Modus umschalten.<br><br>Nachdem Sie auf 'OK' geklickt haben verbinden Sie bitte ein Ende des Netzwerkkabels mit dem Ethernet-Port Ihres PCs und das andere Ende mit dem Ethernet-Port des " + PID_STR + ". Wenn Sie die Schnelleinstellung erneut ausfrufen (über Netzwerkkabel) wollen setzen Sie Ihre PC-IP-Adresse auf " + IP_RNAGE_STR_GLOBAL + " (die Subnetzmaske ist " + LAN_NETMASK_GLOBAL + ").<br><br>Wollen Sie in den Adaptermodus umschalten?";
		else if(o=='JS_hint_ap_sta')
			translated_string="Sie wollen vom AP-Modus zum Adapter-Modus umschalten.<br><br>Nachdem Sie auf 'OK' geklickt haben verbinden Sie bitte ein Ende des Netzwerkkabels mit dem Ethernet-Port Ihres PCs und das andere Ende mit dem Ethernet-Port des " + PID_STR + " . Wenn Sie die Schnelleinstellung erneut ausfrufen (über Netzwerkkabel) wollen setzen Sie Ihre PC-IP-Adresse auf " + IP_RNAGE_STR_GLOBAL + " (die Subnetzmaske ist " + LAN_NETMASK_GLOBAL + ").<br><br>Wollen Sie in den Adaptermodus umschalten?";
		else if(o=='JS_hint_re_sta')
			translated_string="Sie wollen vom Repeater-Modus zum Adaptermodus umschalten.<br><br>Nachdem Sie auf 'OK' geklickt haben verbinden Sie das eine Ende des Ethernet-Kabels mit dem Ethernet-Port Ihres PCs und das andere Ende mit dem Ethernet-Port des " + PID_STR + ". Wenn Sie die Schnelleinstellung aufrufen (über Netzwerkkabel) wollen setzen Sie Ihre PC-IP-Adresse auf " + IP_RNAGE_STR_GLOBAL + " (die Subnetzmaske ist " + LAN_NETMASK_GLOBAL + ").<br><br>Wollen Sie zum Adaptermodus umschalten?";
		else if(o=='JS_hint_gwsta_sta')
			translated_string="Wollen Sie in den Adaptermodus umschalten?";
		else if(o=='JS_hint_gwre_sta')
			translated_string="Sie wollen vom Gateway (Hotspot) -Modus zum Adaptermodus umschalten.<br><br>Nachdem Sie auf 'OK' geklickt haben verbinden Sie bitte ein Ende des Netzwerkkabels mit dem Ethernet-Port Ihres PCs und das andere Ende mit den Ethernet-Port des " + PID_STR + ". Wenn Sie die Einstellung aufrufen (über Netzwerkkabel) wollen setzen Sie bitte Ihre PC_IP-Adresse auf " + IP_RNAGE_STR_GLOBAL + " (die Subnetzmaske ist " + LAN_NETMASK_GLOBAL + ").<br><br>Wollen Sie in den Adaptermodus umschalten?";

		else if(o=='JS_hint_gw_re')
			translated_string="Sie wollen vom Gateway-Modus zum Repeater-Modus umschalten.<br><br>Nachdem Sie auf 'OK' geklickt haben setzen Sie Ihre IP-Konfiguration bitte auf 'IP-Adresse automatisch beziehen', um sich mit dem Internet (über Netzwerkkabel oder Wireless-Zugriff) zu verbinden. Wenn Sie die Schnelleinstellung erneut aufrufen wollen setzen Sie Ihre PC-IP-Adresse auf " + IP_RNAGE_STR_GLOBAL + " (die Subnetzmaske ist " + LAN_NETMASK_GLOBAL + ").<br><br>Wollen Sie in den Repeater-Modus umschalten?";
		else if(o=='JS_hint_ap_re')
			translated_string="Sie wollen vom AP-Modus zum Repeater-Modus umschalten.<br><br>Nachdem Sie auf 'OK' geklickt haben setzen Sie bitte Ihre IP-Konfiguration auf 'IP-Adresse automatisch beziehen', um sich mit dem Internet zu verbinden (über das Netzwerkkabel oder Wireless-Zugriff). Wenn Sie die Schnelleinstellung erneut aufrufen wollen, setzen Sie Ihre PC-Ip-Adresse auf " + IP_RNAGE_STR_GLOBAL + " (die Subnetzmaske ist " + LAN_NETMASK_GLOBAL + ").<br><br>Wollen Sie in den Repeater-Modus umschalten?";
		else if(o=='JS_hint_sta_re')
			translated_string="Sie wollen vom Adapter-Modus zum Repeater-Modus umschalten.<br><br>Nachdem Sie auf 'OK' geklickt haben setzen Sie Ihre IP-Konfiguration auf 'IP-Adresse automatisch beziehen', um sich mit dem Internet zu verbinden (über Netzwerkkabel oder Wireless-Zugriff). Wenn Sie die Schnelleinstellung erneut aufrufen wollen setzen Sie Ihre PC-IP-Adresse auf " + IP_RNAGE_STR_GLOBAL + " (die Subnetzmaske ist " + LAN_NETMASK_GLOBAL + ").<br><br>Wollen Sie zum Repeater-Modus umschalten?";
		else if(o=='JS_hint_gwsta_re')
			translated_string="Wollen Sie in den Repeater-Modus umschalten?";
		else if(o=='JS_hint_gwre_re')
			translated_string="Sie wollen vom Gateway (Hotspot) -Modus zum Repeater-Modus umschalten.<br><br>Nachdem Sie auf 'OK' geklickt haben setzen Sie Ihre IP-Konfiguration bitte auf 'IP-Adresse automatisch beziehen', um sich mit dem Internet (über Netzwerkkabel oder Wireless-Zugriff) zu verbinden. Wenn Sie die Schnelleinstellung erneut aufrufen wollen setzen Sie Ihre PC-IP-Adresse auf " + IP_RNAGE_STR_GLOBAL + " (die Subnetzmaske ist " + LAN_NETMASK_GLOBAL + ").<br><br>Wollen Sie in den Repeater-Modus umschalten?";
			
		else if(o=='JS_hint_others')
			translated_string="Wollen Sie diese Einstellungen wirklich speichern?";

		else if(o=='JS_hint_autoip')
			translated_string="Wenn Sie eine Verbindung zum Internet oder zum WLAN aufbauen wollen setzen Sie Ihre IP-Konfiguration auf '<strong>IP-Adresse automatisch beziehen</strong>'.";

		else if(o=='JS_unsupported')
			translated_string="Nicht unterstützter AP-Typ!";
		else if(o=='JS_validstr3')
			translated_string="Diese Zeichenkette darf kein";
		else if(o=='JS_validstr4')
			translated_string="enthalten";
		else if(o=='authenticated')
			translated_string="Verbunden";
		else if(o=='connecting')
			translated_string="Verbinden";
		else if(o=='default')
			translated_string="Bevorzugtes Wireless-Netzwerk";
		else if(o=='5')
			translated_string="Sehr stark";
		else if(o=='4')
			translated_string="Stark";
		else if(o=='3')
			translated_string="Gut";
		else if(o=='2')
			translated_string="Akzeptabel";
		else if(o=='1')
			translated_string="Schwach";
		else if(o=='LAN_WAN_match')
			translated_string='Die derzeitige LAN-IP- Subnetz und die WAN-IP- Subnetz stehen in Konflikt zueinander! Bitte gehen Sie zu "Erweiterte Einstellungen", um Ihre LAN-IP- Subnetz zu ändern!';
		else if(o=='JS_fieldblank')
		        translated_string="Die Felder dürfen nicht leer bleiben!!!";
		else if(o=='JS_LAN_IP_hint')
			translated_string="<br><br><br><br><i>PS: Die LAN IP-Adresse des " + PID_STR + " wird <b>" + document.form.LANIP.value + "</b></i> sein.";
        }
        else if(v=='JP')
        {
                if(o=='JS_wpa')
                	translated_string="追加ワイヤレス認証と暗号化オプションを選択しました。上書きしてもよろしいですか?";
                else if(o=='JS_GW_AP')
                	translated_string=' "Home Gateway-AP" [ホームゲートウェイ – AP] モードでは、ワイヤレスアクセス経由でしか LAN に接続できません!';
		else if(o=='JS_passzero')
		        translated_string="WPA 事前共有キーは 7 文字未満にします!!! このフィールを空白のままにすると、システムは 12345678 をあなたのパスフレーズとして割り当てます。";
		else if(o=='JS_wepkey')
		        translated_string="完全な WEP キーを入力してください。";
		else if(o=='JS_wepkey1')
		        translated_string="完全な WEP キーを入力してください。\n10 の 16 進法の数字（0～9、a～f、 A～F）または 5 つの ASCII 文字を入力できます。";
		else if(o=='JS_wepkey2')
		        translated_string="完全な WEP キーを入力してください。\n26 の 16 進法の数字（0～9、a～f、 A～F）または 13 つの ASCII 文字を入力できます。";
		else if(o=='JS_wpapass')
		        translated_string="WPA 事前共有キーは 64 文字未満にします!!!";
		else if(o=='JS_LANG_EN')
			translated_string="英語";
		else if(o=='JS_LANG_TW')
			translated_string="繁体字中国語";
		else if(o=='JS_LANG_CN')
			translated_string="簡体字中国語";
		else if(o=='JS_LANG_KR')
			translated_string="韓国語";
		else if(o=='JS_LANG_DE')
			translated_string="ドイツ語";

		else if(o=='JS_hint_ap_gw')
			translated_string="AP モードを Gateway [ゲートウェイ] モードに切り替える準備をしています。<br><br>「OK」をクリックし、IP 設定を'Obtain an IP address automatically' [IP アドレスを自動的に取得する] と設定してインターネットに接続するか、または、Quick Setup [クイックセットアップ]を（ワイヤレスアクセス経由で）開きます。<br><br>Gateway [ゲートウェイ] モードへの切り替えを確定しますか?";
		else if(o=='JS_hint_sta_gw')
			translated_string="Adapter [アダプター] モードをGateway [ゲートウェイ]モードに切り替える準備をしています。<br><br>[OK] をクリックして、付属のイーサネットケーブルの一方の端を「+ PID_STR +」のイーサネットポートに接続し、もう一方の端をローカルネットワークのイーサネットポートに接続してください。 次に、IP 設定を Obtain an IP address automatically [IP アドレスを自動的に取得する] と設定してインターネットに接続するか、または、Quick Setup [クイックセットアップ] を（ワイヤレスアクセス経由で）開きます。<br><br>Gateway [ゲートウェイ] モードへの切り替えを確定しますか?";
		else if(o=='JS_hint_re_gw')
			translated_string="Repeater [リピーター] モードを Gateway [ゲートウェイ] モードに切り替える準備をしています。<br><br>[OK] をクリックして、付属のイーサネットケーブルの一方の端を「+ PID_STR +」のイーサネットポートに接続し、もう一方の端をローカルネットワークのイーサネットポートに接続してください。 次に、IP 設定を'Obtain an IP address automatically' [IP アドレスを自動的に取得する] と設定してインターネットに接続するか、または、Quick Setup [クイックセットアップ] を（ワイヤレスアクセス経由で）開きます。<br><br>Gateway [ゲートウェイ] モードへの切り替えを確定しますか?";
		else if(o=='JS_hint_gwsta_gw')
			translated_string="Gateway [ゲートウェイ] モードへの切り替えを確定しますか?";
		else if(o=='JS_hint_gwre_gw')
			translated_string="Gateway [ゲートウェイ]（ホットスポット）モードをGateway [ゲートウェイ] モードに切り替える準備をしています。<br><br>[OK] をクリックして、付属のイーサネットケーブルの一方の端を「+ PID_STR +」のイーサネットポートに接続し、もう一方の端をローカルネットワークのイーサネットポートに接続してください。 次に、IP 設定を 'Obtain an IP address automatically' [IP アドレスを自動的に取得する] と設定してインターネットに接続するか、または、Quick Setup [クイックセットアップ] を（ワイヤレスアクセス経由で）開きます。<br><br> Gateway [ゲートウェイ] モードへの切り替えを確定しますか?";

		else if(o=='JS_hint_ap_gwre')
			translated_string="AP モードをGateway [ゲートウェイ]（ホットスポット）モードに切り替える準備をしています。<br><br><br><br>[OK] をクリックし、IP 設定を 'Obtain an IP address automatically' [IP アドレスを自動的に取得する] と設定してインターネットに接続するか、または、Quick Setup [クイックセットアップ] を（イーサネットケーブルまたはワイヤレスアクセス経由で）開きます。<br><br>Gateway [ゲートウェイ]（ホットスポット）モードへの切り替えを確定しますか?";
		else if(o=='JS_hint_sta_gwre')
			translated_string="Adapter [アダプター] モードをGateway [ゲートウェイ]（ホットスポット）モードに切り替える準備をしています。<br><br>[OK] をクリックし、IP 設定を'Obtain an IP address automatically' [IP アドレスを自動的に取得する] と設定してインターネットに接続するか、または、Quick Setup [クイックセットアップ]を（イーサネットケーブルまたはワイヤレスアクセス経由で）開きます。<br><br>Gateway [ゲートウェイ]（ホットスポット）モードへの切り替えを確定しますか?";
		else if(o=='JS_hint_re_gwre')
			translated_string="Repeater [リピーター] モードをGateway [ゲートウェイ]（ホットスポット）モードに切り替える準備をしています。<br><br><br><br>[OK] をクリックし、IP 設定を'Obtain an IP address automatically' [IP アドレスを自動的に取得する] と設定してインターネットに接続するか、または、Quick Setup [クイックセットアップ]を（イーサネットケーブルまたはワイヤレスアクセス経由で）開きます。<br><br>Gateway [ゲートウェイ]（ホットスポット）モードへの切り替えを確定しますか?";
		else if(o=='JS_hint_gw_gwre')
			translated_string="Gateway [ゲートウェイ] モードをGateway [ゲートウェイ]（ホットスポット）モードに切り替える準備をしています。<br><br>[OK] をクリックして、インターネットに接続するか、または、Quick Setup [クイックセットアップ]を（イーサネットケーブルまたはワイヤレスアクセス経由で）開きます。<br><br>Gateway [ゲートウェイ]（ホットスポット）モードへの切り替えを確定しますか?";
		else if(o=='JS_hint_gwsta_gwre')
			translated_string="Gateway [ゲートウェイ]（ホットスポット）モードへの切り替えを確定しますか?";

		else if(o=='JS_hint_ap_gwsta')
			translated_string="AP モードをGateway-Adapter [ゲートウェイ-アダプター] モードに切り替える準備をしています。<br><br>[OK] をクリックし、IP 設定を'Obtain an IP address automatically' [IP アドレスを自動的に取得する] と設定してインターネットに接続するか、または、Quick Setup [クイックセットアップ]を（イーサネットケーブル経由で）開きます。<br><br>Gateway-Adapter [ゲートウェイ-アダプター] モードへの切り替えを確定しますか?";
		else if(o=='JS_hint_sta_gwsta')
			translated_string="Adapter [アダプター] モードをGateway-Adapter [ゲートウェイ-アダプター] モードに切り替える準備をしています。<br><br>[OK] をクリックし、IP 設定を'Obtain an IP address automatically' [IP アドレスを自動的に取得する] と設定してインターネットに接続するか、または、Quick Setup [クイックセットアップ]を（イーサネットケーブル経由で）開きます。<br><br>Gateway-Adapter [ゲートウェイ-アダプター] モードへの切り替えを確定しますか?";
		else if(o=='JS_hint_re_gwsta')
			translated_string="Repeater [リピーター] モードをGateway-Adapter [ゲートウェイ-アダプター] モードに切り替える準備をしています。<br><br>[OK] をクリックし、IP 設定を'Obtain an IP address automatically' [IP アドレスを自動的に取得する] と設定してインターネットに接続するか、または、Quick Setup [クイックセットアップ]を（イーサネットケーブル経由で）開きます。<br><br>Gateway-Adapter [ゲートウェイ-アダプター] モードへの切り替えを確定しますか?";
		else if(o=='JS_hint_gw_gwsta')
			translated_string="Gateway [ゲートウェイ] モードをGateway-Adapter [ゲートウェイ-アダプター] モードに切り替える準備をしています。<br><br>[OK] をクリックして、インターネットに接続するか、または、Quick Setup [クイックセットアップ]を（イーサネットケーブル経由で）開きます。<br><br>Gateway-Adapter [ゲートウェイ-アダプター] モードへの切り替えを確定しますか?";
		else if(o=='JS_hint_gwre_gwsta')
			translated_string="Gateway-Adapter [ゲートウェイ-アダプター] モードへの切り替えを確定しますか?";

		else if(o=='JS_hint_gw_ap')
			translated_string="Gateway [ゲートウェイ] モードを AP モードに切り替える準備をしています。<br><br>[OK] をクリックして、Quick Setup [クイックセットアップ]を（ワイヤレスアクセス経由で）開きたい場合は、PC の IP アドレスを「 + IP_RNAGE_STR_GLOBAL + 」（サブネットマスクは「 + LAN_NETMASK_GLOBAL + 」です）)に設定してください。<br><br>AP モードへの切り替えを確定しますか?";
		else if(o=='JS_hint_sta_ap')
			translated_string="Adapter [アダプター] モードを AP モードに切り替える準備をしています。<br><br>[OK] をクリックして、付属のイーサネットケーブルの一方の端を「+ PID_STR +」のイーサネットポートに接続し、もう一方の端をローカルネットワークのイーサネットポートに接続してください。 Quick Setup [クイックセットアップ]を（ワイヤレスアクセス経由で）開きたい場合は、PC の IP アドレスを「 + IP_RNAGE_STR_GLOBAL + 」（サブネットマスクは「 + LAN_NETMASK_GLOBAL + 」です）に設定してください。<br><br>AP モードへの切り替えを確定しますか?";
		else if(o=='JS_hint_re_ap')
			translated_string="Repeater [リピーター] モードを AP モードに切り替える準備をしています。<br><br>[OK] をクリックして、付属のイーサネットケーブルの一方の端を「+ PID_STR +」のイーサネットポートに接続し、もう一方の端をローカルネットワークのイーサネットポートに接続してください。 Quick Setup [クイックセットアップ]を（ワイヤレスアクセス経由で）開きたい場合は、PC の IP アドレスを「 + IP_RNAGE_STR_GLOBAL + 」（サブネットマスクは「 + LAN_NETMASK_GLOBAL + 」です）に設定してください。<br><br>AP モードへの切り替えを確定しますか?";
		else if(o=='JS_hint_gwsta_ap')
			translated_string="AP モードへの切り替えを確定しますか?";
		else if(o=='JS_hint_gwre_ap')
			translated_string="Gateway [ゲートウェイ]（ホットスポット）モードを AP モードに切り替える準備をしています。<br><br>[OK] をクリックして、付属のイーサネットケーブルの一方の端を「+ PID_STR +」のイーサネットポートに接続し、もう一方の端をローカルネットワークのイーサネットポートに接続してください。 Quick Setup [クイックセットアップ]を（ワイヤレスアクセス経由で）開きたい場合は、PC の IP アドレスを「 + IP_RNAGE_STR_GLOBAL + 」（サブネットマスクは「 + LAN_NETMASK_GLOBAL + 」です）に設定してください。<br><br>AP モードへの切り替えを確定しますか?";

		else if(o=='JS_hint_gw_sta')
			translated_string="Gateway [ゲートウェイ] モードをAdapter [アダプター] モードに切り替える準備をしています。<br><br>[OK] をクリックして、付属のイーサネットケーブルの一方の端をお使いの PC のイーサネットケーブルに接続し、もう一方の端を「+ PID_STR +」のイーサネットポートに接続してください。 Quick Setup [クイックセットアップ]を（イーサネットケーブル経由で）開きたい場合は、PC の IP アドレスを「 + IP_RNAGE_STR_GLOBAL + 」（サブネットマスクは「 + LAN_NETMASK_GLOBAL + 」です）に設定してください。<br><br>Adapter [アダプター] モードへの切り替えを確定しますか?";
		else if(o=='JS_hint_ap_sta')
			translated_string="AP モードをAdapter [アダプター] モードに切り替える準備をしています。<br><br>[OK] をクリックして、付属のイーサネットケーブルの一方の端をお使いの PC のイーサネットケーブルに接続し、もう一方の端を「+ PID_STR +」のイーサネットポートに接続してください。 Quick Setup [クイックセットアップ]を（イーサネットケーブル経由で）開きたい場合は、PC の IP アドレスを「 + IP_RNAGE_STR_GLOBAL + 」（サブネットマスクは「 + LAN_NETMASK_GLOBAL + 」です）に設定してください。<br><br>Adapter [アダプター] モードへの切り替えを確定しますか?";
		else if(o=='JS_hint_re_sta')
			translated_string="Repeater [リピーター] モードを Adapter [アダプター] モードに切り替える準備をしています。<br><br>[OK] をクリックして、付属のイーサネットケーブルの一方の端をお使いの PC のイーサネットケーブルに接続し、もう一方の端を「+ PID_STR +」のイーサネットポートに接続してください。 Quick Setup [クイックセットアップ]を（イーサネットケーブル経由で）開きたい場合は、PC の IP アドレスを「 + IP_RNAGE_STR_GLOBAL + 」（サブネットマスクは「 + LAN_NETMASK_GLOBAL + 」です）に設定してください。<br><br>Adapter [アダプター] モードへの切り替えを確定しますか?";
		else if(o=='JS_hint_gwsta_sta')
			translated_string="Adapter [アダプター] モードへの切り替えを確定しますか?";
		else if(o=='JS_hint_gwre_sta')
			translated_string="Gateway [ゲートウェイ]（ホットスポット）モードを Adapter [アダプター] モードに切り替える準備をしています。<br><br>[OK] をクリックして、付属のイーサネットケーブルの一方の端をお使いの PC のイーサネットケーブルに接続し、もう一方の端を「+ PID_STR +」のイーサネットポートに接続してください。 「セットアップ」を（イーサネットケーブル経由で）開きたい場合は、PC の IP アドレスを「 + IP_RNAGE_STR_GLOBAL + 」（サブネットマスクは「 + LAN_NETMASK_GLOBAL + 」です）に設定してください。<br><br>Adapter [アダプター] モードへの切り替えを確定しますか?";

		else if(o=='JS_hint_gw_re')
			translated_string="Gateway [ゲートウェイ] モードをRepeater [リピーター] モードに切り替える準備をしています。<br><br>[OK] をクリックし、IP 設定を'Obtain an IP address automatically' [IP アドレスを自動的に取得する] と設定して、インターネットに（イーサネットケーブルまたはワイヤレスアクセス経由で）接続してください。 Quick Setup [クイックセットアップ]をもう一度開きたい場合は、PC の IP アドレスを「 + IP_RNAGE_STR_GLOBAL + 」（サブネットマスクは「 + LAN_NETMASK_GLOBAL + 」です）。<br><br>Repeater [リピーター] モードへの切り替えを確定しますか?";
		else if(o=='JS_hint_ap_re')
			translated_string="AP モードをRepeater [リピーター] モードに切り替える準備をしています。<br><br>[OK] をクリックし、IP 設定を'Obtain an IP address automatically' [IP アドレスを自動的に取得する] と設定して、インターネットに（イーサネットケーブルまたはワイヤレスアクセス経由で）接続してください。 Quick Setup [クイックセットアップ]をもう一度開きたい場合は、PC の IP アドレスを「 + IP_RNAGE_STR_GLOBAL + 」（サブネットマスクは「 + LAN_NETMASK_GLOBAL + 」です）。<br><br>Repeater [リピーター] モードへの切り替えを確定しますか?";
		else if(o=='JS_hint_sta_re')
			translated_string="Adapter [アダプター] モードをRepeater [リピーター] モードに切り替える準備をしています。<br><br>[OK] をクリックし、IP 設定を'Obtain an IP address automatically' [IP アドレスを自動的に取得する] と設定して、インターネットに（イーサネットケーブルまたはワイヤレスアクセス経由で）接続してください。 Quick Setup [クイックセットアップ]をもう一度開きたい場合は、PC の IP アドレスを「 + IP_RNAGE_STR_GLOBAL + 」（サブネットマスクは「 + LAN_NETMASK_GLOBAL + 」です）。<br><br>Repeater [リピーター] モードへの切り替えを確定しますか?";
		else if(o=='JS_hint_gwsta_re')
			translated_string="Repeater [リピーター] モードへの切り替えを確定しますか?";
		else if(o=='JS_hint_gwre_re')
			translated_string="Gateway [ゲートウェイ]（ホットスポット）モードをRepeater [リピーター] モードに切り替える準備をしています。<br><br>「OK」をクリックし、IP 設定を'Obtain an IP address automatically' [IP アドレスを自動的に取得する] に設定して、インターネットに（イーサネットケーブルまたはワイヤレスアクセス経由で）接続してください。 Quick Setup [クイックセットアップ]をもう一度開きたい場合は、PC の IP アドレスを「 + IP_RNAGE_STR_GLOBAL + 」（サブネットマスクは「 + LAN_NETMASK_GLOBAL + 」です）。<br><br>Repeater [リピーター] モードへの切り替えを確定しますか?";
			
		else if(o=='JS_hint_others')
			translated_string="設定を保存してもよろしいですか?";

		else if(o=='JS_hint_autoip')
			translated_string="インターネットまたは WLAN に接続したい場合は、IP 設定を「<strong>Obtain an IP address automatically [IP アドレスを自動的に取得する] </strong>」に設定してください。";

		else if(o=='JS_unsupported')
			translated_string="AP タイプがサポートされていません!";
		else if(o=='JS_validstr')
		        translated_string="この文字列は次を含むことはできません";
		else if(o=='authenticated')
			translated_string="接続済み";
		else if(o=='connecting')
			translated_string="接続中";
		else if(o=='default')
			translated_string="優先ワイヤレスネットワーク";
		else if(o=='5')
			translated_string="大変強い";
		else if(o=='4')
			translated_string="強い";
		else if(o=='3')
			translated_string="良好";
		else if(o=='2')
			translated_string="受信可能";
		else if(o=='1')
			translated_string="弱い";
		else if(o=='LAN_WAN_match')
			translated_string='現在の LAN IPサブネットと WAN IPサブネット が競合しています。「アドバンスド設定」で LAN IPサブネット設定を変更してください。';
		else if(o=='JS_fieldblank')
		        translated_string="フィールドは空にできません!!!";
		else if(o=='JS_LAN_IP_hint')
			translated_string="<br><br><br><br><i>PS: 「 + PID_STR + 」の LAN IP アドレスは<b>「 + document.form.LANIP.value + 」</b></i>になります。";
        }
        else if(v=='TR')
        {
                if(o=='JS_wpa')
                	translated_string="Ek kablosuz kimlik doğrulaması ve şifreleme seçeneğini seçtiniz, gerçekten üzerine yazdırmak istiyor musunuz?";
                else if(o=='JS_GW_AP')
                	translated_string="LAN'a sadece" + ' "Ana Ağ Geçidi-AP" modunda Kablosuz erişim ile bağlanabilirsiniz!';
		else if(o=='JS_passzero')
		        translated_string="WPA Ön paylaşımlı anahtar 7 karakterden uzun olmalıdır!!! Eğer bu alanı boş bırakırsanız, sistem şifreniz olarak 12345678 atayacaktır.";
		else if(o=='JS_wepkey')
		        translated_string="Lütfen tam WEP anahtarını girin.\n10 veya 26 ONALTILI hane (0~9, a~f, A~F), 5 veya 13 ASCII karakteri girebilirsiniz.";
		else if(o=='JS_wepkey1')
		        translated_string="Lütfen tam WEP anahtarınızı girin.\n10 ONALTILI hane (0~9, a~f, A~F) veya 5 ASCII karakteri girebilirsiniz.";
		else if(o=='JS_wepkey2')
		        translated_string="Lütfen tam WEP anahtarını girin.\n26 ONALTILI hane (0~9, a~f, A~F) veya 13 ASCII karakteri girebilirsiniz.";
		else if(o=='JS_wpapass')
		        translated_string="WPA Ön paylaşımlı anahtar 64 karakterden kısa olmalıdır!!!";
		else if(o=='JS_LANG_EN')
			translated_string="İngilizce";
		else if(o=='JS_LANG_TW')
			translated_string="Geleneksel Çince";
		else if(o=='JS_LANG_CN')
			translated_string="Basitleştirilmiş Çince";
		else if(o=='JS_LANG_KR')
			translated_string="Korece";
		else if(o=='JS_LANG_DE')
			translated_string="Almanca";

		else if(o=='JS_hint_ap_gw')
			translated_string="AP modunu Ağ Geçidi moduna geçirmeye hazırlanıyorsunuz.<br><br>'Tamam'a tıkladıktan sonra, lütfen IP yapılandırmanızı 'Otomatik olarak bir IP adresi al' olarak ayarlayarak internete bağlanın veya Hızlı Kuruluma (kablosuz erişim ile) giriş yapın.<br><br>Ağ geçidi moduna geçmeye kararlı mısınız?";
		else if(o=='JS_hint_sta_gw')
			translated_string="Adaptör modunu Ağ geçidi moduna geçirmeye hazırlanıyorsunuz.<br><br>'Tamam'a tıkladıktan sonra, lütfen verilen Ethernet kablosunun bir ucunu "+ PID_STR +" ethernet bağlantı noktasına takın ve diğer ucunu yerel ağın Ethernet bağlantı noktasına takın. Ardından IP yapılandırmanızı 'Otomatik olarak bir IP adresi al' olarak ayarlayarak internete bağlanın veya Hızlı Kuruluma (kablosuz erişim ile) giriş yapın.<br><br>Ağ geçidi moduna geçmeye kararlı mısınız?";
		else if(o=='JS_hint_re_gw')
			translated_string="Yineleyici modunu Ağ Geçidi moduna değiştirmeye hazırlanıyorsunuz.<br><br>'Tamam'a tıkladıktan sonra, lütfen verilen Ethernet kablosunun bir ucunu "+ PID_STR +" ethernet bağlantı noktasına takın ve diğer ucunu yerel ağın Ethernet bağlantı noktasına takın. Ardından IP yapılandırmanızı 'Otomatik olarak bir IP adresi al' olarak ayarlayarak internete bağlanın veya Hızlı Kuruluma (kablosuz erişim ile) giriş yapın.<br><br>Ağ geçidi moduna geçmeye kararlı mısınız?";
		else if(o=='JS_hint_gwsta_gw')
			translated_string="Ağ geçidi moduna geçmeye kararlı mısınız?";
		else if(o=='JS_hint_gwre_gw')
			translated_string="Ağ Geçidi (Ortak Nokta) modunu Ağ Geçidi moduna değiştirmeye hazırlanıyorsunuz.<br><br>'Tamam'a tıkladıktan sonra, lütfen verilen Ethernet kablosunun bir ucunu "+ PID_STR +" ethernet bağlantı noktasına takın ve diğer ucunu yerel ağın Ethernet bağlantı noktasına takın. Ardından IP yapılandırmanızı 'Otomatik olarak bir IP adresi al' olarak ayarlayarak internete bağlanın veya Hızlı Kuruluma (kablosuz erişim ile) giriş yapın.<br><br>Ağ geçidi moduna geçmeye kararlı mısınız?";

		else if(o=='JS_hint_ap_gwre')
			translated_string="AP modunu Ağ Geçidi (Ortak Nokta) moduna değiştirmeye hazırlanıyorsunuz.<br><br>'Tamam'a tıkladıktan sonra, lütfen IP adresinizin yapılandırmasını 'Otomatik olarak bir IP adresi al' olarak ayarlayarak internete bağlanın veya Hızlı Kuruluma (Ethernet kablosu veya kablosuz erişim ile) giriş yapın.<br><br>Ağ geçidi (Ortak Nokta) moduna geçmeye kararlı mısınız?";
		else if(o=='JS_hint_sta_gwre')
			translated_string="Adaptör modunu Ağ Geçidi (Ortak Nokta) moduna değiştirmeye hazırlanıyorsunuz.<br><br>'Tamam'a tıkladıktan sonra, lütfen IP adresinizin yapılandırmasını 'Otomatik olarak bir IP adresi al' olarak ayarlayarak internete bağlanın veya Hızlı Kuruluma (Ethernet kablosu veya kablosuz erişim ile) giriş yapın.<br><br>Ağ geçidi (Ortak Nokta) moduna geçmeye kararlı mısınız?";
		else if(o=='JS_hint_re_gwre')
			translated_string="Yineleyici modunu Ağ Geçidi (Ortak Nokta) moduna değiştirmeye hazırlanıyorsunuz.<br><br>'Tamam'a tıkladıktan sonra, lütfen IP adresinizin yapılandırmasını 'Otomatik olarak bir IP adresi al' olarak ayarlayarak internete bağlanın veya Hızlı Kuruluma (Ethernet kablosu veya kablosuz erişim ile) giriş yapın.<br><br>Ağ geçidi (Ortak Nokta) moduna geçmeye kararlı mısınız?";
		else if(o=='JS_hint_gw_gwre')
			translated_string="Ağ Geçidi modunu Ağ Geçidi (Ortak Nokta) moduna değiştirmeye hazırlanıyorsunuz.<br><br>'Tamam'a tıkladıktan sonra, internete bağlanabilir veya Hızlı Kuruluma (Ethernet kablosu veya kablosuz erişim ile) giriş yapabilirsiniz.<br><br>Ağ geçidi (Ortak Nokta) moduna geçmeye kararlı mısınız?";
		else if(o=='JS_hint_gwsta_gwre')
			translated_string="Ağ geçidi (Ortak Nokta) moduna geçmeye kararlı mısınız?";

		else if(o=='JS_hint_ap_gwsta')
			translated_string="AP modunu Ağ Geçidi-Adaptör moduna değiştirmeye hazırlanıyorsunuz.<br><br>'Tamam'a tıkladıktan sonra, lütfen IP adresinizin yapılandırmasını 'Otomatik olarak bir IP adresi al' olarak ayarlayarak internete bağlanın veya Hızlı Kuruluma (Ethernet kablosu ile) giriş yapın.<br><br>Ağ geçidi-Adaptör moduna geçmeye kararlı mısınız?";
		else if(o=='JS_hint_sta_gwsta')
			translated_string="Adaptör modunu Ağ Geçidi-Adaptör moduna değiştirmeye hazırlanıyorsunuz .<br><br>'Tamam'a tıkladıktan sonra, lütfen IP adresinizin yapılandırmasını 'Otomatik olarak bir IP adresi al' olarak ayarlayarak internete bağlanın veya Hızlı Kuruluma (Ethernet kablosu ile) giriş yapın.<br><br>Ağ geçidi-Adaptör moduna geçmeye kararlı mısınız?";
		else if(o=='JS_hint_re_gwsta')
			translated_string="Yineleyici modunu Ağ Geçidi-Adaptör moduna değiştirmeye hazırlanıyorsunuz .<br><br>'Tamam'a tıkladıktan sonra, lütfen IP adresinizin yapılandırmasını 'Otomatik olarak bir IP adresi al' olarak ayarlayarak internete bağlanın veya Hızlı Kuruluma (Ethernet kablosu ile) giriş yapın.<br><br>Ağ geçidi-Adaptör moduna geçmeye kararlı mısınız?";
		else if(o=='JS_hint_gw_gwsta')
			translated_string="Ağ Geçidi modunu Ağ Geçidi-Adaptör moduna değiştirmeye hazırlanıyorsunuz.<br><br>'Tamam'a tıkladıktan sonra, lütfen IP adresinizin yapılandırmasını 'Otomatik olarak bir IP adresi al' olarak ayarlayarak internete bağlanın veya Hızlı Kuruluma (Ethernet kablosu ile) giriş yapın.<br><br>Ağ geçidi-Adaptör moduna geçmeye kararlı mısınız?";
		else if(o=='JS_hint_gwre_gwsta')
			translated_string="Ağ geçidi-Adaptör moduna geçmeye kararlı mısınız?";

		else if(o=='JS_hint_gw_ap')
			translated_string="Ağ Geçidi modunu AP moduna değiştirmeye hazırlanıyorsunuz.<br><br>'Tamam'a tıkladıktan sonra, eğer Hızlı Kurulum'a girmek isterseniz (kablosuz erişim ile), lütfen PC'nizin IP adresini " + IP_RNAGE_STR_GLOBAL + " olarak ayarlayın (Alt Ağ Maskesi " + LAN_NETMASK_GLOBAL + ").<br><br>AP moduna geçmeye kararlı mısınız?";
		else if(o=='JS_hint_sta_ap')
			translated_string="Adaptör modunu AP moduna değiştirmeye hazırlanıyorsunuz.<br><br>'Tamam'a tıkladıktan sonra, lütfen verilen Ethernet kablosunun bir ucunu "+ PID_STR +" ethernet bağlantı noktasına takın ve diğer ucunu yerel ağın Ethernet bağlantı noktasına takın. Eğer Hızlı Kuruluma girmek isterseniz (kablosuz erişim ile), lütfen PC'nizin IP adresini " + IP_RNAGE_STR_GLOBAL + " olarak ayarlayın (Alt Ağ Maskesi " + LAN_NETMASK_GLOBAL + ").<br><br>AP moduna geçmeye kararlı mısınız?";
		else if(o=='JS_hint_re_ap')
			translated_string="Yineleyici modunu AP moduna değiştirmeye hazırlanıyorsunuz.<br><br>'Tamam'a tıkladıktan sonra, lütfen verilen Ethernet kablosunun bir ucunu "+ PID_STR +" ethernet bağlantı noktasına takın ve diğer ucunu yerel ağın Ethernet bağlantı noktasına takın. Eğer Hızlı Kuruluma girmek isterseniz (kablosuz erişim ile), lütfen PC'nizin IP adresini " + IP_RNAGE_STR_GLOBAL + " olarak ayarlayın (Alt Ağ Maskesi " + LAN_NETMASK_GLOBAL + ").<br><br>AP moduna geçmeye kararlı mısınız?";
		else if(o=='JS_hint_gwsta_ap')
			translated_string="AP moduna geçmeye kararlı mısınız?";
		else if(o=='JS_hint_gwre_ap')
			translated_string="Ağ Geçidi (Ortak Nokta) modunu AP moduna değiştirmeye hazırlanıyorsunuz.<br><br>'Tamam'a tıkladıktan sonra, lütfen verilen Ethernet kablosunun bir ucunu "+ PID_STR +" ethernet bağlantı noktasına takın ve diğer ucunu yerel ağın Ethernet bağlantı noktasına takın. Eğer Hızlı Kuruluma girmek isterseniz (kablosuz erişim ile), lütfen PC'nizin IP adresini " + IP_RNAGE_STR_GLOBAL + " olarak ayarlayın (Alt Ağ Maskesi " + LAN_NETMASK_GLOBAL + ").<br><br>AP moduna geçmeye kararlı mısınız?";

		else if(o=='JS_hint_gw_sta')
			translated_string="Ağ Geçidi modunu Adaptör moduna değiştirmeye hazırlanıyorsunuz.<br><br>'Tamam'a tıkladıktan sonra, lütfen verilen Ethernet kablosunun bir ucunu "+ PID_STR +" ethernet bağlantı noktasına takın ve diğer ucunu yerel ağın Ethernet bağlantı noktasına takın. Eğer Hızlı Kuruluma girmek isterseniz (kablosuz erişim ile), lütfen PC'nizin IP adresini " + IP_RNAGE_STR_GLOBAL + " olarak ayarlayın (Alt Ağ Maskesi " + LAN_NETMASK_GLOBAL + ").<br><br>AP moduna geçmeye kararlı mısınız?";
		else if(o=='JS_hint_ap_sta')
			translated_string="AP modunu Adaptör moduna değiştirmeye hazırlanıyorsunuz.<br><br>'Tamam'a tıkladıktan sonra, lütfen verilen Ethernet kablosunun bir ucunu "+ PID_STR +" ethernet bağlantı noktasına takın ve diğer ucunu yerel ağın Ethernet bağlantı noktasına takın. Eğer Hızlı Kuruluma girmek isterseniz (kablosuz erişim ile), lütfen PC'nizin IP adresini " + IP_RNAGE_STR_GLOBAL + " olarak ayarlayın (Alt Ağ Maskesi " + LAN_NETMASK_GLOBAL + ").<br><br>Adaptör moduna geçmeye kararlı mısınız?";
		else if(o=='JS_hint_re_sta')
			translated_string="Yineleyici modunu Adaptör moduna değiştirmeye hazırlanıyorsunuz.<br><br>'Tamam'a tıkladıktan sonra, lütfen verilen Ethernet kablosunun bir ucunu "+ PID_STR +" ethernet bağlantı noktasına takın ve diğer ucunu yerel ağın Ethernet bağlantı noktasına takın. Eğer Hızlı Kuruluma girmek isterseniz (kablosuz erişim ile), lütfen PC'nizin IP adresini " + IP_RNAGE_STR_GLOBAL + " olarak ayarlayın (Alt Ağ Maskesi " + LAN_NETMASK_GLOBAL + ").<br><br>Adaptör moduna geçmeye kararlı mısınız?";
		else if(o=='JS_hint_gwsta_sta')
			translated_string="Adaptör moduna geçmeye kararlı mısınız?";
		else if(o=='JS_hint_gwre_sta')
			translated_string="Ağ Geçidi (Ortak Nokta) modunu Adaptör moduna değiştirmeye hazırlanıyorsunuz.<br><br>'Tamam'a tıkladıktan sonra, lütfen verilen Ethernet kablosunun bir ucunu "+ PID_STR +" ethernet bağlantı noktasına takın ve diğer ucunu yerel ağın Ethernet bağlantı noktasına takın. Eğer Hızlı Kuruluma girmek isterseniz (kablosuz erişim ile), lütfen PC'nizin IP adresini " + IP_RNAGE_STR_GLOBAL + " olarak ayarlayın (Alt Ağ Maskesi " + LAN_NETMASK_GLOBAL + ").<br><br>Adaptör moduna geçmeye kararlı mısınız?";

		else if(o=='JS_hint_gw_re')
			translated_string="Ağ Geçidi modunu Yineleyici moduna değiştirmeye hazırlanıyorsunuz.<br><br>'Tamam'a tıkladıktan sonra, lütfen IP adresinizin yapılandırmasını 'Otomatik olarak bir IP adresi al' olarak ayarlayarak internete bağlanın (Ethernet kablosu veya kablosuz erişim ile). Eğer Hızlı Kurulum'a tekrar girmek isterseniz, bu durumda lütfen PC'nizin IP adresini " + IP_RNAGE_STR_GLOBAL + " olarak ayarlayın (Alt Ağ Maskesi " + LAN_NETMASK_GLOBAL + ").<br><br>Yineleyici moduna geçmeye kararlı mısınız?";
		else if(o=='JS_hint_ap_re')
			translated_string="AP modunu Yineleyici moduna değiştirmeye hazırlanıyorsunuz.<br><br>'Tamam'a tıkladıktan sonra, lütfen IP adresinizin yapılandırmasını 'Otomatik olarak bir IP adresi al' olarak ayarlayarak internete bağlanın (Ethernet kablosu veya kablosuz erişim ile). Eğer Hızlı Kurulum'a tekrar girmek isterseniz, bu durumda lütfen PC'nizin IP adresini " + IP_RNAGE_STR_GLOBAL + " olarak ayarlayın (Alt Ağ Maskesi " + LAN_NETMASK_GLOBAL + ").<br><br>Yineleyici moduna geçmeye kararlı mısınız?";
		else if(o=='JS_hint_sta_re')
			translated_string="Adaptör modunu Yineleyici moduna değiştirmeye hazırlanıyorsunuz.<br><br>'Tamam'a tıkladıktan sonra, lütfen IP adresinizin yapılandırmasını 'Otomatik olarak bir IP adresi al' olarak ayarlayarak internete bağlanın (Ethernet kablosu veya kablosuz erişim ile). Eğer Hızlı Kurulum'a tekrar girmek isterseniz, bu durumda lütfen PC'nizin IP adresini " + IP_RNAGE_STR_GLOBAL + " olarak ayarlayın (Alt Ağ Maskesi " + LAN_NETMASK_GLOBAL + ").<br><br>Yineleyici moduna geçmeye kararlı mısınız?";
		else if(o=='JS_hint_gwsta_re')
			translated_string="Yineleyici moduna geçmeye kararlı mısınız?";
		else if(o=='JS_hint_gwre_re')
			translated_string="Ağ Geçidi (Ortak Nokta) modunu Yineleyici moduna değiştirmeye hazırlanıyorsunuz.<br><br>'Tamam'a tıkladıktan sonra, lütfen IP adresinizin yapılandırmasını 'Otomatik olarak bir IP adresi al' olarak ayarlayarak internete bağlanın (Ethernet kablosu veya kablosuz erişim ile). Eğer Hızlı Kurulum'a tekrar girmek isterseniz, bu durumda lütfen PC'nizin IP adresini " + IP_RNAGE_STR_GLOBAL + " olarak ayarlayın (Alt Ağ Maskesi " + LAN_NETMASK_GLOBAL + ").<br><br>Yineleyici moduna geçmeye kararlı mısınız?";
			
		else if(o=='JS_hint_others')
			translated_string="Ayarları kaydetmek istediğinizden emin misiniz?";

		else if(o=='JS_hint_autoip')
			translated_string="İnternete veya WLAN'a bağlanmak isterseniz, lütfen IP yapılandırmanızı '<strong>Otomatik olarak bir IP adresi al</strong>' olarak ayarlayın.";

		else if(o=='JS_unsupported')
			translated_string="Desteklenmeyen AP türü!";
		else if(o=='JS_validstr')
		        translated_string="Bu dizin şunu içeremez";
		else if(o=='authenticated')
			translated_string="Bağlı";
		else if(o=='connecting')
			translated_string="Bağlanıyor";
		else if(o=='default')
			translated_string="Tercih Edilen Kablosuz Ağ";
		else if(o=='5')
			translated_string="Çok Güçlü";
		else if(o=='4')
			translated_string="Güçlü";
		else if(o=='3')
			translated_string="İyi";
		else if(o=='2')
			translated_string="Kabul edilebilir";
		else if(o=='1')
			translated_string="Zayıf";
		else if(o=='LAN_WAN_match')
			translated_string='Mevcut LAN IP alt ağı ve WAN IP alt ağı karışıklığı! Lütfen "Gelişmiş Ayar"a giderek LAN IP alt ağ ayarını değiştirin!';
		else if(o=='JS_fieldblank')
		        translated_string="Bu alan boş olamaz!!!";
		else if(o=='JS_LAN_IP_hint')
			translated_string="<br><br><br><br><i>PS: " + PID_STR + " LAN IP adresi <b>" + document.form.LANIP.value + " </b></i> olacaktır.";
        }
        else
        {
                if(o=='JS_wpa')
                	translated_string="You have chosen an additional wireless authenication and encryption option, do you really like to overwrite it?";
                else if(o=='JS_GW_AP')
                	translated_string='You can only connect to LAN through Wireless access in "Home Gateway-AP" mode!';
		else if(o=='JS_passzero')
		        translated_string="WPA Pre-shared key should be more than 7 characters!!! If you leave this field blank, system will assign 12345678 as your passphrase.";
		else if(o=='JS_wepkey')
		        translated_string="Please enter the complete WEP key.\nYou can enter 10 or 26 HEX digits (0~9, a~f, A~F), 5 or 13 ASCII characters.";
		else if(o=='JS_wepkey1')
		        translated_string="Please enter the complete WEP key.\nYou can enter 10 HEX digits (0~9, a~f, A~F), or 5 ASCII characters.";
		else if(o=='JS_wepkey2')
		        translated_string="Please enter the complete WEP key.\nYou can enter 26 HEX digits (0~9, a~f, A~F), or 13 ASCII characters.";
		else if(o=='JS_wpapass')
		        translated_string="WPA Pre-shared key should be less than 64 characters!!!";
		else if(o=='JS_LANG_EN')
			translated_string="English";
		else if(o=='JS_LANG_TW')
			translated_string="Traditional Chinese";
		else if(o=='JS_LANG_CN')
			translated_string="Simplified Chinese";
		else if(o=='JS_LANG_KR')
			translated_string="Korean";
		else if(o=='JS_LANG_DE')
			translated_string="German";

		else if(o=='JS_hint_ap_gw')
			translated_string="You are preparing to switch AP mode to Gateway mode.<br><br>After clicking 'OK', please set your IP configuration as 'Obtain an IP address automatically' to connect to Internet or enter Quick Setup (through wireless access).<br><br>Do you determine to switch to Gateway mode?";
		else if(o=='JS_hint_sta_gw')
			translated_string="You are preparing to switch Adapter mode to Gateway mode.<br><br>After clicking 'OK', please connect one end of the supplied Ethernet cable to the Ethernet port of the "+ PID_STR +" and the other end to the Ethernet port of local network. Then set your IP configuration as 'Obtain an IP address automatically' to connect to Internet or enter Quick Setup (through wireless access).<br><br>Do you determine to switch to Gateway mode?";
		else if(o=='JS_hint_re_gw')
			translated_string="You are preparing to switch Repeater mode to Gateway mode.<br><br>After clicking 'OK', please connect one end of the supplied Ethernet cable to the Ethernet port of the "+ PID_STR +" and the other end to the Ethernet port of local network. Then set your IP configuration as 'Obtain an IP address automatically' to connect to Internet or enter Quick Setup (through wireless access).<br><br>Do you determine to switch to Gateway mode?";
		else if(o=='JS_hint_gwsta_gw')
			translated_string="Do you determine to switch to Gateway mode?";
		else if(o=='JS_hint_gwre_gw')
			translated_string="You are preparing to switch Gateway(Hotspot) mode to Gateway mode.<br><br>After clicking 'OK', please connect one end of the supplied Ethernet cable to the Ethernet port of the "+ PID_STR +" and the other end to the Ethernet port of local network. Then set your IP configuration as 'Obtain an IP address automatically' to connect to Internet or enter Quick Setup (through wireless access).<br><br>Do you determine to switch to Gateway mode?";

		else if(o=='JS_hint_ap_gwre')
			translated_string="You are preparing to switch AP mode to Gateway(Hotspot) mode.<br><br>After clicking 'OK', please set your IP configuration as 'Obtain an IP address automatically' to connect to Internet or enter Quick Setup (through Ethernet cable or wireless access).<br><br>Do you determine to switch to Gateway(Hotspot) mode?";
		else if(o=='JS_hint_sta_gwre')
			translated_string="You are preparing to switch Adapter mode to Gateway(Hotspot) mode.<br><br>After clicking 'OK', please set your IP configuration as 'Obtain an IP address automatically' to connect to Internet or enter Quick Setup (through Ethernet cable or wireless access).<br><br>Do you determine to switch to Gateway(Hotspot) mode?";
		else if(o=='JS_hint_re_gwre')
			translated_string="You are preparing to switch Repeater mode to Gateway(Hotspot) mode.<br><br>After clicking 'OK', please set your IP configuration as 'Obtain an IP address automatically' to connect to Internet or enter Quick Setup (through Ethernet cable or wireless access).<br><br>Do you determine to switch to Gateway(Hotspot) mode?";
		else if(o=='JS_hint_gw_gwre')
			translated_string="You are preparing to switch Gateway mode to Gateway(Hotspot) mode.<br><br>After clicking 'OK', you can connect to Internet or enter Quick Setup (through Ethernet cable or wireless access).<br><br>Do you determine to switch to Gateway(Hotspot) mode?";
		else if(o=='JS_hint_gwsta_gwre')
			translated_string="Do you determine to switch to Gateway(Hotspot) mode?";

		else if(o=='JS_hint_ap_gwsta')
			translated_string="You are preparing to switch AP mode to Gateway-Adapter mode.<br><br>After clicking 'OK', please set your IP configuration as 'Obtain an IP address automatically' to connect to Internet or enter Quick Setup (through Ethernet cable).<br><br>Do you determine to switch to Gateway-Adapter mode?";
		else if(o=='JS_hint_sta_gwsta')
			translated_string="You are preparing to switch Adapter mode to Gateway-Adapter mode.<br><br>After clicking 'OK', please set your IP configuration as 'Obtain an IP address automatically' to connect to Internet or enter Quick Setup (through Ethernet cable).<br><br>Do you determine to switch to Gateway-Adapter mode?";
		else if(o=='JS_hint_re_gwsta')
			translated_string="You are preparing to switch Repeater mode to Gateway-Adapter mode.<br><br>After clicking 'OK', please set your IP configuration as 'Obtain an IP address automatically' to connect to Internet or enter Quick Setup (through Ethernet cable).<br><br>Do you determine to switch to Gateway-Adapter mode?";
		else if(o=='JS_hint_gw_gwsta')
			translated_string="You are preparing to switch Gateway mode to Gateway-Adapter mode.<br><br>After clicking 'OK', you can connect to Internet or enter Quick Setup (through Ethernet cable).<br><br>Do you determine to switch to Gateway-Adapter mode?";
		else if(o=='JS_hint_gwre_gwsta')
			translated_string="Do you determine to switch to Gateway-Adapter mode?";

		else if(o=='JS_hint_gw_ap')
			translated_string="You are preparing to switch Gateway mode to AP mode.<br><br>After clicking 'OK', if you want to enter Quick Setup (through wireless access), please set your PC's IP address to " + IP_RNAGE_STR_GLOBAL + " (The Subnet Mask is " + LAN_NETMASK_GLOBAL + ").<br><br>Do you determine to switch to AP mode?";
		else if(o=='JS_hint_sta_ap')
			translated_string="You are preparing to switch Adapter mode to AP mode.<br><br>After clicking 'OK', connect one end of the supplied Ethernet cable to the Ethernet port of the "+ PID_STR +" and the other end to the Ethernet port of local network. If you want to enter Quick Setup (through wireless access), please set your PC's IP address to " + IP_RNAGE_STR_GLOBAL + " (The Subnet Mask is " + LAN_NETMASK_GLOBAL + ").<br><br>Do you determine to switch to AP mode?";
		else if(o=='JS_hint_re_ap')
			translated_string="You are preparing to switch Repeater mode to AP mode.<br><br>After clicking 'OK', connect one end of the supplied Ethernet cable to the Ethernet port of the "+ PID_STR +" and the other end to the Ethernet port of local network. If you want to enter Quick Setup (through wireless access), please set your PC's IP address to " + IP_RNAGE_STR_GLOBAL + " (The Subnet Mask is " + LAN_NETMASK_GLOBAL + ").<br><br>Do you determine to switch to AP mode?";
		else if(o=='JS_hint_gwsta_ap')
			translated_string="Do you determine to switch to AP mode?";
		else if(o=='JS_hint_gwre_ap')
			translated_string="You are preparing to switch Gateway(Hotspot) mode to AP mode.<br><br>After clicking 'OK', connect one end of the supplied Ethernet cable to the Ethernet port of the "+ PID_STR +" and the other end to the Ethernet port of local network. If you want to enter Quick Setup (through wireless access), please set your PC's IP address to " + IP_RNAGE_STR_GLOBAL + " (The Subnet Mask is " + LAN_NETMASK_GLOBAL + ").<br><br>Do you determine to switch to AP mode?";

		else if(o=='JS_hint_gw_sta')
			translated_string="You are preparing to switch Gateway mode to Adapter mode.<br><br>After clicking 'OK', please connect one end of the supplied Ethernet cable to the Ethernet port of the your PC and the other end to the Ethernet port of " + PID_STR + " . If you want to enter Quick Setup (through Ethernet cable), please set your PC's IP address to " + IP_RNAGE_STR_GLOBAL + " (The Subnet Mask is " + LAN_NETMASK_GLOBAL + ").<br><br>Do you determine to switch to Adapter mode?";
		else if(o=='JS_hint_ap_sta')
			translated_string="You are preparing to switch AP mode to Adapter mode.<br><br>After clicking 'OK', please connect one end of the supplied Ethernet cable to the Ethernet port of the your PC and the other end to the Ethernet port of " + PID_STR + " . If you want to enter Quick Setup (through Ethernet cable), please set your PC's IP address to " + IP_RNAGE_STR_GLOBAL + " (The Subnet Mask is " + LAN_NETMASK_GLOBAL + ").<br><br>Do you determine to switch to Adapter mode?";
		else if(o=='JS_hint_re_sta')
			translated_string="You are preparing to switch Repeater mode to Adapter mode.<br><br>After clicking 'OK', please connect one end of the supplied Ethernet cable to the Ethernet port of the your PC and the other end to the Ethernet port of " + PID_STR + " . If you want to enter Quick Setup (through Ethernet cable), please set your PC's IP address to " + IP_RNAGE_STR_GLOBAL + " (The Subnet Mask is " + LAN_NETMASK_GLOBAL + ").<br><br>Do you determine to switch to Adapter mode?";
		else if(o=='JS_hint_gwsta_sta')
			translated_string="Do you determine to switch to Adapter mode?";
		else if(o=='JS_hint_gwre_sta')
			translated_string="You are preparing to switch Gateway(Hotspot) mode to Adapter mode.<br><br>After clicking 'OK', please connect one end of the supplied Ethernet cable to the Ethernet port of the your PC and the other end to the Ethernet port of " + PID_STR + " . If you want to enter Setup (through Ethernet cable), please set your PC's IP address to " + IP_RNAGE_STR_GLOBAL + " (The Subnet Mask is " + LAN_NETMASK_GLOBAL + ").<br><br>Do you determine to switch to Adapter mode?";

		else if(o=='JS_hint_gw_re')
			translated_string="You are preparing to switch Gateway mode to Repeater mode.<br><br>After clicking 'OK', please set your IP configuration as 'Obtain an IP address automatically' to connect to Internet (through Ethernet cable or wireless access). If you want to enter Quick Setup again, then please set your PC's IP address to " + IP_RNAGE_STR_GLOBAL + " (The Subnet Mask is " + LAN_NETMASK_GLOBAL + ").<br><br>Do you determine to switch to Repeater mode?";
		else if(o=='JS_hint_ap_re')
			translated_string="You are preparing to switch AP mode to Repeater mode.<br><br>After clicking 'OK', please set your IP configuration as 'Obtain an IP address automatically' to connect to Internet (through Ethernet cable or wireless access). If you want to enter Quick Setup again, then please set your PC's IP address to " + IP_RNAGE_STR_GLOBAL + " (The Subnet Mask is " + LAN_NETMASK_GLOBAL + ").<br><br>Do you determine to switch to Repeater mode?";
		else if(o=='JS_hint_sta_re')
			translated_string="You are preparing to switch Adapter mode to Repeater mode.<br><br>After clicking 'OK', please set your IP configuration as 'Obtain an IP address automatically' to connect to Internet (through Ethernet cable or wireless access). If you want to enter Quick Setup again, then please set your PC's IP address to " + IP_RNAGE_STR_GLOBAL + " (The Subnet Mask is " + LAN_NETMASK_GLOBAL + ").<br><br>Do you determine to switch to Repeater mode?";
		else if(o=='JS_hint_gwsta_re')
			translated_string="Do you determine to switch to Repeater mode?";
		else if(o=='JS_hint_gwre_re')
			translated_string="You are preparing to switch Gateway(Hotspot) mode to Repeater mode.<br><br>After clicking 'OK', please set your IP configuration as 'Obtain an IP address automatically' to connect to Internet (through Ethernet cable or wireless access). If you want to enter Quick Setup again, then please set your PC's IP address to " + IP_RNAGE_STR_GLOBAL + " (The Subnet Mask is " + LAN_NETMASK_GLOBAL + ").<br><br>Do you determine to switch to Repeater mode?";
			
		else if(o=='JS_hint_others')
			translated_string="Are you sure to save settings?";

		else if(o=='JS_hint_autoip')
			translated_string="If you want to connect to Internet or WLAN, please set your IP configuration as '<strong>Obtain an IP address automatically</strong>'.";

		else if(o=='JS_unsupported')
			translated_string="Unsupported AP teyp!";
		else if(o=='JS_validstr')
		        translated_string="This string can not contain";
		else if(o=='authenticated')
			translated_string="Connected";
		else if(o=='connecting')
			translated_string="Connecting";
		else if(o=='default')
			translated_string="Preferred Wireless Network";
		else if(o=='5')
			translated_string="Very Strong";
		else if(o=='4')
			translated_string="Strong";
		else if(o=='3')
			translated_string="Good";
		else if(o=='2')
			translated_string="Acceptable";
		else if(o=='1')
			translated_string="Poor";
		else if(o=='LAN_WAN_match')
			translated_string='Current LAN IP subnet and WAN IP subnet conflict! Please go to "Advanced Setting" to change your LAN IP subnet setting!';
		else if(o=='JS_fieldblank')
		        translated_string="This field can't be blank!!!";
		else if(o=='JS_LAN_IP_hint')
			translated_string="<br><br><br><br><i>PS: LAN IP address of the " + PID_STR + " will be <b>" + document.form.LANIP.value + " </b></i>.";
        }
        return translated_string;
}

function intoa(ip)
{
	n=0;
	vip=0;

	for(i=0;i<ip.length;i++)
	{
	   c = ip.charAt(i);

	   if (c == '.')
	   {
	      vip = vip * 256 + n;
	      n = 0;
	   }
	   else if(c>='0' && c<='9')
     	   {
       	      n = n*10 + (c-'0');
       	   }
        }
        vip = vip*256 + n;
	return(vip);
}

function get_ip_str(ip_int)
{
	v4=ip_int%0x100;
	v3=((ip_int-v4)/0x100)%0x100;
	v2=((ip_int-v4-v3*0x100)/0x10000)%0x100;
	v1=((ip_int-v4-v3*0x100-v2*0x10000)/(0x1000000))%0x100;
	
	return v1+"."+v2+"."+v3+"."+v4;
}

function lip_range(ip, nm, lang) {
	nip=	intoa(ip);
	nnm=	intoa(nm);
	bs=0;
	bs2=0;
	rem=0;
	rem2=0;
	diff=0;
	diff2=0;
	i=0;

	if (lang=='TW')
		or_str=' 或 ';
	else if (lang=='CN')
		or_str=' 或 ';
	else
		or_str=' or ';

	for (bs=nip, bs2=nnm; rem2==0; bs-=rem, bs=bs/2, bs2-=rem2, bs2=bs2/2, i++)
	{
		rem=bs%2;
		rem2=bs2%2;
		if (rem2==0)
			diff=diff + (rem<<i);
	}
	diff2=((1<<(i-1))-1);
	
	lip_start=nip-diff+1;
	lip_end=lip_start+ diff2-2;
/*	
	alert(get_ip_str(lip_start));
	alert(get_ip_str(nip-1));
	alert(get_ip_str(nip+1));
	alert(get_ip_str(lip_end));
*/	
	if (nip==lip_start)
	{
		ip_range_str=get_ip_str(lip_start+1) + ' ~ ' + get_ip_str(lip_end);
	}
	else if (nip==lip_end)
	{
		ip_range_str=get_ip_str(lip_start) + ' ~ ' + get_ip_str(lip_end-1);
	}
	else
	{
		if (lip_start!=nip-1)
		{
			ip_range_str=get_ip_str(lip_start) + ' ~ ' + get_ip_str(nip-1);
		}
		else
		{
			ip_range_str=get_ip_str(lip_start);
		}

		ip_range_str += or_str;
			
		if (nip+1!=lip_end)
		{
			ip_range_str += (get_ip_str(nip+1) + ' ~ ' + get_ip_str(lip_end));
		}
		else
		{
			ip_range_str += get_ip_str(nip+1);
		}
	}

	return ip_range_str;
}

function inputCtrl(o, flag)
{
    if (flag == 0)
    {
       o.disabled = 1;
       o.style.backgroundColor = "gray";
    }
    else
    {
       o.disabled = 0;
       o.style.backgroundColor = "white";
    }
}

function submit_gateway_SP()
{
	if(document.form.op_mode.value=="gw-repeater")
		submit_ure_SP();
	else
		submit_gateway();
}

function submit_gateway()
{
	inputCtrl(document.form.op_mode, 0);
	inputCtrl(document.form.wl_mode_EX, 0);
	inputCtrl(document.form.preferred_lang, 0);
	inputCtrl(document.form.LANIP, 0);
	inputCtrl(document.form.LANNM, 0);
	if(document.form.op_mode.value!="AP")
	{
		inputCtrl(document.form.wan_ip, 0);
		inputCtrl(document.form.wan_mask, 0);
	}
	

	if(document.form.op_mode.value=="gw-repeater")
	{
		inputCtrl(document.form.sta_state, 0);

		inputCtrl(document.form.wf1_ssid, 0);
		inputCtrl(document.form.wf2_ssid, 0);
		inputCtrl(document.form.wf3_ssid, 0);
		inputCtrl(document.form.wf4_ssid, 0);
		inputCtrl(document.form.wf1_key, 0);
		inputCtrl(document.form.wf2_key, 0);
		inputCtrl(document.form.wf3_key, 0);
		inputCtrl(document.form.wf4_key, 0);
		inputCtrl(document.form.wf1_passwd, 0);
		inputCtrl(document.form.wf2_passwd, 0);
		inputCtrl(document.form.wf3_passwd, 0);
		inputCtrl(document.form.wf4_passwd, 0);

		inputCtrl(document.form.wf1_key_org, 0);
		inputCtrl(document.form.wf2_key_org, 0);
		inputCtrl(document.form.wf3_key_org, 0);
		inputCtrl(document.form.wf4_key_org, 0);
		inputCtrl(document.form.wf1_passwd_org, 0);
		inputCtrl(document.form.wf2_passwd_org, 0);
		inputCtrl(document.form.wf3_passwd_org, 0);
		inputCtrl(document.form.wf4_passwd_org, 0);

		inputCtrl(document.form.wl_guest_auth_mode_1_org, 0);
		inputCtrl(document.form.wl_guest_wpa_mode_org, 0);
		inputCtrl(document.form.wl_guest_crypto_1_org, 0);
		inputCtrl(document.form.wl_guest_wpa_psk_1_org, 0);
		inputCtrl(document.form.wl_guest_wep_x_1_org, 0);
		inputCtrl(document.form.wl_guest_key1_1_org, 0);
		inputCtrl(document.form.wl_guest_key2_1_org, 0);
		inputCtrl(document.form.wl_guest_key3_1_org, 0);
		inputCtrl(document.form.wl_guest_key4_1_org, 0);
		inputCtrl(document.form.wl_guest_key_1_org, 0);
	}

	document.form.wl_ssid.value = document.form.ssid.value;
	document.form.wl_ssid2.value = encodeURIComponent(document.form.wl_ssid.value);

	if (document.form.SecurityLevel.value=="1" || document.form.SecurityLevel.value=="2")
	{
		document.form.wl_key1.value=document.form.wl_wpa_psk.value;
		document.form.wl_key2.value=document.form.wl_wpa_psk.value;
		document.form.wl_key3.value=document.form.wl_wpa_psk.value;
		document.form.wl_key4.value=document.form.wl_wpa_psk.value;
	}

	inputCtrl(document.form.wl_wpa_psk_org_encoded, 0);
	inputCtrl(document.form.wl_key1_org, 0);
	inputCtrl(document.form.wl_key2_org, 0);
	inputCtrl(document.form.wl_key3_org, 0);
	inputCtrl(document.form.wl_key4_org, 0);

	document.form.action_mode.value = '  Save  ';
	document.form.submit();	
}

function submit_gateway_re()
{
	inputCtrl(document.form.wl_guest_auth_mode_1_org, 0);
	inputCtrl(document.form.wl_guest_wpa_mode_org, 0);
	inputCtrl(document.form.wl_guest_crypto_1_org, 0);
	inputCtrl(document.form.wl_guest_wpa_psk_1_org, 0);
	inputCtrl(document.form.wl_guest_wep_x_1_org, 0);
	inputCtrl(document.form.wl_guest_key1_1_org, 0);
	inputCtrl(document.form.wl_guest_key2_1_org, 0);
	inputCtrl(document.form.wl_guest_key3_1_org, 0);
	inputCtrl(document.form.wl_guest_key4_1_org, 0);
	inputCtrl(document.form.wl_guest_key_1_org, 0);
	
	document.form.wl_guest_ssid_1.value = document.form.ssid.value;
	document.form.wl_guest_ssid_2.value = encodeURIComponent(document.form.wl_guest_ssid_1.value);

	document.form.wl_guest_auth_mode_1.value=document.form.wl_auth_mode.value;
	document.form.wl_guest_wpa_mode.value=document.form.wl_wpa_mode.value;
	document.form.wl_guest_crypto_1.value=document.form.wl_crypto.value;
	document.form.wl_guest_wpa_psk_1.value=document.form.wl_wpa_psk.value;
	document.form.wl_guest_wep_x_1.value=document.form.wl_wep_x.value;
	document.form.wl_guest_key1_1.value=document.form.wl_key1.value;
	document.form.wl_guest_key2_1.value=document.form.wl_key2.value;
	document.form.wl_guest_key3_1.value=document.form.wl_key3.value;
	document.form.wl_guest_key4_1.value=document.form.wl_key4.value;
	document.form.wl_guest_key_1.value=document.form.wl_key.value;

	document.form.wl_afterburner.value="off";

	if (document.form.SecurityLevel.value=="1" || document.form.SecurityLevel.value=="2")
	{
		document.form.wl_guest_key1_1.value=document.form.wl_wpa_psk.value;
		document.form.wl_guest_key2_1.value=document.form.wl_wpa_psk.value;
		document.form.wl_guest_key3_1.value=document.form.wl_wpa_psk.value;
		document.form.wl_guest_key4_1.value=document.form.wl_wpa_psk.value;
	}

	inputCtrl(document.form.wl_ssid, 0);
	inputCtrl(document.form.wl_ssid2, 0);
	inputCtrl(document.form.wl_auth_mode, 0);
	inputCtrl(document.form.wl_wpa_mode, 0);
	inputCtrl(document.form.wl_crypto, 0);
	inputCtrl(document.form.wl_wpa_psk, 0);
	inputCtrl(document.form.wl_wep_x, 0);
	inputCtrl(document.form.wl_key1, 0);
	inputCtrl(document.form.wl_key2, 0);
	inputCtrl(document.form.wl_key3, 0);
	inputCtrl(document.form.wl_key4, 0);
	inputCtrl(document.form.wl_key, 0);
}

function submit_sta()
{
	if(document.form.add_profile.value=="0")
	{
		document.form.wl_ssid.value = decodeURIComponent(AP_status[select_idx][0]);
		document.form.wl_ssid2.value = encodeURIComponent(document.form.wl_ssid.value);
	
		if (document.form.wl_ssid.value==document.form.wf1_ssid.value)
			document.form.wl_key.value=document.form.wf1_key.value;
		else if (document.form.wl_ssid.value==document.form.wf2_ssid.value)
			document.form.wl_key.value=document.form.wf2_key.value;
		else if (document.form.wl_ssid.value==document.form.wf3_ssid.value)
			document.form.wl_key.value=document.form.wf3_key.value;
		else if (document.form.wl_ssid.value==document.form.wf4_ssid.value)
			document.form.wl_key.value=document.form.wf4_key.value;
	
		var ap_crypto=-1;
		ap_crypto=AP_status[select_idx][2];
	
		if (document.form.no_ask_password.value=="1")	// use settings in profile list
		{
			if (ap_crypto==8 || ap_crypto==9)
				document.form.wl_crypto.value="tkip+aes";
			else if (ap_crypto==2 || ap_crypto==4)
				document.form.wl_crypto.value="tkip";
			else
				document.form.wl_crypto.value="aes";
		}
		else if (ap_crypto==0)
		{
			document.form.wl_auth_mode.value="open";
			document.form.wl_wep_x.value="0";
		}
		else if (ap_crypto==1)
		{
			document.form.wl_auth_mode.value="open";
			if (document.form.wl_wpa_psk.value.length==10)
				document.form.wl_wep_x.value="1";
			else
				document.form.wl_wep_x.value="2";
			
			document.form.wl_key1.value=document.form.wl_wpa_psk.value;
			document.form.wl_key2.value=document.form.wl_wpa_psk.value;
			document.form.wl_key3.value=document.form.wl_wpa_psk.value;
			document.form.wl_key4.value=document.form.wl_wpa_psk.value;
		}
		else if (	ap_crypto==2 ||
				ap_crypto==3 ||
				ap_crypto==4 ||
				ap_crypto==5 ||
				ap_crypto==8 ||
				ap_crypto==9
		)
		{
			document.form.wl_auth_mode.value="psk";
			document.form.wl_wep_x.value="0";
	
			if (ap_crypto==2 || ap_crypto==4)
				document.form.wl_crypto.value="tkip";
			else if (ap_crypto==3 || ap_crypto==5)
				document.form.wl_crypto.value="aes";
			else
				document.form.wl_crypto.value="tkip+aes";
	
			if (ap_crypto==2 || ap_crypto==3 || ap_crypto==8)
				document.form.wl_wpa_mode.value="1";
			else
				document.form.wl_wpa_mode.value="2";
		}
	}
	else
	{
		document.form.no_ask_password.value="0";

		document.form.wl_ssid.value = document.getElementById('prefer_SSID').value;
		document.form.wl_ssid2.value = encodeURIComponent(document.form.wl_ssid.value);
		
		if(document.getElementById('prefer_security').value=="0")
		{
			document.form.wl_auth_mode.value="open";
			document.form.wl_wep_x.value=document.getElementById('wep_crypto').value;
			document.form.wl_wpa_mode.value="1";
		}
		else if(document.getElementById('prefer_security').value=="1")
		{
			document.form.wl_auth_mode.value="shared";
			document.form.wl_wep_x.value=document.getElementById('wep_crypto').value;
			document.form.wl_wpa_mode.value="1";
		}
		else if(document.getElementById('prefer_security').value=="2")
		{
			document.form.wl_auth_mode.value="psk";
			document.form.wl_wep_x.value="0";
			document.form.wl_wpa_mode.value="1";
		}	
		else if(document.getElementById('prefer_security').value=="3")
		{
			document.form.wl_auth_mode.value="psk";
			document.form.wl_wep_x.value="0";
			document.form.wl_wpa_mode.value="2";
		}	

		if(document.form.wl_wep_x.value!="0")
		{
			document.form.wl_crypto.value="tkip";
			document.form.wl_wpa_psk.value="12345678";
			document.form.wl_key.value=document.getElementById("wep_key_index_str").value;
			document.form.wl_key1.value=document.getElementById('wep_key_str').value;
			document.form.wl_key2.value=document.getElementById('wep_key_str').value;
			document.form.wl_key3.value=document.getElementById('wep_key_str').value;
			document.form.wl_key4.value=document.getElementById('wep_key_str').value;
		}
		else if(document.form.wl_auth_mode.value=="psk")
		{
			document.form.wl_crypto.value=document.getElementById('wpa_encryption').value;
			document.form.wl_wpa_psk.value=document.getElementById('wpa_psk').value;
			document.form.wl_key.value="1";
			document.form.wl_key1.value="";
			document.form.wl_key2.value="";
			document.form.wl_key3.value="";
			document.form.wl_key4.value="";
		}
	}

	inputCtrl(document.form.op_mode, 0);
	inputCtrl(document.form.wl_mode_EX, 0);
	inputCtrl(document.form.sta_state, 0);
	inputCtrl(document.form.preferred_lang, 0);
	inputCtrl(document.form.LANIP, 0);
	inputCtrl(document.form.LANNM, 0);
	
	inputCtrl(document.form.wf1_ssid, 0);
	inputCtrl(document.form.wf2_ssid, 0);
	inputCtrl(document.form.wf3_ssid, 0);
	inputCtrl(document.form.wf4_ssid, 0);
	inputCtrl(document.form.wf1_key, 0);
	inputCtrl(document.form.wf2_key, 0);
	inputCtrl(document.form.wf3_key, 0);
	inputCtrl(document.form.wf4_key, 0);
	inputCtrl(document.form.wf1_passwd, 0);
	inputCtrl(document.form.wf2_passwd, 0);
	inputCtrl(document.form.wf3_passwd, 0);
	inputCtrl(document.form.wf4_passwd, 0);

	inputCtrl(document.form.wf1_key_org, 0);
	inputCtrl(document.form.wf2_key_org, 0);
	inputCtrl(document.form.wf3_key_org, 0);
	inputCtrl(document.form.wf4_key_org, 0);
	inputCtrl(document.form.wf1_passwd_org, 0);
	inputCtrl(document.form.wf2_passwd_org, 0);
	inputCtrl(document.form.wf3_passwd_org, 0);
	inputCtrl(document.form.wf4_passwd_org, 0);

	document.form.action_mode.value = '  Save  ';
	document.form.submit();	
}

function submit_ure()
{
	if(document.form.add_profile.value=="0")
	{
		document.form.wl_ssid.value = decodeURIComponent(AP_status[select_idx][0]);
		document.form.wl_ssid2.value = encodeURIComponent(document.form.wl_ssid.value);
	
		if (document.form.wl_ssid.value==document.form.wf1_ssid.value)
			document.form.wl_key.value=document.form.wf1_key.value;
		else if (document.form.wl_ssid.value==document.form.wf2_ssid.value)
			document.form.wl_key.value=document.form.wf2_key.value;
		else if (document.form.wl_ssid.value==document.form.wf3_ssid.value)
			document.form.wl_key.value=document.form.wf3_key.value;
		else if (document.form.wl_ssid.value==document.form.wf4_ssid.value)
			document.form.wl_key.value=document.form.wf4_key.value;
	
		var ap_crypto=-1;
		ap_crypto=AP_status[select_idx][2];
	
		if (document.form.no_ask_password.value=="1")	// use settings in profile list
		{
			if (ap_crypto==8 || ap_crypto==9)
				document.form.wl_crypto.value="tkip+aes";
			else if (ap_crypto==2 || ap_crypto==4)
				document.form.wl_crypto.value="tkip";
			else
				document.form.wl_crypto.value="aes";
		}
		else if (ap_crypto==0)
		{
			document.form.wl_auth_mode.value="open";
			document.form.wl_wep_x.value="0";
		}
		else if (ap_crypto==1)
		{
			document.form.wl_auth_mode.value="open";
			if (document.form.wl_wpa_psk.value.length==10)
				document.form.wl_wep_x.value="1";
			else
				document.form.wl_wep_x.value="2";
			
			document.form.wl_key1.value=document.form.wl_wpa_psk.value;
			document.form.wl_key2.value=document.form.wl_wpa_psk.value;
			document.form.wl_key3.value=document.form.wl_wpa_psk.value;
			document.form.wl_key4.value=document.form.wl_wpa_psk.value;
		}
		else if (	ap_crypto==2 ||
				ap_crypto==3 ||
				ap_crypto==4 ||
				ap_crypto==5 ||
				ap_crypto==8 ||
				ap_crypto==9
		)
		{
			document.form.wl_auth_mode.value="psk";
			document.form.wl_wep_x.value="0";
	
			if (ap_crypto==2 || ap_crypto==4)
				document.form.wl_crypto.value="tkip";
			else if (ap_crypto==3 || ap_crypto==5)
				document.form.wl_crypto.value="aes";
			else
				document.form.wl_crypto.value="tkip+aes";
	
			if (ap_crypto==2 || ap_crypto==3 || ap_crypto==8)
				document.form.wl_wpa_mode.value="1";
			else
				document.form.wl_wpa_mode.value="2";
		}
	}
	else
	{
		document.form.no_ask_password.value="0";

		document.form.wl_ssid.value = document.getElementById('prefer_SSID').value;
		document.form.wl_ssid2.value = encodeURIComponent(document.form.wl_ssid.value);
		
		if(document.getElementById('prefer_security').value=="0")
		{
			document.form.wl_auth_mode.value="open";
			document.form.wl_wep_x.value=document.getElementById('wep_crypto').value;
			document.form.wl_wpa_mode.value="1";
		}
		else if(document.getElementById('prefer_security').value=="1")
		{
			document.form.wl_auth_mode.value="shared";
			document.form.wl_wep_x.value=document.getElementById('wep_crypto').value;
			document.form.wl_wpa_mode.value="1";
		}
		else if(document.getElementById('prefer_security').value=="2")
		{
			document.form.wl_auth_mode.value="psk";
			document.form.wl_wep_x.value="0";
			document.form.wl_wpa_mode.value="1";
		}	
		else if(document.getElementById('prefer_security').value=="3")
		{
			document.form.wl_auth_mode.value="psk";
			document.form.wl_wep_x.value="0";
			document.form.wl_wpa_mode.value="2";
		}	

		if(document.form.wl_wep_x.value!="0")
		{
			document.form.wl_crypto.value="tkip";
			document.form.wl_wpa_psk.value="12345678";
			document.form.wl_key.value=document.getElementById("wep_key_index_str").value;
			document.form.wl_key1.value=document.getElementById('wep_key_str').value;
			document.form.wl_key2.value=document.getElementById('wep_key_str').value;
			document.form.wl_key3.value=document.getElementById('wep_key_str').value;
			document.form.wl_key4.value=document.getElementById('wep_key_str').value;
		}
		else if(document.form.wl_auth_mode.value=="psk")
		{
			document.form.wl_crypto.value=document.getElementById('wpa_encryption').value;
			document.form.wl_wpa_psk.value=document.getElementById('wpa_psk').value;
			document.form.wl_key.value="1";
			document.form.wl_key1.value="";
			document.form.wl_key2.value="";
			document.form.wl_key3.value="";
			document.form.wl_key4.value="";
		}
	}

	inputCtrl(document.form.op_mode, 0);
	inputCtrl(document.form.wl_mode_EX, 0);
	inputCtrl(document.form.sta_state, 0);
	inputCtrl(document.form.preferred_lang, 0);
	inputCtrl(document.form.LANIP, 0);
	inputCtrl(document.form.LANNM, 0);
	
	inputCtrl(document.form.wf1_ssid, 0);
	inputCtrl(document.form.wf2_ssid, 0);
	inputCtrl(document.form.wf3_ssid, 0);
	inputCtrl(document.form.wf4_ssid, 0);
	inputCtrl(document.form.wf1_key, 0);
	inputCtrl(document.form.wf2_key, 0);
	inputCtrl(document.form.wf3_key, 0);
	inputCtrl(document.form.wf4_key, 0);
	inputCtrl(document.form.wf1_passwd, 0);
	inputCtrl(document.form.wf2_passwd, 0);
	inputCtrl(document.form.wf3_passwd, 0);
	inputCtrl(document.form.wf4_passwd, 0);

	inputCtrl(document.form.wf1_key_org, 0);
	inputCtrl(document.form.wf2_key_org, 0);
	inputCtrl(document.form.wf3_key_org, 0);
	inputCtrl(document.form.wf4_key_org, 0);
	inputCtrl(document.form.wf1_passwd_org, 0);
	inputCtrl(document.form.wf2_passwd_org, 0);
	inputCtrl(document.form.wf3_passwd_org, 0);
	inputCtrl(document.form.wf4_passwd_org, 0);

	document.form.action_mode.value = '  Save  ';
	document.form.submit();	
}

function submit_ure_SP()
{
	if(document.form.add_profile.value=="0")
	{
		document.form.wl_ssid_ure.value = decodeURIComponent(AP_status[select_idx][0]);
		document.form.wl_ssid2_ure.value = encodeURIComponent(document.form.wl_ssid_ure.value);

		if (document.form.wl_ssid_ure.value==document.form.wf1_ssid.value)
			document.form.wl_key_ure.value=document.form.wf1_key.value;
		else if (document.form.wl_ssid_ure.value==document.form.wf2_ssid.value)
			document.form.wl_key_ure.value=document.form.wf2_key.value;
		else if (document.form.wl_ssid_ure.value==document.form.wf3_ssid.value)
			document.form.wl_key_ure.value=document.form.wf3_key.value;
		else if (document.form.wl_ssid_ure.value==document.form.wf4_ssid.value)
			document.form.wl_key_ure.value=document.form.wf4_key.value;

		var ap_crypto=-1;
		ap_crypto=AP_status[select_idx][2];

		if (document.form.no_ask_password.value=="1")	// use settings in profile list
		{
			if (ap_crypto==8 || ap_crypto==9)
				document.form.wl_crypto_ure.value="tkip+aes";
			else if (ap_crypto==2 || ap_crypto==4)
				document.form.wl_crypto_ure.value="tkip";
			else
				document.form.wl_crypto_ure.value="aes";
		}
		else if (ap_crypto==0)
		{
			document.form.wl_auth_mode_ure.value="open";
			document.form.wl_wep_x_ure.value="0";
		}
		else if (ap_crypto==1)
		{
			document.form.wl_auth_mode_ure.value="open";
			if (document.form.wl_wpa_psk_ure.value.length==10)
				document.form.wl_wep_x_ure.value="1";
			else
				document.form.wl_wep_x_ure.value="2";
		
			document.form.wl_key1_ure.value=document.form.wl_wpa_psk_ure.value;
			document.form.wl_key2_ure.value=document.form.wl_wpa_psk_ure.value;
			document.form.wl_key3_ure.value=document.form.wl_wpa_psk_ure.value;
			document.form.wl_key4_ure.value=document.form.wl_wpa_psk_ure.value;
		}
		else if (	ap_crypto==2 ||
				ap_crypto==3 ||
				ap_crypto==4 ||
				ap_crypto==5 ||
				ap_crypto==8 ||
				ap_crypto==9
		)
		{
			document.form.wl_auth_mode_ure.value="psk";
			document.form.wl_wep_x_ure.value="0";

			if (ap_crypto==2 || ap_crypto==4)
				document.form.wl_crypto_ure.value="tkip";
			else if (ap_crypto==3 || ap_crypto==5)
				document.form.wl_crypto_ure.value="aes";
			else
				document.form.wl_crypto_ure.value="tkip+aes";

			if (ap_crypto==2 || ap_crypto==3 || ap_crypto==8)
				document.form.wl_wpa_mode_ure.value="1";
			else
				document.form.wl_wpa_mode_ure.value="2";
		}
	}
	else
	{
		document.form.no_ask_password.value="0";

		document.form.wl_ssid_ure.value = document.getElementById('prefer_SSID').value;
		document.form.wl_ssid2_ure.value = encodeURIComponent(document.form.wl_ssid_ure.value);
		
		if(document.getElementById('prefer_security').value=="0")
		{
			document.form.wl_auth_mode_ure.value="open";
			document.form.wl_wep_x_ure.value=document.getElementById('wep_crypto').value;
			document.form.wl_wpa_mode_ure.value="1";
		}
		else if(document.getElementById('prefer_security').value=="1")
		{
			document.form.wl_auth_mode_ure.value="shared";
			document.form.wl_wep_x_ure.value=document.getElementById('wep_crypto').value;
			document.form.wl_wpa_mode_ure.value="1";
		}
		else if(document.getElementById('prefer_security').value=="2")
		{
			document.form.wl_auth_mode_ure.value="psk";
			document.form.wl_wep_x_ure.value="0";
			document.form.wl_wpa_mode_ure.value="1";
		}	
		else if(document.getElementById('prefer_security').value=="3")
		{
			document.form.wl_auth_mode_ure.value="psk";
			document.form.wl_wep_x_ure.value="0";
			document.form.wl_wpa_mode_ure.value="2";
		}	

		if(document.form.wl_wep_x_ure.value!="0")
		{
			document.form.wl_crypto_ure.value="tkip";
			document.form.wl_wpa_psk_ure.value="12345678";
			document.form.wl_key_ure.value=document.getElementById("wep_key_index_str").value;
			document.form.wl_key1_ure.value=document.getElementById('wep_key_str').value;
			document.form.wl_key2_ure.value=document.getElementById('wep_key_str').value;
			document.form.wl_key3_ure.value=document.getElementById('wep_key_str').value;
			document.form.wl_key4_ure.value=document.getElementById('wep_key_str').value;
		}
		else if(document.form.wl_auth_mode_ure.value=="psk")
		{
			document.form.wl_crypto_ure.value=document.getElementById('wpa_encryption').value;
			document.form.wl_wpa_psk_ure.value=document.getElementById('wpa_psk').value;
			document.form.wl_key_ure.value="1";
			document.form.wl_key1_ure.value="";
			document.form.wl_key2_ure.value="";
			document.form.wl_key3_ure.value="";
			document.form.wl_key4_ure.value="";
		}
	}

	inputCtrl(document.form.op_mode, 0);
	inputCtrl(document.form.wl_mode_EX, 0);
	inputCtrl(document.form.sta_state, 0);
	inputCtrl(document.form.preferred_lang, 0);
	inputCtrl(document.form.LANIP, 0);
	inputCtrl(document.form.LANNM, 0);
	inputCtrl(document.form.wan_ip, 0);
	inputCtrl(document.form.wan_mask, 0);

	inputCtrl(document.form.wf1_ssid, 0);
	inputCtrl(document.form.wf2_ssid, 0);
	inputCtrl(document.form.wf3_ssid, 0);
	inputCtrl(document.form.wf4_ssid, 0);
	inputCtrl(document.form.wf1_key, 0);
	inputCtrl(document.form.wf2_key, 0);
	inputCtrl(document.form.wf3_key, 0);
	inputCtrl(document.form.wf4_key, 0);
	inputCtrl(document.form.wf1_passwd, 0);
	inputCtrl(document.form.wf2_passwd, 0);
	inputCtrl(document.form.wf3_passwd, 0);
	inputCtrl(document.form.wf4_passwd, 0);

	inputCtrl(document.form.wf1_key_org, 0);
	inputCtrl(document.form.wf2_key_org, 0);
	inputCtrl(document.form.wf3_key_org, 0);
	inputCtrl(document.form.wf4_key_org, 0);
	inputCtrl(document.form.wf1_passwd_org, 0);
	inputCtrl(document.form.wf2_passwd_org, 0);
	inputCtrl(document.form.wf3_passwd_org, 0);
	inputCtrl(document.form.wf4_passwd_org, 0);

	submit_gateway_re();

	document.form.action_mode.value = '  Save  ';
	document.form.submit();	
}
