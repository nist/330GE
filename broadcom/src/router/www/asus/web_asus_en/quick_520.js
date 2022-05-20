var prevTagLabel = new Array(
                       Array("Country",""),
                       Array("Broadband",""),
                       Array("PPPoE",""),
                       Array("MacHost",""),
                       Array("WANSetting",""),
                       Array("Firewall",""),
                       Array("Wireless",""));

var countryTime = new Array();
var prevTag = 0;
var sameWireless=true;
var tempSVC;
var tempDate;
var tempTime;

function inputCtrl1(o, flag)
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

function inputRCtrl1(o, flag)
{
    if (flag == 0)
    {
       o[0].disabled = 1;   
       o[1].disabled = 1;
    }
    else
    {
       o[0].disabled = 0;
       o[1].disabled = 0;
    }
}

function inputRCtrl2(o, flag)
{
    if (flag == 0)
    {
       o[0].checked = true;
       o[1].checked = false;
    }
    else
    {
       o[0].checked = false;
       o[1].checked = true;
       //o.style.backgroundColor = "white";
    }
}

function inputCCtrl4(o, flag)
{ 
    o[0].checked = false;
    o[1].checked = false;
    o[2].checked = false;
    o[3].checked = false;
    o[4].checked = false;
    o[flag].checked = true;
}

