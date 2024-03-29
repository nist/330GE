/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */
	struct variable variables_Language[] = {
		{"preferred_lang", "", validate_choice, ARGV(
			"EN:English",
			"TW:Traditional Chinese",
			"CN:Simplified Chinese",
			"KR:Korean", 0), FALSE, 0x0}
	};

#ifndef NOUSB
      struct variable variables_IPConnection_PortMappingList[] = {

              {"", "7", validate_choice, ARGV(

                   "TCP",

                   "UDP",

              0), FALSE, 0x0},

		{"", "0", validate_ipaddr, NULL, FALSE, 0x0},

		{"", "0", validate_range, ARGV("0","65535"), FALSE, 0x0},

		{"", "16", validate_ipaddr, NULL, FALSE, 0x0},

		{"", "6", validate_range, ARGV("0","65535"), FALSE, 0x0},

		{"", "25", validate_string, ARGV("24"), FALSE, 0x0},

		{"", "6", validate_range, ARGV("0","4294927695"), FALSE, 0x0},

		{0,0,0,0,0}
      };
#endif

      struct variable variables_IPConnection_ExposedIPList[] = {          

#ifndef NOUSB
                {"", "16", validate_ipaddr, NULL, FALSE, 0x0},
                {"", "16", validate_ipaddr, NULL, FALSE, 0x0},
                {"", "20", validate_string, ARGV("20"), FALSE, 0x0},
#endif
      {0,0,0,0,0}
      };

      struct variable variables_IPConnection_VSList[] = {

		{"vts_port_x", "12", validate_portrange, NULL, FALSE, 0x0},

		{"vts_ipaddr_x", "16", validate_ipaddr, NULL, FALSE, 0x0},

		{"vts_lport_x", "7", validate_range, ARGV("0","4294927695"), FALSE, 0x0},

		{"vts_proto_x", "8", validate_choice, ARGV(

                   "TCP",
              
                   "UDP",
              
                   "BOTH",

                   "OTHER",

              0), FALSE, 0x0},
#ifndef NOUSB
		{"vts_protono_x", "12", validate_range, ARGV("0", "255", ""), FALSE , 0x0},
#endif
		{"vts_desc_x", "20", validate_string, ARGV("20"), FALSE, 0x0},

      {0,0,0,0,0}
      };

      struct variable variables_IPConnection_TriggerList[] = {          
      
                {"autofw_outport_x", "12", validate_portrange, NULL, FALSE, 0x0},
            	              
              {"autofw_outproto_x", "7", validate_choice, ARGV(                
              
                   "TCP",
              
                   "UDP",
              
              0), FALSE, 0x0},
           
                {"autofw_inport_x", "12", validate_portrange, NULL, FALSE, 0x0},
            	              
              {"autofw_inproto_x", "7", validate_choice, ARGV(                
              
                   "TCP",
              
                   "UDP",
              
              0), FALSE, 0x0},
                                                   
                    {"autofw_desc_x", "18", validate_string, ARGV("18"), FALSE, 0x0},                                                                                                                     
                                 
      {0,0,0,0,0}
      };      
        
      struct variable variables_PPPConnection_PPPoERouteList[] = {         
#ifndef NOUSB 
      	              
              {"", "7", validate_choice, ARGV(                
              
                   "0",
              
                   "1",
              
                   "2",
              
              0), FALSE, 0x0},
           	              
              {"", "16", validate_choice, ARGV(                
              
                   "Source",
              
                   "Destination",
              
              0), FALSE, 0x0},
           
                {"", "18", validate_ipaddr, NULL, FALSE, 0x0},
                         
             {"", "11", validate_range, ARGV("0", "32", "1"), FALSE , 0x0},                                          	                                        
#endif                        
      {0,0,0,0,0}
      };      
        
      struct variable variables_FirewallConfig_WLFilterList[] = {          
      
                {"filter_wl_srcip_x", "16", validate_ipaddr, NULL, FALSE, 0x0},
            
                {"filter_wl_srcport_x", "12", validate_portrange, NULL, FALSE, 0x0},
            
                {"filter_wl_dstip_x", "16", validate_ipaddr, NULL, FALSE, 0x0},
            
                {"filter_wl_dstport_x", "12", validate_portrange, NULL, FALSE, 0x0},
            	              
              {"filter_wl_proto_x", "8", validate_choice, ARGV(                
              
                   "TCP",
              
                   "TCP ALL",
              
                   "TCP SYN",
              
                   "TCP ACK",
              
                   "TCP FTN",
              
                   "TCP RST",
              
                   "TCP URG",
              
                   "TCP PSH",
              
                   "UDP",
              
              0), FALSE, 0x0},
                           
      {0,0,0,0,0}
      };      
        
      struct variable variables_FirewallConfig_LWFilterList[] = {          
      
                {"filter_lw_srcip_x", "16", validate_ipaddr, NULL, FALSE, 0x0},

#ifndef NOUSB            
                {"filter_lw_srcport_x", "12", validate_portrange, NULL, FALSE, 0x0},
            
                {"filter_lw_dstip_x", "16", validate_ipaddr, NULL, FALSE, 0x0},
#endif
            
                {"filter_lw_dstport_x", "12", validate_portrange, NULL, FALSE, 0x0},
            	              
              {"filter_lw_proto_x", "8", validate_choice, ARGV(                
              
                   "TCP",
              
                   "TCP ALL",
              
                   "TCP SYN",
              
                   "TCP ACK",
              
                   "TCP FTN",
              
                   "TCP RST",
              
                   "TCP URG",
              
                   "TCP PSH",
              
                   "UDP",
              
              0), FALSE, 0x0},

#ifdef NOUSB       
                    {"filter_lw_desc_x", "20", validate_string, ARGV("20"), FALSE, 0x0},                                                                                                                     
#endif
                                 
      {0,0,0,0,0}
      };      
        
      struct variable variables_FirewallConfig_UrlList[] = {          
                                              
                    {"url_keyword_x", "36", validate_string, ARGV("32"), FALSE, 0x0},                                                                                                                     
                                 
      {0,0,0,0,0}
      };      

#ifndef NOUSB        
      struct variable variables_FirewallConfig_WLocalFilterList[] = {          
      
                {"", "16", validate_ipaddr, NULL, FALSE, 0x0},
            
                {"", "12", validate_portrange, NULL, FALSE, 0x0},
            	              
              {"", "8", validate_choice, ARGV(                
              
                   "TCP",
              
                   "TCP ALL",
              
                   "TCP SYN",
              
                   "TCP ACK",
              
                   "TCP FTN",
              
                   "TCP RST",
              
                   "TCP URG",
              
                   "TCP PSH",
              
                   "UDP",
              
              0), FALSE, 0x0},
                           
      {0,0,0,0,0}
      };      
        
      struct variable variables_FirewallConfig_DWFilterList[] = {          
      
                {"", "16", validate_ipaddr, NULL, FALSE, 0x0},
            
                {"", "12", validate_portrange, NULL, FALSE, 0x0},
            
                {"", "16", validate_ipaddr, NULL, FALSE, 0x0},
            
                {"", "12", validate_portrange, NULL, FALSE, 0x0},
            	              
              {"", "8", validate_choice, ARGV(                
              
                   "TCP",
              
                   "TCP ALL",
              
                   "TCP SYN",
              
                   "TCP ACK",
              
                   "TCP FTN",
              
                   "TCP RST",
              
                   "TCP URG",
              
                   "TCP PSH",
              
                   "UDP",
              
              0), FALSE, 0x0},
                           
      {0,0,0,0,0}
      };      
        
      struct variable variables_FirewallConfig_WDFilterList[] = {          
      
                {"", "16", validate_ipaddr, NULL, FALSE, 0x0},
            
                {"", "12", validate_portrange, NULL, FALSE, 0x0},
            
                {"", "16", validate_ipaddr, NULL, FALSE, 0x0},
            
                {"", "12", validate_portrange, NULL, FALSE, 0x0},
            	              
              {"", "8", validate_choice, ARGV(                
              
                   "TCP",
              
                   "TCP ALL",
              
                   "TCP SYN",
              
                   "TCP ACK",
              
                   "TCP FTN",
              
                   "TCP RST",
              
                   "TCP URG",
              
                   "TCP PSH",
              
                   "UDP",
              
              0), FALSE, 0x0},
                           
      {0,0,0,0,0}
      };      
        
      struct variable variables_FirewallConfig_DLFilterList[] = {          
      
                {"", "16", validate_ipaddr, NULL, FALSE, 0x0},
            
                {"", "12", validate_portrange, NULL, FALSE, 0x0},
            
                {"", "16", validate_ipaddr, NULL, FALSE, 0x0},
            
                {"", "12", validate_portrange, NULL, FALSE, 0x0},
            	              
              {"", "8", validate_choice, ARGV(                
              
                   "TCP",
              
                   "TCP ALL",
              
                   "TCP SYN",
              
                   "TCP ACK",
              
                   "TCP FTN",
              
                   "TCP RST",
              
                   "TCP URG",
              
                   "TCP PSH",
              
                   "UDP",
              
              0), FALSE, 0x0},
                           
      {0,0,0,0,0}
      };      
        
      struct variable variables_FirewallConfig_LDFilterList[] = {          
      
                {"", "16", validate_ipaddr, NULL, FALSE, 0x0},
            
                {"", "12", validate_portrange, NULL, FALSE, 0x0},
            
                {"", "16", validate_ipaddr, NULL, FALSE, 0x0},
            
                {"", "12", validate_portrange, NULL, FALSE, 0x0},
            	              
              {"", "8", validate_choice, ARGV(                
              
                   "TCP",
              
                   "TCP ALL",
              
                   "TCP SYN",
              
                   "TCP ACK",
              
                   "TCP FTN",
              
                   "TCP RST",
              
                   "TCP URG",
              
                   "TCP PSH",
              
                   "UDP",
              
              0), FALSE, 0x0},
                           
      {0,0,0,0,0}
      };      
#endif
        
      struct variable variables_FirewallConfig_MFList[] = {          
      
                {"macfilter_list_x", "32", validate_hwaddr, NULL, FALSE, 0x0},
                            
      {0,0,0,0,0}
      };      
        
      struct variable variables_RouterConfig_GWStatic[] = {          
      
                {"sr_ipaddr_x", "17", validate_ipaddr, NULL, FALSE, 0x0},
            
                {"sr_netmask_x", "17", validate_ipaddr, NULL, FALSE, 0x0},
            
                {"sr_gateway_x", "17", validate_ipaddr, NULL, FALSE, 0x0},
            
                {"sr_matric_x", "5", validate_ipaddr, NULL, FALSE, 0x0},
            	              
              {"sr_if_x", "3", validate_choice, ARGV(                
              
                   "LAN",
              
                   "WAN",
              
              0), FALSE, 0x0},
                           
      {0,0,0,0,0}
      };      
      
#ifndef NOUSB  
      struct variable variables_RouterConfig_StaticRoute[] = {          
      
                {"dr_staticipaddr_x", "20", validate_ipaddr, NULL, FALSE, 0x0},
                         
             {"dr_staticnetmask_x", "11", validate_range, ARGV("0", "32", "1"), FALSE , 0x0},                                          	                                                                      
        
                {"dr_staticgateway_x", "18", validate_ipaddr, NULL, FALSE, 0x0},
                            
      {0,0,0,0,0}
      }; 
     
      struct variable variables_RouterConfig_RipSRoute[] = {          
      
                {"", "17", validate_ipaddr, NULL, FALSE, 0x0},
                         
             {"", "7", validate_range, ARGV("0", "32", "1"), FALSE , 0x0},                                          	                                                                      
                        
      {0,0,0,0,0}
      };      
        
      struct variable variables_RouterConfig_RouteFilter[] = {          
      	              
              {"", "9", validate_choice, ARGV(                
              
                   "0",
              
                   "1",
              
                   "2",
              
                   "3",
              
              0), FALSE, 0x0},
           
                {"", "17", validate_ipaddr, NULL, FALSE, 0x0},
                         
             {"", "8", validate_range, ARGV("0", "32", "1"), FALSE , 0x0},                                          	                                                                      
                        
      {0,0,0,0,0}
      };      
        
      struct variable variables_RouterConfig_RipOffset[] = {          
      	              
              {"", "7", validate_choice, ARGV(                
              
                   "IN",
              
                   "OUT",
              
              0), FALSE, 0x0},
           
                {"", "17", validate_ipaddr, NULL, FALSE, 0x0},
                         
             {"", "6", validate_range, ARGV("0", "32", "1"), FALSE , 0x0},                                          	                                                                      
                        
      {0,0,0,0,0}
      };      
#endif
        
      struct variable variables_LANHostConfig_ManualDHCPList[] = {          
      
                {"dhcp_staticmac_x", "14", validate_hwaddr, NULL, FALSE, 0x0},
            
                {"dhcp_staticip_x", "15", validate_ipaddr, NULL, FALSE, 0x0},
                            
      {0,0,0,0,0}
      };      
       
#ifndef NOUSB 
      struct variable variables_LANHostConfig_DmzManualDHCPList[] = {          
      
                {"", "14", validate_hwaddr, NULL, FALSE, 0x0},
            
                {"", "15", validate_ipaddr, NULL, FALSE, 0x0},
                            
      {0,0,0,0,0}
      };      
        
      struct variable variables_LANHostConfig_ReservedAddressList[] = {          
      
                {"", "24", validate_ipaddr, NULL, FALSE, 0x0},
                            
      {0,0,0,0,0}
      };      
#endif

      struct variable variables_DeviceSecurity11a_ACLList[] = {          
      
                {"wl_maclist_x", "32", validate_hwaddr, NULL, FALSE, 0x0},
                            
      {0,0,0,0,0}
      };      
        