function change_security(o, s, l)
{
    if (o.value == "0")
    {
    	pflag = 0;
    	wflag = 0;

    	authTypeValue = "open"; 	// Open or Shared
    	wepTypeValue = "0";	// None
    	cryptoValue = "tkip";
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
//    	if (confirm("<#JS_wpa#>"))
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

    if (authTypeValue!="")
    {
    	document.form.wl_auth_mode.value = authTypeValue;
    	document.form.wl_crypto.value = cryptoValue;
        document.form.wl_wep_x.value = wepTypeValue;
    }

    if (l==0)
    {
       document.form.wl_wpa_psk.value = "";

       if (o.value == "1")
       {
       		wep = "0000000000";
       		is_wlphrase("WLANConfig11b", 'wl_phrase_x', document.form.wl_wpa_psk);
       }
       else if (o.value == "2")
       {
       		wep = "00000000000000000000000000";
       		is_wlphrase("WLANConfig11b", 'wl_phrase_x', document.form.wl_wpa_psk);
       }
       else
       {
       		wep = "";
       		document.form.wl_key1.value = wep;
       		document.form.wl_key2.value = wep;
       		document.form.wl_key3.value = wep;
       		document.form.wl_key4.value = wep;
       }
    }

    inputCtrl(document.form.wl_wpa_psk, pflag);
    inputCtrl(document.form.wl_key1, wflag);
    inputCtrl(document.form.wl_key2, wflag);
    inputCtrl(document.form.wl_key3, wflag);
    inputCtrl(document.form.wl_key4, wflag);
    inputCtrl(document.form.wl_key,  wflag);

    if (pflag == "1" && l == 0)
    {
        document.form.wl_wpa_psk.focus();
    }
}

function loadSecurity()
{	
    authType = 	document.form.wl_auth_mode.value;
    cryptoType = document.form.wl_crypto.value;
    wepType = document.form.wl_wep_x.value;
    
    if (authType == "open" && wepType == "0")
    {
    	document.form.SecurityLevel.value = "0";
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
    }
    
    change_security(document.form.SecurityLevel, "WLANConfig11b", 1);
}

function loadQuick()
{	    	    	
    /* Transfer NATEnabled, wan_proto to x_Mode, x_Connection Rule */     
    document.form.next_host.value = location.host;

    document.form.wl_ssid.value = decodeURIComponent(document.form.wl_ssid2.value);
    if (document.form.current_page.value=="Basic_GOperation_Content.asp")
    {
//    	document.form.wl_guest_ssid_1.value = decodeURIComponent(document.form.wl_guest_ssid_2.value);

        if (document.form.wl_mode_ex.value=="re")
        {
        	if (document.form.wan_proto.value != 'dhcp' && document.form.wan_proto.value != 'static')
        	{
        		document.form.wan_proto.value = 'dhcp';
        		inputCCtrl4(document.form.x_WANType, 0);
        	}
        	document.form.x_WANType[1].disabled=1;
        	document.form.x_WANType[2].disabled=1;
        	document.form.x_WANType[4].disabled=1;

/*
    	    if (document.form.wan_proto.value == 'pppoe')
    	    {
                document.form.wan_proto.value = 'dhcp';
                inputCCtrl4(document.form.x_WANType, 0);
                
            }
	    document.form.x_WANType[1].disabled=1;
*/
        }

        if (document.form.wl_mode_ex.value!="ap")
        {
		inputCtrl(document.form.wan_hostname, 0);
   		inputCtrl(document.form.wan_hwaddr_x, 0);
   		inputCtrl(document.form.wan_heartbeat_x, 0);
   	}
    }

    if (document.form.wan_route_x.value == 'IP_Routed')
    {
    	if (document.form.wan_nat_x.value == '1')
    		document.form.x_Mode.value = 'Home Gateway';
    	else document.form.x_Mode.value = 'Router';

    	if (document.form.wan_proto.value == 'pppoe')
    	{
    	    inputCCtrl4(document.form.x_WANType, 1);
            inputRCtrl2(document.form.x_DHCPClient, 0);
    	}
    	else if (document.form.wan_proto.value == 'pptp')
    	{
    	    inputCCtrl4(document.form.x_WANType, 2);
            inputRCtrl2(document.form.x_DHCPClient, 1);
    	}
    	else
    	{
    	    if (document.form.wan_proto.value == 'static')
    	    {
    	    	inputCCtrl4(document.form.x_WANType, 3);
    		inputRCtrl2(document.form.x_DHCPClient, 1);
    	    }
    	    else
    	    {
    	    	if (document.form.wan_proto.value == 'bigpond')
    	    		inputCCtrl4(document.form.x_WANType, 4);
    	    	else inputCCtrl4(document.form.x_WANType, 0);
        	inputRCtrl2(document.form.x_DHCPClient, 0);
    	    }
    	}
    	changeWANType();
    }
    else
    {
    	document.form.x_Mode.value = 'Access Point';
    }

//    masq_wepkey();
    loadSecurity();
    sameWireless=true;
    changeMode();
}

function loadMode()
{
    /* Transfer NATEnabled, wan_proto to x_Mode, x_Connection Rule */
     //alert(document.form.wan_proto.value);
    document.form.next_host.value = location.host;

    //alert(document.form.wan_dnsenable_x[0].checked);
    //alert(document.form.wan_dnsenable_x[1].checked);

    if (document.form.wan_nat_x.value == '1')
    {
    	document.form.x_Mode[0].checked = true;
    	document.form.x_Mode[1].checked = false;
    }
    else if (document.form.wan_route_x.value == 'IP_Routed')
    {
    	document.form.x_Mode[0].checked = false;
    	document.form.x_Mode[1].checked = false;
    }
    else
    {
    	document.form.x_Mode[0].checked = false;
       	document.form.x_Mode[1].checked = true;
    }
    //changeMode();
}

function saveMode(o)
{
    //changeMode();
    document.form.next_page.value = 'Main_Operation.asp';

    if (document.form.x_Mode[1].checked == true) //Access Point
    {
        document.form.wan_nat_x.value = '0';
//        document.form.wan_proto.value = 'static';
        document.form.wan_route_x.value = 'IP_Bridged';
        document.form.wl_mode_ex.value = 'ap'
        document.form.wl_guest_enable.value = '0';
        document.form.mssid_enable.value = '0';
        document.form.wl_individual_ure.value='0';
    }
    else if (document.form.x_Mode[0].checked == true) //Home Gateway
    {
        document.form.wan_nat_x.value = '1';
        document.form.wan_route_x.value = 'IP_Routed';
        document.form.wan_proto.value = 'dhcp';
        document.form.wl_mode_ex.value = 'ap'
        document.form.wl_guest_enable.value = '0';
        document.form.mssid_enable.value = '0';
        document.form.wl_individual_ure.value='1';
    }
//    o.value = "  Save  ";
    document.form.action_mode.value = "  Save  ";
    document.form.submit();
}

function setSVCCheck(d1, d2, d3, d4, d5, d6)
{
   str = "";
  
   if (d6 == true ) str = "1" + str;
   else str = "0" + str;
   if (d5 == true ) str = "1" + str;
   else str = "0" + str;
   if (d4 == true ) str = "1" + str;
   else str = "0" + str;
   if (d3 == true ) str = "1" + str;
   else str = "0" + str;
   if (d2 == true ) str = "1" + str;
   else str = "0" + str;
   if (d1 == true ) str = "1" + str;
   else str = "0" + str;   
   
   return str;
}

function saveQuick(o)
{
    window.top.pageChanged = 0;
    window.top.pageChangedCount = 0;

	if (document.form.current_page.value=="Basic_GOperation_Content.asp" &&
	    document.form.wl_mode_ex.value=="re" &&
	    (document.form.x_WANType[1].checked == true)
	   )
	{
		document.form.wan_proto.value = 'dhcp'
		inputCCtrl4(document.form.x_WANType, 0);
		document.form.x_WANType[1].disabled=1;

		if (document.form.preferred_lang.value == 'TW')
			location.href = "Basic_GOperation_Content_TW.asp" + '#' + "Broadband";
		else if (document.form.preferred_lang.value == 'CN')
			location.href = "Basic_GOperation_Content_CN.asp" + '#' + "Broadband";
		else
			location.href = document.form.current_page.value + '#' + "Broadband";
		return false;
	}

	if (!validate_string4(document.form.wl_ssid))
		return false;
	else
		document.form.wl_ssid2.value = encodeURIComponent(document.form.wl_ssid.value);
/*
    document.form.mssid_ssid_x0.value = document.form.wl_ssid.value;
    document.form.mssid_ssid_2_x0.value = encodeURIComponent(document.form.wl_ssid.value);
*/    
//    if (document.form.current_page.value=="Basic_GOperation_Content.asp")
//    	document.form.wl_guest_ssid_2.value = encodeURIComponent(document.form.wl_guest_ssid_1.value);

    if (document.form.x_Mode.value == 'Access Point')
    {
        document.form.wan_nat_x.value = '0';
//        document.form.wan_proto.value = 'static';
        document.form.wan_route_x.value = 'IP_Bridged';
    }
    else
    {
    	if (document.form.x_Mode.value == 'Router')
    	{
        	document.form.wan_nat_x.value = '0';
        	document.form.wan_route_x.value = 'IP_Routed';
    	}
    	else
    	{
        	document.form.wan_nat_x.value = '1';
        	document.form.wan_route_x.value = 'IP_Routed';
	}

        if (document.form.x_WANType[1].checked == true)
        {
           document.form.wan_proto.value = 'pppoe';
        }
        else if (document.form.x_WANType[2].checked == true)
        {
           document.form.wan_proto.value = 'pptp';
        }
        else if (document.form.x_WANType[3].checked == true)
        {
           document.form.wan_proto.value = 'static';
        }
        else if (document.form.x_WANType[4].checked == true)
        {
           document.form.wan_proto.value = 'bigpond';
        }
        else
        {
           document.form.wan_proto.value = 'dhcp';
        }

	if (document.form.current_page.value=="Basic_GOperation_Content.asp")	// copy from onSubmit() in general.js for WL320g
	{
	   	if (document.form.wan_proto.value=="static" ||
	   		document.form.wan_proto.value=="pptp")
	   	{
   			if(!wan_netmask_check(document.form.wan_netmask))
   				return false;

   			if(!validate_ipaddr_final(document.form.wan_ipaddr, 'wan_ipaddr') ||
				!validate_ipaddr_final(document.form.wan_netmask, 'wan_netmask') ||
				!validate_ipaddr_final(document.form.wan_gateway, 'wan_gateway')) return false;
	   	}

	   	if (document.form.wan_dnsenable_x[0].checked != true)
   		{
   			if(!validate_ipaddr_final(document.form.wan_dns1_x, 'wan_dns1_x') ||
			!validate_ipaddr_final(document.form.wan_dns2_x, 'wan_dns2_x')) return false;
			
   			if(document.form.wan_dns1_x.value=="" && document.form.wan_dns2_x.value=="")
   			{
   				if (document.form.wan_dns1_x.value=="")
   					document.form.wan_dns1_x.focus();
   				else if (document.form.wan_dns2_x.value=="")
   					document.form.wan_dns2_x.focus();
				alert(translate_lang('JS_fieldblank',document.form.preferred_lang.value));

				if (document.form.wan_proto.value=="static")
					return false;

   				document.form.wan_dnsenable_x[0].checked = true;
				inputCtrl(document.form.wan_dns1_x, 0);
				inputCtrl(document.form.wan_dns2_x, 0);			
				document.form.wan_dnsenable_x[0].focus();
				return false;
   			}
	   	}
	   	
	   	if (document.form.wl_mode_ex.value=="ap" && document.form.wan_proto.value != "dhcp")
	   		alert(quick_translate_lang('JS_GW_AP',document.form.preferred_lang.value));
	   	else
	   	{
   			inputCtrl(document.form.wan_hostname, 1);
   			inputCtrl(document.form.wan_hwaddr_x, 1);
   			inputCtrl(document.form.wan_heartbeat_x, 1);
	   	}
	}

    	document.form.SecurityLevel.disabled = true;
    	document.form.x_DHCPClient.disabled = true;
    	document.form.x_DHCPClient[0].checked = false;
    	document.form.x_DHCPClient[1].checked = false;
    	inputRCtrl1(document.form.wan_dnsenable_x, 1);
    }

/*
	document.form.wl_guest_auth_mode_1.value=document.form.wl_auth_mode.value;
	document.form.wl_guest_crypto_1.value   =document.form.wl_crypto.value;
	document.form.wl_guest_wep_x_1.value    =document.form.wl_wep_x.value;

//	if (document.form.current_page.value=="Basic_AOperation_Content.asp")
//	{
		if (document.form.wl_mode_ex.value=="re")
		{
			document.form.wl_guest_ssid_1.value   =document.form.wl_ssid.value;
			document.form.wl_guest_ssid_2.value   =encodeURIComponent(document.form.wl_ssid.value);
		}
		else
			inputCtrl(document.form.wl_guest_ssid_1, 0);
//	}
	document.form.wl_guest_wpa_psk_1.value=document.form.wl_wpa_psk.value;
	document.form.wl_guest_key1_1.value   =document.form.wl_key1.value;
	document.form.wl_guest_key2_1.value   =document.form.wl_key2.value;
	document.form.wl_guest_key3_1.value   =document.form.wl_key3.value;
	document.form.wl_guest_key4_1.value   =document.form.wl_key4.value;
	document.form.wl_guest_key_1.value    =document.form.wl_key.value;
*/

	inputCtrl(document.form.wl_wpa_psk, 1);
	inputCtrl(document.form.wl_key1, 1);
	inputCtrl(document.form.wl_key2, 1);
	inputCtrl(document.form.wl_key3, 1);
	inputCtrl(document.form.wl_key4, 1);
	inputCtrl(document.form.wl_key, 1);

//	unmasq_wepkey();
//	if (document.form.wl_mode_ex.value=="re")
//	unmasq_wepkey_guest();

//    o.value = '  Save  ';
    document.form.action_mode.value = '  Save  ';
    return true;
}

function changeMode()
{
    if (document.form.x_Mode.value == 'Home Gateway' || document.form.x_Mode.value == 'Router')
    {
    	 if (document.form.x_WANType[0].checked == true)
    	 {
    	     inputCtrl1(document.form.wan_pppoe_username, 0);
    	     inputCtrl1(document.form.wan_pppoe_passwd, 0);

    	     inputRCtrl1(document.form.x_DHCPClient, 0);
    	     inputRCtrl2(document.form.x_DHCPClient, 0);
    	     inputCtrl1(document.form.wan_ipaddr, 1);
    	     inputCtrl1(document.form.wan_netmask, 1);
    	     inputCtrl1(document.form.wan_gateway, 1);
    	     changeDHCPClient();
    	     changeDNSServer();
    	 }
    	 else
    	 {
    	     // require account
    	     if (document.form.x_WANType[1].checked == true || document.form.x_WANType[2].checked == true || document.form.x_WANType[4].checked == true)
    	     {
    	        inputCtrl1(document.form.wan_pppoe_username, 1);
    	        inputCtrl1(document.form.wan_pppoe_passwd, 1);
    	     }
    	     else
    	     {
    	     	inputCtrl1(document.form.wan_pppoe_username, 0);
    	        inputCtrl1(document.form.wan_pppoe_passwd, 0);
    	     }

    	     // require IP
    	     if (document.form.x_WANType[0].checked == true || document.form.x_WANType[1].checked == true || document.form.x_WANType[4].checked == true)
    	     {
    	     	inputRCtrl1(document.form.x_DHCPClient, 0);
    	     	inputRCtrl2(document.form.x_DHCPClient, 0);
    	     	inputCtrl1(document.form.wan_ipaddr, 1);
    	     	inputCtrl1(document.form.wan_netmask, 1);
    	     	inputCtrl1(document.form.wan_gateway, 1);
    	     }
    	     else if(document.form.x_WANType[2].checked == true) // pptp
    	     {
    	     	inputRCtrl1(document.form.x_DHCPClient, 0);
    	     	inputRCtrl2(document.form.x_DHCPClient, 1);
    	     	inputCtrl1(document.form.wan_ipaddr, 0);
    	     	inputCtrl1(document.form.wan_netmask, 0);
    	     	inputCtrl1(document.form.wan_gateway, 0);
    	     }
    	     else // static
    	     {
    	     	inputRCtrl1(document.form.x_DHCPClient, 0);
    	     	inputRCtrl2(document.form.x_DHCPClient, 1);
    	     	inputCtrl1(document.form.wan_ipaddr, 0);
    	     	inputCtrl1(document.form.wan_netmask, 0);
    	     	inputCtrl1(document.form.wan_gateway, 0);
    	     }
    	     changeDHCPClient();
    	     changeDNSServer();
    	 }
    }
}

function changeWANType()
{
         if (document.form.x_WANType[0].checked == true)
    	 {
    	     inputCtrl1(document.form.wan_pppoe_username, 0);
    	     inputCtrl1(document.form.wan_pppoe_passwd, 0);

    	     inputRCtrl1(document.form.x_DHCPClient, 0);
    	     inputRCtrl2(document.form.x_DHCPClient, 0);
    	     inputCtrl1(document.form.wan_ipaddr, 1);
    	     inputCtrl1(document.form.wan_netmask, 1);
    	     inputCtrl1(document.form.wan_gateway, 1);

    	     inputRCtrl1(document.form.wan_dnsenable_x, 1);
//    	     inputRCtrl2(document.form.wan_dnsenable_x, 0);
    	     inputCtrl1(document.form.wan_dns1_x, 1);
    	     inputCtrl1(document.form.wan_dns2_x, 1);

    	     changeDHCPClient();
    	     changeDNSServer();
    	 }
    	 else
    	 {
    	     // require account
    	     if (document.form.x_WANType[1].checked == true || document.form.x_WANType[2].checked == true || document.form.x_WANType[4].checked == true)
    	     {
    	        inputCtrl1(document.form.wan_pppoe_username, 1);
    	        inputCtrl1(document.form.wan_pppoe_passwd, 1);
    	     }
    	     else
    	     {
    	     	inputCtrl1(document.form.wan_pppoe_username, 0);
    	        inputCtrl1(document.form.wan_pppoe_passwd, 0);
    	     }

    	     // require IP
    	     if (document.form.x_WANType[0].checked == true || document.form.x_WANType[1].checked == true || document.form.x_WANType[4].checked == true)
    	     {
    	     	inputRCtrl1(document.form.x_DHCPClient, 0);
    	     	inputRCtrl2(document.form.x_DHCPClient, 0);
    	     	inputCtrl1(document.form.wan_ipaddr, 1);
    	     	inputCtrl1(document.form.wan_netmask, 1);
    	     	inputCtrl1(document.form.wan_gateway, 1);

    	     	inputRCtrl1(document.form.wan_dnsenable_x, 1);
//    	     	inputRCtrl2(document.form.wan_dnsenable_x, 0);
    	     	inputCtrl1(document.form.wan_dns1_x, 1);
    	     	inputCtrl1(document.form.wan_dns2_x, 1);
    	     }
    	     else if(document.form.x_WANType[2].checked == true) // pptp
    	     {
    	     	inputRCtrl1(document.form.x_DHCPClient, 0);
    	     	inputRCtrl2(document.form.x_DHCPClient, 1);
    	     	inputCtrl1(document.form.wan_ipaddr, 0);
    	     	inputCtrl1(document.form.wan_netmask, 0);
    	     	inputCtrl1(document.form.wan_gateway, 0);

    	     	inputRCtrl1(document.form.wan_dnsenable_x, 1);
//    	     	inputRCtrl2(document.form.wan_dnsenable_x, 1);
    	     	inputCtrl1(document.form.wan_dns1_x, 1); 
    	     	inputCtrl1(document.form.wan_dns2_x, 1);
    	     }
    	     else // static
    	     {
    	     	inputRCtrl1(document.form.x_DHCPClient, 0);
    	     	inputRCtrl2(document.form.x_DHCPClient, 1);
    	     	inputCtrl1(document.form.wan_ipaddr, 0);
    	     	inputCtrl1(document.form.wan_netmask, 0);
    	     	inputCtrl1(document.form.wan_gateway, 0);

    	     	inputRCtrl1(document.form.wan_dnsenable_x, 0);
    	     	inputRCtrl2(document.form.wan_dnsenable_x, 1);
    	     	inputCtrl1(document.form.wan_dns1_x, 1);
    	     	inputCtrl1(document.form.wan_dns2_x, 1);
    	     }
    	     changeDHCPClient();
    	     changeDNSServer();
    	 }
}

function changeDHCPClient()
{
    if (document.form.x_DHCPClient[0].checked == true)
    {
    	inputCtrl1(document.form.wan_ipaddr, 0);
    	inputCtrl1(document.form.wan_netmask, 0);
    	inputCtrl1(document.form.wan_gateway, 0);
    }
    else
    {
    	inputCtrl1(document.form.wan_ipaddr, 1);
    	inputCtrl1(document.form.wan_netmask, 1);
    	inputCtrl1(document.form.wan_gateway, 1);
    }
}

function changeADSLType()
{
    if (document.form.x_WANType[0].checked == true || document.form.x_WANType[3].checked == true)
    {
    	inputCtrl1(document.form.wan_pppoe_username, 0);
    	inputCtrl1(document.form.wan_pppoe_passwd, 0);
    }
    else
    {
    	inputCtrl1(document.form.wan_pppoe_username, 1);
    	inputCtrl1(document.form.wan_pppoe_passwd, 1);
    }
}

function changeDNSServer()
{
    if (document.form.wan_dnsenable_x[0].checked == true)
    {
    	inputCtrl1(document.form.wan_dns1_x, 0);
    	inputCtrl1(document.form.wan_dns2_x, 0);
    }
    else
    {
    	inputCtrl1(document.form.wan_dns1_x, 1);
    	inputCtrl1(document.form.wan_dns2_x, 1);
    }
}

function changeDHCPServer()
{
}

function toTag(s)
{
	if (s == 'Mode')
	{
	    if (document.form.x_Mode.value == 'Access Point')
	    {
	       t = 'Wireless';
	    }
	    else
	    {
	       t = 'Broadband';
	    }
	}
	else if (s == 'Country')
	{
	    if (window.top.isMode() == 'Access Point')
	    {
	       t = 'Wireless';
	    }
	    else
	    {
	       t = 'Broadband';
	    }
	}
	else if (s=='Broadband')
	{
	    if (document.form.x_WANType[1].checked == true || document.form.x_WANType[2].checked == true || document.form.x_WANType[4].checked == true)
	    {
	       changeADSLType();
	       t = 'PPPoE';
	    }
	    else if (document.form.x_WANType[0].checked == true || document.form.x_WANType[4].checked == true)
	    {
	       t = 'MacHost';
	    }
	    else
	    {
	       t = 'WANSetting';
	    }
        }
        else if (s=='PPPoE')
	{
	    if (document.form.x_WANType[1].checked == true || document.form.x_WANType[2].checked == true || document.form.x_WANType[4].checked == true)
	    {
	       if (document.form.wan_pppoe_username.value == "" || document.form.wan_pppoe_passwd.value == "")
	       {
//	    	   alert("<#JS_ADSL#>");
		   alert(quick_translate_lang('JS_ADSL',document.form.preferred_lang.value));
	    	   return;
	       }

	       if (document.form.x_WANType[4].checked == true)
	       {
	          t = 'MacHost';
	       }
	       else if (document.form.x_WANType[2].checked == true)
	       {
	          t = 'WANSetting';
	       }
	       else
	       {
	          t = 'Wireless';
	       }
	    }
	    else
	    {
	       inputRCtrl2(document.form.x_DHCPClient, 1);
	       changeDHCPClient();
	       t = 'WANSetting';
	    }
        }
        else if (s=='PPPoE_IP')
	{
	    t = 'WANSetting';
        }
        else if (s=='MacHost')
	{
	    t = 'WANSetting';
        }
        else if (s=='WANSetting')
	{
	    if ((document.form.x_DHCPClient[1].checked == true) && (document.form.wan_ipaddr.value == "" || document.form.wan_netmask.value == ""))
	    {
//	        alert("<#JS_router_ip#>");
		alert(quick_translate_lang('JS_router_ip',document.form.preferred_lang.value));
	        return;
	    }

	    if (window.top.isMode() == 'Gateway')
	    {
	    	t='Wireless';
	    }
	    else t = 'Wireless';
        }
        else if (s=='LANSetting')
	{
            if (document.form.x_Mode.value == 'Access Point')
	       t = 'Wireless';
	    else
	    {
	       checkSubnet2();
	       t = 'DHCPSetting';
	    }
        }
        else if (s=='DHCPSetting')
        {
            t = 'Wireless';
        }
        else
        {
            t = 'Wireless';
        }

        for(i=0; i< prevTagLabel.length; i++)
        {
           if (prevTagLabel[i][0] == t)
           {
              prevTagLabel[i][1]=s;
              break;
           }
        }
	if (document.form.preferred_lang.value == 'TW')
		location.href = "Basic_GOperation_Content_TW.asp" + '#' + t;
	else if (document.form.preferred_lang.value == 'CN')
		location.href = "Basic_GOperation_Content_CN.asp" + '#' + t;
	else
        	location.href = document.form.current_page.value + '#' + t;
}

function toPrevTag(s)
{
	prevTag = "";

	for(i=0; i< prevTagLabel.length; i++)
        {
           if (prevTagLabel[i][0] == s)
           {
              prevTag=prevTagLabel[i][1];
              break;
           }
        }

	if ( prevTag != "")
	{
//	    location.href = document.form.current_page.value + '#' + prevTag;

		if (document.form.preferred_lang.value == 'TW')
			location.href = "Basic_GOperation_Content_TW.asp" + '#' + prevTag;
		else if (document.form.preferred_lang.value == 'CN')
			location.href = "Basic_GOperation_Content_CN.asp" + '#' + prevTag;
		else
        		location.href = document.form.current_page.value + '#' + prevTag;
	}
	else
	{
//	    location.href = document.form.current_page.value;

		if (document.form.preferred_lang.value == 'TW')
			location.href = "Basic_GOperation_Content_TW.asp";
		else if (document.form.preferred_lang.value == 'CN')
			location.href = "Basic_GOperation_Content_CN.asp";
		else
        		location.href = document.form.current_page.value;
	}
}

function matchSubnet(ip1, ip2, count)
{
	c = 0;
	v1 = 0;
	v2 = 0;

	for(i=0;i<ip1.length;i++)
	{
	   if (ip1.charAt(i) == '.')
	   {
	      c++;

	      if (v1!=v2) return false;
	      v1 = 0;
	      v2 = 0;
	   }
	   else
	   {
	      if (ip2.charAt(i)=='.') return false;

	      v1 = v1*10 + (ip1.charAt(i) - '0');
	      v2 = v2*10 + (ip2.charAt(i) - '0');
	   }
	   if (c==count) return true;
	}
	return false;
}

function subnetPrefix(ip, orig, count)
{
	r='';
	c=0;

	for(i=0;i<ip.length;i++)
	{
	   if (ip.charAt(i) == '.')
	      c++;

	   if (c==count) break;

	   r = r + ip.charAt(i);
	}

	c=0;

	for(i=0;i<orig.length;i++)
	{
	   if (orig.charAt(i) == '.')	
	   {
	      c++;
	   }

	   if (c>=count)
	     r = r + orig.charAt(i);
	}
	return (r);
}

function checkSubnet2()
{
        /* Rule : If addresses in pool are match to subnet, don't change */
        /* Rule : If addresses in pool are not match to subnet, change to subnet.2~subnet.254 */
        if (!matchSubnet(document.form.lan_ipaddr.value, document.form.dhcp_start.value, 3) ||
            !matchSubnet(document.form.lan_ipaddr.value, document.form.dhcp_end.value, 3))
        {
            document.form.dhcp_start.value = subnetPrefix(document.form.lan_ipaddr.value, document.form.dhcp_start.value, 3);
            document.form.dhcp_end.value = subnetPrefix(document.form.lan_ipaddr.value, document.form.dhcp_end.value, 3);
        }
}

function changeCountry(o, s, v)
{
}

function changeFirewall(o, s)
{
  if (s == "WLANConfig11b")
  {
      if (document.form.WLANConfig11b_WEPType.value == 'None' || document.form.WLANConfig11b_WEPType.value == 'FromServer')
      {
         flag = 0;
         wep = "";
      }
      else
      {
         flag = 1;

         if (document.form.WLANConfig11b_WEPType.value == "64bits")
      	    wep = "0000000000";
      	 else  if (document.form.WLANConfig11b_WEPType.value == "128bits")
      	    wep = "00000000000000000000000000";
      	 else
      	    wep = "00000000000000000000000000000000";
      }

      document.form.wl_wpa_psk.value = '';
      document.form.wl_key1.value = wep;
      document.form.wl_key2.value = wep;
      document.form.wl_key3.value = wep;
      document.form.wl_key4.value = wep;

      inputCtrl(document.form.wl_wpa_psk,  flag);
      inputCtrl(document.form.wl_key1,  flag);
      inputCtrl(document.form.wl_key2,  flag);
      inputCtrl(document.form.wl_key3,  flag);
      inputCtrl(document.form.wl_key4,  flag);
      inputCtrl(document.form.wl_key,  flag);
  }
  else
  {
 	return false;
  }

  return true;
}

function is_wlphrase_q(s, o)
{
    if (document.form.SecurityLevel.value=="1" ||
    	document.form.SecurityLevel.value=="2") return(is_wlphrase(s, 'wl_phrase_x', o));
    else return(is_wlphrase(s, 'wl_wpa_psk', o));
}

function validate_wlphrase_q(s, o)
{
    if (document.form.SecurityLevel.value=="1" ||
        document.form.SecurityLevel.value=="2") return(validate_wlphrase(s, 'wl_phrase_x', o));
    else return(validate_wlphrase(s, 'wl_wpa_psk', o));
}

function quick_translate_lang(o,v)
{
        if(v=='TW')
        {
                if(o=='JS_ADSL')
                        translated_string ="如果您選擇連接至一台ADSL數據機，您便不能將此兩個欄位保留空白。";
                else if(o=='JS_router_ip')
                        translated_string="您應該為系統指定一個靜態IP位址!";
                else if(o=='JS_wpa')
                	translated_string="您已選擇另一種無線驗證方式及加密功能，您確定要作覆蓋嗎？";
                else if(o=='JS_GW_AP')
                	translated_string='在Gateway模式下，只有當WAN連線類型被設定為"Automatic IP"時，\n您才能透過乙太網路通訊埠（Ethernet port）進入設定頁面！';
        }
        else if(v=='CN')
        {
                if(o=='JS_ADSL')
                        translated_string ="如果您选择连接至一台ADSL调制解调器，您便不能将此两个栏目保留空白。";
                else if(o=='JS_router_ip')
                        translated_string="您应为系统指定一个静态IP地址!";
                else if(o=='JS_wpa')
                	translated_string="您已选择另一种无线认证方式及加密功能，您确定要覆盖吗？";
                else if(o=='JS_GW_AP')
                	translated_string='在Gateway模式下, 只有当WAN连接类型被设置为"Automatic IP"时，\n您才能透过以太网络端口（Ethernet port）进入设定页面！';
        }
        else if(v=='DE')
        {
                if(o=='JS_ADSL')
                        translated_string ="Sie können diese zwei Felder nicht leer lassen, wenn Sie die Verbindung mit einem DSL-Modem auswählen.";
                else if(o=='JS_router_ip')
                        translated_string="Sie sollten Ihrem System eine statische IP-Adresse zuweisen!";
                else if(o=='JS_wpa')
                	translated_string="Sie haben eine zusätzliche Wireless-Authentifikation und einen Verschlüsselungsmodus ausgewählt. Wollen Sie dies wirklich überschreiben?";
                else if(o=='JS_GW_AP')
                	translated_string='Sie können die Konfigurationsseite über den Ethernet-Port nur erreichen, wenn die Einstellung der WAN-Verbindungsart auf "Automatische IP" eingestellt ist!';
        }
        else if(v=='JP')
        {
                if(o=='JS_ADSL')
                        translated_string ="ADSL モデムへの接続を選択する場合は、これらの 2 つのフィールドは空にできません。";
                else if(o=='JS_router_ip')
                        translated_string="スタティック IP アドレスをシステムに割り当てる必要があります!";
                else if(o=='JS_wpa')
                	translated_string="追加ワイヤレス認証と暗号化オプションを選択しました。上書きしてもよろしいですか?";
                else if(o=='JS_GW_AP')
                	translated_string='WAN 接続タイプを "Automatic IP" [自動 IP] に設定した場合は、\nイーサネットポート経由でしか設定ページを開くことができません!';
        }
        else if(v=='TR')
        {
                if(o=='JS_ADSL')
                        translated_string ="Bir ADSL modemine bağlanmayı seçerseniz, bu iki alanı boş bırakamazsınız.";
                else if(o=='JS_router_ip')
                        translated_string="Bir statik IP adresini sisteme atamalısınız!";
                else if(o=='JS_wpa')
                	translated_string="İlave bir kablosuz doğrulama ve şifreleme seçeneği seçiminiz, gerçekten bunun üzerine yazmak istiyor musunuz?";
                else if(o=='JS_GW_AP')
                	translated_string='WAN Bağlantı türünü "Otomatik IP" olara ayarladığınızda\nyapılandırma sayfasına yalnızca Ethernet bağlantı noktasından girebilirsiniz!';
        }
        else
        {
                if(o=='JS_ADSL')
                        translated_string ="You can't leave these two fields blank, if you choose to connect to an ADSL modem.";
                else if(o=='JS_router_ip')
                        translated_string="You should assign a static IP address to system!";
                else if(o=='JS_wpa')
                	translated_string="You have chosen an additional wireless authenication and encryption option, do you really like to overwrite it?";
                else if(o=='JS_GW_AP')
                	translated_string='You only can enter configuration page through the Ethernet port\nwhen setting WAN Connection Type as "Automatic IP"!';
        }
        return translated_string;
}