#ifndef NOUSB        
      struct variable variables_DeviceSecurity11a_AESList[] = {          
      
                {"", "16", validate_hwaddr, NULL, FALSE, 0x0},
            	              
              {"", "10", validate_choice, ARGV(                
              
                   "64bits",
              
                   "128bits",
              
                   "152bits",
              
              0), FALSE, 0x0},
           
                {"", "32", validate_wlkey, NULL, FALSE, 0x0},
                            
      {0,0,0,0,0}
      };      
        
      struct variable variables_WLANAuthentication11a_LocalAuthDB[] = {          
                                              
                    {"", "18", validate_string, ARGV("16"), FALSE, 0x0},                                                                                                                     
                                                         
                    {"", "18", validate_string, ARGV("16"), FALSE, 0x0},                                                                                                                     
                                 
      {0,0,0,0,0}
      };      
        
      struct variable variables_WLANAuthentication11a_LocalCertDB[] = {          
      	              
              {"", "8", validate_choice, ARGV(                
              
                   "Server",
              
                   "Client",
              
              0), FALSE, 0x0},
           
                {"", "", validate_string, ARGV("32"), FALSE, 0x0}, 
                            
      {0,0,0,0,0}
      };      
#endif
        
      struct variable variables_WLANConfig11b_RBRList[] = {          
      
                {"wl_wdslist_x", "32", validate_hwaddr, NULL, FALSE, 0x0},
                            
      {0,0,0,0,0}
      };

	struct variable variables_WLANConfig11b_MSSIDList[] = {
		{"mssid_index_x", "1", validate_range, ARGV("0", "3"), FALSE, FALSE},
                {"mssid_ssid_x", "32", validate_string, ARGV("32"), FALSE, FALSE},
                {"mssid_ssid_2_x", "32", validate_string, ARGV("32"), FALSE, FALSE},
                {"mssid_vlanid_x", "4", validate_range, ARGV("1", "4095"), FALSE, FALSE},
		{"mssid_auth_mode_x", "6", validate_choice, ARGV(	"open:Open System or Shared Key",
									"shared:Shared Key",
									"psk:WPA-PSK", 0), FALSE, FALSE},
		{"mssid_crypto_x", "9", validate_choice, ARGV(	"tkip:TKIP",
									"aes:AES",
									"tkip+aes:TKIP+AES", 0), FALSE, FALSE},
		{"mssid_wpa_psk_x", "9", validate_string, ARGV("64"), FALSE, FALSE}, 
		{"mssid_wep_x", "2", validate_choice, ARGV(	"0:None",
								"1:WEP-64bits",
								"2:WEP-128bits", 0), FALSE, FALSE},
		{"mssid_phrase_x", "9", validate_string, ARGV("64"), FALSE, FALSE},
		{"mssid_key1_x", "10", validate_wlkey, NULL, FALSE, FALSE},
		{"mssid_key2_x", "10", validate_wlkey, NULL, FALSE, FALSE},
		{"mssid_key3_x", "10", validate_wlkey, NULL, FALSE, FALSE},
		{"mssid_key4_x", "10", validate_wlkey, NULL, FALSE, FALSE},
		{"mssid_key_x", "2", validate_choice, ARGV(	"1:Key1",
								"2:Key2",
								"3:Key3",
								"4:Key4", 0), FALSE, FALSE},
		{"mssid_wpa_gtk_rekey_x", "2", validate_range, ARGV("0", "86400", ""), FALSE, FALSE},
		{"mssid_enable_x", "", validate_range, ARGV("0", "1"), FALSE, FALSE},
		{0,0,0,0,0}
	};

      struct variable variables_DeviceSecurity11b_ACLList[] = {          
      
                {"wl_maclist_x", "32", validate_hwaddr, NULL, FALSE, 0x0},
                            
      {0,0,0,0,0}
      };      

#ifndef NOUSB        
      struct variable variables_DeviceSecurity11b_AESList[] = {          
      
                {"", "16", validate_hwaddr, NULL, FALSE, 0x0},
            	              
              {"", "10", validate_choice, ARGV(                
              
                   "64bits",
              
                   "128bits",
              
                   "152bits",
              
              0), FALSE, 0x0},
           
                {"", "32", validate_wlkey, NULL, FALSE, 0x0},
                            
      {0,0,0,0,0}
      };      
        
      struct variable variables_WLANAuthentication11b_LocalAuthDB[] = {          
                                              
                    {"", "18", validate_string, ARGV("16"), FALSE, 0x0},                                                                                                                     
                                                         
                    {"", "18", validate_string, ARGV("16"), FALSE, 0x0},                                                                                                                     
                                 
      {0,0,0,0,0}
      };      
        
      struct variable variables_WLANAuthentication11b_LocalCertDB[] = {          
      	              
              {"", "8", validate_choice, ARGV(                
              
                   "Server",
              
                   "Client",
              
              0), FALSE, 0x0},
           
                {"", "", validate_string, ARGV("32"), FALSE, 0x0}, 
                            
      {0,0,0,0,0}
      };      
#endif

#ifndef NOUSB        
      struct variable variables_PrinterStatus_x_FUserList[] = {          
                                              
                    {"usb_ftpusername_x", "18", validate_string, ARGV("16"), FALSE, 0x0},                                                                                                                     
                                                         
                    {"usb_ftppasswd_x", "18", validate_string, ARGV("16"), FALSE, 0x0},                                                                                                                     
                              
             {"usb_ftpmaxuser_x", "7", validate_range, ARGV("0", "12", ""), FALSE , 0x0},                                          	                                                                      
        	            
              {"usb_ftprights_x", "16", validate_choice, ARGV(                
              
                   "Read/Write/Erase",
              
                   "Read/Write",
              
                   "Read Only",
              
                   "View Only",
              
                   "Private",
              
                   "Private(WO)",
              
              0), FALSE, 0x0},
                           
      {0,0,0,0,0}
      };      
        
      struct variable variables_PrinterStatus_x_FBanIPList[] = {          
      
                {"usb_ftpbanip_x", "24", validate_ipaddr, NULL, FALSE, 0x0},
                            
      {0,0,0,0,0}
      };      
#endif

#ifndef NOQOS
      struct variable variables_PrinterStatus_x_USRRuleList[] = {
                                                                                                                                               
                       {"qos_service_name_x", "16", validate_string, ARGV("32"), FALSE, 0x0},

                       {"qos_ip_x", "16", validate_ipaddr, NULL, FALSE, 0x0},

                       {"qos_port_x", "12", validate_range, ARGV("0","65535"), FALSE, 0x0},

                       {"qos_prio_x", "3", validate_choice, ARGV(
                        "1",
                        "2",
                        "3",
                        "4",
                        "5",
                        "6",
                        "7",
                        "8",
                        0), FALSE, 0x0},
      {0,0,0,0,0}
      };
#endif
      
/*
 * actions_$ServiceId[] = { $ActionName, $InArgu, $OutArgu, Callback}
 * Used in GetVarRequest
 *  	1. Find serviceId from svcLinks
 *	2. Get value from varaibles$ServiceId by means of nvram_get_x
 * 
 * Used in ActionRequest
 *      1. Find serviceId from svcLinks
 *	2. If $callback is not null, excute it
 * 	   else goto 3
 *	3. Check validate from $Validate_Func of variables_$ServiceId, if not validate return error
 *      4. Set value by means of nvram_set_x or nvram_add_list_x or nvram_del_list_x
 *      5. return
 *
 * variables_$ServiceId[] = { $Name, $Validate_Func, $Argu, $NullOk, $Event}
 * For writable variable
 *     $Validate_Func : check if the variable is ok
 *     $Argu : arguments list pass into $Validate_Func
 *     $NullOk: if variable is allowed as NULL
 *     $Event: if event is sent once variable is changed
 * For read only variable
 *     $Validate_Func == NULL
 *     $Argu : file and field to get value
 *     $NullOk: if variable is allowed as NULL
 *     $Event: if event is sent once variable is changed
 */
/* 
 * Variables are set in order (put dependent variables later). Set
 * nullok to TRUE to ignore zero-length values of the variable itself.
 * For more complicated validation that cannot be done in one pass or
 * depends on additional form components or can throw an error in a
 * unique painful way, write your own validation routine and assign it
 * to a hidden variable (e.g. filter_ip).
 *
 * This data structure is generated automatically by script, maybe not 
 * easy to read. But it really work!!!
*/

      
      
     
      struct variable variables_General[] = {
                                              
                    {"http_username", "", validate_string, ARGV("32"), FALSE, FALSE},                                                                                                                   
                                                         
                    {"http_passwd", "", validate_string, ARGV("32"), FALSE, FALSE},                                                                                                                   
                 
      {"x_SystemUpTime", "Status", NULL, ARGV("general.log","UpTime"), FALSE, FALSE},

      {"x_ProductID", "Status", NULL, ARGV("general.log","ProductID"), FALSE, FALSE},

      {"x_FirmwareVersion", "Status", NULL, ARGV("general.log","FirmwareVer"), FALSE, FALSE},

      {"x_HardwareVersion", "Status", NULL, ARGV("general.log","HardwareVer"), FALSE, FALSE},

      {"x_BootloaderVersion", "Status", NULL, ARGV("general.log","BootloaderVer"), FALSE, FALSE},
                  
      { 0, 0, 0, 0, 0, 0}
      };
   
      struct variable variables_Layer3Forwarding[] = {
                                              
                    {"", "", validate_string, ARGV(""), FALSE, FALSE},                                                                                                                   
                 	
              {"wan_proto", "", validate_choice, ARGV(              
              
                   "dhcp:Automatic IP",
              
                   "static:Static IP",
              
                   "pppoe:PPPoE",
              
                   "pptp:PPTP",
              
                   "bigpond:BigPond",
              
              0), FALSE, FALSE},
           	
              {"wan_mode_x", "", validate_choice, ARGV(              
              
                   "0:Disabled",
              
                   "1:Enabled",
              
                   "2:Auto",
              
              0), FALSE, FALSE},
           	
              {"wan_etherspeed_x", "", validate_choice, ARGV(              
              
                   "auto:Auto negotiation",
              
                   "10half:10Mbps half-duplex",
              
                   "10full:10Mbps full-duplex",
              
                   "100half:100Mpbs half-duplex",
              
                   "100full:100Mpbs full-duplex",
              
              0), FALSE, FALSE},
                             
      { 0, 0, 0, 0, 0, 0}
      };
   
      struct variable variables_WANCommonInterface[] = {
#ifndef NOUSB
      	
              {"", "", validate_choice, ARGV(              
              
                   "Ethernet",
              
              0), FALSE, FALSE},
                                  
                       {"", "", validate_range, ARGV("0","4294927695"), FALSE, FALSE},                                                             
                                           
                       {"", "", validate_range, ARGV("0","4294927695"), FALSE, FALSE},                                                             
                    	
              {"", "", validate_choice, ARGV(              
              
                   "Up",
              
                   "Down",
              
              0), FALSE, FALSE},
                                                   
                    {"", "", validate_string, ARGV(""), FALSE, FALSE},                                                                                                                   
                              
             {"", "", validate_range, ARGV("1", "", "1"), FALSE, FALSE},
                                 
                       {"", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
                                                            
                    {"", "", validate_string, ARGV(""), FALSE, FALSE},                                                                                                                   
                                                         
                    {"", "", validate_string, ARGV(""), FALSE, FALSE},                                                                                                                   
                                        
                       {"", "", validate_range, ARGV("0","4294927695"), FALSE, FALSE},                                                             
                                           
                       {"", "", validate_range, ARGV("0","4294927695"), FALSE, FALSE},                                                             
                                           
                       {"", "", validate_range, ARGV("0","4294927695"), FALSE, FALSE},                                                             
                                           
                       {"", "", validate_range, ARGV("0","4294927695"), FALSE, FALSE},                                                             
                                     
                 {"", "", validate_range, ARGV("0","1"), FALSE, FALSE},                                                       
#endif
                                
      { 0, 0, 0, 0, 0, 0}
      };
   
      struct variable variables_IPConnection[] = {
      	
              {"wan_route_x", "", validate_choice, ARGV(              
              
                   "Unconfigured",
              
                   "IP_Routed",
              
                   "IP_Bridged",
              
              0), FALSE, FALSE},
           	
              {"", "", validate_choice, ARGV(              
              
                   "Unconfigured",
              
                   "IP_Routed",
              
                   "IP_Bridged",
              
              0), FALSE, FALSE},
           
      {"ConnectionStatus", "Status", NULL, ARGV("wan.log","IPLink"), FALSE, FALSE},

      {"Uptime", "Status", NULL, ARGV("wan.log","Uptime"), FALSE, FALSE},
	
              {"", "", validate_choice, ARGV(              
              
                   "ERROR_NONE",
              
              0), FALSE, FALSE},
                            
                 {"wan_nat_x", "", validate_range, ARGV("0","1"), FALSE, FALSE},                                                       
                               
                 {"", "", validate_range, ARGV("0","1"), FALSE, FALSE},                                                       
              
                {"wan_ipaddr", "", validate_ipaddr, NULL, FALSE, FALSE},
            
                {"wan_netmask", "", validate_ipaddr, NULL, FALSE, FALSE},
            
                {"wan_gateway", "", validate_ipaddr, NULL, FALSE, FALSE},
                             
                 {"wan_dnsenable_x", "", validate_range, ARGV("0","1"), FALSE, FALSE},                                                       
              
                {"wan_dns1_x", "", validate_ipaddr, NULL, FALSE, FALSE},
            
                {"wan_dns2_x", "", validate_ipaddr, NULL, FALSE, FALSE},
            
      {"IPTablesInfo", "Status", NULL, ARGV("iptable.log",""), FALSE, FALSE},
                         
                       {"", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
                                     
                 {"", "", validate_range, ARGV("0","1"), FALSE, FALSE},                                                       
                               
                 {"wan_nat_x", "", validate_range, ARGV("0","1"), FALSE, FALSE},                                                       
              
                {"dmz_ip", "", validate_ipaddr, NULL, FALSE, FALSE},
                             
                 {"sp_battle_ips", "", validate_range, ARGV("0","1"), FALSE, FALSE},                                                       
                                       
                       {"", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
                                     
                 {"vts_enable_x", "", validate_range, ARGV("0","1"), FALSE, FALSE},                                                       
                                       
                       {"vts_num_x", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
                                     
                 {"autofw_enable_x", "", validate_range, ARGV("0","1"), FALSE, FALSE},                                                       
                                       
                       {"autofw_num_x", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
#ifndef NOUSB                    
                {"", "", validate_ipaddr, NULL, FALSE, FALSE},
            
                {"", "", validate_portrange, NULL, FALSE, FALSE},
                                                    
                    {"", "", validate_string, ARGV("24"), FALSE, FALSE},                                                                                                                   
                                                         
                    {"", "", validate_string, ARGV(""), FALSE, FALSE},                                                                                                                   
                 
                {"", "", validate_ipaddr, NULL, FALSE, FALSE},
            
                {"", "", validate_portrange, NULL, FALSE, FALSE},
                                                    
                    {"", "", validate_string, ARGV("24"), FALSE, FALSE},                                                                                                                   
                                                         
                    {"", "", validate_string, ARGV(""), FALSE, FALSE},                                                                                                                   
                 
                {"", "", validate_ipaddr, NULL, FALSE, FALSE},
            
                {"", "", validate_portrange, NULL, FALSE, FALSE},
                                                    
                    {"", "", validate_string, ARGV("24"), FALSE, FALSE},                                                                                                                   
                                                         
                    {"", "", validate_string, ARGV(""), FALSE, FALSE},                                                                                                                   
                 
                {"", "", validate_ipaddr, NULL, FALSE, FALSE},
            
                {"", "", validate_portrange, NULL, FALSE, FALSE},
                                                    
                    {"", "", validate_string, ARGV("24"), FALSE, FALSE},                                                                                                                   
                                                         
                    {"", "", validate_string, ARGV(""), FALSE, FALSE},                                                                                                                   
                 
                {"", "", validate_ipaddr, NULL, FALSE, FALSE},
            
                {"", "", validate_portrange, NULL, FALSE, FALSE},
                                                    
                    {"", "", validate_string, ARGV("24"), FALSE, FALSE},                                                                                                                   
                                                         
                    {"", "", validate_string, ARGV(""), FALSE, FALSE},                                                                                                                   
                 
                {"", "", validate_ipaddr, NULL, FALSE, FALSE},
            
                {"", "", validate_portrange, NULL, FALSE, FALSE},
                                                    
                    {"", "", validate_string, ARGV("24"), FALSE, FALSE},                                                                                                                   
                                                         
                    {"", "", validate_string, ARGV(""), FALSE, FALSE},                                                                                                                   
                  
      {"PortMappingList", "Group", validate_group, ARGV(variables_IPConnection_PortMappingList, "16", "58", "PortMappingNumberOfEntries"), FALSE, FALSE},     
#endif
       
      {"ExposedIPList", "Group", validate_group, ARGV(variables_IPConnection_ExposedIPList, "32", "52", "ExposedIPCount"), FALSE, FALSE},     
       
      {"VSList", "Group", validate_group, ARGV(variables_IPConnection_VSList, "24", "75", "vts_num_x"), FALSE, FALSE},     
       
      {"TriggerList", "Group", validate_group, ARGV(variables_IPConnection_TriggerList, "10", "56", "autofw_num_x"), FALSE, FALSE},     
                        
      { 0, 0, 0, 0, 0, 0}
      };
   
      struct variable variables_PPPConnection[] = {
      
#ifndef NOUSB	
              {"", "", validate_choice, ARGV(              
              
                   "Unconfigured",
              
                   "IP_Routed",
              
              0), FALSE, FALSE},
           	
              {"", "", validate_choice, ARGV(              
              
                   "IP_Routed",
              
              0), FALSE, FALSE},
#endif
                                                   
                    {"wan_pppoe_username", "", validate_string, ARGV("64"), FALSE, FALSE},                                                                                                                   
                 
                {"wan_pppoe_passwd", "", validate_string, ARGV("64"), FALSE, FALSE}, 
            
      {"ConnectionStatus", "Status", NULL, ARGV("wan.log","WANLink"), FALSE, FALSE},

#ifndef NOUSB
                       
                       {"", "", validate_range, ARGV("0","4294927695"), FALSE, FALSE},                                                             
                                           
                       {"", "", validate_range, ARGV("0","4294927695"), FALSE, FALSE},                                                             
                                           
                       {"", "", validate_range, ARGV("0","4294927695"), FALSE, FALSE},                                                             
                    	
              {"", "", validate_choice, ARGV(              
              
                   "ERROR_NONE",
              
              0), FALSE, FALSE},
                                  
                       {"", "", validate_range, ARGV("0","4294927695"), FALSE, FALSE},                                                             
                                           
#endif
                       {"wan_pppoe_idletime", "", validate_range, ARGV("0","4294927695"), FALSE, FALSE},                                                             
                    
                {"wan_pppoe_txonly_x", "", validate_string, ARGV(""), FALSE, FALSE}, 
                         
             {"wan_pppoe_mtu", "", validate_range, ARGV("576", "1492", ""), FALSE, FALSE},
                     
             {"wan_pppoe_mru", "", validate_range, ARGV("576", "1492", ""), FALSE, FALSE},
                               
                       {"", "", validate_range, ARGV("0","4294927695"), FALSE, FALSE},                                                             
                                                            
                    {"wan_pppoe_service", "", validate_string, ARGV("32"), FALSE, FALSE},                                                                                                                   
                                                         
                    {"wan_pppoe_ac", "", validate_string, ARGV("32"), FALSE, FALSE},                                                                                                                   
                                  
                 {"wan_pppoe_relay_x", "", validate_range, ARGV("0","1"), FALSE, FALSE},                                                       
                                                      
                    {"wan_hostname", "", validate_string, ARGV("32"), FALSE, FALSE},                                                                                                                   
                 
                {"wan_hwaddr_x", "", validate_hwaddr, NULL, FALSE, FALSE},
                                                    
                    {"wan_heartbeat_x", "", validate_string, ARGV(""), FALSE, FALSE},                                                                                                                   
#ifndef NOUSB
                 
                {"", "", validate_ipaddr, NULL, FALSE, FALSE},
            
                {"", "", validate_ipaddr, NULL, FALSE, FALSE},
            
                {"", "", validate_ipaddr, NULL, FALSE, FALSE},
#endif
            
      {"x_WANType", "Status", NULL, ARGV("wan.log","wan_proto_t"), FALSE, FALSE},

      {"x_WANIPAddress", "Status", NULL, ARGV("wan.log","wan_ipaddr_t"), FALSE, FALSE},

      {"x_WANSubnetMask", "Status", NULL, ARGV("wan.log","wan_netmask_t"), FALSE, FALSE},

      {"x_WANGateway", "Status", NULL, ARGV("wan.log","wan_gateway_t"), FALSE, FALSE},

      {"x_WANDNSServer", "Status", NULL, ARGV("wan.log","wan_dns_t"), FALSE, FALSE},

      {"x_WANLink", "Status", NULL, ARGV("wan.log","wan_status_t"), FALSE, FALSE},
                                        
                    {"", "", validate_string, ARGV(""), FALSE, FALSE},                                                                                                                   
                 
      {"x_DDNSStatus", "Status", NULL, ARGV("ddns.log","DDNSStatus"), FALSE, FALSE},

#ifndef NOUSB
                         
                       {"", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
                    	
              {"", "", validate_choice, ARGV(              
              
                   "0:PPPoE Session 0",
              
                   "1:PPPoE Session 1",
              
                   "2:PPPoE Session 2",
              
              0), FALSE, FALSE},
                      
                 {"", "", validate_range, ARGV("0","1"), FALSE, FALSE},                                                       
                                                      
                    {"", "", validate_string, ARGV("64"), FALSE, FALSE},                                                                                                                   
                 
                {"", "", validate_string, ARGV("64"), FALSE, FALSE}, 
                                   
                       {"", "", validate_range, ARGV("0","4294927695"), FALSE, FALSE},                                                             
                    
                {"", "", validate_string, ARGV(""), FALSE, FALSE}, 
                         
             {"", "", validate_range, ARGV("576", "1492", ""), FALSE, FALSE},
                     
             {"", "", validate_range, ARGV("576", "1492", ""), FALSE, FALSE},
                                                
                    {"", "", validate_string, ARGV("32"), FALSE, FALSE},                                                                                                                   
                                                         
                    {"", "", validate_string, ARGV("32"), FALSE, FALSE},                                                                                                                   
                                  
                 {"", "", validate_range, ARGV("0","1"), FALSE, FALSE},                                                       
                                                      
                    {"", "", validate_string, ARGV("64"), FALSE, FALSE},                                                                                                                   
                 
                {"", "", validate_string, ARGV("64"), FALSE, FALSE}, 
                                   
                       {"", "", validate_range, ARGV("0","4294927695"), FALSE, FALSE},                                                             
                    
                {"", "", validate_string, ARGV(""), FALSE, FALSE}, 
                         
             {"", "", validate_range, ARGV("576", "1492", ""), FALSE, FALSE},
                     
             {"", "", validate_range, ARGV("576", "1492", ""), FALSE, FALSE},
                                                
                    {"", "", validate_string, ARGV("32"), FALSE, FALSE},                                                                                                                   
                                                         
                    {"", "", validate_string, ARGV("32"), FALSE, FALSE},                                                                                                                   
                                          
                       {"", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
                     
      {"PPPoERouteList", "Group", validate_group, ARGV(variables_PPPConnection_PPPoERouteList, "4", "50", "PPPoERouteCount"), FALSE, FALSE},     
#endif
                        
      { 0, 0, 0, 0, 0, 0}
      };
   
      struct variable variables_EthernetLink[] = {
#ifndef NOUSB      	
              {"", "", validate_choice, ARGV(              
              
                   "Up",
              
                   "Down",
              
              0), FALSE, FALSE},
#endif
                             
      { 0, 0, 0, 0, 0, 0}
      };
   
      struct variable variables_FirewallConfig[] = {
#ifndef NOUSB
                               
                       {"", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
                                             
                       {"", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
                                             
                       {"", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
             
#endif                        
                 {"fw_enable_x", "", validate_range, ARGV("0","1"), FALSE, FALSE},                                                       
              	
              {"fw_log_x", "", validate_choice, ARGV(              
              
                   "none:None",
              
                   "drop:Dropped",
              
                   "accept:Accepted",
              
                   "both:Both",
              
              0), FALSE, FALSE},
                            
                 {"misc_natlog_x", "", validate_range, ARGV("0","1"), FALSE, FALSE},                                                       
                               
                 {"misc_http_x", "", validate_range, ARGV("0","1"), FALSE, FALSE},                                                       
                           
             {"misc_httpport_x", "", validate_range, ARGV("1024", "65535", ""), FALSE, FALSE},
                         
                 {"misc_lpr_x", "", validate_range, ARGV("0","1"), FALSE, FALSE},                                                       
                               
                 {"misc_ping_x", "", validate_range, ARGV("0","1"), FALSE, FALSE},                                                       
                               
                 {"fw_wl_enable_x", "", validate_range, ARGV("0","1"), FALSE, FALSE},                                                       
              
                {"filter_wl_date_x", "", validate_portrange, NULL, FALSE, FALSE},
            
                {"filter_wl_time_x", "", validate_portrange, NULL, FALSE, FALSE},
            	
              {"filter_wl_default_x", "", validate_choice, ARGV(              
              
                   "DROP",
              
                   "ACCEPT",
              
              0), FALSE, FALSE},
           
                {"filter_wl_icmp_x", "", validate_portrange, NULL, FALSE, FALSE},
            
                {"", "", validate_portrange, NULL, FALSE, FALSE},
                             
                 {"fw_lw_enable_x", "", validate_range, ARGV("0","1"), FALSE, FALSE},                                                       
              
                {"filter_lw_date_x", "", validate_portrange, NULL, FALSE, FALSE},
            
                {"filter_lw_time_x", "", validate_portrange, NULL, FALSE, FALSE},
            	
              {"filter_lw_default_x", "", validate_choice, ARGV(              
              
                   "DROP",
              
                   "ACCEPT",
              
              0), FALSE, FALSE},
           
                {"filter_lw_icmp_x", "", validate_portrange, NULL, FALSE, FALSE},
            
      {"FirewallLog", "Status", NULL, ARGV("firewall.log",""), FALSE, FALSE},

      {"SystemLog", "Status", NULL, ARGV("syslog.log",""), FALSE, FALSE},

      {"SystemCmd", "Status", NULL, ARGV("syscmd.log",""), FALSE, FALSE},
                 
                 {"url_enable_x", "", validate_range, ARGV("0","1"), FALSE, FALSE},                                                       
              
                {"url_date_x", "", validate_portrange, NULL, FALSE, FALSE},
            
                {"url_time_x", "", validate_portrange, NULL, FALSE, FALSE},
            	
              {"", "", validate_choice, ARGV(              
              
                   "DROP",
              
                   "ACCEPT",
              
              0), FALSE, FALSE},
                                    
                       {"url_num_x", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
                                             
                       {"filter_wl_num_x", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
                                             
                       {"filter_lw_num_x", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
#ifndef NOUSB
                                     
                 {"", "", validate_range, ARGV("0","1"), FALSE, FALSE},                                                       
              
                {"", "", validate_portrange, NULL, FALSE, FALSE},
            
                {"", "", validate_portrange, NULL, FALSE, FALSE},
            	
              {"", "", validate_choice, ARGV(              
              
                   "DROP",
              
                   "ACCEPT",
              
              0), FALSE, FALSE},
           
                {"", "", validate_portrange, NULL, FALSE, FALSE},
            	
              {"", "", validate_choice, ARGV(              
              
                   "None",
              
                   "Dropped",
              
                   "Accepted",
              
                   "Both",
              
              0), FALSE, FALSE},
                                    
                       {"", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
                                             
                       {"", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
                                     
                 {"", "", validate_range, ARGV("0","1"), FALSE, FALSE},                                                       
              	
              {"", "", validate_choice, ARGV(              
              
                   "Both",
              
                   "802.11a only",
              
                   "802.11g only",
              
              0), FALSE, FALSE},
           
                {"", "", validate_ipaddr, NULL, FALSE, FALSE},
            
                {"", "", validate_ipaddr, NULL, FALSE, FALSE},
                             
                 {"", "", validate_range, ARGV("0","1"), FALSE, FALSE},                                                       
              	
              {"", "", validate_choice, ARGV(              
              
                   "DROP",
              
                   "ACCEPT",
              
              0), FALSE, FALSE},
           
                {"", "", validate_portrange, NULL, FALSE, FALSE},
            	
              {"", "", validate_choice, ARGV(              
              
                   "DROP",
              
                   "ACCEPT",
              
              0), FALSE, FALSE},
           
                {"", "", validate_portrange, NULL, FALSE, FALSE},
            	
              {"", "", validate_choice, ARGV(              
              
                   "None",
              
                   "Dropped",
              
                   "Accepted",
              
                   "Both",
              
              0), FALSE, FALSE},
                                    
                       {"", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
                                             
                       {"", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
                                     
                 {"", "", validate_range, ARGV("0","1"), FALSE, FALSE},                                                       
              	
              {"", "", validate_choice, ARGV(              
              
                   "DROP",
              
                   "ACCEPT",
              
              0), FALSE, FALSE},
           
                {"", "", validate_portrange, NULL, FALSE, FALSE},
            	
              {"", "", validate_choice, ARGV(              
              
                   "DROP",
              
                   "ACCEPT",
              
              0), FALSE, FALSE},
           
                {"", "", validate_portrange, NULL, FALSE, FALSE},
            	
              {"", "", validate_choice, ARGV(              
              
                   "None",
              
                   "Dropped",
              
                   "Accepted",
              
                   "Both",
              
              0), FALSE, FALSE},
                                    
                       {"", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
                                             
                       {"", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
                                             
                       {"", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
#endif
                    	
              {"macfilter_enable_x", "", validate_choice, ARGV(              
              
                   "0:Disable",
              
                   "1:Accept",
              
                   "2:Reject",
              
              0), FALSE, FALSE},
                                    
                       {"macfilter_num_x", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
                     
      {"WLFilterList", "Group", validate_group, ARGV(variables_FirewallConfig_WLFilterList, "32", "63", "filter_wl_num_x"), FALSE, FALSE},     
       
      {"LWFilterList", "Group", validate_group, ARGV(variables_FirewallConfig_LWFilterList, "32", "63", "filter_lw_num_x"), FALSE, FALSE},     
       
      {"UrlList", "Group", validate_group, ARGV(variables_FirewallConfig_UrlList, "128", "36", "url_num_x"), FALSE, FALSE},     

#ifndef NOUSB
       
      {"WLocalFilterList", "Group", validate_group, ARGV(variables_FirewallConfig_WLocalFilterList, "32", "36", "WanLocalRuleCount"), FALSE, FALSE},     
       
      {"DWFilterList", "Group", validate_group, ARGV(variables_FirewallConfig_DWFilterList, "32", "63", "DmzWanRuleCount"), FALSE, FALSE},     
       
      {"WDFilterList", "Group", validate_group, ARGV(variables_FirewallConfig_WDFilterList, "32", "63", "WanDmzRuleCount"), FALSE, FALSE},     
       
      {"DLFilterList", "Group", validate_group, ARGV(variables_FirewallConfig_DLFilterList, "32", "63", "DmzLanRuleCount"), FALSE, FALSE},     
       
      {"LDFilterList", "Group", validate_group, ARGV(variables_FirewallConfig_LDFilterList, "32", "63", "LanDmzRuleCount"), FALSE, FALSE},     
#endif
       
      {"MFList", "Group", validate_group, ARGV(variables_FirewallConfig_MFList, "16", "32", "macfilter_num_x"), FALSE, FALSE},     
                        
      { 0, 0, 0, 0, 0, 0}
      };
   
      struct variable variables_RouterConfig[] = {
                       
                 {"sr_enable_x", "", validate_range, ARGV("0","1"), FALSE, FALSE},                                                       
              	
              {"sr_rip_x", "", validate_choice, ARGV(              
              
                   "0:Disabled",
              
                   "1:LAN",
              
                   "2:WAN",
              
                   "3:BOTH",
              
              0), FALSE, FALSE},
                                    
                       {"sr_num_x", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
                                     
                 {"dr_static_rip_x", "", validate_range, ARGV("0","1"), FALSE, FALSE},                                                       
                           
             {"dr_static_matric_x", "", validate_range, ARGV("1", "16", "1"), FALSE, FALSE},
        	
              {"dr_default_x", "", validate_choice, ARGV(              
              
                   "0:be redistributed",
              
                   "1:not be redistributed",
              
              0), FALSE, FALSE},
           
      {"RouteInfo", "Status", NULL, ARGV("route.log",""), FALSE, FALSE},
                         
                       {"dr_static_rip_x", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
                                             
                       {"dr_staticnum_x", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
#ifndef NOUSB
                                             
                       {"", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
                                     
                 {"", "", validate_range, ARGV("0","1"), FALSE, FALSE},                                                       
              	
              {"", "", validate_choice, ARGV(              
              
                   "RIP1",
              
                   "RIP2",
              
                   "Both",
              
              0), FALSE, FALSE},
                            
                 {"", "", validate_range, ARGV("0","1"), FALSE, FALSE},                                                       
              	
              {"", "", validate_choice, ARGV(              
              
                   "Disable",
              
                   "Text",
              
                   "MD5",
              
              0), FALSE, FALSE},
                                                   
                    {"", "", validate_string, ARGV("16"), FALSE, FALSE},                                                                                                                   
                                  
                 {"", "", validate_range, ARGV("0","1"), FALSE, FALSE},                                                       
              	
              {"", "", validate_choice, ARGV(              
              
                   "RIP1",
              
                   "RIP2",
              
                   "Both",
              
              0), FALSE, FALSE},
                            
                 {"", "", validate_range, ARGV("0","1"), FALSE, FALSE},                                                       
              	
              {"", "", validate_choice, ARGV(              
              
                   "Disable",
              
                   "Text",
              
                   "MD5",
              
              0), FALSE, FALSE},
                                                   
                    {"", "", validate_string, ARGV("16"), FALSE, FALSE},                                                                                                                   
                                          
                       {"", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
                                             
                       {"", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
                                             
                       {"", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
                    	
              {"", "", validate_choice, ARGV(              
              
                   "1:processed",
              
                   "0:dropped",
              
              0), FALSE, FALSE},
           	
              {"", "", validate_choice, ARGV(              
              
                   "1:processed",
              
                   "0:dropped",
              
              0), FALSE, FALSE},
           	
              {"", "", validate_choice, ARGV(              
              
                   "1:processed",
              
                   "0:dropped",
              
              0), FALSE, FALSE},
           	
              {"", "", validate_choice, ARGV(              
              
                   "1:processed",
              
                   "0:dropped",
              
              0), FALSE, FALSE},
                        
             {"", "", validate_range, ARGV("1", "16", "1"), FALSE, FALSE},
        	
              {"", "", validate_choice, ARGV(              
              
                   "0:none",
              
                   "1:routes specified below",
              
                   "2:routes not specified below",
              
              0), FALSE, FALSE},
           	
              {"", "", validate_choice, ARGV(              
              
                   "0:none",
              
                   "1:routes specified below",
              
                   "2:routes not specified below",
              
              0), FALSE, FALSE},
                                    
                       {"", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
                                             
                       {"", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                               
#endif             
                     
      {"GWStatic", "Group", validate_group, ARGV(variables_RouterConfig_GWStatic, "6", "59", "sr_num_x"), FALSE, FALSE},     
      
#ifndef NOUSB 
      {"StaticRoute", "Group", validate_group, ARGV(variables_RouterConfig_StaticRoute, "16", "46", "dr_staticnum_x"), FALSE, FALSE},     
#endif

#ifndef NOUSB       
      {"RipSRoute", "Group", validate_group, ARGV(variables_RouterConfig_RipSRoute, "16", "24", "RipSRouteCount"), FALSE, FALSE},     
       
      {"RouteFilter", "Group", validate_group, ARGV(variables_RouterConfig_RouteFilter, "16", "33", "RipDRouteCount"), FALSE, FALSE},     
       
      {"RipOffset", "Group", validate_group, ARGV(variables_RouterConfig_RipOffset, "16", "30", "RipOffsetCount"), FALSE, FALSE},     
#endif
                        
      { 0, 0, 0, 0, 0, 0}
      };
   
      struct variable variables_LANHostConfig[] = {
                               
                       {"", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
                                     
                 {"lan_proto_x", "", validate_range, ARGV("0","1"), FALSE, FALSE},                                                       
              
                {"lan_ipaddr", "", validate_ipaddr, NULL, FALSE, FALSE},
            
                {"lan_netmask", "", validate_ipaddr, NULL, FALSE, FALSE},

//		{"admin_ipaddr", "", validate_ipaddr, NULL, FALSE, FALSE},
            
      {"x_LANIPAddress", "Status", NULL, ARGV("lan.log","lan_ipaddr_t"), FALSE, FALSE},

      {"x_LANSubnetMask", "Status", NULL, ARGV("lan.log","lan_netmask_t"), FALSE, FALSE},

      {"x_LANGateway", "Status", NULL, ARGV("lan.log","lan_gateway_t"), FALSE, FALSE},
                                        
                    {"lan_hostname", "", validate_string, ARGV("32"), FALSE, FALSE},                                                                                                                   
                 
                {"lan_gateway", "", validate_ipaddr, NULL, FALSE, FALSE},
                             
                 {"dhcp_enable_x", "", validate_range, ARGV("0","1"), FALSE, FALSE},                                                       
#ifndef NOUSB                               
                 {"", "", validate_range, ARGV("0","1"), FALSE, FALSE},                                                       
              
                {"", "", validate_ipaddr, NULL, FALSE, FALSE},
#endif
                                                    
                    {"lan_domain", "", validate_string, ARGV("32"), FALSE, FALSE},                                                                                                                   
                 
                {"dhcp_start", "", validate_ipaddr, NULL, FALSE, FALSE},
            
                {"dhcp_end", "", validate_ipaddr, NULL, FALSE, FALSE},
                         
             {"dhcp_lease", "", validate_range, ARGV("1", "86400", ""), FALSE, FALSE},
        
                {"dhcp_gateway_x", "", validate_ipaddr, NULL, FALSE, FALSE},
            
                {"dhcp_dns1_x", "", validate_ipaddr, NULL, FALSE, FALSE},
            
      {"x_LDNSServer2", "Status", NULL, ARGV("LANHostConfig","lan_ipaddr"), FALSE, FALSE},

                {"dhcp_wins_x", "", validate_ipaddr, NULL, FALSE, FALSE},
                             
                 {"dhcp_static_x", "", validate_range, ARGV("0","1"), FALSE, FALSE},                                                       
                                       
                       {"dhcp_staticnum_x", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
                                             
                       {"", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
                    
      {"DHCPLog", "Status", NULL, ARGV("leases.log",""), FALSE, FALSE},

#ifndef NOUSB                 
                 {"", "", validate_range, ARGV("0","1"), FALSE, FALSE},                                                       
                                                      
                    {"", "", validate_string, ARGV("32"), FALSE, FALSE},                                                                                                                   
                 
                {"", "", validate_ipaddr, NULL, FALSE, FALSE},
            
                {"", "", validate_ipaddr, NULL, FALSE, FALSE},
                                   
                       {"", "", validate_range, ARGV("0","4294927695"), FALSE, FALSE},                                                             
                    
                {"", "", validate_ipaddr, NULL, FALSE, FALSE},
            
      {"x_DmzLDNSServer2", "Status", NULL, ARGV("FirewallConfig","DmzIP"), FALSE, FALSE},

                {"", "", validate_ipaddr, NULL, FALSE, FALSE},
                             
                 {"", "", validate_range, ARGV("0","1"), FALSE, FALSE},                                                       
                                                              {"", "", validate_range, ARGV("0","65535"), FALSE, FALSE},     
                                   #endif
                    
      {"DmzDHCPLog", "Status", NULL, ARGV("dleases.log",""), FALSE, FALSE},
                 
                 {"upnp_enable", "", validate_range, ARGV("0","1"), FALSE, FALSE},                                                       
              
                {"log_ipaddr", "", validate_ipaddr, NULL, FALSE, FALSE},
            	
              {"time_zone", "", validate_choice, 
#ifndef NOUSB             	
		 ARGV(

                   "UCT11:(GMT-11:00) Midway Island, Samoa",
              
                   "UCT10:(GMT-10:00) Hawaii",
              
                   "NAST9NADT:(GMT-09:00) Alaska",
              
                   "PST8PDT:(GMT-08:00) Pacific Time",
              
                   "MST7MDT:(GMT-07:00) Mountain Time",
              
                   "MST7:(GMT-07:00) Arizona",
              
                   "CST6CDT:(GMT-06:00) Central Time",
              
                   "UCT6:(GMT-06:00) Middle America",
              
                   "UCT5:(GMT-05:00) Indiana East, Colombia",
              
                   "EST5EDT:(GMT-05:00) Eastern Time",
              
                   "AST4ADT:(GMT-04:00) Atlantic Time, Brazil West",
              
                   "UCT4:(GMT-04:00) Bolivia, Venezuela",
              
                   "UCT3:(GMT-03:00) Guyana",
              
                   "EBST3EBDT:(GMT-03:00) Brazil East, Greenland",
              
                   "NORO2:(GMT-02:00) Mid-Atlantic",
              
                   "UCT0:(GMT) Gambia, Liberia, Morocco",
              
                   "GMT0BST:(GMT) England",
              
                   "MET-1METDST:(GMT+01:00) Netherland, France, Italy",
              
                   "MEZ-1MESZ:(GMT+01:00) Germany",
              
                   "UCT-1:(GMT+01:00) Tunisia",
              
                   "EET-2EETDST:(GMT+02:00) Greece, Ukraine, Turkey",
              
                   "SAST-2:(GMT+02:00) South Africa",
              
                   "IST-3IDT:(GMT+03:00) Iraq, Jordan, Kuwait",
              
                   "MSK-3MSD:(GMT+03:00) Moscow Winter Time",
              
                   "UCT-4:(GMT+04:00) Armenia",
              
                   "UCT-5:(GMT+05:00) Pakistan, Russia",
              
                   "UCT-6:(GMT+06:00) Bangladesh, Russia",
              
                   "UCT-7:(GMT+07:00) Thailand, Russia",
              
                   "CST-8:(GMT+08:00) China Coast, Hong Kong",
              
                   "CCT-8:(GMT+08:00) Taipei",
              
                   "SST-8:(GMT+08:00) Singapore",
              
                   "WAS-8WAD:(GMT+08:00) Australia West",
              
                   "JST-9:(GMT+09:00) Japan, Korea",
              
                   "KST-9KDT:(GMT+09:00) Korean",
              
                   "UCT-10:(GMT+10:00) Guam, Russia",
              
                   "EST-10EDT:(GMT+10:00) Australia",
              
                   "UCT-11:(GMT+11:00) Solomon Islands",
              
                   "UCT-12:(GMT+12:00) Fiji",
                   "NZST-12NZDT:(GMT+12:00) New Zealand",
  
              0)

#else
	      NULL
#endif
		    , FALSE, FALSE},
                                                   
                    {"time_interval", "", validate_string, ARGV(""), FALSE, FALSE},                                                                                                                   
                                                         
                    {"ntp_server0", "", validate_string, ARGV(""), FALSE, FALSE},                                                                                                                   
                                                         
                    {"ntp_server1", "", validate_string, ARGV(""), FALSE, FALSE},                                                                                                                   
#ifndef NOUSB
                                  
                 {"", "", validate_range, ARGV("0","1"), FALSE, FALSE},                                                       
              
                {"", "", validate_ipaddr, NULL, FALSE, FALSE},
#endif
                             
                 {"ddns_enable_x", "", validate_range, ARGV("0","1"), FALSE, FALSE},                                                       
              	
              {"ddns_server_x", "", validate_choice, ARGV(              
              
                   "WWW.DYNDNS.ORG",
              
                   "WWW.DYNDNS.ORG(CUSTOM)",
              
                   "WWW.DYNDNS.ORG(STATIC)",
              
                   "WWW.TZO.COM",
              
                   "WWW.ZONEEDIT.COM",
              
              0), FALSE, FALSE},
                                                   
                    {"ddns_username_x", "", validate_string, ARGV("32"), FALSE, FALSE},                                                                                                                   
                 
                {"ddns_passwd_x", "", validate_string, ARGV("32"), FALSE, FALSE}, 
                                                    
                    {"ddns_hostname_x", "", validate_string, ARGV("32"), FALSE, FALSE},                                                                                                                   
                                  
                 {"ddns_wildcard_x", "", validate_range, ARGV("0","1"), FALSE, FALSE},                                                       
              
      {"x_DDNSStatus", "Status", NULL, ARGV("ddns.log","DDNSStatus"), FALSE, FALSE},
 
      {"ManualDHCPList", "Group", validate_group, ARGV(variables_LANHostConfig_ManualDHCPList, "8", "29", "dhcp_staticnum_x"), FALSE, FALSE},     
      
#ifndef NOUSB 
      {"DmzManualDHCPList", "Group", validate_group, ARGV(variables_LANHostConfig_DmzManualDHCPList, "8", "29", "DmzManualDHCPCount"), FALSE, FALSE},     
       
      {"ReservedAddressList", "Group", validate_group, ARGV(variables_LANHostConfig_ReservedAddressList, "6", "24", "ReservedAddressListCount"), FALSE, FALSE}, 
#endif
                        
      { 0, 0, 0, 0, 0, 0}
      };
   
      struct variable variables_WLANConfig11a[] = {

#ifndef NOUSB
      
                {"", "", validate_string, NULL, FALSE, FALSE},
                                     
                       {"", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
                                                            
                    {"", "", validate_string, ARGV("32"), FALSE, FALSE},                                                                                                                   
                 
                {"", "", validate_wlchannel, NULL, FALSE, FALSE},
            
                {"", "", validate_string, ARGV(""), FALSE, FALSE}, 
            	
              {"", "", validate_choice, ARGV(              
              
                   "0:Best",
              
                   "1:6",
              
                   "2:9",
              
                   "3:12",
              
                   "4:18",
              
                   "5:24",
              
                   "6:36",
              
                   "7:48",
              
                   "8:54",
              
              0), FALSE, FALSE},
           
                {"", "", validate_string, ARGV(""), FALSE, FALSE}, 
            	
              {"", "", validate_choice, ARGV(              
              
                   "Open System",
              
                   "Shared Key",
              
              0), FALSE, FALSE},
           
                {"", "", validate_wlwep, NULL, FALSE, FALSE},
            
                {"", "", validate_string, ARGV("32"), FALSE, FALSE}, 
            
                {"", "", validate_wlkey, NULL, FALSE, FALSE},
            
                {"", "", validate_wlkey, NULL, FALSE, FALSE},
            
                {"", "", validate_wlkey, NULL, FALSE, FALSE},
            
                {"", "", validate_wlkey, NULL, FALSE, FALSE},
            	
              {"", "", validate_choice, ARGV(              
              
                   "Key1",
              
                   "Key2",
              
                   "Key3",
              
                   "Key4",
              
              0), FALSE, FALSE},
                            
                 {"", "", validate_range, ARGV("0","1"), FALSE, FALSE},                                                       
                               
                 {"", "", validate_range, ARGV("0","1"), FALSE, FALSE},                                                       
#endif
              
      {"WirelessLog", "Status", NULL, ARGV("wlan11a.log",""), FALSE, FALSE},
                  
      { 0, 0, 0, 0, 0, 0}
      };
   
      struct variable variables_DeviceSecurity11a[] = {
                               
                       {"", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
                    	
              {"wl_macmode", "", validate_choice, ARGV(              
              
                   "disabled:Disable",
              
                   "allow:Accept",
              
                   "deny:Reject",
              
              0), FALSE, FALSE},
           	
              {"wl_macapply_x", "", validate_choice, ARGV(              
              
                   "Both",
              
                   "802.11a only",
              
                   "802.11g only",
              
              0), FALSE, FALSE},
                                    
                       {"wl_macnum_x", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
                                             
                       {"", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
                    
                {"", "", validate_string, ARGV(""), FALSE, FALSE}, 
             
      {"ACLList", "Group", validate_group, ARGV(variables_DeviceSecurity11a_ACLList, "64", "32", "wl_macnum_x"), FALSE, FALSE},     
#ifndef NOUSB
       
      {"AESList", "Group", validate_group, ARGV(variables_DeviceSecurity11a_AESList, "64", "58", "AESListCount"), FALSE, FALSE},     
#endif
                        
      { 0, 0, 0, 0, 0, 0}
      };
   
      struct variable variables_WLANAuthentication11a[] = {
                               
                       {"", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
                    
                {"wl_radius_ipaddr", "", validate_ipaddr, NULL, FALSE, FALSE},
                                     
                       {"wl_radius_port", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
                    
                {"wl_radius_key", "", validate_string, ARGV("64"), FALSE, FALSE}, 
#ifndef NOUSB
            	
              {"", "", validate_choice, ARGV(              
              
                   "0:Disabled",
              
                   "1:LEAP",
              
                   "3:EAP/TLS",
              
                   "2:PEAP/MSCHAPv2",
              
                   "4:PEAP/TLS",
              
              0), FALSE, FALSE},
                                                   
                    {"", "", validate_string, ARGV(""), FALSE, FALSE},                                                                                                                   
                                                         
                    {"", "", validate_string, ARGV(""), FALSE, FALSE},                                                                                                                   
                  
      {"LocalAuthDB", "Group", validate_group, ARGV(variables_WLANAuthentication11a_LocalAuthDB, "32", "36", "AuthDBNumberOfEntries"), FALSE, FALSE},     
       
      {"LocalCertDB", "Group", validate_group, ARGV(variables_WLANAuthentication11a_LocalCertDB, "32", "40", "CertDBNumberOfEntries"), FALSE, FALSE},     
#endif

      { 0, 0, 0, 0, 0, 0}
      };

	struct variable variables_WLANConfig11b[] = {
	{"", "", validate_range, ARGV("0","65535"), FALSE, FALSE},
	{"wl_country_code", "", validate_string, NULL, FALSE, FALSE},
	{"wl_ssid", "", validate_string, ARGV("32"), FALSE, FALSE},
	{"wl_ssid2", "", validate_string, ARGV("96"), FALSE, FALSE},
	{"", "", validate_range, ARGV("0","65535"), FALSE, FALSE},
	{"wl_mode_x", "", validate_choice, ARGV("0:AP Only",
						"1:WDS Only",
						"2:Hybrid", 0), FALSE, FALSE},
	{"wl_channel", "", validate_wlchannel, NULL, FALSE, FALSE},
	{"wl_wdsapply_x", "", validate_range, ARGV("0","1"), FALSE, FALSE},                                                       
	{"wl_lazywds", "", validate_range, ARGV("0","1"), FALSE, FALSE},                                                       
	{"wl_wdsnum_x", "", validate_range, ARGV("0","65535"), FALSE, FALSE},
	{"wl_gmode", "", validate_choice, ARGV(	"1:Auto",
						"4:54G Only",
						"0:802.11B Only", 0), FALSE, FALSE},
	{"wl_gmode_protection_x", "", validate_string, ARGV(""), FALSE, FALSE}, 
	{"wl_auth_mode", "", validate_choice, ARGV(	"open:Open System or Shared Key",
                   					"shared:Shared Key",
							"psk:WPA-PSK",
							"wpa:WPA",
							"radius:Radius with 802.1x", 0), FALSE, FALSE},
	{"wl_wpa_mode", "", validate_choice, ARGV(	"0:WPA-PSK/WPA2-PSK",
							"1:WPA-PSK",
							"2:WPA2-PSK", 0), FALSE, FALSE},
//	{"wl_nat", "", validate_choice, ARGV(	"0:Disable",
//						"1:Enable", 0), FALSE, FALSE},
        {"wl_hotspot", "", validate_choice, ARGV(      "0:Disable",
                                                        "1:Enable", 0), FALSE, FALSE},
	{"wl_crypto", "", validate_choice, ARGV("tkip:TKIP",
						"aes:AES",
						"tkip+aes:TKIP+AES", 0), FALSE, FALSE},
	{"wl_wpa_psk", "", validate_string, ARGV("64"), FALSE, FALSE}, 
	{"wl_wep_x", "", validate_choice, ARGV(	"0:None",
						"1:WEP-64bits",
						"2:WEP-128bits", 0), FALSE, FALSE},
	{"wl_phrase_x", "", validate_string, ARGV("64"), FALSE, FALSE}, 
	{"wl_key1", "", validate_wlkey, NULL, FALSE, FALSE},
	{"wl_key2", "", validate_wlkey, NULL, FALSE, FALSE},
	{"wl_key3", "", validate_wlkey, NULL, FALSE, FALSE},
	{"wl_key4", "", validate_wlkey, NULL, FALSE, FALSE},
	{"wl_key", "", validate_choice, ARGV(	"1:Key1",
						"2:Key2",
						"3:Key3",
						"4:Key4", 0), FALSE, FALSE},
	{"wl_key_type", "", validate_choice, ARGV(	"0:HEX",
							"1:ASCII", 0), FALSE, FALSE},
	{"wl_wpa_gtk_rekey", "", validate_range, ARGV("0", "86400", ""), FALSE, FALSE},
	{"wl_connect_none_default", "", validate_range, ARGV("0","1"), FALSE, FALSE},
	{"wl_connect_selected", "", validate_range, ARGV("0","1"), FALSE, FALSE},
//	{"wl_ap_index", "", validate_range, ARGV("1","4"), FALSE, FALSE},
	{"wl_ap_ssid", "", validate_string, ARGV("32"), FALSE, FALSE},
	{"wl_ap_order", "", validate_range, ARGV("0","1"), FALSE, FALSE},
	{"wl_individual_ure", "", validate_range, ARGV("0","1"), FALSE, FALSE},
	{"", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
	{"wl_afterburner", "", validate_choice, ARGV(	"off:Disabled",
							"auto:Enabled", 0), FALSE, FALSE},
	{"wl_closed", "", validate_range, ARGV("0","1"), FALSE, FALSE},                                                       
	{"wl_ap_isolate", "", validate_range, ARGV("0","1"), FALSE, FALSE},                                                       
	{"wl_rate", "", validate_choice, ARGV(	"0:Auto",
						"1000000:1",
						"2000000:2",
						"5500000:5.5",
						"6000000:6",
						"9000000:9",
						"11000000:11",
						"12000000:12",
						"18000000:18",
						"24000000:24",
						"36000000:36",
						"48000000:48",
						"54000000:54", 0), FALSE, FALSE},
	{"wl_rateset", "", validate_choice, ARGV(	"default:Default",
							"all:All",
							"12:1, 2 Mbps", 0), FALSE, FALSE},
	{"wl_frag", "", validate_range, ARGV("256", "2346", ""), FALSE, FALSE},
	{"wl_rts", "", validate_range, ARGV("0", "2347", ""), FALSE, FALSE},
	{"wl_dtim", "", validate_range, ARGV("1", "255", ""), FALSE, FALSE},
	{"wl_bcn", "", validate_range, ARGV("1", "65535", ""), FALSE, FALSE},
	{"wl_frameburst", "", validate_choice, ARGV(	"off:Disabled",
							"on:Enabled", 0), FALSE, FALSE},
	{"wl_mode_ex", "", validate_choice, ARGV(	"ap:AP or WDS",
							"sta:Station",
							"wet:Ethernet Bridge",
							"re:URE", 0), FALSE, FALSE},
	{"wl_radio_x", "", validate_range, ARGV("0","1"), FALSE, FALSE},                                                       
	{"wl_radio_date_x", "", validate_portrange, NULL, FALSE, FALSE},
	{"wl_radio_time_x", "", validate_portrange, NULL, FALSE, FALSE},
	{"wl_radio_power_x", "", validate_range, ARGV("1", "84", ""), FALSE, FALSE},
	{"WirelessLog", "Status", NULL, ARGV("wlan11b.log",""), FALSE, FALSE},
	{"", "", validate_string, ARGV(""), FALSE, FALSE},                                                                                                                   
	{"wl_wme", "", validate_choice, ARGV(	"off:Disabled",
						"on:Enabled", 0), FALSE, FALSE},
	{"wl_wme_no_ack", "", validate_choice, ARGV(	"off:Disabled",
							"on:Enabled", 0), FALSE, FALSE},
	{"wl_wme_ap_bk", "", validate_string, ARGV(""), FALSE, FALSE},                                                                                                                   
	{"wl_wme_ap_be", "", validate_string, ARGV(""), FALSE, FALSE},                                                                                                                   
	{"wl_wme_ap_vi", "", validate_string, ARGV(""), FALSE, FALSE},                                                                                                                   
	{"wl_wme_ap_vo", "", validate_string, ARGV(""), FALSE, FALSE},                                                                                                                                                                            
	{"wl_wme_sta_bk", "", validate_string, ARGV(""), FALSE, FALSE},                                                                                                                                                                          
	{"wl_wme_sta_be", "", validate_string, ARGV(""), FALSE, FALSE},                                                                                                                   
	{"wl_wme_sta_vi", "", validate_string, ARGV(""), FALSE, FALSE},                                                                                                                   
	{"wl_wme_sta_vo", "", validate_string, ARGV(""), FALSE, FALSE},                                                                                                                   
	{"wl_preauth", "", validate_range, ARGV("0","1"), FALSE, FALSE},                                                       
	{"wl_net_reauth", "", validate_string, ARGV(""), FALSE, FALSE},                                                                             

{"wl_guest_enable", "0", 0, 0, 0, 0},
{"wl_guest_ssid_1", "guest", 0, 0, 0, 0},
{"wl_guest_ssid_2", "guest", 0, 0, 0, 0},
{"wl_guest_auth_mode_1", "open", 0, 0, 0, 0},
{"wl_guest_wpa_mode", "0", 0, 0, 0, 0},
{"wl_guest_crypto_1", "0", 0, 0, 0, 0},
{"wl_guest_wpa_psk_1", "", 0, 0, 0, 0},
{"wl_guest_wep_x_1", "0", 0, 0, 0, 0},
{"wl_guest_key_type_1", "1", 0, 0, 0, 0},
{"wl_guest_phrase_x_1", "", 0, 0, 0, 0},
{"wl_guest_key1_1", "", 0, 0, 0, 0},
{"wl_guest_key2_1", "", 0, 0, 0, 0},
{"wl_guest_key3_1", "", 0, 0, 0, 0},
{"wl_guest_key4_1", "", 0, 0, 0, 0},
{"wl_guest_key_1", "1", 0, 0, 0, 0},
{"wl_guest_wpa_gtk_rekey_1", "0", 0, 0, 0, 0},
{"lan1_ipaddr", "192.168.2.1", 0, 0, 0, 0},
{"lan1_netmask", "255.255.255.0", 0, 0, 0, 0},
{"dhcp1_enable_x", "1", 0, 0, 0, 0},
{"dhcp1_start", "192.168.2.2", 0, 0, 0, 0},
{"dhcp1_end", "192.168.2.254", 0, 0, 0, 0},
{"lan1_lease", "86400", 0, 0, 0, 0},

	{"mssid_enable", "", validate_range, ARGV("0","1"), FALSE, FALSE},
	{"vlans_enable", "", validate_range, ARGV("0","1"), FALSE, FALSE},
	{"mssidnum_x", "", validate_range, ARGV("0","65535"), FALSE, FALSE},

	{"RBRList", "Group", validate_group, ARGV(variables_WLANConfig11b_RBRList, "16", "32", "wl_wdsnum_x"), FALSE, FALSE},
	{"MSSIDList", "Group", validate_group, ARGV(variables_WLANConfig11b_MSSIDList, "3", "78", "mssidnum_x"), FALSE, FALSE},
                        
	{0, 0, 0, 0, 0, 0}
	};

      struct variable variables_DeviceSecurity11b[] = {
                               
                       {"", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
                    	
              {"wl_macmode", "", validate_choice, ARGV(              
              
                   "disabled:Disable",
              
                   "allow:Accept",
              
                   "deny:Reject",
              
              0), FALSE, FALSE},
           	
              {"wl_macapply_x", "", validate_choice, ARGV(              
              
                   "Both",
              
                   "802.11a only",
              
                   "802.11g only",
              
              0), FALSE, FALSE},
                                    
                       {"wl_macnum_x", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
                                             
                       {"", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
                    
                {"", "", validate_string, ARGV(""), FALSE, FALSE}, 
             
      {"ACLList", "Group", validate_group, ARGV(variables_DeviceSecurity11b_ACLList, "64", "32", "wl_macnum_x"), FALSE, FALSE},     

#ifndef NOUSB
       
      {"AESList", "Group", validate_group, ARGV(variables_DeviceSecurity11b_AESList, "64", "58", "AESListCount"), FALSE, FALSE},     
#endif
                        
      { 0, 0, 0, 0, 0, 0}
      };
   
      struct variable variables_WLANAuthentication11b[] = {
                               
                       {"", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
                    
                {"wl_radius_ipaddr", "", validate_ipaddr, NULL, FALSE, FALSE},
                                     
                       {"wl_radius_port", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
                    
                {"wl_radius_key", "", validate_string, ARGV("64"), FALSE, FALSE}, 
            	
#ifndef NOUSB
              {"", "", validate_choice, ARGV(              
              
                   "0:Disabled",
              
                   "1:LEAP",
              
                   "3:EAP/TLS",
              
                   "2:PEAP/MSCHAPv2",
              
                   "4:PEAP/TLS",
              
              0), FALSE, FALSE},
                                                   
                    {"", "", validate_string, ARGV(""), FALSE, FALSE},                                                                                                                   
                                                         
                    {"", "", validate_string, ARGV(""), FALSE, FALSE},                                                                                                                   
                  
      {"LocalAuthDB", "Group", validate_group, ARGV(variables_WLANAuthentication11b_LocalAuthDB, "32", "36", "AuthDBNumberOfEntries"), FALSE, FALSE},     
       
      {"LocalCertDB", "Group", validate_group, ARGV(variables_WLANAuthentication11b_LocalCertDB, "32", "40", "CertDBNumberOfEntries"), FALSE, FALSE},     
#endif
                        
      { 0, 0, 0, 0, 0, 0}
      };
   
      struct variable variables_PrinterStatus[] = {
      
      {"x_PrinterModel", "Status", NULL, ARGV("printer_status.log","printer_model_t"), FALSE, FALSE},

      {"x_PrinterStatus", "Status", NULL, ARGV("printer_status.log","printer_status_t"), FALSE, FALSE},

      {"x_PrinterUser", "Status", NULL, ARGV("printer_status.log","printer_user_t"), FALSE, FALSE},
                                        
                    {"", "", validate_string, ARGV(""), FALSE, FALSE},                                                                                                                   
#ifndef NOUSB                 	
              {"usb_webenable_x", "", validate_choice, ARGV(              
              
                   "0:Disabled",
              
                   "1:LAN Only",
              
                   "2:LAN and WAN",
              
              0), FALSE, FALSE},
           	
              {"usb_webmode_x", "", validate_choice, ARGV(              
              
                   "0:ActiveX Only",
              
                   "1:ActiveX and Refresh",
              
                   "2:Refresh Only",
              
              0), FALSE, FALSE},
           	
              {"usb_webdriver_x", "", validate_choice, ARGV(              
              
                   "0:PWC 8.8",
              
                   "1:OV511 2.10",
              
              0), FALSE, FALSE},
           	
              {"usb_webimage_x", "", validate_choice, ARGV(              
              
                   "0:640 X 480",
              
                   "1:320 X 240",
              
                   "2:160 X 120",
              
              0), FALSE, FALSE},
           	
              {"usb_websense_x", "", validate_choice, ARGV(              
              
                   "0:Low",
              
                   "1:Medium",
              
                   "2:High",
              
              0), FALSE, FALSE},
                                    
                       {"usb_webrectime_x", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
                                 
             {"usb_webfresh_x", "", validate_range, ARGV("1", "65535", ""), FALSE, FALSE},
                                                
                    {"usb_webcaption_x", "", validate_string, ARGV(""), FALSE, FALSE},                                                                                                                   
                              
             {"usb_webhttpport_x", "", validate_range, ARGV("1024", "65535", ""), FALSE, FALSE},
        
                {"usb_webhttpcheck_x", "", validate_string, ARGV(""), FALSE, FALSE}, 
                         
             {"usb_webactivex_x", "", validate_range, ARGV("1024", "65535", ""), FALSE, FALSE},
                         
                 {"usb_websecurity_x", "", validate_range, ARGV("0","1"), FALSE, FALSE},                                                       
              
                {"usb_websecurity_date_x", "", validate_portrange, NULL, FALSE, FALSE},
            
                {"usb_websecurity_time_x", "", validate_portrange, NULL, FALSE, FALSE},
                                                    
                    {"usb_websendto_x", "", validate_string, ARGV(""), FALSE, FALSE},                                                                                                                   
                                                         
                    {"usb_webmserver_x", "", validate_string, ARGV(""), FALSE, FALSE},                                                                                                                   
                                                         
                    {"usb_websubject_x", "", validate_string, ARGV(""), FALSE, FALSE},                                                                                                                   
                                  
                 {"usb_webattach_x", "", validate_range, ARGV("0","1"), FALSE, FALSE},                                                       
              	
              {"", "", validate_choice, ARGV(              
              
                   "0:None",
              
                   "1:PIN 1",
              
                   "2:PIN 2",
              
              0), FALSE, FALSE},
           	
              {"usb_webremote_x", "", validate_choice, ARGV(              
              
                   "0:LAN Only",
              
              0), FALSE, FALSE},
           
                {"usb_webremote1_x", "", validate_ipaddr, NULL, FALSE, FALSE},
            
                {"usb_webremote2_x", "", validate_ipaddr, NULL, FALSE, FALSE},
            
                {"usb_webremote3_x", "", validate_ipaddr, NULL, FALSE, FALSE},
            
                {"usb_webremote4_x", "", validate_ipaddr, NULL, FALSE, FALSE},
            
                {"usb_webremote5_x", "", validate_ipaddr, NULL, FALSE, FALSE},
            
                {"usb_webremote6_x", "", validate_ipaddr, NULL, FALSE, FALSE},
            
      {"x_FEject", "Status", NULL, ARGV("ddns.log","DDNSStatus"), FALSE, FALSE},
                 
                 {"usb_ftpenable_x", "", validate_range, ARGV("0","1"), FALSE, FALSE},                                                       
                               
                 {"usb_ftpanonymous_x", "", validate_range, ARGV("0","1"), FALSE, FALSE},                                                       
                               
                 {"usb_ftpsuper_x", "", validate_range, ARGV("0","1"), FALSE, FALSE},                                                       
                           
             {"usb_ftpport_x", "", validate_range, ARGV("1", "65535", ""), FALSE, FALSE},
                     
             {"usb_ftpmax_x", "", validate_range, ARGV("1", "12", ""), FALSE, FALSE},
                                 
                       {"usb_ftptimeout_x", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
                                             
                       {"usb_ftpstaytimeout_x", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
                                                            
                    {"usb_ftpscript_x", "", validate_string, ARGV(""), FALSE, FALSE},                                                                                                                   
                 	
              {"", "", validate_choice, ARGV(              
              
                   "0:English",
              
                   "1:Traditional Chinese",
              
                   "2:Simplified Chinese",
              
                   "3:Korean",
              
              0), FALSE, FALSE},
                                    
                       {"usb_ftpnum_x", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
                                             
                       {"usb_bannum_x", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                            
                                             
                       {"", "", validate_range, ARGV("0","65535"), FALSE, FALSE},                                                                                             
      {"x_FUserList", "Group", validate_group, ARGV(variables_PrinterStatus_x_FUserList, "32", "59", "usb_ftpnum_x"), FALSE, FALSE},     
       
      {"x_FBanIPList", "Group", validate_group, ARGV(variables_PrinterStatus_x_FBanIPList, "16", "24", "usb_bannum_x"), FALSE, FALSE},   
#endif

#ifndef NOQOS 
      {"qos_rulenum_x", "8", validate_range, ARGV("0","8"), FALSE, FALSE},

      {"qos_enable", "", validate_range, ARGV("0","1"), FALSE, FALSE},

      {"qos_global_enable", "", validate_range, ARGV("0","1"), FALSE, FALSE},

      {"qos_service_enable", "", validate_range, ARGV("0","1"), FALSE, FALSE},

      {"qos_tos_prio", "", validate_range, ARGV("0","1"), FALSE, FALSE},

      {"qos_pshack_prio", "", validate_range, ARGV("0","1"), FALSE, FALSE},

      {"qos_shortpkt_prio", "", validate_range, ARGV("0","1"), FALSE, FALSE},

      {"qos_dfragment_enable", "", validate_range, ARGV("0","1"), FALSE, FALSE},

      {"qos_dfragment_size", "", validate_range, ARGV("0","100"), FALSE, FALSE},

      {"qos_service_ubw", "", validate_range, ARGV("0","100"), FALSE, FALSE},

      {"x_USRRuleList", "Group", validate_group, ARGV(variables_PrinterStatus_x_USRRuleList, "8", "47", "qos_rulenum_x"), FALSE, FALSE},
#endif                        
      { 0, 0, 0, 0, 0, 0}
      };
     
#ifndef NOUSB 
      struct action actions_General[] = {
      
          { "x_GetSystemInfo",
          ARGV(
          
          0),
          ARGV(
          
          0),
          ARGV(
          
             "NewSystemUpTime",
             
             "NewProductID",
             
             "NewFirmwareVersion",
             
             "NewHardwareVersion",
             
             "NewBootloaderVersion",
             
          0), 
          ARGV(
          
             "x_SystemUpTime", 
             
             "x_ProductID", 
             
             "x_FirmwareVersion", 
             
             "x_HardwareVersion", 
             
             "x_BootloaderVersion", 
             
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
                          
      { 0, 0, 0, 0}
      };
      
      struct action actions_Layer3Forwarding[] = {
      
          { "SetDefaultConnectionService",
          ARGV(
          
             "NewDefaultConnectionService",
             
          0),
          ARGV(
          
             "DefaultConnectionService",
             
          0),
          ARGV(
          
          0), 
          ARGV(
          
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "GetDefaultConnectionService",
          ARGV(
          
          0),
          ARGV(
          
          0),
          ARGV(
          
             "NewDefaultConnectionService",
             
          0), 
          ARGV(
          
             "DefaultConnectionService", 
             
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "GetConnectionType",
          ARGV(
          
          0),
          ARGV(
          
          0),
          ARGV(
          
             "NewConnectionType",
             
          0), 
          ARGV(
          
             "x_ConnectionType", 
             
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "SetConnectionType",
          ARGV(
          
             "NewConnectionType",
             
          0),
          ARGV(
          
             "x_ConnectionType",
             
          0),
          ARGV(
          
          0), 
          ARGV(
          
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
                          
      { 0, 0, 0, 0}
      };
      
      struct action actions_WANCommonInterface[] = {
      
          { "SetEnabledForInternet",
          ARGV(
          
             "NewEnabledForInternet",
             
          0),
          ARGV(
          
             "EnabledForInternet",
             
          0),
          ARGV(
          
          0), 
          ARGV(
          
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "GetEnabledForInternet",
          ARGV(
          
          0),
          ARGV(
          
          0),
          ARGV(
          
             "NewEnabledForInternet",
             
          0), 
          ARGV(
          
             "EnabledForInternet", 
             
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "GetCommonLinkProperties",
          ARGV(
          
          0),
          ARGV(
          
          0),
          ARGV(
          
             "NewWANAccessType",
             
             "NewLayer1UpstreamMaxBitRate",
             
             "NewLayer1DownstreamMaxBitRate",
             
             "NewPhysicalLinkStatus",
             
          0), 
          ARGV(
          
             "WANAccessType", 
             
             "Layer1UpstreamMaxBitRate", 
             
             "Layer1DownstreamMaxBitRate", 
             
             "PhysicalLinkStatus", 
             
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "GetWANAccessProvider",
          ARGV(
          
          0),
          ARGV(
          
          0),
          ARGV(
          
             "NewWANAccessProvider",
             
          0), 
          ARGV(
          
             "WANAccessProvider", 
             
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "GetMaximumActiveConnections",
          ARGV(
          
          0),
          ARGV(
          
          0),
          ARGV(
          
             "NewMaximumActiveConnections",
             
          0), 
          ARGV(
          
             "MaximumActiveConnections", 
             
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "GetTotalBytesSent",
          ARGV(
          
          0),
          ARGV(
          
          0),
          ARGV(
          
             "NewTotalBytesSent",
             
          0), 
          ARGV(
          
             "TotalBytesSent", 
             
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "GetTotalBytesReceived",
          ARGV(
          
          0),
          ARGV(
          
          0),
          ARGV(
          
             "NewTotalBytesReceived",
             
          0), 
          ARGV(
          
             "TotalBytesReceived", 
             
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "GetTotalPacketsSent",
          ARGV(
          
          0),
          ARGV(
          
          0),
          ARGV(
          
             "NewTotalPacketsSent",
             
          0), 
          ARGV(
          
             "TotalPacketsSent", 
             
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "GetTotalPacketsReceived",
          ARGV(
          
          0),
          ARGV(
          
          0),
          ARGV(
          
             "NewTotalPacketsReceived",
             
          0), 
          ARGV(
          
             "TotalPacketsReceived", 
             
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "GetActiveConnection",
          ARGV(
          
             "NewActiveConnectionIndex",
             
          0),
          ARGV(
          
             "NumberOfActiveConnections",
             
          0),
          ARGV(
          
             "NewActiveConnDeviceContainer",
             
             "NewActiveConnectionServiceID",
             
          0), 
          ARGV(
          
             "ActiveConnectionDeviceContainer", 
             
             "ActiveConnectionServiceID", 
             
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
                          
      { 0, 0, 0, 0}
      };
      
      struct action actions_IPConnection[] = {
      
          { "SetConnectionType",
          ARGV(
          
             "NewConnectionType",
             
          0),
          ARGV(
          
             "ConnectionType",
             
          0),
          ARGV(
          
          0), 
          ARGV(
          
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "GetConnectionTypeInfo",
          ARGV(
          
          0),
          ARGV(
          
          0),
          ARGV(
          
             "NewConnectionType",
             
             "NewPossibleConnectionTypes",
             
          0), 
          ARGV(
          
             "ConnectionType", 
             
             "PossibleConnectionTypes", 
             
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "GetStatusInfo",
          ARGV(
          
          0),
          ARGV(
          
          0),
          ARGV(
          
             "NewConnectionStatus",
             
             "NewLastConnectionError",
             
             "NewUptime",
             
          0), 
          ARGV(
          
             "ConnectionStatus", 
             
             "LastConnectionError", 
             
             "Uptime", 
             
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "GetNATRSIPStatus",
          ARGV(
          
          0),
          ARGV(
          
          0),
          ARGV(
          
             "NewRSIPAvailable",
             
             "NewNATEnabled",
             
          0), 
          ARGV(
          
             "RSIPAvailable", 
             
             "NATEnabled", 
             
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "GetGenericPortMappingEntry",
          ARGV(
          
             "NewPortMappingIndex",
             
          0),
          ARGV(
          
             "PortMappingNumberOfEntries",
             
          0),
          ARGV(
          
             "NewRemoteHost",
             
             "NewExternalPort",
             
             "NewProtocol",
             
             "NewInternalPort",
             
             "NewInternalClient",
             
             "NewEnabled",
             
             "NewPortMappingDescription",
             
             "NewLeaseDuration",
             
          0), 
          ARGV(
          
             "RemoteHost", 
             
             "ExternalPort", 
             
             "PortMappingProtocol", 
             
             "InternalPort", 
             
             "InternalClient", 
             
             "PortMappingEnabled", 
             
             "PortMappingDescription", 
             
             "PortMappingLeaseDuration", 
             
          0), 
          ARGV(       
          
             "PortMappingList", 
             "External", 
                      
          0),
          NULL                    
          },
      
          { "GetSpecificPortMappingEntry ",
          ARGV(
          
             "NewRemoteHost",
             
             "NewExternalPort",
             
             "NewProtocol",
             
          0),
          ARGV(
          
             "RemoteHost",
             
             "ExternalPort",
             
             "PortMappingProtocol",
             
          0),
          ARGV(
          
             "NewInternalPort",
             
             "NewInternalClient",
             
             "NewEnabled",
             
             "NewPortMappingDescription",
             
             "NewLeaseDuration",
             
          0), 
          ARGV(
          
             "InternalPort", 
             
             "InternalClient", 
             
             "PortMappingEnabled", 
             
             "PortMappingDescription", 
             
             "PortMappingLeaseDuration", 
             
          0), 
          ARGV(       
          
             "PortMappingList", 
             "External", 
                      
          0),
          NULL                    
          },
      
          { "AddPortMapping ",
          ARGV(
          
             "NewRemoteHost",
             
             "NewExternalPort",
             
             "NewProtocol",
             
             "NewInternalPort",
             
             "NewInternalClient",
             
             "NewEnabled",
             
             "NewPortMappingDescription",
             
             "NewLeaseDuration",
             
          0),
          ARGV(
          
             "RemoteHost",
             
             "ExternalPort",
             
             "PortMappingProtocol",
             
             "InternalPort",
             
             "InternalClient",
             
             "PortMappingEnabled",
             
             "PortMappingDescription",
             
             "PortMappingLeaseDuration",
             
          0),
          ARGV(
          
          0), 
          ARGV(
          
          0), 
          ARGV(       
          
             "PortMappingList", 
             "External", 
                      
          0),
          NULL                    
          },
      
          { "DeletePortMapping",
          ARGV(
          
             "NewRemoteHost",
             
             "NewExternalPort",
             
             "NewProtocol",
             
          0),
          ARGV(
          
             "RemoteHost",
             
             "ExternalPort",
             
             "PortMappingProtocol",
             
          0),
          ARGV(
          
          0), 
          ARGV(
          
          0), 
          ARGV(       
          
             "PortMappingList", 
             "External", 
                      
          0),
          NULL                    
          },
      
          { "GetExternalIPAddress",
          ARGV(
          
          0),
          ARGV(
          
          0),
          ARGV(
          
             "NewExternalIPAddress",
             
          0), 
          ARGV(
          
             "ExternalIPAddress", 
             
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
                          
      { 0, 0, 0, 0}
      };
      
      struct action actions_PPPConnection[] = {
      
          { "SetConnectionType",
          ARGV(
          
             "NewConnectionType",
             
          0),
          ARGV(
          
             "ConnectionType",
             
          0),
          ARGV(
          
          0), 
          ARGV(
          
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "GetConnectionTypeInfo",
          ARGV(
          
          0),
          ARGV(
          
          0),
          ARGV(
          
             "NewConnectionType",
             
             "NewPossibleConnectionTypes",
             
          0), 
          ARGV(
          
             "ConnectionType", 
             
             "PossibleConnectionTypes", 
             
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "ConfigureConnection",
          ARGV(
          
             "NewUserName",
             
             "NewPassword",
             
          0),
          ARGV(
          
             "UserName",
             
             "Password",
             
          0),
          ARGV(
          
          0), 
          ARGV(
          
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "RequestConnection",
          ARGV(
          
          0),
          ARGV(
          
          0),
          ARGV(
          
          0), 
          ARGV(
          
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "RequestTermination",
          ARGV(
          
          0),
          ARGV(
          
          0),
          ARGV(
          
          0), 
          ARGV(
          
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "ForceTermination",
          ARGV(
          
          0),
          ARGV(
          
          0),
          ARGV(
          
          0), 
          ARGV(
          
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "SetAutoDisconnectTime",
          ARGV(
          
             "NewAutoDisconnectTime",
             
          0),
          ARGV(
          
             "AutoDisconnectTime",
             
          0),
          ARGV(
          
          0), 
          ARGV(
          
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "SetIdleDisconnectTime",
          ARGV(
          
             "NewIdleDisconnectTime",
             
          0),
          ARGV(
          
             "IdleDisconnectTime",
             
          0),
          ARGV(
          
          0), 
          ARGV(
          
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "SetWarnDisconnectDelay",
          ARGV(
          
             "NewWarnDisconnectDelay",
             
          0),
          ARGV(
          
             "WarnDisconnectDelay",
             
          0),
          ARGV(
          
          0), 
          ARGV(
          
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "GetStatusInfo",
          ARGV(
          
          0),
          ARGV(
          
          0),
          ARGV(
          
             "NewConnectionStatus",
             
             "NewLastConnectionError",
             
             "NewUptime",
             
          0), 
          ARGV(
          
             "ConnectionStatus", 
             
             "LastConnectionError", 
             
             "Uptime", 
             
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "GetLinkLayerMaxBitRates",
          ARGV(
          
          0),
          ARGV(
          
          0),
          ARGV(
          
             "NewUpstreamMaxBitRate",
             
             "NewDownstreamMaxBitRate",
             
          0), 
          ARGV(
          
             "UpstreamMaxBitRate", 
             
             "DownstreamMaxBitRate", 
             
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "GetUserName",
          ARGV(
          
          0),
          ARGV(
          
          0),
          ARGV(
          
             "NewUserName",
             
          0), 
          ARGV(
          
             "UserName", 
             
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "GetPassword",
          ARGV(
          
          0),
          ARGV(
          
          0),
          ARGV(
          
             "NewPassword",
             
          0), 
          ARGV(
          
             "Password", 
             
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "GetAutoDisconnectTime",
          ARGV(
          
          0),
          ARGV(
          
          0),
          ARGV(
          
             "NewAutoDisconnectTime",
             
          0), 
          ARGV(
          
             "AutoDisconnectTime", 
             
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "GetIdleDisconnectTime",
          ARGV(
          
          0),
          ARGV(
          
          0),
          ARGV(
          
             "NewIdleDisconnectTime",
             
          0), 
          ARGV(
          
             "IdleDisconnectTime", 
             
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "GetWarnDisconnectDelay",
          ARGV(
          
          0),
          ARGV(
          
          0),
          ARGV(
          
             "NewWarnDisconnectDelay",
             
          0), 
          ARGV(
          
             "WarnDisconnectDelay", 
             
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "GetNATRSIPStatus",
          ARGV(
          
          0),
          ARGV(
          
          0),
          ARGV(
          
             "NewRSIPAvailable",
             
             "NewNATEnabled",
             
          0), 
          ARGV(
          
             "RSIPAvailable", 
             
             "NATEnabled", 
             
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "GetGenericPortMappingEntry",
          ARGV(
          
             "NewPortMappingIndex",
             
          0),
          ARGV(
          
             "PortMappingNumberOfEntries",
             
          0),
          ARGV(
          
             "NewRemoteHost",
             
             "NewExternalPort",
             
             "NewProtocol",
             
             "NewInternalPort",
             
             "NewInternalClient",
             
             "NewEnabled",
             
             "NewPortMappingDescription",
             
             "NewLeaseDuration",
             
          0), 
          ARGV(
          
             "RemoteHost", 
             
             "ExternalPort", 
             
             "PortMappingProtocol", 
             
             "InternalPort", 
             
             "InternalClient", 
             
             "PortMappingEnabled", 
             
             "PortMappingDescription", 
             
             "PortMappingLeaseDuration", 
             
          0), 
          ARGV(       
          
             "PortMappingList", 
             "External", 
                      
          0),
          NULL                    
          },
      
          { "GetSpecificPortMappingEntry ",
          ARGV(
          
             "NewRemoteHost",
             
             "NewExternalPort",
             
             "NewProtocol",
             
          0),
          ARGV(
          
             "RemoteHost",
             
             "ExternalPort",
             
             "PortMappingProtocol",
             
          0),
          ARGV(
          
             "NewInternalPort",
             
             "NewInternalClient",
             
             "NewEnabled",
             
             "NewPortMappingDescription",
             
             "NewLeaseDuration",
             
          0), 
          ARGV(
          
             "InternalPort", 
             
             "InternalClient", 
             
             "PortMappingEnabled", 
             
             "PortMappingDescription", 
             
             "PortMappingLeaseDuration", 
             
          0), 
          ARGV(       
          
             "PortMappingList", 
             "External", 
                      
          0),
          NULL                    
          },
      
          { "AddPortMapping ",
          ARGV(
          
             "NewRemoteHost",
             
             "NewExternalPort",
             
             "NewProtocol",
             
             "NewInternalPort",
             
             "NewInternalClient",
             
             "NewEnabled",
             
             "NewPortMappingDescription",
             
             "NewLeaseDuration",
             
          0),
          ARGV(
          
             "RemoteHost",
             
             "ExternalPort",
             
             "PortMappingProtocol",
             
             "InternalPort",
             
             "InternalClient",
             
             "PortMappingEnabled",
             
             "PortMappingDescription",
             
             "PortMappingLeaseDuration",
             
          0),
          ARGV(
          
          0), 
          ARGV(
          
          0), 
          ARGV(       
          
             "PortMappingList", 
             "External", 
                      
          0),
          NULL                    
          },
      
          { "DeletePortMapping",
          ARGV(
          
             "NewRemoteHost",
             
             "NewExternalPort",
             
             "NewProtocol",
             
          0),
          ARGV(
          
             "RemoteHost",
             
             "ExternalPort",
             
             "PortMappingProtocol",
             
          0),
          ARGV(
          
          0), 
          ARGV(
          
          0), 
          ARGV(       
          
             "PortMappingList", 
             "External", 
                      
          0),
          NULL                    
          },
      
          { "GetExternalIPAddress",
          ARGV(
          
          0),
          ARGV(
          
          0),
          ARGV(
          
             "NewExternalIPAddress",
             
          0), 
          ARGV(
          
             "ExternalIPAddress", 
             
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
                          
      { 0, 0, 0, 0}
      };
      
      struct action actions_EthernetLink[] = {
      
          { "GetEthernetLinkStatus",
          ARGV(
          
          0),
          ARGV(
          
          0),
          ARGV(
          
             "NewEthernetLinkStatus",
             
          0), 
          ARGV(
          
             "EthernetLinkStatus", 
             
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
                          
      { 0, 0, 0, 0}
      };
      
      struct action actions_LANHostConfig[] = {
      
          { "SetDHCPServerConfigurable",
          ARGV(
          
             "NewDHCPServerConfigurable",
             
          0),
          ARGV(
          
             "DHCPServerConfigurable",
             
          0),
          ARGV(
          
          0), 
          ARGV(
          
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "GetDHCPServerConfigurable",
          ARGV(
          
          0),
          ARGV(
          
          0),
          ARGV(
          
             "NewDHCPServerConfigurable",
             
          0), 
          ARGV(
          
             "DHCPServerConfigurable", 
             
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "SetDHCPRelay",
          ARGV(
          
             "NewDHCPRelay",
             
          0),
          ARGV(
          
             "DHCPRelay",
             
          0),
          ARGV(
          
          0), 
          ARGV(
          
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "GetDHCPRelay",
          ARGV(
          
          0),
          ARGV(
          
          0),
          ARGV(
          
             "NewDHCPRelay",
             
          0), 
          ARGV(
          
             "DHCPRelay", 
             
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "SetSubnetMask",
          ARGV(
          
             "NewSubnetMask",
             
          0),
          ARGV(
          
             "SubnetMask",
             
          0),
          ARGV(
          
          0), 
          ARGV(
          
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "GetSubnetMask",
          ARGV(
          
          0),
          ARGV(
          
          0),
          ARGV(
          
             "NewSubnetMask",
             
          0), 
          ARGV(
          
             "SubnetMask", 
             
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "SetIPRouter",
          ARGV(
          
             "NewIPRouters",
             
          0),
          ARGV(
          
             "IPRouters",
             
          0),
          ARGV(
          
          0), 
          ARGV(
          
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "DeleteIPRouter",
          ARGV(
          
             "NewIPRouters",
             
          0),
          ARGV(
          
             "IPRouters",
             
          0),
          ARGV(
          
          0), 
          ARGV(
          
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "GetIPRoutersList",
          ARGV(
          
          0),
          ARGV(
          
          0),
          ARGV(
          
             "NewIPRouters",
             
          0), 
          ARGV(
          
             "IPRouters", 
             
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "SetDomainName",
          ARGV(
          
             "NewDomainName",
             
          0),
          ARGV(
          
             "DomainName",
             
          0),
          ARGV(
          
          0), 
          ARGV(
          
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "GetDomainName",
          ARGV(
          
          0),
          ARGV(
          
          0),
          ARGV(
          
             "NewDomainName",
             
          0), 
          ARGV(
          
             "DomainName", 
             
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "SetAddressRange",
          ARGV(
          
             "NewMinAddress",
             
             "NewMaxAddress",
             
          0),
          ARGV(
          
             "MinAddress",
             
             "MaxAddress",
             
          0),
          ARGV(
          
          0), 
          ARGV(
          
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "GetAddressRange",
          ARGV(
          
          0),
          ARGV(
          
          0),
          ARGV(
          
             "NewMinAddress",
             
             "NewMaxAddress",
             
          0), 
          ARGV(
          
             "MinAddress", 
             
             "MaxAddress", 
             
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "SetReservedAddress",
          ARGV(
          
             "NewReservedAddresses",
             
          0),
          ARGV(
          
             "ReservedAddresses",
             
          0),
          ARGV(
          
          0), 
          ARGV(
          
          0), 
          ARGV(       
          
             "ReservedAddressList", 
             "Add", 
                      
          0),
          NULL                    
          },
      
          { "DeleteReservedAddress",
          ARGV(
          
             "NewReservedAddresses",
             
          0),
          ARGV(
          
             "ReservedAddresses",
             
          0),
          ARGV(
          
          0), 
          ARGV(
          
          0), 
          ARGV(       
          
             "ReservedAddressList", 
             "Del", 
                      
          0),
          NULL                    
          },
      
          { "GetReservedAddresses",
          ARGV(
          
          0),
          ARGV(
          
          0),
          ARGV(
          
             "NewReservedAddresses",
             
          0), 
          ARGV(
          
             "ReservedAddresses", 
             
          0), 
          ARGV(       
          
             "ReservedAddressList", 
             "Get", 
                      
          0),
          NULL                    
          },
      
          { "SetDNSServer",
          ARGV(
          
             "NewDNSServers",
             
          0),
          ARGV(
          
             "DNSServers",
             
          0),
          ARGV(
          
          0), 
          ARGV(
          
          0), 
          ARGV(       
          
             "DNSServerList", 
             "Add", 
                      
          0),
          NULL                    
          },
      
          { "DeleteDNSServer",
          ARGV(
          
             "NewDNSServers",
             
          0),
          ARGV(
          
             "DNSServers",
             
          0),
          ARGV(
          
          0), 
          ARGV(
          
          0), 
          ARGV(       
          
             "DNSServerList", 
             "Del", 
                      
          0),
          NULL                    
          },
      
          { "GetDNSServers",
          ARGV(
          
          0),
          ARGV(
          
          0),
          ARGV(
          
             "NewDNSServers",
             
          0), 
          ARGV(
          
             "DNSServers", 
             
          0), 
          ARGV(       
          
             "DNSServerList", 
             "Get", 
                      
          0),
          NULL                    
          },
      
          { "GetHostName",
          ARGV(
          
          0),
          ARGV(
          
          0),
          ARGV(
          
             "NewHostName",
             
          0), 
          ARGV(
          
             "x_HostName", 
             
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
      
          { "SetHostName",
          ARGV(
          
             "NewHostName",
             
          0),
          ARGV(
          
             "x_HostName",
             
          0),
          ARGV(
          
          0), 
          ARGV(
          
          0), 
          ARGV(       
                      
          0),
          NULL                    
          },
                          
      { 0, 0, 0, 0}
      };
#else
struct action actions_Layer3Forwarding[];
struct action actions_LANHostConfig[];
struct action actions_EthernetLink[];
struct action actions_IPConnection[];
struct action actions_PPPConnection[];
struct action actions_WANCommonInterface[];
#endif
  
struct action actions_General[];
struct action actions_FirewallConfig[];
struct action actions_RouterConfig[];
struct action actions_WLANConfig[];
struct action actions_DeviceSecurity[];
struct action actions_WLANAuthentication[];
struct action actions_PrinterStatus[];
struct variable variables_WLANAuthentication11b[];
struct action actions_Language[];
//struct action actions_SNMP[];

struct svcLink svcLinks[] = {            
           {"General", 	"urn:schemas-upnp-org:service:General:1", variables_General, actions_General},
           {"LANHostConfig", "urn:schemas-upnp-org:service:Layer3Forwarding:1", variables_LANHostConfig, actions_LANHostConfig},
           {"Layer3Forwarding", "urn:schemas-upnp-org:service:LANHostConfigManagement:0.8", variables_Layer3Forwarding, actions_Layer3Forwarding},
           {"WANCommonInterface", "urn:schemas-upnp-org:service:WANCommonInterfaceConfig:1", variables_WANCommonInterface, actions_WANCommonInterface},           
           {"IPConnection", "urn:schemas-upnp-org:service:WANIPConnection:1", variables_IPConnection, actions_IPConnection},
           {"PPPConnection", "urn:schemas-upnp-org:service:WANPPPConnection:1", variables_PPPConnection, actions_PPPConnection},
           {"EthernetLink", "urn:schemas-upnp-org:service:WANEthernetLinkConfig:1", variables_EthernetLink, actions_EthernetLink},
           {"FirewallConfig", "urn:schemas-upnp-org:service:FirewallConfig:1", variables_FirewallConfig, actions_FirewallConfig},
           {"RouterConfig", "urn:schemas-upnp-org:service:RouterConfig:1", variables_RouterConfig, actions_RouterConfig},           
           {"WLANConfig11a", "urn:schemas-upnp-org:service:WLANConfiguration:1", variables_WLANConfig11a, actions_WLANConfig},
           {"DeviceSecurity11a", "urn:schemas-upnp-org:service:DeviceSecurity:1", variables_DeviceSecurity11a, actions_DeviceSecurity},
           {"WLANAuthentication11a", "urn:schemas-upnp-org:service:WLANAuthentication:1", variables_WLANAuthentication11a, actions_WLANAuthentication},
           {"WLANConfig11b", "urn:schemas-upnp-org:service:WLANConfiguration:1", variables_WLANConfig11b, actions_WLANConfig},
           {"DeviceSecurity11b", "urn:schemas-upnp-org:service:DeviceSecurity:1", variables_DeviceSecurity11b, actions_DeviceSecurity},
           {"WLANAuthentication11b", "urn:schemas-upnp-org:service:WLANAuthentication:1", variables_WLANAuthentication11b, actions_WLANAuthentication},         
           {"PrinterStatus", "urn:schemas-upnp-org:service:PrinterStatus:1", variables_PrinterStatus, actions_PrinterStatus},
           {"LANGUAGE", "urn:schemas-upnp-org:service:LANGUAGE:1", variables_Language, actions_Language},
//           {"SNMP", "urn:schemas-upnp-org:service:SNMP:1", variables_SNMP, actions_SNMP},
           {0, 0, 0}
      };
	
