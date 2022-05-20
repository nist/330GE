/*general*/
var nav;
var change;
var keyPressed;
var wItem;
var ip = "";
var chanList = 0;
var wep1, wep2, wep3, wep4;
var varload=0;
var final_flag = 0;
if (navigator.appName.indexOf("Microsoft")== -1)
{nav = true;
}
else
{nav = false;
document.onkeydown = MicrosoftEventHandler_KeyDown;
}
function NetscapeEventHandler_KeyDown(e)
{keyPressed = e.which;
if (keyPressed<48 && keyPressed!=16)
{keyPressed = 0;
}
return true;
}
function IEKey()
{window.top.pageChanged = 1;
return (event.keyCode);
}
function NSKey()
{window.top.pageChanged = 1;
return(0);
}
function changeDate()
{window.top.pageChanged = 1;
return true;
}
function MicrosoftEventHandler_KeyDown()
{return true;
}
function loadMode_SP()
{/* Transfer NATEnabled, wan_proto to x_Mode, x_Connection Rule */
document.form.next_host.value = location.host;
if (document.form.wan_nat_x.value == '1')
{document.form.x_Mode[0].checked = true;
document.form.x_Mode[1].checked = false;
}
else if (document.form.wan_route_x.value == 'IP_Routed')
{document.form.x_Mode[0].checked = false;
document.form.x_Mode[1].checked = false;
}
else
{document.form.x_Mode[0].checked = false;
document.form.x_Mode[1].checked = true;
}
}
function saveMode_SP(o)
{if (document.form.x_Mode[1].checked == true) 
{document.form.wan_nat_x.value = '0';
document.form.wan_route_x.value = 'IP_Bridged';
document.form.wl_mode_ex.value = 'ap'
document.form.wl_guest_enable.value = '0';
document.form.mssid_enable.value = '0';
document.form.wl_individual_ure.value='0';
}
else if (document.form.x_Mode[0].checked == true) 
{document.form.wan_nat_x.value = '1';
document.form.wan_route_x.value = 'IP_Routed';
document.form.wan_proto.value = 'dhcp';
document.form.wl_mode_ex.value = 'ap'
document.form.wl_guest_enable.value = '0';
document.form.mssid_enable.value = '0';
document.form.wl_individual_ure.value='1';
}
document.form.action_mode.value = "  Save  ";
document.form.submit();
}
function inputCtrl(o, flag)
{if (flag == 0)
{o.disabled = 1;
o.style.backgroundColor = "gray";
}
else
{o.disabled = 0;
o.style.backgroundColor = "white";
}
}
function inputCtrl1(o, flag)
{if (flag == 0)
{o.disabled = 1;
o.style.backgroundColor = "gray";
}
else
{o.disabled = 0;
o.style.backgroundColor = "white";
}
}
function str2val(v)
{for(i=0; i<v.length; i++)
{if (v.charAt(i) !='0') break;
}
return v.substring(i);
}
function inputRCtrl1(o, flag)
{if (flag == 0)
{o[0].disabled = 1;
o[1].disabled = 1;
}
else
{o[0].disabled = 0;
o[1].disabled = 0;
}
}
function inputRCtrl2(o, flag)
{if (flag == 0)
{o[0].checked = true;
o[1].checked = false;
}
else
{o[0].checked = false;
o[1].checked = true;
}
}
function buttonOver(o)
{o.style.backgroundColor = "#FFFFCC";
}
function buttonOut(o) {
o.style.backgroundColor = "#C0C0C0";
}
function checkPass(o, o1, o2)
{if (o1.value==o2.value && validate_string(o1) && validate_string(o2))
{window.top.pageChanged=0;
document.form.action_mode.value = "  Save  ";
return true;
}
alert(translate_lang('JS_checkpass',document.form.preferred_lang2.value));
return false;
}
function checkPass2(o, o1, o2)
{if (o1.value==o2.value)
{document.form.action_mode.value = "  Save  ";
return true;
}
alert(translate_lang('JS_checkpass',document.form.preferred_lang2.value));
o1.focus();
return false;
}
function markGroup(o, s, c, b) {
var bFlag, cFlag;
var i;
document.form.group_id.value = s;
bFlag = 0;
cFlag = 0;
i = 0;
if (b == " Add ")
{if (s=='RBRList') 
{if (document.form.wl_wdsnum_x_0.value >= c) cFlag=1;
else if (!validate_hwaddr(document.form.wl_wdslist_x_0)) return false;
else if (document.form.wl_wdslist_x_0.value=="") bFlag=1;
else if (!validate_duplicate(document.form.RBRList_s, document.form.wl_wdslist_x_0.value, 12, 0)) return false;
}
else if (s == 'ACLList') 
{if (document.form.wl_macnum_x_0.value >= c) cFlag=1;
else if (!validate_hwaddr(document.form.wl_maclist_x_0)) return false;
else if (document.form.wl_maclist_x_0.value=="") bFlag=1;
else if (!validate_duplicate(document.form.ACLList_s, document.form.wl_maclist_x_0.value, 12, 0)) return false;
}
else if (s == 'MFList') 
{if (document.form.wl_macnum_x_0.value >= c) cFlag=1;
else if (!validate_hwaddr(document.form.macfilter_list_x_0)) return false;
else if (document.form.macfilter_list_x_0.value=="") bFlag=1;
else if (!validate_duplicate(document.form.MFList_s, document.form.macfilter_list_x_0.value, 12, 0)) return false;
}
else if (s=='ManualDHCPList')
{if (!validate_ipaddr_final(document.form.dhcp_start, 'dhcp_start') ||
!validate_ipaddr_final(document.form.dhcp_end, 'dhcp_end') ||
!validate_ipaddr_final(document.form.dhcp_gateway_x, 'dhcp_gateway_x') ||
!validate_ipaddr_final(document.form.dhcp_dns1_x, 'dhcp_dns1_x') ||
!validate_ipaddr_final(document.form.dhcp_wins_x, 'dhcp_wins_x') ) return false;
if (intoa(document.form.dhcp_start.value)>intoa(document.form.dhcp_end.value))
{tmp = document.form.dhcp_start.value;
document.form.dhcp_start.value = document.form.dhcp_end.value;
document.form.dhcp_end.value = tmp;
}
if (document.form.dhcp_staticnum_x_0.value >= c) cFlag=1;
else if (!validate_hwaddr(document.form.dhcp_staticmac_x_0) ||
!validate_ipaddr(document.form.dhcp_staticip_x_0, "staticip") ||
!validate_ipaddr2(document.form.dhcp_staticip_x_0)) return false;
else if (document.form.dhcp_staticmac_x_0.value=="" ||
document.form.dhcp_staticip_x_0.value=="") bFlag=1;
else if (!validate_duplicate(document.form.ManualDHCPList_s, document.form.dhcp_staticmac_x_0.value, 12, 0)) return false;
else if (!validate_duplicate(document.form.ManualDHCPList_s, document.form.dhcp_staticip_x_0.value, 15, 14)) return false;
}
else if (s=='PPPoERouteList')
{if (document.form.PPPConnection_PPPoERouteCount_0.value > c) cFlag=1;
else if (!validate_ipaddr(document.form.PPPConnection_x_PPPoEIP_0, "") ||
!validate_range(document.form.PPPConnection_x_PPPoEMask_0, 0, 32)) return false;
else if (document.form.PPPConnection_x_PPPoEIP_0.value=="" ||
document.form.PPPConnection_x_PPPoEMask_0.value=="") bFlag=1;
}
else if (s=='GWStatic')
{if (document.form.sr_num_x_0.value > c) cFlag=1;
else if (!validate_ipaddr(document.form.sr_ipaddr_x_0, "") ||
!validate_ipaddr(document.form.sr_netmask_x_0, "") ||
!validate_ipaddr(document.form.sr_gateway_x_0, "")) return false;
else if (document.form.sr_ipaddr_x_0.value=="" ||
document.form.sr_netmask_x_0.value=="" ||
document.form.sr_gateway_x_0.value=="") bFlag=1;
else if (!validate_duplicate_noalert(document.form.GWStatic_s, document.form.sr_ipaddr_x_0.value, 15, 0) &&
!validate_duplicate_noalert(document.form.GWStatic_s, document.form.sr_netmask_x_0.value, 15, 17) &&
!validate_duplicate_noalert(document.form.GWStatic_s, document.form.sr_gateway_x_0.value, 15, 34) &&
!validate_duplicate(document.form.GWStatic_s, document.form.sr_if_x_0.value, 3, 56)
) return false;
}
else if (s=='VSList')
{if (window.top.isModel()!="WL520" && window.top.isModel()!="SnapAP")
{if (document.form.vts_num_x_0.value >= c) cFlag=1;
else if (document.form.vts_ipaddr_x_0.value=="") bFlag=1;
else if (document.form.vts_proto_x_0.value == "OTHER")
{if (!validate_ipaddr(document.form.vts_ipaddr_x_0, "") ||
!validate_portrange(document.form.vts_port_x_0, "") ||
!validate_range(document.form.vts_protono_x_0, 0, 255)) return false;
else if (document.form.vts_protono_x_0.value=="") bFlag=1;
document.form.vts_port_x_0.value = "";
}
else
{if (!validate_ipaddr(document.form.vts_ipaddr_x_0, "") ||
!validate_portrange(document.form.vts_port_x_0, "") ||
!validate_range_sp(document.form.vts_lport_x_0, 1, 65535)) return false;
else if (document.form.vts_port_x_0.value=="") bFlag=1;
else if (!validate_duplicate_noalert(document.form.VSList_s, document.form.vts_port_x_0.value,11, 0) &&
!validate_duplicate_noalert(document.form.VSList_s, document.form.vts_ipaddr_x_0.value, 15, 12) &&
!validate_duplicate(document.form.VSList_s, document.form.vts_proto_x_0.value, 5, 35)
)
return false;
document.form.vts_protono_x_0.value = "";
}
}
else
{if (document.form.vts_num_x_0.value >= c) cFlag=1;
else if (document.form.vts_ipaddr_x_0.value=="") bFlag=1;
else
{if (!validate_ipaddr(document.form.vts_ipaddr_x_0, "") ||
!validate_portrange(document.form.vts_port_x_0, "")) return false;
else if (document.form.vts_port_x_0.value=="") bFlag=1;
else
{for(i=0; i< document.form.VSList_s.options.length; i++)
{if(entry_cmp(document.form.VSList_s.options[i].text.substring(35).toLowerCase(), document.form.vts_proto_x_0.value.toLowerCase(), 5)==0)
{if(!(portrange_min(document.form.vts_port_x_0.value, 11) > portrange_max(document.form.VSList_s.options[i].text.substring(0).toLowerCase(), 11) ||
portrange_max(document.form.vts_port_x_0.value, 11) < portrange_min(document.form.VSList_s.options[i].text.substring(0).toLowerCase(), 11))
)
{alert(translate_lang("JS_duplicate",document.form.preferred_lang.value));
return false;
}
if(entry_cmp(document.form.VSList_s.options[i].text.substring(12).toLowerCase(), document.form.vts_ipaddr_x_0.value.toLowerCase(), 15)==0
)
{if(document.form.vts_lport_x_0.value.length!=0)
{if(entry_cmp(document.form.VSList_s.options[i].text.substring(28).toLowerCase(), "", 5)==0)
{if(!(portrange_min(document.form.vts_lport_x_0.value, 5) > portrange_max(document.form.VSList_s.options[i].text.substring(0).toLowerCase(), 11) ||
portrange_max(document.form.vts_lport_x_0.value, 5) < portrange_min(document.form.VSList_s.options[i].text.substring(0).toLowerCase(), 11))
)
{alert(translate_lang("JS_duplicate",document.form.preferred_lang.value));
return false;
}
}
else
{if(portrange_min(document.form.vts_lport_x_0.value,5) == portrange_min(document.form.VSList_s.options[i].text.substring(28).toLowerCase(), 5))
{alert(translate_lang("JS_duplicate",document.form.preferred_lang.value));
return false;
}
}
}
else
{if(entry_cmp(document.form.VSList_s.options[i].text.substring(28).toLowerCase(), "", 5)==0)
{if(!(portrange_min(document.form.vts_port_x_0.value, 11) > portrange_max(document.form.VSList_s.options[i].text.substring(0).toLowerCase(), 11) ||
portrange_max(document.form.vts_port_x_0.value, 11) < portrange_min(document.form.VSList_s.options[i].text.substring(0).toLowerCase(), 11))
)
{alert(translate_lang("JS_duplicate",document.form.preferred_lang.value));
return false;
}
}
else
{if(!(portrange_min(document.form.vts_port_x_0.value, 11) > portrange_min(document.form.VSList_s.options[i].text.substring(28).toLowerCase(), 5) ||
portrange_max(document.form.vts_port_x_0.value, 11) < portrange_min(document.form.VSList_s.options[i].text.substring(28).toLowerCase(), 5))
)
{alert(translate_lang("JS_duplicate",document.form.preferred_lang.value));
return false;
}
}
}
}
}
}
}
}
}
}
else if (s=='TriggerList')
{if (document.form.autofw_num_x_0.value >= c) cFlag=1;
else if (!validate_portrange(document.form.autofw_inport_x_0, "") ||
!validate_portrange(document.form.autofw_outport_x_0, "")) return false;
else if (document.form.autofw_inport_x_0.value=="" ||
document.form.autofw_outport_x_0.value=="") bFlag=1;
else if (!validate_duplicate_noalert(document.form.TriggerList_s, document.form.autofw_outport_x_0.value, 5, 0)&&
!validate_duplicate(document.form.TriggerList_s, document.form.autofw_outproto_x_0.value, 3, 12))
return false;
}
else if (s=='WLFilterList')
{if (document.form.filter_wl_num_x_0.value >= c) cFlag=1;
else if (!validate_iprange(document.form.filter_wl_srcip_x_0, "") ||
!validate_portrange(document.form.filter_wl_srcport_x_0, "") ||
!validate_iprange(document.form.filter_wl_dstip_x_0, "") ||
!validate_portrange(document.form.filter_wl_dstport_x_0, ""))
return false;
else if (document.form.filter_wl_srcip_x_0.value=="" &&
document.form.filter_wl_srcport_x_0.value=="" &&
document.form.filter_wl_dstip_x_0.value=="" &&
document.form.filter_wl_dstport_x_0.value=="")
bFlag=1;
}
else if (s=='LWFilterList')
{if (window.top.isModel()!="WL520" && window.top.isModel()!="SnapAP")
{if (document.form.filter_lw_num_x_0.value >= c) cFlag=1;
else if (!validate_iprange(document.form.filter_lw_srcip_x_0, "") ||
!validate_portrange(document.form.filter_lw_srcport_x_0, "") ||
!validate_iprange(document.form.filter_lw_dstip_x_0, "") ||
!validate_portrange(document.form.filter_lw_dstport_x_0, ""))
return false;
else if (document.form.filter_lw_srcip_x_0.value=="" &&
document.form.filter_lw_srcport_x_0.value=="" &&
document.form.filter_lw_dstip_x_0.value=="" &&
document.form.filter_lw_dstport_x_0.value=="")
bFlag=1;
}
else
{if (document.form.filter_lw_num_x_0.value >= c) cFlag=1;
else if (!validate_iprange(document.form.filter_lw_srcip_x_0, "") ||
!validate_portrange(document.form.filter_lw_dstport_x_0, ""))
return false;
else if (document.form.filter_lw_srcip_x_0.value=="" && document.form.filter_lw_dstport_x_0.value=="")
bFlag=1;
else if (!validate_duplicate_noalert(document.form.LWFilterList_s, document.form.filter_lw_srcip_x_0.value, 15, 0) &&
!validate_duplicate_noalert(document.form.LWFilterList_s, document.form.filter_lw_dstport_x_0.value, 11, 16) &&
!validate_duplicate(document.form.LWFilterList_s, document.form.filter_lw_proto_x_0.value, 7, 28))
return false;
}
}
else if (s=='UrlList')
{if (document.form.url_num_x_0.value >= c) cFlag=1;
else if (document.form.url_keyword_x_0.value=="") bFlag=1;
else if (!validate_duplicate(document.form.UrlList_s, document.form.url_keyword_x_0.value, 32, 0))
return false;
}
}
else if (b == " SaveProfile ")
{if (s=='ProfileList') 
{if (document.form.pf1_ssid2.value!="" &&
document.form.pf2_ssid2.value!="" &&
document.form.pf3_ssid2.value!="" &&
document.form.pf4_ssid2.value!=""
)
{if (document.form.wl_ssid.value!=decodeURIComponent(document.form.pf1_ssid2.value) &&
document.form.wl_ssid.value!=decodeURIComponent(document.form.pf2_ssid2.value) &&
document.form.wl_ssid.value!=decodeURIComponent(document.form.pf3_ssid2.value) &&
document.form.wl_ssid.value!=decodeURIComponent(document.form.pf4_ssid2.value)
)
{alert(translate_lang("JS_itemlimit1",document.form.preferred_lang.value) + 4 + ' ' + translate_lang("JS_itemlimit2",document.form.preferred_lang.value));
return false;
}
}
if (document.form.exmode.value != "ap")
{inputCtrl(document.form.wl_ssid, 1);
inputCtrl(document.form.wl_ssid2, 1);
inputCtrl(document.form.wl_auth_mode, 1);
inputCtrl(document.form.wl_crypto, 1);
inputCtrl(document.form.wl_wpa_psk, 1);
inputCtrl(document.form.wl_wep_x, 1);
inputCtrl(document.form.wl_key_type, 1);
inputCtrl(document.form.wl_phrase_x, 1);
inputCtrl(document.form.wl_key1, 1);
inputCtrl(document.form.wl_key2, 1);
inputCtrl(document.form.wl_key3, 1);
inputCtrl(document.form.wl_key4, 1);
inputCtrl(document.form.wl_key, 1);
}
}
}
if (bFlag==1)
alert(translate_lang('JS_fieldblank',document.form.preferred_lang.value));
else if(cFlag==1)
alert(translate_lang("JS_itemlimit1",document.form.preferred_lang.value) + c + ' ' + translate_lang("JS_itemlimit2",document.form.preferred_lang.value));
else
{if (s=='WLFilterList')
updateDateTime("Advanced_Firewall_Content.asp");
else if (s=='LWFilterList')
updateDateTime("Advanced_Firewall_Content.asp");
else if (s=='UrlList')
updateDateTime("Advanced_URLFilter_Content.asp");
else if (s=='LocalCertDB')
window.top.pageChanged = 0;
window.top.pageChangedCount = 0;
document.form.action_mode.value = b;
return true;
}
return false;
}
function portrange_min(o, v)
{var num = 0;
var common_index = o.substring(0, v).indexOf(':');
if(common_index == -1)
num = parseInt(o.substring(0, v));
else
num = parseInt(o.substring(0, common_index));
return num;
}
function portrange_max(o, v)
{var num = 0;
var common_index = o.substring(0, v).indexOf(':');
if(common_index == -1)
num = parseInt(o.substring(0, v));
else
num = parseInt(o.substring(common_index+1, v+1));
return num;
}
function isBlank(s) {
for(i=0; i<s.length; i++) {
c=s.charAt(i);
if((c!=' ')&&(c!='\n')&&(c!='\t'))return false;}
return true;
}
function numbersonly()
{if (keyPressed>47 && keyPressed<58)
return true;
else
return false;
}
function check_ptl()
{if(keyPressed==38)
return false;
else
return true;
}
function linkOver(o)
{}
function linkOut(o) {
}
function linkTo(s)
{window.top.location=s;
}
function refreshList()
{parent.location.href = 'index.asp';
}
function onApply(o)
{parent.upperframe.location.href="Restarting.html";
parent.upperframe.location.href="Restarting.html";
}
function entry_cmp(entry, match, len)
{var j;
if(entry.length<match.length) return (1);
for(j=0; j<entry.length && j<len; j++)
{c1 = entry.charCodeAt(j);
if (j>=match.length) c2=160;
else c2 = match.charCodeAt(j);
if (c1==160) c1 = 32;
if (c2==160) c2 = 32;
if (c1>c2) return (1);
else if (c1<c2) return(-1);
}
return 0;
}
function validate_duplicate_noalert(o, v, l, off)
{var i;
for (i=0; i<o.options.length; i++)
{if (entry_cmp(o.options[i].text.substring(off).toLowerCase(), v.toLowerCase(), l)==0)
{return false;
}
}
return true;
}
function validate_duplicate(o, v, l, off)
{var i;
for (i=0; i<o.options.length; i++)
{if (entry_cmp(o.options[i].text.substring(off).toLowerCase(), v.toLowerCase(), l)==0)
{alert(translate_lang("JS_duplicate",document.form.preferred_lang.value));
return false;
}
}
return true;
}
function is_hwaddr()
{if (!nav) keyPressed = IEKey();
else keyPressed=NSKey();
if ((keyPressed>47 && keyPressed<58)||(keyPressed>64 && keyPressed<71)||(keyPressed>96 && keyPressed<103))
return true;
else if (keyPressed==0)
return true;
else
return false;
}
function validate_hwaddr(o)
{if (o.value.length == 0) return true;
if(o.value != "")
{if(o.value.length == 12)
{for(i=0; i<o.value.length; i++)
{c=o.value.charAt(i);
if (!(c>='0'&&c<='9') && !(c>='a'&&c<='f') && !(c>='A'&&c<='F'))
{alert(translate_lang('JS_validmac',document.form.preferred_lang.value));
o.value = "";
o.focus();
return false;
}
}
return true;
}
}
alert(translate_lang('JS_validmac',document.form.preferred_lang.value));
o.value = "";
o.focus();
return false;
}
function page_changed(o)
{window.top.pageChanged = 1;
}
function is_string(o)
{if (!nav) keyPressed = IEKey();
else keyPressed = NSKey();
if (keyPressed==0) return true;
else if (keyPressed>=32 && keyPressed<=126) return true;
alert(translate_lang('JS_validchar',document.form.preferred_lang.value));
return false;
}
function validate_string0(o)
{for(i=0;i<o.value.length;i++)
{c = o.value.charCodeAt(i);
if (!validate_sharechar3(c))
{if (document.form.preferred_lang.value!="DE")
alert(translate_lang('JS_validstr2',document.form.preferred_lang.value) + ' \"' + o.value.charAt(i) + '\" !');
else
alert(translate_lang('JS_validstr3',document.form.preferred_lang.value) + ' \"' + o.value.charAt(i) + '\"' + translate_lang('JS_validstr4',document.form.preferred_lang.value) + '!');
o.value="";
o.focus();
return false;
}
}
}
function validate_string(o)
{if (!o.value)
{return true;
}
if (o.value.charAt(0)=='"')
{alert(translate_lang('JS_validstr1',document.form.preferred_lang.value) + " " + '\"');
o.value="";
o.focus();
return false;
}
else
{inv="";
for(i=0; i<o.value.length; i++)
{if (o.value.charAt(i)<' '|| o.value.charAt(i)>'~' || o.value.charAt(i)=='"')
{inv = inv + o.value.charAt(i);
}
}
if (inv!="")
{if (document.form.preferred_lang.value!="DE")
alert(translate_lang('JS_validstr2',document.form.preferred_lang.value) + " " + inv + "!");
else
alert(translate_lang('JS_validstr3',document.form.preferred_lang.value) + " " + inv + translate_lang('JS_validstr4',document.form.preferred_lang.value) + '!');
o.value="";
o.focus();
return false;
}
}
return true;
}
function validate_string2(o)
{if (!validate_string(o))
{return false;
}
for(i=0;i<o.value.length;i++)
{c = o.value.charCodeAt(i);
if (!validate_sharechar(c))
{if (document.form.preferred_lang.value!="DE")
alert(translate_lang('JS_validstr2',document.form.preferred_lang.value) + ' \"' + o.value.charAt(i) + '\" !');
else
alert(translate_lang('JS_validstr3',document.form.preferred_lang.value) + ' \"' + o.value.charAt(i) + '\"' + translate_lang('JS_validstr4',document.form.preferred_lang.value) + '!');
o.value="";
o.focus();
return false;
}
}
return true;
}
function validate_string3(o)
{if (!validate_string(o))
{return false;
}
for(i=0;i<o.value.length;i++)
{c = o.value.charCodeAt(i);
if (!validate_sharechar2(c))
{if (document.form.preferred_lang.value!="DE")
alert(translate_lang('JS_validstr2',document.form.preferred_lang.value) + ' \"' + o.value.charAt(i) + '\" !');
else
alert(translate_lang('JS_validstr3',document.form.preferred_lang.value) + ' \"' + o.value.charAt(i) + '\"' + translate_lang('JS_validstr4',document.form.preferred_lang.value) + '!');
o.value="";
o.focus();
return false;
}
}
return true;
}
function validate_string4(o)
{for(i=0;i<o.value.length;i++)
{c = o.value.charCodeAt(i);
if (!validate_sharechar3(c))
{if (document.form.preferred_lang.value!="DE")
alert(translate_lang('JS_validstr2',document.form.preferred_lang.value) + ' \"' + o.value.charAt(i) + '\" !');
else
alert(translate_lang('JS_validstr3',document.form.preferred_lang.value) + ' \"' + o.value.charAt(i) + '\"' + translate_lang('JS_validstr4',document.form.preferred_lang.value) + '!');
o.value="";
o.focus();
return false;
}
}
if (document.form.current_page.value == "Advanced_Wireless_Content.asp")
{if (document.form.wl_ssid.value == "")
{o.value = "default";
o.focus();
return false;
}
else if (document.form.wl_ssid.value == decodeURIComponent(document.form.wl_guest_ssid_2.value) &&
document.form.exmode.value == "ap"
)
{o.value = decodeURIComponent(document.form.wl_guest_ssid_2.value)+"2";
if (o.value.length>32)
o.value=o.value.substring(0, 31)+"2";
o.focus();
return false;
}
}
else if (document.form.current_page.value == "Advanced_WirelessGuest_Content.asp")
{if (document.form.wl_guest_ssid_1.value == "")
{o.value = "guest";
o.focus();
return false;
}
else if (document.form.wl_guest_ssid_1.value == decodeURIComponent(document.form.wl_ssid_pri.value))
{document.form.wl_guest_ssid_1.value = decodeURIComponent(document.form.wl_ssid_pri.value)+"2";
if (document.form.wl_guest_ssid_1.value.length>32)
document.form.wl_guest_ssid_1.value=document.form.wl_guest_ssid_1.value.substring(0, 31)+"2";
o.focus();
return false;
}
}
else if (document.form.current_page.value == "Advanced_WAdvanced_Content.asp")
{if (document.form.wl_guest_ssid_1.value == "")
{o.value = "guest";
o.focus();
return false;
}
else if (document.form.wl_guest_ssid_1.value == decodeURIComponent(document.form.wl_ssid_pri.value) &&
document.form.wl_mode_ex.value != "re"
)
{document.form.wl_guest_ssid_1.value = decodeURIComponent(document.form.wl_ssid_pri.value)+"2";
if (document.form.wl_guest_ssid_1.value.length>32)
document.form.wl_guest_ssid_1.value=document.form.wl_guest_ssid_1.value.substring(0, 31)+"2";
o.focus();
return false;
}
}
return true;
}
function validate_sharechar(ch)
{if (ch>=48&&ch<=57) return true;
if (ch>=97&&ch<=122) return true;
if (ch>=65&&ch<=90) return true;
if (ch==45) return true;
return false;
}
function validate_sharechar2(ch)
{if (ch>=48&&ch<=57) return true;
else if (ch>=97&&ch<=122) return true;
else if (ch>=65&&ch<=90) return true;
else if (ch==45 || ch==46) return true;
return false;
}
function validate_sharechar3(ch)
{if (ch>=32&&ch<=126) return true;
return false;
}
function validate_string_length(o, len)
{if (o.value.length<len)
{alert(translate_lang('JS_validstrlen',document.form.preferred_lang.value)+len.toString()+translate_lang('JS_validstrlen2',document.form.preferred_lang.value));
o.value="00000000";
o.focus();
return false;
}
return true;
}
function is_number(o)
{if (!nav) keyPressed = IEKey();
else keyPressed = NSKey();
if (keyPressed==0) return true;
if (keyPressed>47 && keyPressed<58)
{if (keyPressed==48 && o.length == 0) return false;
return true;
}
else
{return false;
}
}
function validate_range(o, min, max)
{for(i=0; i<o.value.length; i++)
{if (o.value.charAt(i)<'0' || o.value.charAt(i)>'9')
{alert(translate_lang("JS_validrange",document.form.preferred_lang.value) +  min + ' , ' + max + '.');
o.value = min;
o.focus();
return false;
}
}
if(o.value<min || o.value>max)
{alert(translate_lang("JS_validrange",document.form.preferred_lang.value) +  min + ' , ' + max + '.');
o.value = min;
o.focus();
return false;
}
else
{o.value = str2val(o.value);
if(o.value=="") o.value="0";
return true;
}
}
function validate_range_sp(o, min, max)
{if (o.value.length==0) return true;
if(o.value<min || o.value>max)
{alert(translate_lang("JS_validrange",document.form.preferred_lang.value) +  min + ' to ' + max + '.');
o.value = min;
o.focus();
return false;
}
else
{o.value = str2val(o.value);
if(o.value=="") o.value="0";
return true;
}
}
function change_ipaddr(o)
{}
function is_ipaddr(o)
{if (!nav) keyPressed = IEKey();
else keyPressed=NSKey();
if (keyPressed==0)
{return true;
}
if (o.value.length>=16) return false;
if ((keyPressed>47 && keyPressed<58))
{j = 0;
for(i=0; i<o.value.length; i++)
{if (o.value.charAt(i)=='.')
{j++;
}
}
if (j<3 && i>=3)
{if (o.value.charAt(i-3)!='.' && o.value.charAt(i-2)!='.' && o.value.charAt(i-1)!='.')
{o.value = o.value + '.';
}
}
return true;
}
else if (keyPressed == 46)
{j = 0;
for(i=0; i<o.value.length; i++)
{if (o.value.charAt(i)=='.')
{j++;
}
}
if (o.value.charAt(i-1)=='.' || j==3)
{return false;
}
return true;
}
else
{return false;
}
return false;
}
function intoa(ip)
{n=0;
vip=0;
for(i=0;i<ip.length;i++)
{c = ip.charAt(i);
if (c == '.')
{vip = vip * 256 + n;
n = 0;
}
else if(c>='0' && c<='9')
{n = n*10 + (c-'0');
}
}
vip = vip*256 + n;
return(vip);
}
function requireWANIP(v)
{if (v=='wan_ipaddr' || v=='wan_netmask' || v=='lan_ipaddr' ||
v=='lan_netmask' || v=='lan1_ipaddr' || v=='lan1_netmask')
{if(document.form.wan_proto.value=="static" || document.form.wan_proto.value=="pptp") return 1;
else return 0;
}
else return 0;
}
function matchSubnet2(ip1, sb1, ip2, sb2)
{nsb1 = intoa(sb1.value);
nsb2 = intoa(sb2.value);
if(nsb1<nsb2) nsb=nsb1;
else nsb=nsb2;
if ((intoa(ip1)&nsb)==(intoa(ip2)&nsb)) return 1;
else return 0;
}
function validate_ipaddr2(o, v)
{num = -1;
pos = 0;
if (o.value.length==0) 
{alert(translate_lang('JS_fieldblank',document.form.preferred_lang.value));
o.focus();
return false;
}
for(i=0; i<o.value.length; i++) {
c=o.value.charAt(i);
if(c>='0' && c<='9')
{if ( num==-1 )
{num = (c-'0');
}
else
{num = num*10 + (c-'0');
}
}
else
{if ( num<0 || num>255 || c!='.')
{alert(o.value + translate_lang('JS_validip',document.form.preferred_lang.value));
o.value = "";
o.focus();
return false;
}
if (pos==0) v1=num;
else if (pos==1) v2=num;
else if (pos==2) v3=num;
num = -1;
pos++;
}
}
if (pos!=3 || num<0 || num>255)
{alert(o.value + translate_lang('JS_validip',document.form.preferred_lang.value));
o.value = "";
o.focus();
return false;
}
else v4=num;
return true;
}
function validate_ipaddr(o, v)
{if(final_flag) return true;
num = -1;
pos = 0;
if (o.value.length==0) 
{if (v=='dhcp_start' || v=='dhcp_end' || v=='wan_ipaddr' || v=='dhcp1_start' || v=='dhcp1_end' ||
v=='lan_ipaddr' || v=='lan_netmask' || v=='lan1_ipaddr' || v=='lan1_netmask')
{alert(translate_lang('JS_fieldblank',document.form.preferred_lang.value));
if (v=='wan_ipaddr')
{document.form.wan_ipaddr.value = "10.1.1.1";
document.form.wan_netmask.value = "255.0.0.0";
}
else if (v=='lan_ipaddr')
{document.form.lan_ipaddr.value = "192.168.1.1";
document.form.lan_netmask.value = "255.255.255.0";
}
else if (v=='lan1_ipaddr')
{document.form.lan1_ipaddr.value = "192.168.168.1";
document.form.lan1_netmask.value = "255.255.255.0";
}
else if (v=='lan_netmask') document.form.lan_netmask.value = "255.255.255.0";
else if (v=='lan1_netmask') document.form.lan1_netmask.value = "255.255.255.0";
else if (v=='dhcp_start') document.form.dhcp_start.value = document.form.dhcp_end.value;
else if (v=='dhcp_end') document.form.dhcp_end.value = document.form.dhcp_start.value;
else if (v=='dhcp1_start') document.form.dhcp1_start.value = document.form.dhcp1_end.value;
else if (v=='dhcp1_end') document.form.dhcp1_end.value = document.form.dhcp1_start.value;
o.focus();
return false;
}
else return true;
}
if(v=='wan_ipaddr' && document.form.wan_netmask.value=="")
document.form.wan_netmask.value="255.255.255.0";
for(i=0; i<o.value.length; i++) {
c=o.value.charAt(i);
if(c>='0' && c<='9')
{if ( num==-1 )
{num = (c-'0');
}
else
{num = num*10 + (c-'0');
}
}
else
{if ( num<0 || num>255 || c!='.')
{alert(o.value + translate_lang('JS_validip',document.form.preferred_lang.value));
o.value = "";
o.focus();
return false;
}
if (pos==0) v1=num;
else if (pos==1) v2=num;
else if (pos==2) v3=num;
num = -1;
pos++;
}
}
if (pos!=3 || num<0 || num>255)
{alert(o.value + translate_lang('JS_validip',document.form.preferred_lang.value));
o.value = "";
o.focus();
return false;
}
else v4=num;
if (v=='dhcp_start' || v=='dhcp_end' || v=='wan_ipaddr' || v=='dhcp1_start' || v=='dhcp1_end' || v=='lan_ipaddr' || v=='lan1_ipaddr' || v=='staticip')
{if((v!='wan_ipaddr') && (v1==255||v2==255||v3==255||v4==255||v1==0||v4==0||v1==127||v1==224))
{alert(o.value + translate_lang('JS_validip',document.form.preferred_lang.value));
o.value = "";
o.focus();
return false;
}
if (requireWANIP(v) && (
(v=='wan_ipaddr' &&  matchSubnet2(o.value, document.form.wan_netmask, document.form.lan_ipaddr.value, document.form.lan_netmask)) ||
(v=='lan_ipaddr' &&  matchSubnet2(o.value, document.form.lan_netmask, document.form.wan_ipaddr.value, document.form.wan_netmask)) ||
(window.top.isModel3() == 'WL-330gE'  && v=='wan_ipaddr' && matchSubnet2(o.value, document.form.wan_netmask, document.form.lan1_ipaddr.value, document.form.lan1_netmask)) ||
(window.top.isModel3() == 'WL-330gE'  && v=='lan1_ipaddr' && matchSubnet2(o.value, document.form.lan1_netmask, document.form.wan_ipaddr.value, document.form.wan_netmask))
))
{alert(o.value + translate_lang('JS_validip',document.form.preferred_lang.value));
if (v=='wan_ipaddr')
{document.form.wan_ipaddr.value = "10.1.1.1";
document.form.wan_netmask.value = "255.0.0.0";
}
else if(v=='lan_ipaddr')
{document.form.lan_ipaddr.value = "192.168.1.1";
document.form.lan_netmask.value = "255.255.255.0";
}
else if(v=='lan1_ipaddr')
{document.form.lan1_ipaddr.value = "192.168.168.1";
document.form.lan1_netmask.value = "255.255.255.0";
}
o.focus();
return false;
}
}
else if(v=='lan_netmask' || v=='lan1_netmask')
{if(v1==255&&v2==255&&v3==255&&v4==255)
{alert(o.value + translate_lang('JS_validip',document.form.preferred_lang.value));
o.value = "";
o.focus();
return false;
}
var nm_valid_flag=false;
var nm=intoa(o.value), nm_valid;
var rem=1;
for(nm_valid=0xffffffff; nm_valid!=0; nm_valid-=rem,rem*=2)
{if (nm==nm_valid)
{nm_valid_flag=true;
break;
}
}
if (!nm_valid_flag)
{alert(o.value + translate_lang('JS_validip',document.form.preferred_lang.value));
o.value = "255.255.255.0";
o.focus();
return false;
}
}
if (requireWANIP(v) && (
(v=='wan_netmask' &&  matchSubnet2(document.form.wan_ipaddr.value, o, document.form.lan_ipaddr.value, document.form.lan_netmask)) ||
(v=='lan_netmask' &&  matchSubnet2(document.form.lan_ipaddr.value, o, document.form.wan_ipaddr.value, document.form.wan_netmask)) ||
(window.top.isModel3() == 'WL-330gE'  && v=='wan_netmask' &&  matchSubnet2(document.form.wan_ipaddr.value, o, document.form.lan_ipaddr.value, document.form.lan_netmask)) ||
(window.top.isModel3() == 'WL-330gE' && v=='lan1_netmask' &&  matchSubnet2(document.form.lan1_ipaddr.value, o, document.form.wan_ipaddr.value, document.form.wan_netmask))
))
{alert(o.value + translate_lang('JS_validip',document.form.preferred_lang.value));
if (v=='wan_netmask')
{document.form.wan_ipaddr.value = "10.1.1.1";
document.form.wan_netmask.value = "255.0.0.0";
}
else if(v=='lan_netmask')
{document.form.lan_ipaddr.value = "192.168.1.1";
document.form.lan_netmask.value = "255.255.255.0";
}
else if(v=='lan1_netmask')
{document.form.lan1_ipaddr.value = "192.168.168.1";
document.form.lan1_netmask.value = "255.255.255.0";
}
o.focus();
return false;
}
if(v=='lan1_ipaddr')
{if((window.top.isModel3() == 'WL550gE'||window.top.isModel3() == 'WL-330gE') && matchSubnet(document.form.lan_ipaddr.value, document.form.lan1_ipaddr.value, 3))
{alert(o.value + translate_lang('JS_validip',document.form.preferred_lang.value));
o.value = "";
o.focus();
return false;
}
}
else if(v=='lan_ipaddr' && window.top.isBand() != 'b')
{if((window.top.isModel3() == 'WL550gE'||window.top.isModel3() == 'WL-330gE') && matchSubnet(document.form.lan_ipaddr.value, document.form.lan1_ipaddr.value, 3))
{alert(o.value + translate_lang('JS_validip',document.form.preferred_lang.value));
o.value = "";
o.focus();
return false;
}
}
o.value = v1 + "." + v2 + "." + v3 + "." + v4;
if ((v1 > 0) && (v1 < 127)) mask = "255.0.0.0";
else if ((v1 > 127) && (v1 < 192)) mask = "255.255.0.0";
else if ((v1 > 191) && (v1 < 224)) mask = "255.255.255.0";
else mask = "0.0.0.0";
if (v=='wan_ipaddr' && document.form.wan_netmask.value=="")
{document.form.wan_netmask.value = mask;
}
else if (v=='lan_ipaddr' && document.form.lan_netmask.value=="" )
{document.form.lan_netmask.value = mask;
}
else if (v=='dhcp_start')
{if (!matchSubnet(document.form.lan_ipaddr.value, document.form.dhcp_start.value, 3))
{alert(o.value + translate_lang('JS_validip',document.form.preferred_lang.value));
o.focus();
return false;
}
if (intoa(o.value)>intoa(document.form.dhcp_end.value))
{tmp = document.form.dhcp_start.value;
document.form.dhcp_start.value = document.form.dhcp_end.value;
document.form.dhcp_end.value = tmp;
}
}
else if (v=='dhcp_end')
{if (!matchSubnet(document.form.lan_ipaddr.value, document.form.dhcp_end.value, 3))
{alert(o.value + translate_lang('JS_validip',document.form.preferred_lang.value));
o.focus();
return false;
}
if (intoa(document.form.dhcp_start.value)>intoa(o.value))
{tmp = document.form.dhcp_start.value;
document.form.dhcp_start.value = document.form.dhcp_end.value;
document.form.dhcp_end.value = tmp;
}
}
else if (v=='lan1_ipaddr')
{if(document.form.lan1_netmask.value=="" )
document.form.lan1_netmask.value = mask;
}
else if (v=='dhcp1_start')
{if (!matchSubnet(document.form.lan1_ipaddr.value, document.form.dhcp1_start.value, 3))
{alert(o.value + translate_lang('JS_validip',document.form.preferred_lang.value));
o.focus();
return false;
}
if (intoa(o.value)>intoa(document.form.dhcp1_end.value))
{tmp = document.form.dhcp1_start.value;
document.form.dhcp1_start.value = document.form.dhcp1_end.value;
document.form.dhcp1_end.value = tmp;
}
}
else if (v=='dhcp1_end')
{if (!matchSubnet(document.form.lan1_ipaddr.value, document.form.dhcp1_end.value, 3))
{alert(o.value + translate_lang('JS_validip',document.form.preferred_lang.value));
o.focus();
return false;
}
if (intoa(document.form.dhcp1_start.value)>intoa(o.value))
{tmp = document.form.dhcp1_start.value;
document.form.dhcp1_start.value = document.form.dhcp1_end.value;
document.form.dhcp1_end.value = tmp;
}
}
return true;
}
function validate_ipaddr_final(o, v)
{num = -1;
pos = 0;
if (o.value.length==0) 
{if (v=='dhcp_start' || v=='dhcp_end' || v=='wan_ipaddr' || v=='dhcp1_start' || v=='dhcp1_end' ||
v=='lan_ipaddr' || v=='lan_netmask' || v=='lan1_ipaddr' || v=='lan1_netmask')
{alert(translate_lang('JS_fieldblank',document.form.preferred_lang.value));
if (v=='wan_ipaddr')
{document.form.wan_ipaddr.value = "10.1.1.1";
document.form.wan_netmask.value = "255.0.0.0";
}
else if (v=='lan_ipaddr')
{document.form.lan_ipaddr.value = "192.168.1.1";
document.form.lan_netmask.value = "255.255.255.0";
}
else if (v=='lan1_ipaddr')
{document.form.lan1_ipaddr.value = "192.168.168.1";
document.form.lan1_netmask.value = "255.255.255.0";
}
else if (v=='lan_netmask') document.form.lan_netmask.value = "255.255.255.0";
else if (v=='lan1_netmask') document.form.lan1_netmask.value = "255.255.255.0";
o.focus();
return false;
}
else return true;
}
if(v=='wan_ipaddr' && document.form.wan_netmask.value=="")
document.form.wan_netmask.value="255.255.255.0";
for(i=0; i<o.value.length; i++) {
c=o.value.charAt(i);
if(c>='0' && c<='9')
{if ( num==-1 )
{num = (c-'0');
}
else
{num = num*10 + (c-'0');
}
}
else
{if ( num<0 || num>255 || c!='.')
{alert(o.value + translate_lang('JS_validip',document.form.preferred_lang.value));
o.value = "";
o.focus();
return false;
}
if (pos==0) v1=num;
else if (pos==1) v2=num;
else if (pos==2) v3=num;
num = -1;
pos++;
}
}
if (pos!=3 || num<0 || num>255)
{alert(o.value + translate_lang('JS_validip',document.form.preferred_lang.value));
o.value = "";
o.focus();
return false;
}
else v4=num;
if (v=='dhcp_start' || v=='dhcp_end' || v=='dhcp1_start' || v=='wan_ipaddr' || v=='dhcp1_end' || v=='lan_ipaddr' || v=='lan1_ipaddr' || v=='staticip')
{if((v!='wan_ipaddr') && (v1==255||v2==255||v3==255||v4==255||v1==0||v4==0||v1==127||v1==224))
{alert(o.value + translate_lang('JS_validip',document.form.preferred_lang.value));
o.value = "";
o.focus();
return false;
}
if (requireWANIP(v) && (
(v=='wan_ipaddr' &&  matchSubnet2(o.value, document.form.wan_netmask, document.form.lan_ipaddr.value, document.form.lan_netmask)) ||
(v=='lan_ipaddr' &&  matchSubnet2(o.value, document.form.lan_netmask, document.form.wan_ipaddr.value, document.form.wan_netmask)) ||
(window.top.isModel3() == 'WL-330gE'  && v=='wan_ipaddr' && matchSubnet2(o.value, document.form.wan_netmask, document.form.lan1_ipaddr.value, document.form.lan1_netmask)) ||
(window.top.isModel3() == 'WL-330gE' && v=='lan1_ipaddr' && matchSubnet2(o.value, document.form.lan1_netmask, document.form.wan_ipaddr.value, document.form.wan_netmask))
))
{alert(o.value + translate_lang('JS_validip',document.form.preferred_lang.value));
if (v=='wan_ipaddr')
{document.form.wan_ipaddr.value = "10.1.1.1";
document.form.wan_netmask.value = "255.0.0.0";
}
else if(v=='lan_ipaddr')
{document.form.lan_ipaddr.value = "192.168.1.1";
document.form.lan_netmask.value = "255.255.255.0";
}
else if(v=='lan1_ipaddr')
{document.form.lan1_ipaddr.value = "192.168.168.1";
document.form.lan1_netmask.value = "255.255.255.0";
}
o.focus();
return false;
}
}
else if(v=='lan_netmask' || v=='lan1_netmask')
{if(v1==255&&v2==255&&v3==255&&v4==255)
{alert(o.value + translate_lang('JS_validip',document.form.preferred_lang.value));
o.value = "";
o.focus();
return false;
}
var nm_valid_flag=false;
var nm=intoa(o.value), nm_valid;
var rem=1;
for(nm_valid=0xffffffff; nm_valid!=0; nm_valid-=rem,rem*=2)
{if (nm==nm_valid)
{nm_valid_flag=true;
break;
}
}
if (!nm_valid_flag)
{alert(o.value + translate_lang('JS_validip',document.form.preferred_lang.value));
o.value = "255.255.255.0";
o.focus();
return false;
}
}
if (requireWANIP(v) && (
(v=='wan_netmask' &&  matchSubnet2(document.form.wan_ipaddr.value, o, document.form.lan_ipaddr.value, document.form.lan_netmask)) ||
(v=='lan_netmask' &&  matchSubnet2(document.form.lan_ipaddr.value, o, document.form.wan_ipaddr.value, document.form.wan_netmask)) ||
(window.top.isModel3() == 'WL-330gE'  && v=='wan_netmask' &&  matchSubnet2(document.form.wan_ipaddr.value, o, document.form.lan_ipaddr.value, document.form.lan_netmask)) ||
(window.top.isModel3() == 'WL-330gE' && v=='lan1_netmask' &&  matchSubnet2(document.form.lan1_ipaddr.value, o, document.form.wan_ipaddr.value, document.form.wan_netmask))
))
{alert(o.value + translate_lang('JS_validip',document.form.preferred_lang.value));
if (v=='wan_netmask')
{document.form.wan_ipaddr.value = "10.1.1.1";
document.form.wan_netmask.value = "255.0.0.0";
}
else if(v=='lan_netmask')
{document.form.lan_ipaddr.value = "192.168.1.1";
document.form.lan_netmask.value = "255.255.255.0";
}
else if(v=='lan1_netmask')
{document.form.lan1_ipaddr.value = "192.168.168.1";
document.form.lan1_netmask.value = "255.255.255.0";
}
o.focus();
return false;
}
if(v=='lan1_ipaddr')
{if((window.top.isModel3() == 'WL550gE'||window.top.isModel3() == 'WL-330gE') && matchSubnet(document.form.lan_ipaddr.value, document.form.lan1_ipaddr.value, 3))
{alert(o.value + translate_lang('JS_validip',document.form.preferred_lang.value));
o.value = "";
o.focus();
return false;
}
}
else if(v=='lan_ipaddr' && window.top.isBand() != 'b')
{if((window.top.isModel3() == 'WL550gE'||window.top.isModel3() == 'WL-330gE') && matchSubnet(document.form.lan_ipaddr.value, document.form.lan1_ipaddr.value, 3))
{alert(o.value + translate_lang('JS_validip',document.form.preferred_lang.value));
o.value = "";
o.focus();
return false;
}
}
o.value = v1 + "." + v2 + "." + v3 + "." + v4;
if ((v1 > 0) && (v1 < 127)) mask = "255.0.0.0";
else if ((v1 > 127) && (v1 < 192)) mask = "255.255.0.0";
else if ((v1 > 191) && (v1 < 224)) mask = "255.255.255.0";
else mask = "0.0.0.0";
if (v=='wan_ipaddr' && document.form.wan_netmask.value=="")
{document.form.wan_netmask.value = mask;
}
else if (v=='lan_ipaddr' && document.form.lan_netmask.value=="" )
{document.form.lan_netmask.value = mask;
}
else if (v=='dhcp_start')
{if (!matchSubnet(document.form.dhcp_end.value, document.form.dhcp_start.value, 3))
{alert(o.value + translate_lang('JS_validip',document.form.preferred_lang.value));
o.focus();
return false;
}
}
else if (v=='dhcp_end')
{if (!matchSubnet(document.form.dhcp_start.value, document.form.dhcp_end.value, 3))
{alert(o.value + translate_lang('JS_validip',document.form.preferred_lang.value));
o.focus();
return false;
}
checkSubnet();
}
else if (v=='lan1_ipaddr')
{if(document.form.lan1_netmask.value=="") document.form.lan1_netmask.value = mask;
}
/*
else if (v=='dhcp1_start')
{if (!matchSubnet(document.form.lan1_ipaddr.value, document.form.dhcp1_start.value, 3))
{alert(o.value + translate_lang('JS_validip',document.form.preferred_lang.value));
o.focus();
return false;
}
}
else if (v=='dhcp1_end')
{if (!matchSubnet(document.form.lan1_ipaddr.value, document.form.dhcp1_end.value, 3))
{alert(o.value + translate_lang('JS_validip',document.form.preferred_lang.value));
o.focus();
return false;
}
}
*/
return true;
}
function is_ipaddrport(o)
{if (!nav) keyPressed = IEKey();
else keyPressed=NSKey();
if (keyPressed==0)
{return true;
}
if ((keyPressed>47 && keyPressed<58) || keyPressed == 46 || keyPressed == 58)
{return true;
}
return false;
}
function validate_ipaddrport(o, v)
{num = -1;
pos = 0;
if (o.value.length==0)
return true;
str = o.value;
portidx = str.indexOf(":");
if (portidx!=-1)
{port = str.substring(portidx+1);
len = portidx;
if (port>65535)
{alert(port + translate_lang('JS_validport',document.form.preferred_lang.value));
o.value = "";
o.focus();
return false;
}
}
else
{len = o.value.length;
}
for(i=0; i<len; i++) {
c=o.value.charAt(i);
if(c>='0' && c<='9')
{if ( num==-1 )
{num = (c-'0');
}
else
{num = num*10 + (c-'0');
}
}
else
{if ( num<0 || num>255 || c!='.')
{alert(o.value + translate_lang('JS_validip',document.form.preferred_lang.value));
o.value = "";
o.focus();
return false;
}
num = -1;
pos++;
}
}
if (pos!=3 || num<0 || num>255)
{alert(o.value + translate_lang('JS_validip',document.form.preferred_lang.value));
o.value = "";
o.focus();
return false;
}
if (v=='ExternalIPAddress' && document.form.wan_netmask.value == '')
{document.form.wan_netmask.value = "255.255.255.0";
}
else if (v=='IPRouters' && document.form.lan_netmask.value == '')
{document.form.lan_netmask.value = "255.255.255.0";
}
return true;
}
function change_iprange(o)
{}
function is_iprange(o)
{if (!nav) keyPressed = IEKey();
else keyPressed=NSKey();
if (keyPressed==0)
{return true;
}
if (o.value.length>=15) return false;
if ((keyPressed>47 && keyPressed<58))
{j = 0;
for(i=0; i<o.value.length; i++)
{if (o.value.charAt(i)=='.')
{j++;
}
}
if (j<3 && i>=3)
{if (o.value.charAt(i-3)!='.' && o.value.charAt(i-2)!='.' && o.value.charAt(i-1)!='.')
o.value = o.value + '.';
}
return true;
}
else if (keyPressed == 46)
{j = 0;
for(i=0; i<o.value.length; i++)
{if (o.value.charAt(i)=='.')
{j++;
}
}
if (o.value.charAt(i-1)=='.' || j==3)
{return false;
}
return true;
}
else if (keyPressed == 42) /* '*' */
{return true;
}
else
{return false;
}
return false;
}
function validate_iprange(o, v)
{num = -1;
pos = 0;
if (o.value.length==0)
return true;
for(i=0; i<o.value.length; i++)
{c=o.value.charAt(i);
if(c>='0'&&c<='9')
{if ( num==-1 )
{num = (c-'0');
}
else
{num = num*10 + (c-'0');
}
}
else if (c=='*'&&num==-1)
{num = 0;
}
else
{if ( num<0 || num>255 || (c!='.'))
{alert(o.value + translate_lang('JS_validip',document.form.preferred_lang.value));
o.value = "";
o.focus();
return false;
}
num = -1;
pos++;
}
}
if (pos!=3 || num<0 || num>255)
{alert(o.value + translate_lang('JS_validip',document.form.preferred_lang.value));
o.value = "";
o.focus();
return false;
}
if (v=='ExternalIPAddress' && document.form.wan_netmask.value == '')
{document.form.wan_netmask.value = "255.255.255.0";
}
else if (v=='IPRouters' && document.form.lan_netmask.value == '')
{document.form.lan_netmask.value = "255.255.255.0";
}
return true;
}
function is_portrange(o)
{if (!nav) keyPressed = IEKey();
else keyPressed=NSKey();
if (keyPressed==0) return true;
if (o.value.length>11) return false;
if ((keyPressed>47 && keyPressed<58))
{return true;
}
else if (keyPressed == 58 && o.value.length>0) 
{for(i=0; i<o.value.length; i++)
{c=o.value.charAt(i);
if (c==':' || c=='>' || c=='<' || c=='=')
return false;
}
return true;
}
else if (keyPressed==60 || keyPressed==62) 
{if (o.value.length==0)
{return true;
}
else
return false;
}
else
{return false;
}
}
function validate_portrange(o, v)
{if (o.value.length==0)
return true;
prev = -1;
num = -1;
for(i=0; i<o.value.length; i++)
{c=o.value.charAt(i);
if (c>='0'&&c<='9')
{if(num==-1) num=0;
num = num*10 + (c-'0');
}
else
{if (num>65535 || num==0 || (c!=':' && c!='>' && c!='<'))
{alert(num + translate_lang('JS_validport',document.form.preferred_lang.value));
o.value = "";
o.focus();
return false;
}
if (c=='>') prev = -2;
else if (c=='<') prev = -3;
else
{prev = num;
num = 0;
}
}
}
if ((num>65535 && prev!=-3) || (num<1&&prev!=-2) || (prev>num) || (num>=65535&&prev==-2) || (num<=1&&prev==-3))
{if (num>65535)
alert(num + translate_lang('JS_validport',document.form.preferred_lang.value));
else
alert(o.value + translate_lang('JS_validportrange',document.form.preferred_lang.value));
o.value = "";
o.focus();
return false;
}
else
{if (prev==-2)
{if (num==65535) o.value = num;
else o.value = (num+1) + ":65535";
}
else if (prev==-3)
{if (num==1) o.value = num;
else o.value = "1:" + (num-1);
}
else if (prev!=-1) o.value = prev + ":" + num;
else o.value = num;
}
return true;
}
function is_portlist(o)
{if (!nav) keyPressed = IEKey();
else keyPressed = NSKey();
if (keyPressed == 0 ) return true;
if (o.value.length>36) return false;
if ((keyPressed>47 && keyPressed<58) || keyPressed == 32)
{return true;
}
else
{return false;
}
}
function validate_portlist(o, v)
{if (o.value.length==0)
return true;
num = 0;
for(i=0; i<o.value.length; i++)
{c=o.value.charAt(i);
if (c>='0'&&c<='9')
{num = num*10 + (c-'0');
}
else
{if (num>255)
{alert(num + translate_lang('JS_validport',document.form.preferred_lang.value));
o.value = "";
o.focus();
return false;
}
num = 0;
}
}
if (num>255)
{alert(num + translate_lang('JS_validport',document.form.preferred_lang.value));
o.value = "";
o.focus();
return false;
}
return true;
}
function add_option(o, s, f)
{tail = o.options.length;
o.options[tail] = new Option(s);
o.options[tail].value = s;
if (f==1)
{o.options[tail].selected = f;
}
}
function add_option_match(o, s, f)
{tail = o.options.length;
o.options[tail] = new Option(s);
o.options[tail].value = s;
if (f==s)
{o.options[tail].selected = 1;
return(1);
}
else return(0);
}
function add_option_match_x(o, s, f)
{tail = o.options.length;
o.options[tail] = new Option(s);
o.options[tail].value = tail;
if (tail==f)
{o.options[tail].selected = 1;
return(1);
}
else return(0);
}
function add_option_x(o, i, s, f)
{tail = o.options.length;
o.options[tail] = new Option(s);
o.options[tail].value = i;
if (f==1)
{o.options[tail].selected = f;
}
}
function free_options(o)
{count = o.options.length;
for (i=0; i<count; i++)
{o.options[0].value = null;
o.options[0] = null;
}
}
function find_option(o)
{count = o.options.length;
for (i=0; i<count; i++)
{if (o.options[i].selected)
return(o.options[i].value);
}
return(null);
}
function add_options(o, arr, orig)
{for (i = 0; i < arr.length; i++)
{if (orig == arr[i])
add_option(o, arr[i], 1);
else
add_option(o, arr[i], 0);
}
}
function add_options_x(o, arr, orig)
{for (i = 0; i < arr.length; i++)
{if (orig == i)
add_option_x(o, i, arr[i], 1);
else
add_option_x(o, i, arr[i], 0);
}
}
function add_options_x2(o, varr, arr, orig)
{for (i = 0; i < arr.length; i++)
{if (orig == varr[i])
add_option_x(o, varr[i], arr[i], 1);
else
add_option_x(o, varr[i], arr[i], 0);
}
}
function rcheck(o)
{if (o[0].checked == true)
return("1");
else
return("0");
}
function SaveChannelList(o)
{count = o.options.length;
chanList = new Array;
for (i=0; i<count; i++)
{chanList[chanList.length] = o.options[i].value;
}
}
function RestoreChannelList(d, isOut, isTurbo)
{for (i=0; i< chanList.length; i++)
{if (isOut=="0"&&chanList[i]>=100) return;
if (chanList[i] == 999) return;
d[d.length] = chanList[i];
}
}
function RefreshChannelList(isOut, isTurbo)
{orig = document.form.WLANConfig11a_Channel.value;
free_options(document.form.WLANConfig11a_Channel);
if (isTurbo=="1")
{if (isOut == "1")
{items = new Array("42","50","58","152","160");
}
else
{items = new Array("42","50","58");
}
}
else
{items = new Array;
RestoreChannelList(items, isOut, isTurbo);
}
add_options(document.form.WLANConfig11a_Channel, items, orig);
}
function RefreshRateList(isTurbo)
{orig = document.form.WLANConfig11a_DataRate.value;
free_options(document.form.WLANConfig11a_DataRate);
if (isTurbo=="1")
{items = new Array("Best","12","18","24","36","48","72","96","108");
}
else
{items = new Array("Best","6","9","12","18","24","36","48","54");
}
add_options_x(document.form.WLANConfig11a_DataRate, items, orig);
}
function RefreshRateSetList(gmode, chg)
{orig = document.form.WLANConfig11b_DataRate.value;
free_options(document.form.WLANConfig11b_DataRate);
if (gmode != "3")
{if (gmode == "1")
{items = new Array("1 & 2 Mbps", "Default", "All");
}
else
{items = new Array("1 & 2 Mbps", "Default", "All");
}
if (chg) orig = 1;
}
else
{items = new Array("1 & 2 Mbps", "Default");
if (chg) orig = 1;
}
add_options_x(document.form.WLANConfig11b_DataRate, items, orig);
}
function getDateCheck(str, pos)
{if (str.charAt(pos) == '1')
return true;
else
return false;
}
function getTimeRange(str, pos)
{if (pos == 0)
return str.substring(0,2);
else if (pos == 1)
return str.substring(2,4);
else if (pos == 2)
return str.substring(4,6);
else if (pos == 3)
return str.substring(6,8);
}
function setDateCheck(d1, d2, d3, d4, d5, d6, d7)
{str = "";
if (d7.checked == true ) str = "1" + str;
else str = "0" + str;
if (d6.checked == true ) str = "1" + str;
else str = "0" + str;
if (d5.checked == true ) str = "1" + str;
else str = "0" + str;
if (d4.checked == true ) str = "1" + str;
else str = "0" + str;
if (d3.checked == true ) str = "1" + str;
else str = "0" + str;
if (d2.checked == true ) str = "1" + str;
else str = "0" + str;
if (d1.checked == true ) str = "1" + str;
else str = "0" + str;
return str;
}
function setTimeRange(sh, sm, eh, em)
{return(sh.value+sm.value+eh.value+em.value);
}
function load_wizard()
{}
function decodeSTR(o)
{num = -1;
str = "";
if (o.value.length == 0)
return str;
for(i=0; i<o.value.length; i++) {
c=o.value.charAt(i);
if(c>='0' && c<='9')
{if ( num == -1 )
{num = (c-'0');
}
else
{num = num*10 + (c-'0');
}
}
else
{if ( num<32 || num>126 || c!='#')
{alert(o.value + ' is not a valid char!');
str = "";
return str;
}
str = str + String.fromCharCode(num);
num = -1;
}
}
return str;
}
function encodeSTR(o)
{str = "";
if (o.value.length == 0)
return str;
for(i=0; i<o.value.length; i++) {
num=o.value.charCodeAt(i);
if(num>=32 && num<=126)
{str = str + num.toString() + "#";
}
else
{alert(o.value + ' is not a valid char!');
str = "";
return str;
}
}
return str;
}
function load_body()
{document.form.next_host.value = location.host;
no_pppoe_flag=0;
var i;
if (document.form.current_page.value == "Advanced_Wireless_Content.asp")
{if (document.form.wan_route_mode.value == "IP_Bridged" && document.form.exmode.value == "sta")
document.form.next_page.value = "Advanced_WAdvanced_Content.asp";
else if (document.form.wan_route_mode.value == "IP_Bridged" && document.form.exmode.value == "re")
document.form.next_page.value = "Advanced_ACL_Content.asp";
else if (document.form.wan_route_mode.value == "IP_Routed" && document.form.exmode.value == "sta")
document.form.next_page.value = "Advanced_WAdvanced_Content_GW.asp";
else if (document.form.wan_route_mode.value == "IP_Routed" && document.form.exmode.value == "re")
document.form.next_page.value = "Advanced_ACL_Content.asp";
document.form.wl_ssid.value = decodeURIComponent(document.form.wl_ssid2.value);
document.form.wl_wpa_psk.value = decodeURIComponent(document.form.wl_wpa_psk_org.value);
document.form.wl_phrase_x.value = decodeURIComponent(document.form.wl_phrase_x_org.value);
document.form.wl_key1.value = decodeURIComponent(document.form.wl_key1_org.value);
document.form.wl_key2.value = decodeURIComponent(document.form.wl_key2_org.value);
document.form.wl_key3.value = decodeURIComponent(document.form.wl_key3_org.value);
document.form.wl_key4.value = decodeURIComponent(document.form.wl_key4_org.value);
if (document.form.exmode.value != "ap")
{if (document.getElementById("profile_list")!=null)
{if(nav)
{if (document.form.preferred_lang.value=="TW")
document.getElementById("profile_list").innerHTML = "請新增無線網路至列表中！";
else if (document.form.preferred_lang.value=="CN")
document.getElementById("profile_list").innerHTML = "请新增无线网络至列表中！";
else if (document.form.preferred_lang.value=="DE")
document.getElementById("profile_list").innerHTML = "Es ist kein Profil in der Liste!";
else if (document.form.preferred_lang.value=="JP")
document.getElementById("profile_list").innerHTML = "リストにプロファイルがありません！";
}
else
{if (document.form.preferred_lang.value=="TW")
document.getElementById("profile_list").innerText = "請新增無線網路至列表中！";
else if (document.form.preferred_lang.value=="CN")
document.getElementById("profile_list").innerText = "请新增无线网络至列表中！";
else if (document.form.preferred_lang.value=="DE")
document.getElementById("profile_list").innerText = "Es ist kein Profil in der Liste!";
else if (document.form.preferred_lang.value=="JP")
document.getElementById("profile_list").innerText = "リストにプロファイルがありません！";
}
}
i=0;
if (document.form.pf1_ssid2.value!="" && document.form.pf_ssid!=null)
{if (document.form.pf_ssid[i]==null)
document.form.pf_ssid.value=decodeURIComponent(document.form.pf1_ssid2.value);
else
document.form.pf_ssid[i].value=decodeURIComponent(document.form.pf1_ssid2.value);
i++;
}
if (document.form.pf2_ssid2.value!="" && (document.form.pf_ssid!=null || document.form.pf_ssid[i]!=null))
{if (document.form.pf_ssid[i]==null)
document.form.pf_ssid.value=decodeURIComponent(document.form.pf2_ssid2.value);
else
document.form.pf_ssid[i].value=decodeURIComponent(document.form.pf2_ssid2.value);
i++;
}
if (document.form.pf3_ssid2.value!="" && (document.form.pf_ssid!=null || document.form.pf_ssid[i]!=null))
{if (document.form.pf_ssid[i]==null)
document.form.pf_ssid.value=decodeURIComponent(document.form.pf3_ssid2.value);
else
document.form.pf_ssid[i].value=decodeURIComponent(document.form.pf3_ssid2.value);
i++;
}
if (document.form.pf4_ssid2.value!="" && (document.form.pf_ssid!=null || document.form.pf_ssid[i]!=null))
{if (document.form.pf_ssid[i]==null)
document.form.pf_ssid.value=decodeURIComponent(document.form.pf4_ssid2.value);
else
document.form.pf_ssid[i].value=decodeURIComponent(document.form.pf4_ssid2.value);
}
opts=document.form.wl_ap_ssid.options;
for(i=0;i<opts.length;i++)
opts[i].text=opts[i].value;
if (document.form.wl_connect_selected[0].checked == true)
{inputCtrl(document.form.wl_ap_order, 0);
inputCtrl(document.form.wl_ap_ssid, 1);
inputRCtrl2(document.form.wl_connect_none_default, 1);
inputRCtrl1(document.form.wl_connect_none_default, 0);
}
else
{inputCtrl(document.form.wl_ap_order, 1);
inputCtrl(document.form.wl_ap_ssid, 0);
inputRCtrl1(document.form.wl_connect_none_default, 1);
}
}
if (document.form.exmode.value == "re" || document.form.exmode.value == "sta")
{wl_auth_mode_reconf();
}
wl_auth_mode_change(1);
if (document.form.wl_gmode_protection_x.value == "1")
document.form.wl_gmode_check.checked = true;
else
document.form.wl_gmode_check.checked = false;
if (window.top.isBand() == 'b' || document.form.wl_gmode.value=="0")
inputCtrl(document.form.wl_gmode_check, 0);
}
else if (document.form.current_page.value == "Advanced_ACL_Content.asp")
{if (document.form.wan_route_mode.value == "IP_Bridged" && document.form.exmode.value == "re")
document.form.next_page.value = "Advanced_WAdvanced_Content_URE.asp";
else if (document.form.wan_route_mode.value == "IP_Routed" && document.form.exmode.value == "re")
document.form.next_page.value = "Advanced_WAdvanced_Content_GW_URE.asp";
}
else if (document.form.current_page.value == "Advanced_WSecurity_Content.asp" )
{if (document.form.wan_route_mode.value == "IP_Routed" && document.form.exmode.value == "ap")
document.form.next_page.value = "Advanced_WirelessGuest_Content.asp";
else if (document.form.wan_route_mode.value == "IP_Routed" && document.form.exmode.value == "re")
document.form.next_page.value = "Advanced_WAdvanced_Content_GW_URE.asp";
else if (document.form.wan_route_mode.value == "IP_Routed" && document.form.exmode.value == "sta")
document.form.next_page.value = "Advanced_WAdvanced_Content_GW.asp";
else if (document.form.wan_route_mode.value == "IP_Bridged" && document.form.exmode.value == "ap")
document.form.next_page.value = "Advanced_WAdvanced_Content.asp";
else if (document.form.wan_route_mode.value == "IP_Bridged" && document.form.exmode.value == "re")
document.form.next_page.value = "Advanced_WAdvanced_Content_URE.asp";
document.form.wl_radius_key.value = decodeURIComponent(document.form.wl_radius_key_org.value);
}
else if (document.form.current_page.value == "Advanced_WirelessGuest_Content.asp" )
{document.form.wl_guest_ssid_1.value = decodeURIComponent(document.form.wl_guest_ssid_2.value);
document.form.wl_guest_wpa_psk_1.value = decodeURIComponent(document.form.wl_guest_wpa_psk_1_org.value);
document.form.wl_guest_phrase_x_1.value = decodeURIComponent(document.form.wl_guest_phrase_x_1_org.value);
document.form.wl_guest_key1_1.value = decodeURIComponent(document.form.wl_guest_key1_1_org.value);
document.form.wl_guest_key2_1.value = decodeURIComponent(document.form.wl_guest_key2_1_org.value);
document.form.wl_guest_key3_1.value = decodeURIComponent(document.form.wl_guest_key3_1_org.value);
document.form.wl_guest_key4_1.value = decodeURIComponent(document.form.wl_guest_key4_1_org.value);
final_flag = 1; 
wl_auth_mode_reconf_guest();
wl_auth_mode_change_guest(1);
if (document.form.exmode.value == "re")
{document.form.wl_guest_enable[0].disabled=1;
document.form.wl_guest_enable[1].disabled=1;
}
}
else if (document.form.current_page.value == "Advanced_WMode_Content.asp")
{if (window.top.isCard()=='ralink')
{document.form.wl_mode_x.options[1].value = null;
document.form.wl_mode_x.options[1] = null;
change_wireless_bridge2(document.form.wl_mode_x.value,
rcheck(document.form.wl_wdsapply_x),
1, 0);
}
else if (window.top.isModel2()=='WL530')
{change_wireless_bridge2(document.form.wl_mode_x.value,
rcheck(document.form.wl_wdsapply_x),
1, 0);
}
else
{change_wireless_bridge(document.form.wl_mode_x.value,
rcheck(document.form.wl_wdsapply_x),
"0", 0);
}
}
else if (document.form.current_page.value == "Advanced_WAdvanced_Content.asp")
{document.form.wl_guest_ssid_1.value = decodeURIComponent(document.form.wl_guest_ssid_2.value);
document.form.wl_guest_wpa_psk_1.value = decodeURIComponent(document.form.wl_guest_wpa_psk_1_org.value);
document.form.wl_guest_phrase_x_1.value = decodeURIComponent(document.form.wl_guest_phrase_x_1_org.value);
document.form.wl_guest_key1_1.value = decodeURIComponent(document.form.wl_guest_key1_1_org.value);
document.form.wl_guest_key2_1.value = decodeURIComponent(document.form.wl_guest_key2_1_org.value);
document.form.wl_guest_key3_1.value = decodeURIComponent(document.form.wl_guest_key3_1_org.value);
document.form.wl_guest_key4_1.value = decodeURIComponent(document.form.wl_guest_key4_1_org.value);
if (window.top.isCard()!='ralink')
{wl_rate_change();
final_flag = 1; 
wl_auth_mode_change_guest(1);
wl_mode_ex_change();
}
if (window.top.isModel()!="WL520" && window.top.isModel()!="SnapAP" && window.top.isModel()!="WL300" && window.top.isModel()!="WL331" && window.top.isCard()!='ralink')
{if (window.top.isBand() == 'b') inputCtrl(document.form.wl_frameburst, 0);
document.form.wl_radio_date_x_Sun.checked = getDateCheck(document.form.wl_radio_date_x.value, 0);
document.form.wl_radio_date_x_Mon.checked = getDateCheck(document.form.wl_radio_date_x.value, 1);
document.form.wl_radio_date_x_Tue.checked = getDateCheck(document.form.wl_radio_date_x.value, 2);
document.form.wl_radio_date_x_Wed.checked = getDateCheck(document.form.wl_radio_date_x.value, 3);
document.form.wl_radio_date_x_Thu.checked = getDateCheck(document.form.wl_radio_date_x.value, 4);
document.form.wl_radio_date_x_Fri.checked = getDateCheck(document.form.wl_radio_date_x.value, 5);
document.form.wl_radio_date_x_Sat.checked = getDateCheck(document.form.wl_radio_date_x.value, 6);
document.form.wl_radio_time_x_starthour.value = getTimeRange(document.form.wl_radio_time_x.value, 0);
document.form.wl_radio_time_x_startmin.value = getTimeRange(document.form.wl_radio_time_x.value, 1);
document.form.wl_radio_time_x_endhour.value = getTimeRange(document.form.wl_radio_time_x.value, 2);
document.form.wl_radio_time_x_endmin.value = getTimeRange(document.form.wl_radio_time_x.value, 3);
}
}
else if (document.form.current_page.value == "Advanced_LANWAN_Content.asp")
{final_flag = 1; 
document.form.wan_pppoe_username.value = decodeURIComponent(document.form.wan_pppoe_username_org.value);
document.form.wan_pppoe_passwd.value = decodeURIComponent(document.form.wan_pppoe_passwd_org.value);
if (!(document.form.exmode.value == "ap" || document.form.exmode.value == "sta"))
{inputCtrl(document.form.wan_hostname, 0);
inputCtrl(document.form.wan_hwaddr_x, 0);
inputCtrl(document.form.wan_heartbeat_x, 0);
}
if (document.form.exmode.value == "re")
{if (document.form.wan_proto.value!="dhcp" && document.form.wan_proto.value!="static")
document.form.wan_proto.value="dhcp";
document.form.wan_proto.options[2].value = null;
document.form.wan_proto.options[2] = null;
no_pppoe_flag=1;
document.form.wan_proto.options[2].value = null;
document.form.wan_proto.options[2] = null;
document.form.wan_proto.options[2].value = null;
document.form.wan_proto.options[2] = null;
}
if (document.form.wan_dnsenable_x[0].checked == true)
{inputCtrl(document.form.wan_dns1_x, 0);
inputCtrl(document.form.wan_dns2_x, 0);
}
else
{inputCtrl(document.form.wan_dns1_x, 1);
inputCtrl(document.form.wan_dns2_x, 1);
}
if (window.top.isModel()!="SnapAP")
{change_wan_type(document.form.wan_proto.value);
change_wan_type2(document.form.wan_proto.value);
if (document.form.wan_pppoe_txonly_x.value == "1")
{document.form.wan_pppoe_idletime_check.checked = true;
}
if (window.top.isModel2()=='WL520' && window.top.isModel3()!='WL550gE' && window.top.isModel3()!='WL-330gE')
{document.form.wan_mode_x.options[2].value = null;
document.form.wan_mode_x.options[2] = null;
}
}
if (window.top.isModel()!="WL500" || window.top.supportCDMA()!="1")
{if (document.form.exmode.value != "re")
{if (no_pppoe_flag==1)
{document.form.wan_proto.options[4].value = null;
document.form.wan_proto.options[4] = null;
}
else
{document.form.wan_proto.options[5].value = null;
document.form.wan_proto.options[5] = null;
}
}
else
{document.form.wan_proto.options[2].value = null;
document.form.wan_proto.options[2] = null;
}
}
}
else if (document.form.current_page.value == "Advanced_PortTrigger_Content.asp")
{wItem = new Array(
new Array("Quicktime 4 Client", "554", "TCP", "6970:32000", "UDP"),
new Array("Real Audio", "7070", "TCP", "6970:7170", "UDP"));
free_options(document.form.TriggerKnownApps);
add_option(document.form.TriggerKnownApps, "User Defined", 1);
for (i = 0; i < wItem.length; i++)
{add_option(document.form.TriggerKnownApps, wItem[i][0], 0);
}
}
else if (document.form.current_page.value == "Advanced_VirtualServer_Content.asp")
{wItem = new Array(
new Array("FTP", "20:21", "TCP"),
new Array("TELNET", "23", "TCP"),
new Array("SMTP", "25", "TCP"),
new Array("DNS", "53", "UDP"),
new Array("FINGER", "79", "TCP"),
new Array("HTTP", "80", "TCP"),
new Array("POP3", "110", "TCP"),
new Array("SNMP", "161", "UDP"),
new Array("SNMP TRAP", "162", "UDP"));
free_options(document.form.KnownApps);
add_option(document.form.KnownApps, "User Defined", 1);
for (i = 0; i < wItem.length; i++)
{add_option(document.form.KnownApps, wItem[i][0], 0);
}
}
else if (document.form.current_page.value == "Advanced_BasicFirewall_Content.asp")
{change_firewall(rcheck(document.form.fw_enable_x));
}
else if (document.form.current_page.value == "Advanced_Firewall_Content.asp")
{wItem = new Array(
new Array("WWW", "80", "TCP"),
new Array("TELNET", "23", "TCP"),
new Array("FTP", "20:21", "TCP"));
if (window.top.isModel()!="WL520" && window.top.isModel()!="SnapAP")
{free_options(document.form.WLKnownApps);
add_option(document.form.WLKnownApps, "User Defined", 1);
for (i = 0; i < wItem.length; i++)
{add_option(document.form.WLKnownApps, wItem[i][0], 0);
}
}
free_options(document.form.LWKnownApps);
add_option(document.form.LWKnownApps, "User Defined", 1);
for (i = 0; i < wItem.length; i++)
{add_option(document.form.LWKnownApps, wItem[i][0], 0);
}
if (window.top.isModel()!="WL520" && window.top.isModel()!="SnapAP")
{document.form.filter_wl_date_x_Sun.checked = getDateCheck(document.form.filter_wl_date_x.value, 0);
document.form.filter_wl_date_x_Mon.checked = getDateCheck(document.form.filter_wl_date_x.value, 1);
document.form.filter_wl_date_x_Tue.checked = getDateCheck(document.form.filter_wl_date_x.value, 2);
document.form.filter_wl_date_x_Wed.checked = getDateCheck(document.form.filter_wl_date_x.value, 3);
document.form.filter_wl_date_x_Thu.checked = getDateCheck(document.form.filter_wl_date_x.value, 4);
document.form.filter_wl_date_x_Fri.checked = getDateCheck(document.form.filter_wl_date_x.value, 5);
document.form.filter_wl_date_x_Sat.checked = getDateCheck(document.form.filter_wl_date_x.value, 6);
document.form.filter_wl_time_x_starthour.value = getTimeRange(document.form.filter_wl_time_x.value, 0);
document.form.filter_wl_time_x_startmin.value = getTimeRange(document.form.filter_wl_time_x.value, 1);
document.form.filter_wl_time_x_endhour.value = getTimeRange(document.form.filter_wl_time_x.value, 2);
document.form.filter_wl_time_x_endmin.value = getTimeRange(document.form.filter_wl_time_x.value, 3);
}
document.form.filter_lw_date_x_Sun.checked = getDateCheck(document.form.filter_lw_date_x.value, 0);
document.form.filter_lw_date_x_Mon.checked = getDateCheck(document.form.filter_lw_date_x.value, 1);
document.form.filter_lw_date_x_Tue.checked = getDateCheck(document.form.filter_lw_date_x.value, 2);
document.form.filter_lw_date_x_Wed.checked = getDateCheck(document.form.filter_lw_date_x.value, 3);
document.form.filter_lw_date_x_Thu.checked = getDateCheck(document.form.filter_lw_date_x.value, 4);
document.form.filter_lw_date_x_Fri.checked = getDateCheck(document.form.filter_lw_date_x.value, 5);
document.form.filter_lw_date_x_Sat.checked = getDateCheck(document.form.filter_lw_date_x.value, 6);
document.form.filter_lw_time_x_starthour.value = getTimeRange(document.form.filter_lw_time_x.value, 0);
document.form.filter_lw_time_x_startmin.value = getTimeRange(document.form.filter_lw_time_x.value, 1);
document.form.filter_lw_time_x_endhour.value = getTimeRange(document.form.filter_lw_time_x.value, 2);
document.form.filter_lw_time_x_endmin.value = getTimeRange(document.form.filter_lw_time_x.value, 3);
change_internet_firewall("1");
}
else if (document.form.current_page.value == "Advanced_URLFilter_Content.asp")
{document.form.url_date_x_Sun.checked = getDateCheck(document.form.url_date_x.value, 0);
document.form.url_date_x_Mon.checked = getDateCheck(document.form.url_date_x.value, 1);
document.form.url_date_x_Tue.checked = getDateCheck(document.form.url_date_x.value, 2);
document.form.url_date_x_Wed.checked = getDateCheck(document.form.url_date_x.value, 3);
document.form.url_date_x_Thu.checked = getDateCheck(document.form.url_date_x.value, 4);
document.form.url_date_x_Fri.checked = getDateCheck(document.form.url_date_x.value, 5);
document.form.url_date_x_Sat.checked = getDateCheck(document.form.url_date_x.value, 6);
document.form.url_time_x_starthour.value = getTimeRange(document.form.url_time_x.value, 0);
document.form.url_time_x_startmin.value = getTimeRange(document.form.url_time_x.value, 1);
document.form.url_time_x_endhour.value = getTimeRange(document.form.url_time_x.value, 2);
document.form.url_time_x_endmin.value = getTimeRange(document.form.url_time_x.value, 3);
}
else if (document.form.current_page.value == "Advanced_DHCP_Content.asp" ||
document.form.current_page.value == "Advanced_RDHCP_Content.asp")
{final_flag = 1; 
}
else if (document.form.current_page.value == "Advanced_DMZDHCP_Content.asp")
{}
else if (document.form.current_page.value == "Advanced_GWStaticRoute_Content.asp")
{if (document.form.preferred_lang.value == "TW")
document.form.next_page.value = "Advanced_DDNS_Content_TW.asp";
else if (document.form.preferred_lang.value == "CN")
document.form.next_page.value = "Advanced_DDNS_Content_CN.asp";
}
else if (document.form.current_page.value == "Advanced_DDNS_Content.asp")
{document.form.ddns_username_x.value = decodeURIComponent(document.form.ddns_username_x_org.value);
document.form.ddns_passwd_x.value = decodeURIComponent(document.form.ddns_passwd_x_org.value);
}
else if (document.form.current_page.value == "Advanced_APLAN_Content.asp")
{if (document.form.lan_proto_x[0].checked == true)
{inputCtrl(document.form.lan_ipaddr, 0);
inputCtrl(document.form.lan_netmask, 0);
inputCtrl(document.form.lan_gateway, 0);
}
else
{inputCtrl(document.form.lan_ipaddr, 1);
inputCtrl(document.form.lan_netmask, 1);
inputCtrl(document.form.lan_gateway, 1);
}
}
else if (document.form.current_page.value == "Advanced_DMZIP11g_Content.asp" ||
document.form.current_page.value == "Advanced_DMZIP_Content.asp")
{change_wireless_firewall();
}
else if (document.form.current_page.value == "Advanced_DMZIP_Content.asp")
{if (document.form.LANHostConfig_lan_proto_x[0].checked == true)
{inputCtrl(document.form.lan_ipaddr, 0);
inputCtrl(document.form.lan_netmask, 0);
inputCtrl(document.form.lan_gateway, 0);
}
else
{inputCtrl(document.form.lan_ipaddr, 1);
inputCtrl(document.form.lan_netmask, 1);
inputCtrl(document.form.lan_gateway, 1);
}
}
else if (document.form.current_page.value == "Main_GStatus_Content.asp")
{if (document.form.exmode.value != "ap")
{document.form.PPPConnection_x_WANAction_button.disabled = 1;
document.form.PPPConnection_x_WANAction_button1.disabled = 1;
}
}
else if (document.form.current_page.value == "Advanced_WirelessSTAStatus_Content.asp")
{document.form.ssid.value = decodeURIComponent(document.form.ssid2.value);
}
else if (document.form.current_page.value == "Advanced_WirelessUREStatus_Content.asp")
{document.form.ssid.value = decodeURIComponent(document.form.ssid2.value);
}
else if (document.form.current_page.value == "Main_AStatus_Content_AP.asp")
{if (document.form.lan_ipaddr_t.value == "")
{document.form.lan_ipaddr_t.value=document.form.lan_ipaddr.value;
document.form.lan_netmask_t.value=document.form.lan_netmask.value;
document.form.lan_gateway_t.value=document.form.lan_gateway.value;
}
}
change = 0;
}
function unload_body()
{return true;
}
function change_internet_firewall(r)
{if (window.top.isModel()!="WL520" && window.top.isModel()!="SnapAP" && window.top.isFlash() != '2MB')
{if (r=="1")
{document.form.InternetFirewall_img.src = "graph/internet_some.gif";
}
else
{document.form.InternetFirewall_img.src = "graph/internet_none.gif";
}
}
}
function change_wireless_firewall()
{if (window.top.isModel() == "WL600")
{if (document.form.FirewallConfig_DmzEnable[0].checked == true )
{if (document.form.FirewallConfig_DmzDevices.value == "Both")
document.form.WirelessFirewall_img.src = "graph/wf_both.gif";
else if (document.form.FirewallConfig_DmzDevices.value == "802.11a only")
document.form.WirelessFirewall_img.src = "graph/wf_a.gif";
else if (document.form.FirewallConfig_DmzDevices.value == "802.11g only")
document.form.WirelessFirewall_img.src = "graph/wf_g.gif";
}
else  document.form.WirelessFirewall_img.src = "graph/wf_none.gif";
}
else
{if (document.form.FirewallConfig_DmzEnable[0].checked == true)
{document.form.WirelessFirewall_img.src = "graph/wf_g.gif";
}
else document.form.WirelessFirewall_img.src = "graph/wf_none.gif";
}
}
function change_firewall(r)
{if (r == "0")
{inputRCtrl1(document.form.misc_http_x, 0);
inputRCtrl2(document.form.misc_http_x, 1);
inputCtrl(document.form.misc_httpport_x, 0);
if (window.top.isModel()!="WL520" && window.top.isModel()!="SnapAP")
{if (window.top.isFlash() != '2MB' && window.top.isModel()!= "WL331" )
{inputRCtrl1(document.form.misc_lpr_x, 0);
inputRCtrl2(document.form.misc_lpr_x, 1);
}
inputRCtrl1(document.form.misc_ping_x, 0);
inputRCtrl2(document.form.misc_ping_x, 1);
}
}
else
{inputRCtrl1(document.form.misc_http_x, 1);
inputCtrl(document.form.misc_httpport_x, 1);
if (window.top.isModel()!="WL520" && window.top.isModel()!="SnapAP")
{if (window.top.isFlash() != '2MB' && window.top.isModel()!="WL331")
{inputRCtrl1(document.form.misc_lpr_x, 1);
}
inputRCtrl1(document.form.misc_ping_x, 1);
}
}
}
function change_wireless_bridge(m, a, r, mflag)
{if (a=="0" && r == "0" && mflag != 1)
{document.form.wl_mode_x.value = "0";
m = "0";
}
if (m == "0")
{wdsimage = "wds_ap";
inputRCtrl2(document.form.wl_wdsapply_x, 1);
inputRCtrl1(document.form.wl_wdsapply_x, 0);
}
else
{if (a=="0" && r == "0")
{inputRCtrl2(document.form.wl_wdsapply_x, 0);
}
inputRCtrl1(document.form.wl_wdsapply_x, 1);
if (m == "1")
wdsimage = "wds_wds";
else
wdsimage = "wds_mixed";
if (a == "0")
{if (r == "0")
wdsimage += "_connect";
else
wdsimage += "_anony";
}
else
{if (r == "0")
wdsimage += "_connect";
else
wdsimage += "_both";
}
if (document.form.wl_channel.value == "0")
{alert(translate_lang('JS_fixchannel',document.form.preferred_lang.value));
document.form.wl_channel.options[0].selected = 0;
document.form.wl_channel.options[1].selected = 1;
}
}
wdsimage = "graph/" + wdsimage + ".gif";
if (window.top.isFlash() != '2MB')
document.form.WirelessBridge_img.src = wdsimage;
}
function change_wireless_bridge2(m, a, r, mflag)
{if (a=="0" && r == "0" && mflag != 1)
{document.form.wl_mode_x.value = "0";
m = "0";
}
if (m == "0")
{wdsimage = "wds_ap";
inputRCtrl2(document.form.wl_wdsapply_x, 1);
inputRCtrl1(document.form.wl_wdsapply_x, 0);
}
else
{if (a=="0" && r == "0")
{inputRCtrl2(document.form.wl_wdsapply_x, 0);
}
inputRCtrl1(document.form.wl_wdsapply_x, 1);
if (m == "1")
wdsimage = "wds_wds";
else
wdsimage = "wds_mixed";
if (a == "0")
{if (r == "0")
wdsimage += "_connect";
else
wdsimage += "_anony";
}
else
{if (r == "0")
wdsimage += "_connect";
else
wdsimage += "_both";
}
if (document.form.wl_channel.value == "0")
{alert(translate_lang('JS_fixchannel',document.form.preferred_lang.value));
document.form.wl_channel.options[0].selected = 0;
document.form.wl_channel.options[1].selected = 1;
}
}
wdsimage = "graph/" + wdsimage + ".gif";
if (window.top.isFlash() != '2MB' && window.top.isModel2()!='WL530')
document.form.WirelessBridge_img.src = wdsimage;
}
function onSubmit()
{change = 0;
window.top.pageChanged = 0;
window.top.pageChangedCount = 0;
if (document.form.current_page.value == "Advanced_APLAN_Content.asp")
{if(!validate_ipaddr_final(document.form.lan_ipaddr, 'lan_ipaddr') ||
!validate_ipaddr_final(document.form.lan_netmask, 'lan_netmask')) return false;
}
else if (document.form.current_page.value == "Advanced_LANWAN_Content.asp")
{if (document.form.wan_proto.value=="static" ||
document.form.wan_proto.value=="pptp")
{if(!wan_netmask_check(document.form.wan_netmask))
return false;
if(!validate_ipaddr_final(document.form.wan_ipaddr, 'wan_ipaddr') ||
!validate_ipaddr_final(document.form.wan_netmask, 'wan_netmask') ||
!validate_ipaddr_final(document.form.wan_gateway, 'wan_gateway')) return false;
}
if (document.form.wan_dnsenable_x[0].checked != true)
{if(!validate_ipaddr_final(document.form.wan_dns1_x, 'wan_dns1_x') ||
!validate_ipaddr_final(document.form.wan_dns2_x, 'wan_dns2_x')) return false;
if(document.form.wan_dns1_x.value=="" && document.form.wan_dns2_x.value=="")
{if (document.form.wan_dns1_x.value=="")
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
if(!validate_ipaddr_final(document.form.lan_ipaddr, 'lan_ipaddr') ||
!validate_ipaddr_final(document.form.lan_netmask, 'lan_netmask')) return false;
checkSubnet();
if (document.form.exmode.value != "ap")
{inputCtrl(document.form.wan_hostname, 1);
inputCtrl(document.form.wan_hwaddr_x, 1);
inputCtrl(document.form.wan_heartbeat_x, 1);
}
if (document.form.exmode.value == "ap" &&
document.form.wan_nat.value == "1" &&
document.form.wan_proto.value != "dhcp"
)
alert(translate_lang('JS_GW_AP',document.form.preferred_lang.value));
}
else if (document.form.current_page.value == "Advanced_RLANWAN_Content.asp")
{checkSubnet();
}
else if (document.form.current_page.value == "Advanced_DMZIP_Content.asp")
{checkDmzSubnet();
}
else if (document.form.current_page.value == "Advanced_Firewall_Content.asp")
{updateDateTime(document.form.current_page.value);
}
else if (document.form.current_page.value == "Advanced_BasicFirewall_Content.asp")
{inputRCtrl1(document.form.misc_http_x, 1);
if (window.top.isModel()!="WL520" && window.top.isModel()!="SnapAP")
{if ( window.top.isFlash() != '2MB' && window.top.isModel()!="WL331")
{inputRCtrl1(document.form.misc_lpr_x, 1);
}
inputRCtrl1(document.form.misc_ping_x, 1);
}
}
else if (document.form.current_page.value == "Advanced_URLFilter_Content.asp")
{updateDateTime(document.form.current_page.value);
}
else if (document.form.current_page.value == "Advanced_WebCam_Content.asp")
{updateDateTime(document.form.current_page.value);
}
else if (document.form.current_page.value == "Advanced_WirelessAPScan_Content.asp")
{if (document.form.apinfo_n.value!="0")
for(i=0;i<eval(document.form.apinfo_n.value);i++)
{inputCtrl(document.form.RSSID[i], 0);
inputCtrl(document.form.RSSIDE[i], 0);
}
}
else if (document.form.current_page.value == "Advanced_WAdvanced_Content.asp")
{if (!validate_string4(document.form.wl_guest_ssid_1))
return false;
else
document.form.wl_guest_ssid_2.value = encodeURIComponent(document.form.wl_guest_ssid_1.value);
updateDateTime(document.form.current_page.value);
inputCtrl(document.form.wl_guest_phrase_x_1, 1);
inputCtrl(document.form.wl_guest_wep_x_1, 1);
inputCtrl(document.form.wl_guest_key_type_1, 1);
inputCtrl(document.form.wl_guest_key1_1, 1);
inputCtrl(document.form.wl_guest_key2_1, 1);
inputCtrl(document.form.wl_guest_key3_1, 1);
inputCtrl(document.form.wl_guest_key4_1, 1);
inputCtrl(document.form.wl_guest_key_1, 1);
if (document.form.wl_mode_ex.value == "re" || document.form.wl_mode_ex.value == "sta")
{document.form.wl_guest_enable.value=0;
wl_auth_mode_reconf();
}
if (document.form.wl_mode_ex.value == "re")
{if (document.form.wan_proto.value == "pppoe")
document.form.wan_proto.value = "dhcp";
}
inputCtrl(document.form.wl_guest_wpa_psk_1_org, 0);
inputCtrl(document.form.wl_guest_phrase_x_1_org, 0);
inputCtrl(document.form.wl_guest_key1_1_org, 0);
inputCtrl(document.form.wl_guest_key2_1_org, 0);
inputCtrl(document.form.wl_guest_key3_1_org, 0);
inputCtrl(document.form.wl_guest_key4_1_org, 0);
}
else if (document.form.current_page.value == "Advanced_WMode_Content.asp")
{if (document.form.wl_mode_x.value == "0")
{inputRCtrl1(document.form.wl_wdsapply_x, 1);
inputRCtrl2(document.form.wl_wdsapply_x, 1);
}
}
else if (document.form.current_page.value == "Advanced_Wireless_Content.asp")
{if (!validate_string4(document.form.wl_ssid))
return false;
else
document.form.wl_ssid2.value = encodeURIComponent(document.form.wl_ssid.value);
if(!validate_wlphrase('WLANConfig11b', 'wl_wpa_psk', document.form.wl_wpa_psk))
return false;
inputCtrl(document.form.exmode, 0);
inputCtrl(document.form.wan_route_mode, 0);
inputCtrl(document.form.wl_phrase_x, 1);
inputCtrl(document.form.wl_wep_x, 1);
inputCtrl(document.form.wl_key_type, 1);
inputCtrl(document.form.wl_key1, 1);
inputCtrl(document.form.wl_key2, 1);
inputCtrl(document.form.wl_key3, 1);
inputCtrl(document.form.wl_key4, 1);
inputCtrl(document.form.wl_key, 1);
if (document.form.exmode.value != "ap")
{inputCtrl(document.form.wl_ap_order, 1);
inputCtrl(document.form.wl_ap_ssid, 1);
inputRCtrl1(document.form.wl_connect_none_default, 1);
}
if (document.form.exmode.value != "ap")
{inputCtrl(document.form.wl_ssid, 0);
inputCtrl(document.form.wl_ssid2, 0);
inputCtrl(document.form.wl_auth_mode, 0);
inputCtrl(document.form.wl_crypto, 0);
inputCtrl(document.form.wl_wpa_psk, 0);
inputCtrl(document.form.wl_wep_x, 0);
inputCtrl(document.form.wl_key_type, 0);
inputCtrl(document.form.wl_phrase_x, 0);
inputCtrl(document.form.wl_key1, 0);
inputCtrl(document.form.wl_key2, 0);
inputCtrl(document.form.wl_key3, 0);
inputCtrl(document.form.wl_key4, 0);
inputCtrl(document.form.wl_key, 0);
}
inputCtrl(document.form.wl_wpa_psk_org, 0);
inputCtrl(document.form.wl_phrase_x_org, 0);
inputCtrl(document.form.wl_key1_org, 0);
inputCtrl(document.form.wl_key2_org, 0);
inputCtrl(document.form.wl_key3_org, 0);
inputCtrl(document.form.wl_key4_org, 0);
}
else if (document.form.current_page.value == "Advanced_WirelessGuest_Content.asp")
{if (!validate_string4(document.form.wl_guest_ssid_1))
return false;
else
document.form.wl_guest_ssid_2.value = encodeURIComponent(document.form.wl_guest_ssid_1.value);
if (!validate_ipaddr_final(document.form.lan1_ipaddr, 'lan1_ipaddr') ||
!validate_ipaddr_final(document.form.lan1_netmask, 'lan1_netmask') ||
!validate_ipaddr_final(document.form.dhcp1_start, 'dhcp1_start') ||
!validate_ipaddr_final(document.form.dhcp1_end, 'dhcp1_end')) return false;
if (!matchSubnet(document.form.dhcp1_start.value, document.form.dhcp1_end.value, 3))
{alert(document.form.dhcp1_start.value + translate_lang('JS_validip',document.form.preferred_lang.value));
document.form.dhcp1_start.focus();
return false;
}
checkSubnetGuest();
if (intoa(document.form.dhcp1_start.value)>intoa(document.form.dhcp1_end.value))
{tmp = document.form.dhcp1_start.value;
document.form.dhcp1_start.value = document.form.dhcp1_end.value;
document.form.dhcp1_end.value = tmp;
}
inputCtrl(document.form.wl_guest_phrase_x_1, 1);
inputCtrl(document.form.wl_guest_wep_x_1, 1);
inputCtrl(document.form.wl_guest_key_type_1, 1);
inputCtrl(document.form.wl_guest_key1_1, 1);
inputCtrl(document.form.wl_guest_key2_1, 1);
inputCtrl(document.form.wl_guest_key3_1, 1);
inputCtrl(document.form.wl_guest_key4_1, 1);
inputCtrl(document.form.wl_guest_key_1, 1);
inputCtrl(document.form.wl_guest_wpa_psk_1_org, 0);
inputCtrl(document.form.wl_guest_phrase_x_1_org, 0);
inputCtrl(document.form.wl_guest_key1_1_org, 0);
inputCtrl(document.form.wl_guest_key2_1_org, 0);
inputCtrl(document.form.wl_guest_key3_1_org, 0);
inputCtrl(document.form.wl_guest_key4_1_org, 0);
}
else if (document.form.current_page.value == "Advanced_DHCP_Content.asp")
{if (!validate_ipaddr_final(document.form.dhcp_start, 'dhcp_start') ||
!validate_ipaddr_final(document.form.dhcp_end, 'dhcp_end') ||
!validate_ipaddr_final(document.form.dhcp_gateway_x, 'dhcp_gateway_x') ||
!validate_ipaddr_final(document.form.dhcp_dns1_x, 'dhcp_dns1_x') ||
!validate_ipaddr_final(document.form.dhcp_wins_x, 'dhcp_wins_x') ) return false;
if (intoa(document.form.dhcp_start.value)>intoa(document.form.dhcp_end.value))
{tmp = document.form.dhcp_start.value;
document.form.dhcp_start.value = document.form.dhcp_end.value;
document.form.dhcp_end.value = tmp;
}
if (!validate_string2(document.form.lan_domain))
return false;
}
return true;
}
function onSubmitCtrl(o, s)
{document.form.action_mode.value = s;
return (onSubmit());
}
function load_upload()
{varload=0;
}
function onSubmitCtrlOnly(o, s)
{if (s!='Upload' && s!='Upload1') document.form.action_mode.value = s;
if (s=='Upload1')
{if(document.form.file.value.length==0)
{alert(translate_lang('JS_fieldblank',document.form.preferred_lang.value));
document.form.file.focus();
return false;
}
if (varload)
{alert(translate_lang('JS_FirmUpgrade',document.form.preferred_lang.value));
return false;
}
else document.form.submit();
varload++;
}
return true;
}
function onSubmitApply(s)
{window.top.pageChanged = 0;
window.top.pageChangedCount = 0;
if (document.form.current_page.value == "Advanced_PortMapping_Content.asp")
{if (s == "0")
{action = document.form.IPConnection_MappedAction_0;
local = document.form.IPConnection_MappedIP_0;
port = document.form.IPConnection_MappedInPort_0;
desc = document.form.IPConnection_MappedDescript_0;
}
else if (s == "1")
{action = document.form.IPConnection_MappedAction_1;
local = document.form.IPConnection_MappedIP_1;
port = document.form.IPConnection_MappedInPort_1;
desc = document.form.IPConnection_MappedDescript_1;
}
else if (s == "2")
{action = document.form.IPConnection_MappedAction_2;
local = document.form.IPConnection_MappedIP_2;
port = document.form.IPConnection_MappedInPort_2;
desc = document.form.IPConnection_MappedDescript_2;
}
else if (s == "3")
{action = document.form.IPConnection_MappedAction_3;
local = document.form.IPConnection_MappedIP_3;
port = document.form.IPConnection_MappedInPort_3;
desc = document.form.IPConnection_MappedDescript_3;
}
else if (s == "4")
{action = document.form.IPConnection_MappedAction_4;
local = document.form.IPConnection_MappedIP_4;
port = document.form.IPConnection_MappedInPort_4;
desc = document.form.IPConnection_MappedDescript_4;
}
else if (s == "5")
{action = document.form.IPConnection_MappedAction_5;
local = document.form.IPConnection_MappedIP_5;
port = document.form.IPConnection_MappedInPort_5;
desc = document.form.IPConnection_MappedDescript_5;
}
if (action.value == "Set")
{if (!validate_ipaddr(local, "") ||
!validate_portrange(port, ""))
{return false;
}
else if (local.value=="" || port.value == "")
{alert(translate_lang('JS_ipblank1',document.form.preferred_lang.value));
return false;
}
}
document.form.action.value = action.value;
document.form.action_mode.value = action.value;
document.form.action_script.value = "portmapping.sh" + " " + action.value + " " + local.value + " " + port.value;
}
else
{document.form.action.value = "Update";
document.form.action_mode.value = "Update";
document.form.action_script.value = s;
}
return true;
}
function setup_script(s)
{if (document.form.current_page.value == "Advanced_ACL_Content.asp")
{document.form.action_script.value = s;
}
}
function change_common(o, s, v)
{change = 1;
window.top.pageChanged = 1;
if (v == "wl_auth_mode") /* Handle AuthenticationMethod Change */
{wl_auth_mode_change(0);
if (o.value == "psk")
{opts=document.form.wl_auth_mode.options;
if (opts[opts.selectedIndex].text == "WPA-Personal" || opts[opts.selectedIndex].text == "WPA-PSK")
document.form.wl_wpa_mode.value="1";
else if (opts[opts.selectedIndex].text == "WPA2-Personal" || opts[opts.selectedIndex].text == "WPA2-PSK")
document.form.wl_wpa_mode.value="2";
else if (opts[opts.selectedIndex].text == "WPA-Auto-Personal")
document.form.wl_wpa_mode.value="0";
document.form.wl_wpa_psk.focus();
}
else if (o.value == "shared" || o.value == "radius")
document.form.wl_phrase_x.focus();
}
else if (s=="WLANConfig11b" && v=="wl_afterburner")
{opts=document.form.wl_afterburner.options;
opts2=document.form.wl_wme.options;
if (opts[opts.selectedIndex].text == "Enabled" && opts2[opts2.selectedIndex].text == "Enabled")
{alert(translate_lang('JS_AB_WMM_ME',document.form.preferred_lang.value));
document.form.wl_wme.options[1-opts2.selectedIndex].selected=1;
}
}
else if (s=="WLANConfig11b" && v=="wl_wme")
{opts2=document.form.wl_afterburner.options;
opts=document.form.wl_wme.options;
if (opts[opts.selectedIndex].text == "Enabled" && opts2[opts2.selectedIndex].text == "Enabled")
{alert(translate_lang('JS_WMM_AB_ME',document.form.preferred_lang.value));
document.form.wl_afterburner.options[1-opts2.selectedIndex].selected=1;
}
}
else if (s=="WLANConfig11b" && v=="wl_gmode")
{if (window.top.isBand() == 'b' || document.form.wl_gmode.value=="0")
inputCtrl(document.form.wl_gmode_check, 0);
else
inputCtrl(document.form.wl_gmode_check, 1);
}
else if (v == "wl_mode_ex")
{wl_mode_ex_change();
}
else if (v == "wl_wep_x") /* Handle AuthenticationMethod Change */
{change_wlweptype(o, "WLANConfig11b");
}
else if (v == "wl_key_type")
{change_wlweptype(document.form.wl_wep_x, "WLANConfig11b");
}
else if (v == "wl_mode_x")
{if (window.top.isCard()=='ralink' || window.top.isModel2()=='WL530')
{change_wireless_bridge2(document.form.wl_mode_x.value,
rcheck(document.form.wl_wdsapply_x),
1, 1);
}
else change_wireless_bridge(o.value, rcheck(document.form.wl_wdsapply_x), "0", 1);
}
else if (v == "wl_crypto")
{wl_auth_mode_change(0);
}
else if (v == "wl_guest_auth_mode_1") /* Handle AuthenticationMethod Change */
{wl_auth_mode_change_guest(0);
if (o.value == "psk")
{opts=document.form.wl_guest_auth_mode_1.options;
if (opts[opts.selectedIndex].text == "WPA-Personal" || opts[opts.selectedIndex].text == "WPA-PSK")
document.form.wl_guest_wpa_mode.value="1";
else if (opts[opts.selectedIndex].text == "WPA2-Personal" || opts[opts.selectedIndex].text == "WPA2-PSK")
document.form.wl_guest_wpa_mode.value="2";
else if (opts[opts.selectedIndex].text == "WPA-Auto-Personal")
document.form.wl_guest_wpa_mode.value="0";
document.form.wl_guest_wpa_psk_1.focus();
}
else if (o.value == "shared" || o.value == "radius")
document.form.wl_guest_phrase_x_1.focus();
}
else if (v == "wl_guest_wep_x_1") /* Handle AuthenticationMethod Change */
{change_wlweptype_guest(o, "WLANConfig11b");
}
else if (v == "wl_guest_key_type_1")
{change_wlweptype_guest(document.form.wl_guest_wep_x_1, "WLANConfig11b");
}
else if (v == "wl_guest_crypto_1")
{wl_auth_mode_change_guest(0);
}
else if (v=="wan_proto")
{change_wan_type(o.value);
change_wan_type2(o.value);
}
else if (s == "FirewallConfig" && v=="DmzDevices")
{change_wireless_firewall();
}
else if (s == "FirewallConfig" && v=="WanLanDefaultAct")
{if (o.value == "DROP")
alert(translate_lang('JS_WanLanAlert',document.form.preferred_lang.value));
}
else if (s == "FirewallConfig" && v=="LanWanDefaultAct")
{if (o.value == "DROP")
alert(translate_lang('JS_WanLanAlert',document.form.preferred_lang.value));
}
else if (s=="WLANConfig11b" && v=="x_Mode11g")
{RefreshRateSetList(document.form.WLANConfig11b_x_Mode11g.value, true);
}
else if (s=="WLANConfig11b" && v=="Channel" && document.form.current_page.value=="Advanced_WMode_Content.asp")
{if (document.form.WLANConfig11b_x_APMode.value != "0" && document.form.WLANConfig11b_Channel.value == "0")
{alert(translate_lang('JS_fixchannel',document.form.preferred_lang.value));
document.form.WLANConfig11b_Channel.options[0].selected = 0;
document.form.WLANConfig11b_Channel.options[1].selected = 1;
}
}
return true;
}
function change_common_radio(o, s, v, r)
{change = 1;
window.top.pageChanged = 1;
if (v=='wl_wdsapply_x')
{if (window.top.isCard()=='ralink'  || window.top.isModel2()=='WL530' )
{change_wireless_bridge2(document.form.wl_mode_x.value,
rcheck(document.form.wl_wdsapply_x),
1, 0);
}
else change_wireless_bridge(document.form.wl_mode_x.value, r, "0", 0);
}
else if (v=='wl_lazywds')
{change_wireless_bridge(document.form.wl_mode_x.value, rcheck(document.form.wl_wdsapply_x), r, 0);
}
else if (s=="WLANConfig11b" && v=="wl_gmode_check")
{if (document.form.wl_gmode_check.checked == true)
document.form.wl_gmode_protection_x.value = "1";
else
document.form.wl_gmode_protection_x.value = "0";
}
else if (v=="wan_dnsenable_x")
{if (r == '1')
{inputCtrl(document.form.wan_dns1_x, 0);
inputCtrl(document.form.wan_dns2_x, 0);
}
else
{inputCtrl(document.form.wan_dns1_x, 1);
inputCtrl(document.form.wan_dns2_x, 1);
}
}
else if (v=="fw_enable_x")
{change_firewall(r);
}
else if (v=="lan_proto_x")
{if (r == '1')
{inputCtrl(document.form.lan_ipaddr, 0);
inputCtrl(document.form.lan_netmask, 0);
inputCtrl(document.form.lan_gateway, 0);
}
else
{inputCtrl(document.form.lan_ipaddr, 1);
inputCtrl(document.form.lan_netmask, 1);
inputCtrl(document.form.lan_gateway, 1);
}
}
else if (s=='FirewallConfig' && v=='DmzEnable')
{change_wireless_firewall();
}
else if (s=='FirewallConfig' && v=='WanLanFirewallEnable')
{change_internet_firewall("1");
}
else if (s=="PPPConnection" && v=="wan_pppoe_idletime")
{if (document.form.wan_pppoe_idletime_check.checked)
{document.form.wan_pppoe_txonly_x.value = "1";
}
else
{document.form.wan_pppoe_txonly_x.value = "0";
}
}
else if (s=="PPPConnection" && v=="x_IdleTime1")
{if (document.form.PPPConnection_x_IdleTime1_check.checked)
{document.form.PPPConnection_x_IdleTxOnly1.value = "1";
}
else
{document.form.PPPConnection_x_IdleTxOnly1.value = "0";
}
}
else if (s=="PPPConnection" && v=="x_MultiPPPoEEnable1")
{if (document.form.PPPConnection_x_MultiPPPoEEnable1[0].checked == true)
{flag=1;
}
else
{flag=0;
}
inputCtrl(document.form.PPPConnection_x_UserName1, flag);
inputCtrl(document.form.PPPConnection_x_Password1, flag);
inputCtrl(document.form.PPPConnection_x_IdleTime1, flag);
inputCtrl(document.form.PPPConnection_x_IdleTime1_check, flag);
inputCtrl(document.form.PPPConnection_x_PPPoEMTU1, flag);
inputCtrl(document.form.PPPConnection_x_PPPoEMRU1, flag);
inputCtrl(document.form.PPPConnection_x_ServiceName1, flag);
inputCtrl(document.form.PPPConnection_x_AccessConcentrator1, flag);
}
else if (s=="PPPConnection" && v=="x_IdleTime2")
{if (document.form.PPPConnection_x_IdleTime2_check.checked)
{document.form.PPPConnection_x_IdleTxOnly2.value = "1";
}
else
{document.form.PPPConnection_x_IdleTxOnly2.value = "0";
}
}
else if (s=="PPPConnection" && v=="x_MultiPPPoEEnable2")
{if (document.form.PPPConnection_x_MultiPPPoEEnable2[0].checked == true)
{flag=1;
}
else
{flag=0;
}
inputCtrl(document.form.PPPConnection_x_UserName2, flag);
inputCtrl(document.form.PPPConnection_x_Password2, flag);
inputCtrl(document.form.PPPConnection_x_IdleTime2, flag);
inputCtrl(document.form.PPPConnection_x_IdleTime2_check, flag);
inputCtrl(document.form.PPPConnection_x_PPPoEMTU2, flag);
inputCtrl(document.form.PPPConnection_x_PPPoEMRU2, flag);
inputCtrl(document.form.PPPConnection_x_ServiceName2, flag);
inputCtrl(document.form.PPPConnection_x_AccessConcentrator2, flag);
}
else if (v == "wl_connect_selected")
{if (document.form.wl_connect_selected[0].checked == true)
{inputCtrl(document.form.wl_ap_order, 0);
inputCtrl(document.form.wl_ap_ssid, 1);
inputRCtrl2(document.form.wl_connect_none_default, 1);
inputRCtrl1(document.form.wl_connect_none_default, 0);
}
else
{inputCtrl(document.form.wl_ap_order, 1);
inputCtrl(document.form.wl_ap_ssid, 0);
inputRCtrl1(document.form.wl_connect_none_default, 1);
}
}
else if (v == "pf1_delete")
{if (document.form.pf2_delete!=null)
document.form.pf2_delete.checked=false;
if (document.form.pf3_delete!=null)
document.form.pf3_delete.checked=false;
if (document.form.pf4_delete!=null)
document.form.pf4_delete.checked=false;
}
else if (v == "pf2_delete")
{if (document.form.pf1_delete!=null)
document.form.pf1_delete.checked=false;
if (document.form.pf3_delete!=null)
document.form.pf3_delete.checked=false;
if (document.form.pf4_delete!=null)
document.form.pf4_delete.checked=false;
}
else if (v == "pf3_delete")
{if (document.form.pf1_delete!=null)
document.form.pf1_delete.checked=false;
if (document.form.pf2_delete!=null)
document.form.pf2_delete.checked=false;
if (document.form.pf4_delete!=null)
document.form.pf4_delete.checked=false;
}
else if (v == "pf4_delete")
{if (document.form.pf1_delete!=null)
document.form.pf1_delete.checked=false;
if (document.form.pf2_delete!=null)
document.form.pf2_delete.checked=false;
if (document.form.pf3_delete!=null)
document.form.pf3_delete.checked=false;
}
return true;
}
function valid_WPAPSK(o)
{if (o.value.length>=64)
{o.value = o.value.substring(0, 63);
alert(translate_lang('JS_wpapass',document.form.preferred_lang.value));
return false;
}
return true;
}
function change_wlweptype(o, s)
{if (document.form.wl_auth_mode.value!="psk")
{change = 1;
window.top.pageChanged = 1;
}
document.form.wl_phrase_x.value = "";
if (o.value=="0")
{wflag = 0;
wep = "";
document.form.wl_key1.value = wep;
document.form.wl_key2.value = wep;
document.form.wl_key3.value = wep;
document.form.wl_key4.value = wep;
}
else
{wflag = 1;
key_type = document.form.wl_key_type.value;
if (key_type=="0")
{if (o.value=="1")
{wep = "0000000000";
}
else  if (o.value=="2")
{wep = "00000000000000000000000000";
}
else
{wep = "00000000000000000000000000000000";
}
}
else
{if (o.value=="1")
{wep = "00000";
}
else  if (o.value=="2")
{wep = "0000000000000";
}
else
{wep = "0000000000000000";
}
}
is_wlphrase("WLANConfig11b", "wl_phrase_x", document.form.wl_phrase_x);
}
inputCtrl(document.form.wl_key_type, wflag);
inputCtrl(document.form.wl_phrase_x, wflag);
inputCtrl(document.form.wl_key1,     wflag);
inputCtrl(document.form.wl_key2,     wflag);
inputCtrl(document.form.wl_key3,     wflag);
inputCtrl(document.form.wl_key4,     wflag);
inputCtrl(document.form.wl_key,      wflag);
wl_wep_change();
if (wflag=="1")
{document.form.wl_phrase_x.focus();
}
return true;
}
function change_wlweptype_sp(o, s)
{if (document.form.wl_auth_mode.value!="psk")
{change = 1;
window.top.pageChanged = 1;
}
document.form.wl_phrase_x.value = "";
if (o.value=="0")
{wflag = 0;
wep = "";
document.form.wl_key1.value = wep;
document.form.wl_key2.value = wep;
document.form.wl_key3.value = wep;
document.form.wl_key4.value = wep;
}
else
{wflag = 1;
key_type = document.form.wl_key_type.value;
if (key_type=="0")
{if (o.value=="1")
{wep = "0000000000";
}
else  if (o.value=="2")
{wep = "00000000000000000000000000";
}
else
{wep = "00000000000000000000000000000000";
}
}
else
{if (o.value=="1")
{wep = "00000";
}
else  if (o.value=="2")
{wep = "0000000000000";
}
else
{wep = "0000000000000000";
}
}
if (!validate_wlkey_sp(document.form.wl_key1, "WLANConfig11b") ||
!validate_wlkey_sp(document.form.wl_key2, "WLANConfig11b") ||
!validate_wlkey_sp(document.form.wl_key3, "WLANConfig11b") ||
!validate_wlkey_sp(document.form.wl_key4, "WLANConfig11b")
)
is_wlphrase("WLANConfig11b", "wl_phrase_x", document.form.wl_phrase_x);
}
inputCtrl(document.form.wl_key_type, wflag);
inputCtrl(document.form.wl_phrase_x, wflag);
inputCtrl(document.form.wl_key1,     wflag);
inputCtrl(document.form.wl_key2,     wflag);
inputCtrl(document.form.wl_key3,     wflag);
inputCtrl(document.form.wl_key4,     wflag);
inputCtrl(document.form.wl_key,      wflag);
wl_wep_change();
if (wflag=="1")
{document.form.wl_phrase_x.focus();
}
return true;
}
function change_widzard(o, id)
{if (document.form.current_page.value == "Advanced_PortTrigger_Content.asp")
{for (i = 0; i < wItem.length; i++)
{if (wItem[i][0]!=null)
{if (o.value == wItem[i][0])
{optIdx = i;
document.form.autofw_outport_x_0.value = wItem[optIdx][1];
document.form.autofw_outproto_x_0.value = wItem[optIdx][2];
document.form.autofw_inport_x_0.value = wItem[optIdx][3];
document.form.autofw_inproto_x_0.value = wItem[optIdx][4];
document.form.autofw_desc_x_0.value = wItem[optIdx][0];
}
}
}
}
else if (document.form.current_page.value == "Advanced_VirtualServer_Content.asp")
{for (i = 0; i < wItem.length; i++)
{if (wItem[i][0]!=null)
{if (o.value == wItem[i][0])
{optIdx = i;
if (wItem[optIdx][2]=="TCP")
document.form.vts_proto_x_0.options[0].selected = 1;
else if (wItem[optIdx][2]=="UDP")
document.form.vts_proto_x_0.options[1].selected = 1;
else
document.form.vts_proto_x_0.options[2].selected = 1;
document.form.vts_ipaddr_x_0.value = ip;
document.form.vts_port_x_0.value = wItem[optIdx][1];
document.form.vts_desc_x_0.value = wItem[optIdx][0] + " Server (" + wItem[optIdx][1] + ")";
}
}
}
}
else if (document.form.current_page.value == "Advanced_Firewall_Content.asp")
{for (i = 0; i < wItem.length; i++)
{if (wItem[i][0]!=null)
{if (o.value == wItem[i][0])
{optIdx = i;
if ( id == "WLKnownApps")
{if (wItem[optIdx][2]=="TCP")
document.form.filter_wl_proto_x_0.options[0].selected = 1;
else if (wItem[optIdx][2]=="UDP")
document.form.filter_wl_proto_x_0.options[8].selected = 1;
document.form.filter_wl_srcport_x_0.value = wItem[optIdx][1];
}
else 
{if (wItem[optIdx][2]=="TCP")
document.form.filter_lw_proto_x_0.options[0].selected = 1;
else if (wItem[optIdx][2]=="UDP")
document.form.filter_lw_proto_x_0.options[8].selected = 1;
document.form.filter_lw_dstport_x_0.value = wItem[optIdx][1];
}
}
}
}
}
}
function is_wlkey(o, s)
{if (!nav)
keyPressed = IEKey();
else
keyPressed = NSKey();
if (keyPressed == 0)
return true;
window.top.pageChanged = 1;
if (document.form.current_page.value == "Advanced_WirelessGuest_Content.asp" ||
document.form.current_page.value == "Advanced_WAdvanced_Content.asp"
)
{wep = document.form.wl_guest_wep_x_1.value;
key_type = document.form.wl_guest_key_type_1.value;
}
else
{wep = document.form.wl_wep_x.value;
key_type = document.form.wl_key_type.value;
}
if (key_type == "0")
{if ((keyPressed>47 && keyPressed<58)||(keyPressed>64 && keyPressed<71)||(keyPressed>96 && keyPressed<103))
{if (wep == "1")
{if(o.value != "" && o.value.length > 10) return false;
}
else if (wep == "2")
{if(o.value != "" && o.value.length > 26) return false;
}
else return false;
}
else
return false;
}
else
{if (keyPressed>=32 && keyPressed<=126)
{if (wep == "1")
{if(o.value != "" && o.value.length > 5) return false;
}
else if (wep == "2")
{if(o.value != "" && o.value.length > 13) return false;
}
else return false;
}
else
return false;
}
return true;
}
function change_wlkey(o, s)
{if (document.form.current_page.value == "Advanced_WirelessGuest_Content.asp" ||
document.form.current_page.value == "Advanced_WAdvanced_Content.asp"
)
{wep = document.form.wl_guest_wep_x_1.value;
key_type = document.form.wl_guest_key_type_1.value;
}
else
{wep = document.form.wl_wep_x.value;
key_type = document.form.wl_key_type.value;
}
if (wep == "1")
{if (key_type == "0")
{if(o.value.length > 10)
o.value = o.value.substring(0, 10);
}
else
{if(o.value.length > 5)
o.value = o.value.substring(0, 5);
}
}
else if (wep == "2")
{if (key_type == "0")
{if(o.value.length > 26)
o.value = o.value.substring(0, 26);
}
else
{if(o.value.length > 13)
o.value = o.value.substring(0, 13);
}
}
else return false;
}
function validate_timerange(o, p)
{if (o.value.length==0) o.value = "00";
else if (o.value.length==1) o.value = "0" + o.value;
if (o.value.charAt(0)<'0' || o.value.charAt(0)>'9') o.value = "00";
else if (o.value.charAt(1)<'0' || o.value.charAt(1)>'9') o.value = "00";
else if (p==0 || p==2)
{if(o.value>23) o.value = "00";
}
else
{if(o.value>59) o.value = "00";
}
return true;
}
function validate_wlkey(o, s)
{if (document.form.current_page.value == "Advanced_WirelessGuest_Content.asp" ||
document.form.current_page.value == "Advanced_WAdvanced_Content.asp"
)
{wep = document.form.wl_guest_wep_x_1.value;
key_type = document.form.wl_guest_key_type_1.value;
}
else
{wep = document.form.wl_wep_x.value;
key_type = document.form.wl_key_type.value;
}
if (key_type == "0")
{if (wep == "1")
{if(o.value.length == 10)
return;
o.value = "0000000000";
}
else if (wep == "2")
{if(o.value.length == 26)
return;
o.value = "00000000000000000000000000";
}
else return;
}
else
{if (wep == "1")
{if(o.value.length == 5)
return;
o.value = "00000";
}
else if (wep == "2")
{if(o.value.length == 13)
return;
o.value = "0000000000000";
}
else return;
}
if (wep == "1")
{if (key_type == "0")
alert(translate_lang('JS_wepkey_11',document.form.preferred_lang.value));
else
alert(translate_lang('JS_wepkey_21',document.form.preferred_lang.value));
}
else
{if (key_type == "0")
alert(translate_lang('JS_wepkey_12',document.form.preferred_lang.value));
else
alert(translate_lang('JS_wepkey_22',document.form.preferred_lang.value));
}
return false;
}
function validate_wlkey_sp(o, s)
{if (document.form.current_page.value == "Advanced_WirelessGuest_Content.asp" ||
document.form.current_page.value == "Advanced_WAdvanced_Content.asp"
)
{wep = document.form.wl_guest_wep_x_1.value;
key_type = document.form.wl_guest_key_type_1.value;
}
else
{wep = document.form.wl_wep_x.value;
key_type = document.form.wl_key_type.value;
}
if (key_type == "0")
{if (wep == "1")
{if(o.value.length == 10)
return true;
}
else if (wep == "2")
{if(o.value.length == 26)
return true;
}
else return false;
}
else
{if (wep == "1")
{if(o.value.length == 5)
return true;
}
else if (wep == "2")
{if(o.value.length == 13)
return true;
}
else return false;
}
return false;
}
/* Factory Reset Warning */
function confirmRestore(){
if(confirm('WARNING:\nAll your settings will be lost!\nProceed or not?')) {
return true;
}
else
{top.location.href = "Advanced_Content.html";
return false;
}
}
function validate_wlphrase(s, v, o)
{if (v == "wl_wpa_psk" || v == "wl_guest_wpa_psk_1" || v== "mssid_wpa_psk_x_0")
{if( (v == "wl_wpa_psk" && document.form.wl_auth_mode.value == "psk") ||
(v == "wl_guest_wpa_psk_1" && document.form.wl_guest_auth_mode_1.value == "psk") ||
(v == "mssid_wpa_psk_x_0" && document.form.mssid_auth_mode_x_0.value == "psk")
)
{if (o.value.length < 8)
{alert(translate_lang('JS_passzero',document.form.preferred_lang.value));
if (v == "wl_wpa_psk")
document.form.wl_wpa_psk.value = "12345678";
else if (v == "wl_guest_wpa_psk_1")
document.form.wl_guest_wpa_psk_1.value = "12345678";
else if (v == "mssid_wpa_psk_x_0")
document.form.mssid_wpa_psk_x_0.value = "12345678";
return false;
}
}
}
else
{if (!validate_string(o))
{is_wlphrase(s, v, o);
return(false);
}
}
return true;
}
function add_portmapping()
{widzard = window.open("Advanced_PortForwarding_Widzard.asp", "PortForwarding_Widzard",
"toolbar=no,location=no,directories=no,status=no,menubar=no,scrollbars=yes,resizable=no,copyhistory=no,width=600,height=400");
if (!widzard.opener) widzard.opener = self;
}
function add_vsmapping()
{widzard = window.open("Advanced_VirtualServer_Widzard.asp", "VirtualServer_Widzard",
"toolbar=no,location=no,directories=no,status=no,menubar=no,scrollbars=yes,resizable=no,copyhistory=no,width=600,height=400");
if (!widzard.opener) widzard.opener = self;
}
function openHelp(o, name)
{urlstr = "Advanced_" + name + "_Widzard.asp"
widzard = window.open(urlstr, "Help_Widzard",
"toolbar=no,location=no,directories=no,status=no,menubar=no,scrollbars=yes,resizable=no,copyhistory=no,width=600,height=500");
if (!widzard.opener) widzard.opener = self;
}
function matchSubnet(ip1, ip2, count)
{var c = 0;
var v1 = 0;
var v2 = 0;
for(i=0;i<ip1.length;i++)
{if (ip1.charAt(i) == '.')
{if (ip2.charAt(i) != '.')
return false;
c++;
if (v1!=v2) return false;
v1 = 0;
v2 = 0;
}
else
{if (ip2.charAt(i)=='.') return false;
v1 = v1*10 + (ip1.charAt(i) - '0');
v2 = v2*10 + (ip2.charAt(i) - '0');
}
if (c==count) return true;
}
return false;
}
function subnetPrefix(ip, orig, count)
{r='';
c=0;
for(i=0;i<ip.length;i++)
{if (ip.charAt(i) == '.')
c++;
if (c==count) break;
r = r + ip.charAt(i);
}
c=0;
for(i=0;i<orig.length;i++)
{if (orig.charAt(i) == '.')
{c++;
}
if (c>=count)
r = r + orig.charAt(i);
}
return (r);
}
function checkSubnet()
{/* Rule : If addresses in pool are match to subnet, don't change */
/* Rule : If addresses in pool are not match to subnet, change to subnet.2~subnet.254 */
if (!matchSubnet(document.form.lan_ipaddr.value, document.form.dhcp_start.value, 3) ||
!matchSubnet(document.form.lan_ipaddr.value, document.form.dhcp_end.value, 3))
{if (confirm(translate_lang('JS_DHCP1',document.form.preferred_lang.value)))
{document.form.dhcp_start.value = subnetPrefix(document.form.lan_ipaddr.value, document.form.dhcp_start.value, 3);
document.form.dhcp_end.value = subnetPrefix(document.form.lan_ipaddr.value, document.form.dhcp_end.value, 3);
}
}
}
function checkDmzSubnet()
{/* Rule : If addresses in pool are match to subnet, don't change */
/* Rule : If addresses in pool are not match to subnet, change to subnet.2~subnet.254 */
if (!matchSubnet(document.form.FirewallConfig_DmzIP.value, document.form.LANHostConfig_DmzMinAddress.value, 3) ||
!matchSubnet(document.form.FirewallConfig_DmzIP.value, document.form.LANHostConfig_DmzMaxAddress.value, 3))
{if (confirm(translate_lang('JS_DHCP2',document.form.preferred_lang.value)))
{document.form.LANHostConfig_DmzMinAddress.value = subnetPrefix(document.form.FirewallConfig_DmzIP.value, document.form.LANHostConfig_DmzMinAddress.value, 3);
document.form.LANHostConfig_DmzMaxAddress.value = subnetPrefix(document.form.FirewallConfig_DmzIP.value, document.form.LANHostConfig_DmzMaxAddress.value, 3);
}
}
}
function checkSubnetGuest()
{/* Rule : If addresses in pool are match to subnet, don't change */
/* Rule : If addresses in pool are not match to subnet, change to subnet.2~subnet.254 */
if (!matchSubnet(document.form.lan1_ipaddr.value, document.form.dhcp1_start.value, 3) ||
!matchSubnet(document.form.lan1_ipaddr.value, document.form.dhcp1_end.value, 3))
{if (confirm(translate_lang('JS_DHCP1',document.form.preferred_lang.value)))
{document.form.dhcp1_start.value = subnetPrefix(document.form.lan1_ipaddr.value, document.form.dhcp1_start.value, 3);
document.form.dhcp1_end.value = subnetPrefix(document.form.lan1_ipaddr.value, document.form.dhcp1_end.value, 3);
}
}
}
function wan_netmask_check(o) {
var wip =intoa(document.form.wan_ipaddr.value);
var gw =intoa(document.form.wan_gateway.value);
var nm =intoa(document.form.wan_netmask.value);
var lip=intoa(document.form.lan_ipaddr.value);
var lnm=intoa(document.form.lan_netmask.value);
var rem=1;
var nm_valid;
var nm_valid_flag=false;
for(nm_valid=0xffffffff; nm_valid!=0; nm_valid-=rem,rem*=2)
{if (nm==nm_valid)
{nm_valid_flag=true;;
break;
}
}
if (!nm_valid_flag)
{alert(o.value + translate_lang('JS_validip',document.form.preferred_lang.value));
o.value = "255.255.255.0";
o.focus();
return false;
}
if (wip==0x0 || (wip&lnm)==(lip&lnm))
{alert(o.value + translate_lang('JS_validip',document.form.preferred_lang.value));
document.form.wan_ipaddr.value="1.1.1.1";
document.form.wan_netmask.value="255.0.0.0";
o.focus();
return false;
}
if (gw==0 || gw==0xffffffff || (wip&nm)==(gw&nm))
{return true;
}
else
{/*
rem=1;
for(nm=0xffffffff; nm!=0; nm-=rem,rem*=2)
{if ((wip&nm)==(gw&nm))
break;
}
v4=nm%0x100;
v3=((nm-v4)/0x100)%0x100;
v2=((nm-v4-v3*0x100)/0x10000)%0x100;
v1=((nm-v4-v3*0x100-v2*0x10000)/(0x1000000))%0x100;
o.value=v1+"."+v2+"."+v3+"."+v4;
*/
alert(o.value + translate_lang('JS_validip',document.form.preferred_lang.value));
o.focus();
return false;
}
}
function updateDateTime(s)
{if (s == "Advanced_Firewall_Content.asp")
{if (window.top.isModel()!="WL520" && window.top.isModel()!="SnapAP")
{document.form.filter_wl_date_x.value = setDateCheck(
document.form.filter_wl_date_x_Sun,
document.form.filter_wl_date_x_Mon,
document.form.filter_wl_date_x_Tue,
document.form.filter_wl_date_x_Wed,
document.form.filter_wl_date_x_Thu,
document.form.filter_wl_date_x_Fri,
document.form.filter_wl_date_x_Sat);
document.form.filter_wl_time_x.value = setTimeRange(
document.form.filter_wl_time_x_starthour,
document.form.filter_wl_time_x_startmin,
document.form.filter_wl_time_x_endhour,
document.form.filter_wl_time_x_endmin);
}
document.form.filter_lw_date_x.value = setDateCheck(
document.form.filter_lw_date_x_Sun,
document.form.filter_lw_date_x_Mon,
document.form.filter_lw_date_x_Tue,
document.form.filter_lw_date_x_Wed,
document.form.filter_lw_date_x_Thu,
document.form.filter_lw_date_x_Fri,
document.form.filter_lw_date_x_Sat);
document.form.filter_lw_time_x.value = setTimeRange(
document.form.filter_lw_time_x_starthour,
document.form.filter_lw_time_x_startmin,
document.form.filter_lw_time_x_endhour,
document.form.filter_lw_time_x_endmin);
}
else if (s == "Advanced_URLFilter_Content.asp")
{document.form.url_date_x.value = setDateCheck(
document.form.url_date_x_Sun,
document.form.url_date_x_Mon,
document.form.url_date_x_Tue,
document.form.url_date_x_Wed,
document.form.url_date_x_Thu,
document.form.url_date_x_Fri,
document.form.url_date_x_Sat);
document.form.url_time_x.value = setTimeRange(
document.form.url_time_x_starthour,
document.form.url_time_x_startmin,
document.form.url_time_x_endhour,
document.form.url_time_x_endmin);
}
else if (s == "Advanced_WebCam_Content.asp")
{document.form.usb_websecurity_date_x.value = setDateCheck(
document.form.usb_websecurity_date_x_Sun,
document.form.usb_websecurity_date_x_Mon,
document.form.usb_websecurity_date_x_Tue,
document.form.usb_websecurity_date_x_Wed,
document.form.usb_websecurity_date_x_Thu,
document.form.usb_websecurity_date_x_Fri,
document.form.usb_websecurity_date_x_Sat);
document.form.usb_websecurity_time_x.value = setTimeRange(
document.form.usb_websecurity_time_x_starthour,
document.form.usb_websecurity_time_x_startmin,
document.form.usb_websecurity_time_x_endhour,
document.form.usb_websecurity_time_x_endmin);
}
else if (s == "Advanced_WAdvanced_Content.asp" && window.top.isModel()!="WL300" && window.top.isModel()!="WL331")
{if (window.top.isModel()!="WL520" && window.top.isModel()!="SnapAP")
{document.form.wl_radio_date_x.value = setDateCheck(
document.form.wl_radio_date_x_Sun,
document.form.wl_radio_date_x_Mon,
document.form.wl_radio_date_x_Tue,
document.form.wl_radio_date_x_Wed,
document.form.wl_radio_date_x_Thu,
document.form.wl_radio_date_x_Fri,
document.form.wl_radio_date_x_Sat);
document.form.wl_radio_time_x.value = setTimeRange(
document.form.wl_radio_time_x_starthour,
document.form.wl_radio_time_x_startmin,
document.form.wl_radio_time_x_endhour,
document.form.wl_radio_time_x_endmin);
}
}
}
function startMenu(s)
{if (s=='Quick')
{document.form.action_mode.value = "Next";
document.form.submit();
}
else
{widzard = window.open("Advanced_PrinterSetup_Widzard.asp", "Printer Setup Widzard",
"toolbar=no,location=no,directories=no,status=no,menubar=no,scrollbars=yes,resizable=no,copyhistory=no,width=600,height=400");
if (!widzard.opener) widzard.opener = self;
}
}
function openWidzard(u, t)
{url = '"' + u + '"';
title = '"' + t + '"';
widzard = window.open(u, "Widzard",
"toolbar=no,location=no,directories=no,status=no,menubar=no,scrollbars=yes,resizable=no,copyhistory=no,width=600,height=400");
if (!widzard.opener) widzard.opener = self;
}
function openWidzardNone(u, t)
{}
function openLink(s)
{if (s=='x_DDNSServer')
{if (document.form.ddns_server_x.value.indexOf("WWW.DYNDNS.ORG")!=-1)
tourl = "https://www.dyndns.org/account/create.html"
else if (document.form.ddns_server_x.value == 'WWW.ZONEEDIT.COM')
tourl = "https://www.zoneedit.com/signup.html"
else
tourl = "https://controlpanel.tzo.com/cgi-bin/tzopanel.exe"
link = window.open(tourl, "DDNSLink",
"toolbar=yes,location=yes,directories=no,status=yes,menubar=yes,scrollbars=yes,resizable=yes,copyhistory=no,width=640,height=480");
}
else if (s=='x_NTPServer1')
{tourl = "http://ntp.isc.org/bin/view/Servers/WebHome"
link = window.open(tourl, "NTPLink",
"toolbar=yes,location=yes,directories=no,status=yes,menubar=yes,scrollbars=yes,resizable=yes,copyhistory=no,width=640,height=480");
}
if (!link.opener) link.opener = self;
}
function blur_body()
{alert(translate_lang('JS_focus',document.form.preferred_lang2.value));
}
/* Used when WEP is changed */
function changeWEPType()
{if (document.form.wl_wep.value == "0")
{flag = 0;
}
else
{flag = 1;
}
inputCtrl(document.form.wl_phrase_x,  flag);
inputCtrl(document.form.wl_key1, flag);
inputCtrl(document.form.wl_key2, flag);
inputCtrl(document.form.wl_key3, flag);
inputCtrl(document.form.wl_key4, flag);
inputCtrl(document.form.wl_key,  flag);
}
/* input : s: service id, v: value name, o: current value*/
/* output: wep key1~4*/
function is_wlphrase(s, v, o)
{window.top.pageChanged = 1;
var pseed = new Array;
var wep_key = new Array(5);
if (v=='wl_wpa_psk' || v=='wl_guest_wpa_psk_1' || v=='mssid_wpa_psk_x_0')
return(valid_WPAPSK(o));
if (document.form.current_page.value != "Advanced_WirelessGuest_Content.asp" &&
document.form.current_page.value != "Advanced_WAdvanced_Content.asp"
)
{wepType = document.form.wl_wep_x.value;
key_type = document.form.wl_key_type.value;
wepKey1 = document.form.wl_key1;
wepKey2 = document.form.wl_key2;
wepKey3 = document.form.wl_key3;
wepKey4 = document.form.wl_key4;
}
else
{wepType = document.form.wl_guest_wep_x_1.value;
key_type = document.form.wl_guest_key_type_1.value;
wepKey1 = document.form.wl_guest_key1_1;
wepKey2 = document.form.wl_guest_key2_1;
wepKey3 = document.form.wl_guest_key3_1;
wepKey4 = document.form.wl_guest_key4_1;
}
phrase = o.value;
if(wepType == "1")
{for (i=0; i<phrase.length; i++)
{pseed[i%4] ^= phrase.charCodeAt(i);
}
randNumber = pseed[0] | (pseed[1]<<8) | (pseed[2]<<16) | (pseed[3]<<24);
for (j=0; j<5; j++)
{randNumber = ((randNumber * 0x343fd) % 0x1000000);
randNumber = ((randNumber + 0x269ec3) % 0x1000000);
wep_key[j] = ((randNumber>>16) & 0xff);
}
wepKey1.value = binl2hex_c(wep_key);
for (j=0; j<5; j++)
{randNumber = ((randNumber * 0x343fd) % 0x1000000);
randNumber = ((randNumber + 0x269ec3) % 0x1000000);
wep_key[j] = ((randNumber>>16) & 0xff);
}
wepKey2.value = binl2hex_c(wep_key);
for (j=0; j<5; j++)
{randNumber = ((randNumber * 0x343fd) % 0x1000000);
randNumber = ((randNumber + 0x269ec3) % 0x1000000);
wep_key[j] = ((randNumber>>16) & 0xff);
}
wepKey3.value = binl2hex_c(wep_key);
for (j=0; j<5; j++)
{randNumber = ((randNumber * 0x343fd) % 0x1000000);
randNumber = ((randNumber + 0x269ec3) % 0x1000000);
wep_key[j] = ((randNumber>>16) & 0xff);
}
wepKey4.value = binl2hex_c(wep_key);
if (key_type == "1")
{wepKey1.value = wepKey1.value.substr(0, 5);
wepKey2.value = wepKey2.value.substr(0, 5);
wepKey3.value = wepKey3.value.substr(0, 5);
wepKey4.value = wepKey4.value.substr(0, 5);
}
}
else if (wepType == "2" || wepType == "3")
{password = "";
if (phrase.length>0)
{for (i=0; i<64; i++)
{ch = phrase.charAt(i%phrase.length);
password = password + ch;
}
}
password = calcMD5(password);
if (wepType == "2")
{wepKey1.value = password.substr(0, 26);
}
else
{wepKey1.value = password.substr(0, 32);
}
if (key_type == "1")
{if (wepType == "2")
wepKey1.value = wepKey1.value.substr(0, 13);
else
wepKey1.value = wepKey1.value.substr(0, 16);
}
wepKey2.value = wepKey1.value;
wepKey3.value = wepKey1.value;
wepKey4.value = wepKey1.value;
}
return true;
}
function wl_wep_change()
{var mode = document.form.wl_auth_mode.value;
var wep = document.form.wl_wep_x.value;
if (window.top.isModel()=="WL520" || window.top.isModel()=="SnapAP" || window.top.isCard()=="ralink")
{if (mode == "wpa" || mode == "wpa2" || mode == "psk")
{if (mode != "wpa" && mode != "wpa2")
inputCtrl(document.form.wl_crypto,  1);
if (mode != "wpa" && mode != "wpa2")
inputCtrl(document.form.wl_wpa_psk,  1);
inputCtrl(document.form.wl_wpa_gtk_rekey,  1);
inputCtrl(document.form.wl_wep_x,  0);
inputCtrl(document.form.wl_key_type, 0);
inputCtrl(document.form.wl_phrase_x,  0);
inputCtrl(document.form.wl_key1, 0);
inputCtrl(document.form.wl_key2, 0);
inputCtrl(document.form.wl_key3, 0);
inputCtrl(document.form.wl_key4, 0);
inputCtrl(document.form.wl_key,  0);
}
else
{inputCtrl(document.form.wl_crypto,  0);
inputCtrl(document.form.wl_wpa_psk,  0);
inputCtrl(document.form.wl_wpa_gtk_rekey,  0);
inputCtrl(document.form.wl_wep_x,  1);
if (wep != "0")
{inputCtrl(document.form.wl_key_type, 1);
inputCtrl(document.form.wl_phrase_x,  1);
inputCtrl(document.form.wl_key1,  1);
inputCtrl(document.form.wl_key2,  1);
inputCtrl(document.form.wl_key3,  1);
inputCtrl(document.form.wl_key4,  1);
inputCtrl(document.form.wl_key,   1);
}
else {
inputCtrl(document.form.wl_key_type, 0);
inputCtrl(document.form.wl_phrase_x,  0);
inputCtrl(document.form.wl_key1,  0);
inputCtrl(document.form.wl_key2,  0);
inputCtrl(document.form.wl_key3,  0);
inputCtrl(document.form.wl_key4,  0);
inputCtrl(document.form.wl_key,   0);
}
}
}
else
{/* enable/disable network key 1 to 4 */
if (wep != "0") {
if (mode == "wpa" || mode == "wpa2" || mode == "psk" || mode == "radius") {
inputCtrl(document.form.wl_key1,  0);
inputCtrl(document.form.wl_key4,  0);
}
else {
inputCtrl(document.form.wl_key1,  1);
inputCtrl(document.form.wl_key4,  1);
}
inputCtrl(document.form.wl_key2,  1);
inputCtrl(document.form.wl_key3,  1);
} else
{inputCtrl(document.form.wl_phrase_x,  0);
inputCtrl(document.form.wl_key1,  0);
inputCtrl(document.form.wl_key2,  0);
inputCtrl(document.form.wl_key3,  0);
inputCtrl(document.form.wl_key4,  0);
}
/* enable/disable key index */
if (wep != "0")
inputCtrl(document.form.wl_key,  1);
else
inputCtrl(document.form.wl_key,  0);
/* enable/disable gtk rotation interval */
if (wep != "0")
inputCtrl(document.form.wl_wpa_gtk_rekey,  0);
else {
if (mode == "wpa" || mode == "wpa2" || mode == "psk")
inputCtrl(document.form.wl_wpa_gtk_rekey,  1);
else
inputCtrl(document.form.wl_wpa_gtk_rekey,  0);
}
}
}
function change_wep_type(mode)
{orig = document.form.wl_wep_x.value;
free_options(document.form.wl_wep_x);
if (mode == "shared" || mode == "radius")
{vitems = new Array("1", "2");
items = new Array("WEP-64bits", "WEP-128bits");
}
else
{vitems = new Array("0", "1", "2");
items = new Array("None", "WEP-64bits", "WEP-128bits");
}
add_options_x2(document.form.wl_wep_x, vitems, items, orig);
if (mode == "psk")
document.form.wl_wep_x.value="0";
if (((mode == "shared" || mode == "radius") && orig == "0") || mode == "psk")
{change_wlweptype(document.form.wl_wep_x, "WLANConfig11b");
}
}
function wl_auth_mode_reconf()
{if ( document.form.wl_auth_mode.value=="psk" &&
document.form.wl_crypto.value=="tkip+aes")
document.form.wl_crypto.value=="tkip";
if ( document.form.wl_auth_mode.value=="radius" ||
document.form.wl_auth_mode.value=="wpa" ||
document.form.wl_auth_mode.value=="wpa2"
)
document.form.wl_auth_mode.value="open";
}
function wl_auth_mode_change(isload)
{var mode = document.form.wl_auth_mode.value;
var i, cur, algos;
if (parent.titleFrame.document.form.wl_mode_ex.value == 're')
{if (mode == "wpa" || mode == "wpa2" || mode == "radius")
mode = "open";
}
inputCtrl(document.form.wl_wep_x,  1);
/* enable/disable crypto algorithm */
if (mode == "wpa" || mode == "wpa2" || mode == "psk")
inputCtrl(document.form.wl_crypto,  1);
else
inputCtrl(document.form.wl_crypto,  0);
/* enable/disable psk passphrase */
if (mode == "psk")
inputCtrl(document.form.wl_wpa_psk,  1);
else
inputCtrl(document.form.wl_wpa_psk,  0);
/* update wl_crypto */
if (mode == "psk") {
/* Save current crypto algorithm */
for (i = 0; i < document.form.wl_crypto.length; i++) {
if (document.form.wl_crypto[i].selected) {
cur = document.form.wl_crypto[i].value;
break;
}
}
if (window.top.isModel()=="SnapAP" || window.top.isBand() == 'b' )
algos = new Array("TKIP");
else if (document.form.exmode.value=="sta" || document.form.exmode.value=="re")
{algos = new Array("TKIP", "AES");
if (document.form.wl_wpa_mode.value=="0")
document.form.wl_wpa_mode.value="1";
}
else
{opts=document.form.wl_auth_mode.options;
if (opts[opts.selectedIndex].text == "WPA-Auto-Personal")
algos = new Array("TKIP", "AES", "TKIP+AES");
else
algos = new Array("TKIP+AES");
}
/* Reconstruct algorithm array from new crypto algorithms */
document.form.wl_crypto.length = algos.length;
for (var i in algos) {
document.form.wl_crypto[i] = new Option(algos[i], algos[i].toLowerCase());
document.form.wl_crypto[i].value = algos[i].toLowerCase();
if (algos[i].toLowerCase() == cur)
document.form.wl_crypto[i].selected = true;
}
}
else if(mode == "wpa") {
algos = new Array("TKIP");
document.form.wl_crypto.length = algos.length;
document.form.wl_crypto[0] = new Option(algos[0], algos[0].toLowerCase());
}
else if(mode == "wpa2") {
algos = new Array("AES");
document.form.wl_crypto.length = algos.length;
document.form.wl_crypto[0] = new Option(algos[0], algos[0].toLowerCase());
}
change_wep_type(mode);
/* Save current network key index */
for (i = 0; i < document.form.wl_key.length; i++)
{if (document.form.wl_key[i].selected) {
cur = document.form.wl_key[i].value;
break;
}
}
/* Define new network key indices */
if (mode == "wpa" || mode == "wpa2" || mode == "psk" || mode == "radius")
algos = new Array("2", "3");
else
{algos = new Array("1", "2", "3", "4");
if (!isload) cur = "1";
}
/* Reconstruct network key indices array from new network key indices */
document.form.wl_key.length = algos.length;
for (var i in algos) {
document.form.wl_key[i] = new Option(algos[i], algos[i]);
document.form.wl_key[i].value = algos[i];
if (algos[i] == cur)
document.form.wl_key[i].selected = true;
}
wl_wep_change();
if ((mode == "wpa" || mode == "wpa2" || mode == "psk"))
{if (window.top.isModel()=="WL520" || window.top.isModel()=="SnapAP" || window.top.isBand()=='b' )
{}
else if(document.form.wl_crypto[2].selected == true)
{document.form.wl_wep_x.value = 0;
inputCtrl(document.form.wl_wep_x,  0);
inputCtrl(document.form.wl_key_type, 0);
inputCtrl(document.form.wl_phrase_x,  0);
inputCtrl(document.form.wl_key1, 0);
inputCtrl(document.form.wl_key2, 0);
inputCtrl(document.form.wl_key3, 0);
inputCtrl(document.form.wl_key4, 0);
inputCtrl(document.form.wl_key,  0);
}
}
else
{change_wlweptype_sp(document.form.wl_wep_x, "WLANConfig11b");
if (isload)
window.top.pageChanged = 0;
}
}
function wl_mode_ex_change(load_from_quick_setup)
{var mode = document.form.wl_mode_ex.value;
if (document.form.current_page.value=="Advanced_WAdvanced_Content.asp")
{if (mode == "sta")
{inputRCtrl1(document.form.wl_closed, 0);
inputRCtrl1(document.form.wl_ap_isolate, 0);
inputCtrl(document.form.wl_dtim, 0);
inputCtrl(document.form.wl_bcn, 0);
}
else
{inputRCtrl1(document.form.wl_closed, 1);
inputRCtrl1(document.form.wl_ap_isolate, 1);
inputCtrl(document.form.wl_dtim, 1);
inputCtrl(document.form.wl_bcn, 1);
}
if (mode == "sta" || mode == "re")
document.form.wl_guest_enable.value="0";
if (mode == "re")
{if (document.form.wan_nat.value=="1")
document.form.wl_individual_ure.value="1";
else
document.form.wl_individual_ure.value="0";
if (document.form.wl_afterburner.value!="off" && load_from_quick_setup==null)
alert(translate_lang('JS_NO_AB',document.form.preferred_lang.value));
document.form.wl_afterburner.value="off";
inputCtrl(document.form.wl_afterburner, 0);
}
else
inputCtrl(document.form.wl_afterburner, 1);
validate_string4(document.form.wl_guest_ssid_1);
}
}
function wl_rate_change()
{orig = document.form.wl_rate.value;
free_options(document.form.wl_rate);
if (document.form.wl_gmode.value == "1")
{vitems = new Array("0", "1000000", "2000000", "5500000", "6000000", "9000000", "11000000", "12000000", "18000000", "24000000", "36000000", "48000000", "54000000");
if (document.form.preferred_lang.value=="TR")
items = new Array("Otomatik", "1", "2", "5.5", "6", "9", "11", "12", "18", "24", "36", "48", "54");
else
items = new Array("Auto", "1", "2", "5.5", "6", "9", "11", "12", "18", "24", "36", "48", "54");
}
else if (document.form.wl_gmode.value == "4")
{vitems = new Array("0", "6000000", "9000000", "12000000", "18000000", "24000000", "36000000", "48000000", "54000000");
if (document.form.preferred_lang.value=="TR")
items = new Array("Otomatik", "6", "9", "12", "18", "24", "36", "48", "54");
else
items = new Array("Auto", "6", "9", "12", "18", "24", "36", "48", "54");
}
else
{vitems = new Array("0", "1000000", "2000000", "5500000", "11000000");
if (document.form.preferred_lang.value=="TR")
items = new Array("Otomatik", "1", "2", "5.5", "11");
else
items = new Array("Auto", "1", "2", "5.5", "11");
}
add_options_x2(document.form.wl_rate, vitems, items, orig);
}
function change_wan_type(v)
{if (v == "static")
{inputCtrl(document.form.wan_ipaddr, 1);
inputCtrl(document.form.wan_netmask, 1);
inputCtrl(document.form.wan_gateway, 1);
inputCtrl(document.form.wan_pppoe_username, 0);
inputCtrl(document.form.wan_pppoe_passwd, 0);
inputCtrl(document.form.wan_pppoe_idletime, 0);
inputCtrl(document.form.wan_pppoe_mtu, 0);
inputCtrl(document.form.wan_pppoe_mru, 0);
inputCtrl(document.form.wan_pppoe_idletime_check, 0);
inputCtrl(document.form.wan_pppoe_service, 0);
inputCtrl(document.form.wan_pppoe_ac, 0);
}
else if(v == "pppoe")
{inputCtrl(document.form.wan_ipaddr, 0);
inputCtrl(document.form.wan_netmask, 0);
inputCtrl(document.form.wan_gateway, 0);
inputCtrl(document.form.wan_pppoe_username, 1);
inputCtrl(document.form.wan_pppoe_passwd, 1);
inputCtrl(document.form.wan_pppoe_idletime, 1);
inputCtrl(document.form.wan_pppoe_idletime_check, 1);
inputCtrl(document.form.wan_pppoe_mtu, 1);
inputCtrl(document.form.wan_pppoe_mru, 1);
inputCtrl(document.form.wan_pppoe_service, 1);
inputCtrl(document.form.wan_pppoe_ac, 1);
}
else if(v == "pptp")
{inputCtrl(document.form.wan_ipaddr, 1);
inputCtrl(document.form.wan_netmask, 1);
inputCtrl(document.form.wan_gateway, 1);
inputCtrl(document.form.wan_pppoe_username, 1);
inputCtrl(document.form.wan_pppoe_passwd, 1);
inputCtrl(document.form.wan_pppoe_idletime, 1);
inputCtrl(document.form.wan_pppoe_idletime_check, 1);
inputCtrl(document.form.wan_pppoe_mtu, 0);
inputCtrl(document.form.wan_pppoe_mru, 0);
inputCtrl(document.form.wan_pppoe_service, 0);
inputCtrl(document.form.wan_pppoe_ac, 0);
}
else if (v == "bigpond" || v == "cdma")
{inputCtrl(document.form.wan_ipaddr, 0);
inputCtrl(document.form.wan_netmask, 0);
inputCtrl(document.form.wan_gateway, 0);
inputCtrl(document.form.wan_pppoe_username, 1);
inputCtrl(document.form.wan_pppoe_passwd, 1);
inputCtrl(document.form.wan_pppoe_idletime, 0);
inputCtrl(document.form.wan_pppoe_idletime_check, 0);
inputCtrl(document.form.wan_pppoe_mtu, 0);
inputCtrl(document.form.wan_pppoe_mru, 0);
inputCtrl(document.form.wan_pppoe_service, 0);
inputCtrl(document.form.wan_pppoe_ac, 0);
}
else
{inputCtrl(document.form.wan_ipaddr, 0);
inputCtrl(document.form.wan_netmask, 0);
inputCtrl(document.form.wan_gateway, 0);
inputCtrl(document.form.wan_pppoe_username, 0);
inputCtrl(document.form.wan_pppoe_passwd, 0);
inputCtrl(document.form.wan_pppoe_idletime, 0);
inputCtrl(document.form.wan_pppoe_idletime_check, 0);
inputCtrl(document.form.wan_pppoe_mtu, 0);
inputCtrl(document.form.wan_pppoe_mru, 0);
inputCtrl(document.form.wan_pppoe_service, 0);
inputCtrl(document.form.wan_pppoe_ac, 0);
}
if (document.form.exmode.value != "ap")
inputRCtrl1(document.form.wan_pppoe_relay_x, 0);
else
inputRCtrl1(document.form.wan_pppoe_relay_x, 1);
}
function change_wan_type2(v)
{inputCtrl1(document.form.wan_dns1_x, 1);
inputCtrl1(document.form.wan_dns2_x, 1);
if (v == "pppoe" || v == "bigpond")
{inputRCtrl1(document.form.wan_dnsenable_x, 1);
}
else if(v == "pptp")
{inputRCtrl1(document.form.wan_dnsenable_x, 1);
}
else if(v == "static")
{inputRCtrl1(document.form.wan_dnsenable_x, 0);
inputRCtrl2(document.form.wan_dnsenable_x, 1);
}
else
{inputRCtrl1(document.form.wan_dnsenable_x, 1);
}
if (document.form.wan_dnsenable_x[0].checked == true)
{inputCtrl1(document.form.wan_dns1_x, 0);
inputCtrl1(document.form.wan_dns2_x, 0);
}
else
{inputCtrl1(document.form.wan_dns1_x, 1);
inputCtrl1(document.form.wan_dns2_x, 1);
}
}
function masq_wepkey()
{wep1 = document.form.wl_key1.value;
wep2 = document.form.wl_key2.value;
wep3 = document.form.wl_key3.value;
wep4 = document.form.wl_key4.value;
if (wep1.length == 10)
{wep = "**********";
}
else if (wep1.length = 26)
{wep = "**************************";
}
else wep = "";
document.form.wl_key1.value = wep;
document.form.wl_key2.value = wep;
document.form.wl_key3.value = wep;
document.form.wl_key4.value = wep;
}
function unmasq_wepkey()
{if (document.form.wl_key1.value.indexOf("**********") != -1)
document.form.wl_key1.value = wep1;
if (document.form.wl_key2.value.indexOf("**********") != -1)
document.form.wl_key2.value = wep2;
if (document.form.wl_key3.value.indexOf("**********") != -1)
document.form.wl_key3.value = wep3;
if (document.form.wl_key4.value.indexOf("**********") != -1)
document.form.wl_key4.value = wep4;
}
function masq_wepkey_guest()
{wep1 = document.form.wl_guest_key1_1.value;
wep2 = document.form.wl_guest_key2_1.value;
wep3 = document.form.wl_guest_key3_1.value;
wep4 = document.form.wl_guest_key4_1.value;
if (wep1.length == 10)
{wep = "**********";
}
else if (wep1.length = 26)
{wep = "**************************";
}
else wep = "";
document.form.wl_guest_key1_1.value = wep;
document.form.wl_guest_key2_1.value = wep;
document.form.wl_guest_key3_1.value = wep;
document.form.wl_guest_key4_1.value = wep;
}
function unmasq_wepkey_guest()
{if (document.form.wl_guest_key1_1.value.indexOf("**********") != -1)
document.form.wl_guest_key1_1.value = wep1;
if (document.form.wl_guest_key2_1.value.indexOf("**********") != -1)
document.form.wl_guest_key2_1.value = wep2;
if (document.form.wl_guest_key3_1.value.indexOf("**********") != -1)
document.form.wl_guest_key3_1.value = wep3;
if (document.form.wl_guest_key4_1.value.indexOf("**********") != -1)
document.form.wl_guest_key4_1.value = wep4;
}
function wl_wep_change_guest()
{var mode = document.form.wl_guest_auth_mode_1.value;
var wep = document.form.wl_guest_wep_x_1.value;
if (window.top.isModel()=="WL520" || window.top.isModel()=="SnapAP" || window.top.isCard()=="ralink")
{if (mode == "wpa" || mode == "psk")
{inputCtrl(document.form.wl_guest_crypto_1,  1);
inputCtrl(document.form.wl_guest_wpa_psk_1,  1);
inputCtrl(document.form.wl_guest_wpa_gtk_rekey_1,  1);
inputCtrl(document.form.wl_guest_wep_x_1,  0);
inputCtrl(document.form.wl_guest_key_type_1, 0);
inputCtrl(document.form.wl_guest_phrase_x_1,  0);
inputCtrl(document.form.wl_guest_key1_1, 0);
inputCtrl(document.form.wl_guest_key2_1, 0);
inputCtrl(document.form.wl_guest_key3_1, 0);
inputCtrl(document.form.wl_guest_key4_1, 0);
inputCtrl(document.form.wl_guest_key_1,  0);
}
else
{inputCtrl(document.form.wl_guest_crypto_1,  0);
inputCtrl(document.form.wl_guest_wpa_psk_1,  0);
inputCtrl(document.form.wl_guest_wpa_gtk_rekey_1,  0);
inputCtrl(document.form.wl_guest_wep_x_1,  1);
if (wep != "0")
{inputCtrl(document.form.wl_guest_key_type_1, 1);
inputCtrl(document.form.wl_guest_phrase_x_1,  1);
inputCtrl(document.form.wl_guest_key1_1,  1);
inputCtrl(document.form.wl_guest_key2_1,  1);
inputCtrl(document.form.wl_guest_key3_1,  1);
inputCtrl(document.form.wl_guest_key4_1,  1);
inputCtrl(document.form.wl_guest_key_1,   1);
}
else {
inputCtrl(document.form.wl_guest_key_type_1, 0);
inputCtrl(document.form.wl_guest_phrase_x_1,  0);
inputCtrl(document.form.wl_guest_key1_1,  0);
inputCtrl(document.form.wl_guest_key2_1,  0);
inputCtrl(document.form.wl_guest_key3_1,  0);
inputCtrl(document.form.wl_guest_key4_1,  0);
inputCtrl(document.form.wl_guest_key_1,   0);
}
}
}
else
{/* enable/disable network key 1 to 4 */
if (wep != "0") {
if (mode == "wpa" || mode == "wpa2" || mode == "psk" || mode == "radius") {
inputCtrl(document.form.wl_guest_key1_1,  0);
inputCtrl(document.form.wl_guest_key4_1,  0);
}
else {
inputCtrl(document.form.wl_guest_key1_1,  1);
inputCtrl(document.form.wl_guest_key4_1,  1);
}
inputCtrl(document.form.wl_guest_key2_1,  1);
inputCtrl(document.form.wl_guest_key3_1,  1);
} else
{inputCtrl(document.form.wl_guest_phrase_x_1,  0);
inputCtrl(document.form.wl_guest_key1_1,  0);
inputCtrl(document.form.wl_guest_key2_1,  0);
inputCtrl(document.form.wl_guest_key3_1,  0);
inputCtrl(document.form.wl_guest_key4_1,  0);
}
/* enable/disable key index */
if (wep != "0")
inputCtrl(document.form.wl_guest_key_1,  1);
else
inputCtrl(document.form.wl_guest_key_1,  0);
/* enable/disable gtk rotation interval */
if (wep != "0")
inputCtrl(document.form.wl_guest_wpa_gtk_rekey_1,  0);
else {
if (mode == "wpa" || mode == "wpa2" || mode == "psk")
inputCtrl(document.form.wl_guest_wpa_gtk_rekey_1,  1);
else
inputCtrl(document.form.wl_guest_wpa_gtk_rekey_1,  0);
}
}
}
function change_wep_type_guest(mode)
{orig = document.form.wl_guest_wep_x_1.value;
free_options(document.form.wl_guest_wep_x_1);
if (mode == "shared" || mode == "radius")
{vitems = new Array("1", "2");
items = new Array("WEP-64bits", "WEP-128bits");
}
else
{vitems = new Array("0", "1", "2");
items = new Array("None", "WEP-64bits", "WEP-128bits");
}
add_options_x2(document.form.wl_guest_wep_x_1, vitems, items, orig);
if (mode == "psk")
document.form.wl_guest_wep_x_1.value="0";
if (((mode == "shared" || mode == "radius") && orig == "0") || mode == "psk")
{change_wlweptype_guest(document.form.wl_guest_wep_x_1, "WLANConfig11b");
}
}
function wl_auth_mode_reconf_guest()
{if (document.form.wl_guest_auth_mode_1.value=="radius" ||
document.form.wl_guest_auth_mode_1.value=="wpa" ||
document.form.wl_guest_auth_mode_1.value=="wpa2"
)
document.form.wl_guest_auth_mode_1.value="open";
}
function wl_auth_mode_change_guest(isload)
{var mode = document.form.wl_guest_auth_mode_1.value;
var i, cur, algos;
inputCtrl(document.form.wl_guest_wep_x_1,  1);
/* enable/disable crypto algorithm */
if (mode == "wpa" || mode == "wpa2" || mode == "psk")
inputCtrl(document.form.wl_guest_crypto_1,  1);
else
inputCtrl(document.form.wl_guest_crypto_1,  0);
/* enable/disable psk passphrase */
if (mode == "psk")
inputCtrl(document.form.wl_guest_wpa_psk_1,  1);
else
inputCtrl(document.form.wl_guest_wpa_psk_1,  0);
/* update wl_crypto */
if (mode == "wpa" || mode == "wpa2" || mode == "psk") {
/* Save current crypto algorithm */
for (i = 0; i < document.form.wl_guest_crypto_1.length; i++) {
if (document.form.wl_guest_crypto_1[i].selected) {
cur = document.form.wl_guest_crypto_1[i].value;
break;
}
}
if (window.top.isModel()=="SnapAP" || window.top.isBand() == 'b' )
algos = new Array("TKIP");
else
{opts=document.form.wl_guest_auth_mode_1.options;
if (opts[opts.selectedIndex].text == "WPA-Auto-Personal")
algos = new Array("TKIP", "AES", "TKIP+AES");
else
algos = new Array("TKIP+AES");
}
/* Reconstruct algorithm array from new crypto algorithms */
document.form.wl_guest_crypto_1.length = algos.length;
for (var i in algos) {
document.form.wl_guest_crypto_1[i] = new Option(algos[i], algos[i].toLowerCase());
document.form.wl_guest_crypto_1[i].value = algos[i].toLowerCase();
if (algos[i].toLowerCase() == cur)
document.form.wl_guest_crypto_1[i].selected = true;
}
}
change_wep_type_guest(mode);
/* Save current network key index */
for (i = 0; i < document.form.wl_guest_key_1.length; i++)
{if (document.form.wl_guest_key_1[i].selected) {
cur = document.form.wl_guest_key_1[i].value;
break;
}
}
/* Define new network key indices */
if (mode == "wpa" || mode == "wpa2" || mode == "psk" || mode == "radius")
algos = new Array("2", "3");
else
{algos = new Array("1", "2", "3", "4");
if (!isload) cur = "1";
}
/* Reconstruct network key indices array from new network key indices */
document.form.wl_guest_key_1.length = algos.length;
for (var i in algos) {
document.form.wl_guest_key_1[i] = new Option(algos[i], algos[i]);
document.form.wl_guest_key_1[i].value = algos[i];
if (algos[i] == cur)
document.form.wl_guest_key_1[i].selected = true;
}
wl_wep_change_guest();
if ((mode == "wpa" || mode == "wpa2" || mode == "psk"))
{if (window.top.isModel()=="WL520" || window.top.isModel()=="SnapAP" || window.top.isBand()=='b' )
{}
else if(document.form.wl_guest_crypto_1[2].selected == true)
{document.form.wl_guest_wep_x_1.value = 0;
inputCtrl(document.form.wl_guest_wep_x_1,  0);
inputCtrl(document.form.wl_guest_key_type_1, 0);
inputCtrl(document.form.wl_guest_phrase_x_1,  0);
inputCtrl(document.form.wl_guest_key1_1, 0);
inputCtrl(document.form.wl_guest_key2_1, 0);
inputCtrl(document.form.wl_guest_key3_1, 0);
inputCtrl(document.form.wl_guest_key4_1, 0);
inputCtrl(document.form.wl_guest_key_1,  0);
}
}
else
{change_wlweptype_guest_sp(document.form.wl_guest_wep_x_1, "WLANConfig11b");
if (isload)
window.top.pageChanged = 0;
}
}
function change_wlweptype_guest(o, s)
{if (document.form.wl_guest_auth_mode_1.value!="psk")
{change = 1;
window.top.pageChanged = 1;
}
document.form.wl_guest_phrase_x_1.value = "";
if (o.value=="0")
{wflag = 0;
wep = "";
document.form.wl_guest_key1_1.value = wep;
document.form.wl_guest_key2_1.value = wep;
document.form.wl_guest_key3_1.value = wep;
document.form.wl_guest_key4_1.value = wep;
}
else
{wflag = 1;
key_type = document.form.wl_guest_key_type_1.value;
if (key_type=="0")
{if (o.value=="1")
{wep = "0000000000";
}
else  if (o.value=="2")
{wep = "00000000000000000000000000";
}
else
{wep = "00000000000000000000000000000000";
}
}
else
{if (o.value=="1")
{wep = "00000";
}
else  if (o.value=="2")
{wep = "0000000000000";
}
else
{wep = "0000000000000000";
}
}
is_wlphrase("WLANConfig11b", "wl_phrase_x", document.form.wl_guest_phrase_x_1);
}
inputCtrl(document.form.wl_guest_key_type_1, wflag);
inputCtrl(document.form.wl_guest_phrase_x_1, wflag);
inputCtrl(document.form.wl_guest_key1_1,     wflag);
inputCtrl(document.form.wl_guest_key2_1,     wflag);
inputCtrl(document.form.wl_guest_key3_1,     wflag);
inputCtrl(document.form.wl_guest_key4_1,     wflag);
inputCtrl(document.form.wl_guest_key_1,      wflag);
wl_wep_change_guest();
if (wflag=="1")
{document.form.wl_guest_phrase_x_1.focus();
}
return true;
}
function change_wlweptype_guest_sp(o, s)
{if (document.form.wl_guest_auth_mode_1.value!="psk")
{change = 1;
window.top.pageChanged = 1;
}
document.form.wl_guest_phrase_x_1.value = "";
if (o.value=="0")
{wflag = 0;
wep = "";
document.form.wl_guest_key1_1.value = wep;
document.form.wl_guest_key2_1.value = wep;
document.form.wl_guest_key3_1.value = wep;
document.form.wl_guest_key4_1.value = wep;
}
else
{wflag = 1;
key_type = document.form.wl_guest_key_type_1.value;
if (key_type=="0")
{if (o.value=="1")
{wep = "0000000000";
}
else  if (o.value=="2")
{wep = "00000000000000000000000000";
}
else
{wep = "00000000000000000000000000000000";
}
}
else
{if (o.value=="1")
{wep = "00000";
}
else  if (o.value=="2")
{wep = "0000000000000";
}
else
{wep = "0000000000000000";
}
}
if (!validate_wlkey_sp(document.form.wl_guest_key1_1, "WLANConfig11b") ||
!validate_wlkey_sp(document.form.wl_guest_key2_1, "WLANConfig11b") ||
!validate_wlkey_sp(document.form.wl_guest_key3_1, "WLANConfig11b") ||
!validate_wlkey_sp(document.form.wl_guest_key4_1, "WLANConfig11b")
)
is_wlphrase("WLANConfig11b", "wl_phrase_x", document.form.wl_guest_phrase_x_1);
}
inputCtrl(document.form.wl_guest_key_type_1, wflag);
inputCtrl(document.form.wl_guest_phrase_x_1, wflag);
inputCtrl(document.form.wl_guest_key1_1,     wflag);
inputCtrl(document.form.wl_guest_key2_1,     wflag);
inputCtrl(document.form.wl_guest_key3_1,     wflag);
inputCtrl(document.form.wl_guest_key4_1,     wflag);
inputCtrl(document.form.wl_guest_key_1,      wflag);
wl_wep_change_guest();
if (wflag=="1")
{document.form.wl_guest_phrase_x_1.focus();
}
return true;
}
function translate_lang(o,v)
{translated_string="";
if(v=='TW')
{if(o=='JS_fieldblank')
translated_string ="欄位不可以空白!!!";
else if(o=='JS_FirstTime')
translated_string="感謝您購買「華碩無線路由器」，您要直接啟動「快速安裝」程式嗎？";
else if(o=='JS_checkpass')
translated_string="兩組密碼字串不符!!!";
else if(o=='JS_duplicate')
translated_string="本項早已納入表格內!!!";
else if(o=='JS_notduplicate')
translated_string="本項未納入表格內!!!";
else if(o=='JS_validrange')
translated_string="請輸入一數值介於";
else if(o=='JS_validmac')
translated_string="請輸入完整包含12個16進位制字母的MAC位址。";
else if(o=='JS_validchar')
translated_string="無效字元!";
else if(o=='JS_validstrlen')
translated_string="此字串的最小長度：";
else if(o=='JS_validstrlen2')
translated_string="。重置為00000000。";
else if(o=='JS_wanip_required')
translated_string="需填入WAN IP!";
else if(o=='JS_wepkey')
translated_string="請輸入完整的WEP金鑰。";
else if(o=='JS_wepkey_11')
translated_string="請輸入完整的WEP金鑰。\n您可以輸入 10 個十六進位字元 (0~9, a~f, A~F)。";
else if(o=='JS_wepkey_12')
translated_string="請輸入完整的WEP金鑰。\n您可以輸入 26 個十六進位字元 (0~9, a~f, A~F)。";
else if(o=='JS_wepkey_21')
translated_string="請輸入完整的WEP金鑰。\n您可以輸入 5 個ASCII字元。";
else if(o=='JS_wepkey_22')
translated_string="請輸入完整的WEP金鑰。\n您可以輸入 13 個ASCII字元。";
else if(o=='JS_focus')
translated_string="超出集中範圍!!!";
else if(o=='JS_itemlimit1')
translated_string="本表僅允許 ";
else if(o=='JS_itemlimit2')
translated_string="項目!!!";
else if(o=='JS_validstr1')
translated_string="本字串無法起自";
else if(o=='JS_validstr2')
translated_string="本字串無法包含";
else if(o=='JS_validip')
translated_string=" 不是有效的IP位址!";
else if(o=='JS_validport')
translated_string="不是有效的通訊埠編號!";
else if(o=='JS_validportrange')
translated_string="不是有效的通訊埠範圍！";
else if(o=='JS_fixchannel')
translated_string="請設定適合WDS的固定通道!!!";
else if(o=='JS_ipblank')
translated_string="當您將IP或子網路遮罩或預設閘道器欄位保留空白，連線類型即會設定為「自動IP」!";
else if(o=='JS_ipblank1')
translated_string="本地IP或通訊埠範圍欄位不得空白!!!";
else if(o=='JS_FirmUpgrade')
translated_string="上傳及寫入韌體需花費約30秒。請勿重複按'上傳'。";
else if(o=='JS_WanLanAlert')
translated_string="選擇「丟棄」時，會將全部WAN to LAN封包當中除了與過濾表內相符以外的部份丟棄。請謹慎使用。";
else if(o=='JS_LanWanAlert')
translated_string="選擇「丟棄」時，會將全部LAN to WAN封包當中除了與過濾表內相符以外的部份丟棄。請謹慎使用。";
else if(o=='JS_wpapass')
translated_string="WPA預先共用金鑰不得超出63個字元!!!";
else if(o=='JS_passzero')
translated_string="WPA預先共用金鑰不得少於8個字元!!!如果您將本欄位保留空白，系統將會指定12345678作為您的通關密語。";
else if(o=='JS_to')
translated_string="，";
else if(o == 'JS_RESET')
translated_string = "警告:\n您更改的所有設定會遺失!\n要繼續嗎?";
else if(o == 'JS_DHCP1')
translated_string = "DHCP伺服器的設定值與目前LAN的IP位址不符。您要採用自動的方式變更嗎？";
else if(o == 'JS_DHCP2')
translated_string = "DHCP伺服器的設定值與目前「無線防火牆」的IP位址不符。您要採用自動的方式變更嗎？";
else if(o == 'JS_nowpa')
translated_string = "在此模式下不支援WPA/Radius with 802.1x";
else if(o == 'JS_no8021X')
translated_string = "在此模式下不支援Radius with 802.1x (no WPA-Enterprise/WPA2-Enterprise)";
else if(o == 'JS_GW_AP')
translated_string='在Gateway模式下，只有當WAN連線類型被設定為"Automatic IP"時，\n您才能透過乙太網路通訊埠（Ethernet port）進入設定頁面！';
else if(o == 'JS_AB_WMM_ME')
translated_string='AfterBurner與WMM不能同時啟用，WMM已被關閉！';
else if(o == 'JS_WMM_AB_ME')
translated_string='WMM與AfterBurner不能同時啟用，AfterBurner已被關閉！';
else if(o == 'JS_NO_AB')
translated_string='AfterBurner不能在Repeater或Gateway(Hotspot)模式下開啟！';
}
else if(v=='CN')
{if(o=='JS_fieldblank')
translated_string="栏目不可以空白!!!";
else if(o=='JS_FirstTime')
translated_string="感谢您购买「华硕无线路由器」，您要直接启动「快速设置」程序吗？";
else if(o=='JS_checkpass')
translated_string="兩組密碼字符串不符!!!";
else if(o=='JS_duplicate')
translated_string="本项早已纳入表格内!!!";
else if(o=='JS_notduplicate')
translated_string="本项未纳入表格内!!!";
else if(o=='JS_validrange')
translated_string="请输入一数值介于";
else if(o=='JS_validmac')
translated_string="请输入完整的包含12个16进制字符的MAC地址。";
else if(o=='JS_validchar')
translated_string="无效字符!";
else if(o=='JS_validstrlen')
translated_string="本字符串的最小长度：";
else if(o=='JS_validstrlen2')
translated_string="。重置为00000000。";
else if(o=='JS_wanip_required')
translated_string="需填入WAN IP!";
else if(o=='JS_wepkey')
translated_string="请输入完整的WEP密钥。";
else if(o=='JS_wepkey_11')
translated_string="请输入完整的WEP密钥。\n您可以输入 10 个十六进制字符 (0~9, a~f, A~F)。";
else if(o=='JS_wepkey_12')
translated_string="请输入完整的WEP密钥。\n您可以输入 26 个十六进制字符 (0~9, a~f, A~F)。";
else if(o=='JS_wepkey_21')
translated_string="请输入完整的WEP密钥。\n您可以输入 5 个ASCII字符。";
else if(o=='JS_wepkey_22')
translated_string="请输入完整的WEP密钥。\n您可以输入 13 个ASCII字符。";
else if(o=='JS_focus')
translated_string="超出集中范围!!!";
else if(o=='JS_itemlimit1')
translated_string="本表仅允许";
else if(o=='JS_itemlimit2')
translated_string="项目!!!";
else if(o=='JS_validstr1')
translated_string="本字符串无法起始自";
else if(o=='JS_validstr2')
translated_string="本字符串无法包含";
else if(o=='JS_validip')
translated_string=" 不是有效的IP地址!";
else if(o=='JS_validport')
translated_string="不是有效的端口编号!";
else if(o=='JS_validportrange')
translated_string="不是有效的端口范围！";
else if(o=='JS_fixchannel')
translated_string="请设置适合WDS的固定频道!!!";
else if(o=='JS_ipblank')
translated_string="当您将IP或子网掩码栏目保留空白，连接类型即会设置为「自动获取IP」!";
else if(o=='JS_ipblank1')
translated_string="本地IP或端口范围栏目不得空白!!!";
else if(o=='JS_FirmUpgrade')
translated_string="上传及写入韧体需花费约30秒。请勿重复按'上传'。";
else if(o=='JS_WanLanAlert')
translated_string="选择「丢弃」时，会将全部WAN to LAN封包当中除了与过滤表内相符以外的部份丢弃。请谨慎使用。";
else if(o=='JS_LanWanAlert')
translated_string="选择「丢弃」时，会将全部LAN to WAN封包当中除了与过滤表内相符以外的部份丢弃。请谨慎使用。";
else if(o=='JS_wpapass')
translated_string="WPA预先共享金钥不得多于63个字符!!!";
else if(o=='JS_passzero')
translated_string="WPA预先共享金钥不得少于8个字符!!!如果您将本栏目保留空白，系统将会指定12345678作为您的密码。";
else if(o=='JS_to')
translated_string="，";
else if(o == 'JS_RESET')
translated_string = "警告：\n您所有的设置将会遗失!\n是否还要继续？";
else if(o == 'JS_DHCP1')
translated_string = "DHCP服务器的设置与目前LAN的IP地址不符。您要采用自动的方式变更吗？";
else if(o == 'JS_DHCP2')
translated_string = "DHCP服务器的设置与目前「无线防火墙」的IP地址不符。您要采用自动的方式变更吗？";
else if(o == 'JS_nowpa')
translated_string = "在此模式下不支援WPA/Radius with 802.1x";
else if(o == 'JS_no8021X')
translated_string = "在此模式下不支援Radius with 802.1x (no WPA-Enterprise/WPA2-Enterprise)";
else if(o == 'JS_GW_AP')
translated_string='在Gateway模式下, 只有当WAN连接类型被设置为"Automatic IP"时，\n您才能透过以太网络端口（Ethernet port）进入设定页面！';
else if(o == 'JS_AB_WMM_ME')
translated_string='AfterBurner与WMM不能同时启用，WMM已被关闭！';
else if(o == 'JS_WMM_AB_ME')
translated_string='WMM与AfterBurner不能同时启用，AfterBurner已被关闭！';
else if(o == 'JS_NO_AB')
translated_string='AfterBurner不能在Repeater或Gateway(Hotspot)模式下开启！';
}
else if(v=='DE')
{if(o=='JS_fieldblank')
translated_string ="Die Felder dürfen nicht leer bleiben!!!";
else if(o=='JS_FirstTime')
translated_string="Vielen Dank für den Erwerb des ASUS Wireless Routers, wollen Sie die Schnelleinstellung sofort starten?";
else if(o=='JS_checkpass')
translated_string="Die zwei Passwörter stimmen nicht überein!!!";
else if(o=='JS_duplicate')
translated_string="Dieser Eintrag war schon in der Tabelle!!!";
else if(o=='JS_notduplicate')
translated_string="Dieser Eintrag war nicht in der Tabelle!!!";
else if(o=='JS_validrange')
translated_string="Bitte geben Sie einen Wert zwischen";
else if(o=='JS_validmac')
translated_string="Bitte geben Sie die komplette MAC-Adresse, welche 12 hexadezimale Zeichen enthält, ein.";
else if(o=='JS_validchar')
translated_string="Ungültiges Zeichen!";
else if(o=='JS_validstrlen')
translated_string="Mindestlänge dieser Zeichenkette: ";
else if(o=='JS_validstrlen2')
translated_string=". Es wurde auf 00000000 zurückgesetzt.";
else if(o=='JS_wanip_required')
translated_string="WAN-IP wird benötigt!";
else if(o=='JS_wepkey')
translated_string="Bitte geben Sie den vollständigen WEP-Schlüssel ein.";
else if(o=='JS_wepkey_11')
translated_string="Bitte tragen Sie den vollständigen WEP-Key ein. \nGeben Sie 10 HEX digits (0~9, a~f, A~F) ein.";
else if(o=='JS_wepkey_12')
translated_string="Bitte tragen Sie den vollständigen WEP-Key ein. \nGeben Sie 26 HEX digits (0~9, a~f, A~F) ein.";
else if(o=='JS_wepkey_21')
translated_string="Bitte tragen Sie den vollständigen WEP-Key ein. \nGeben Sie 5 ASCII characters ein.";
else if(o=='JS_wepkey_22')
translated_string="Bitte tragen Sie den vollständigen WEP-Key ein. \nGeben Sie 13 ASCII characters ein.";
else if(o=='JS_focus')
translated_string="Out of focus!!!";
else if(o=='JS_itemlimit1')
translated_string="This table only allow";
else if(o=='JS_itemlimit2')
translated_string="items!!!";
else if(o=='JS_validstr1')
translated_string="Diese Zeichenkette darf nicht mit \" starten";
else if(o=='JS_validstr2')
translated_string="Diese Zeichenkette darf kein ??? enthalten";
else if(o=='JS_validstr3')
translated_string="Diese Zeichenkette darf kein";
else if(o=='JS_validstr4')
translated_string="enthalten";
else if(o=='JS_validip')
translated_string=" ist keine gültige IP-Adresse!";
else if(o=='JS_validport')
translated_string="ist keine gültige Port-Nummer!";
else if(o=='JS_validportrange')
translated_string="ist kein gültiger Port-Bereich!";
else if(o=='JS_fixchannel')
translated_string="Bitte setzen Sie für WDS einen festen Kanal!!!";
else if(o=='JS_ipblank')
translated_string="Wenn Sie die Felder für die IP, Subnetzmaske oder Standard-Gateway leer lassen wird die Verbindungsart auf Automatische IP eingestellt!";
else if(o=='JS_ipblank1')
translated_string="Die Felder für die lokale IP oder den Port-Bereich dürfen nicht leer bleiben!!!";
else if(o=='JS_FirmUpgrade')
translated_string="Es dauert etwa 30 Sekunden, um die Firmware hochzuladen und zu programmieren. Klicken Sie bitte nicht zweimal auf die Schaltfläche 'Upload'.";
else if(o=='JS_WanLanAlert')
translated_string="Die Auswahl von DROP wird alle WAN zu LAN Pakete, ausgenommen diejenigen, die mit der Filtertabelle übereinstimmen ablehnen. Bitte vorsichtig benutzen.";
else if(o=='JS_LanWanAlert')
translated_string="Die Auswahl von DROP wird alle LAN zu WAN Pakete, ausgenommen diejenigen, die mit der Filtertabelle übereinstimmen ablehnen. Bitte vorsichtig benutzen.";
else if(o=='JS_wpapass')
translated_string="Gleiche gemeinsam benutzte WPA-Schlüssel sollten weniger als 64 Zeichen haben!!!";
else if(o=='JS_passzero')
translated_string="Gleiche gemeinsam benutzte WPA-Schlüssel sollten mehr als 7 Zeichen besitzen!!! Wenn Sie dieses Feld leer lassen wird das System 12345678 als Ihr Passwort festlegen.";
else if(o=='JS_to')
translated_string=",";
else if(o == 'JS_RESET')
translated_string = "WARNUNG:\nAlle Ihre Einstellungen gehen verloren!\nFortfahren oder nicht?";
else if(o == 'JS_DHCP1')
translated_string = "Die Einstellung des DHCP-Servers stimmt nich mit der derzeitigen IP-Adresse des LANs überein. Wollen Sie dies automatisch ändern?";
else if(o == 'JS_DHCP2')
translated_string = "Die Einstellung des DHCP-Servers stimmt nich mit der derzeitigen IP-Adresse der Wireless Firewall überein. Wollen Sie dies automatisch ändern?";
else if(o == 'JS_nowpa')
translated_string = "WPA/Radius mit 802.1x wird im URE-Modus nicht unterstützt!;";
else if(o == 'JS_no8021X')
translated_string = "Radius mit 802.1x (kein WPA-Enterprise/WPA2-Enterprise) wird in diesem Modus nicht unterstützt!";
else if(o == 'JS_GW_AP')
translated_string='Sie können die Konfigurationsseite über den Ethernet-Port nur erreichen, wenn die Einstellung der WAN-Verbindungsart auf "Automatische IP" eingestellt ist!';
else if(o == 'JS_AB_WMM_ME')
translated_string='AfterBurner und WMM schließen sich gegenseitig aus. WMM ist nun deaktiviert!';
else if(o == 'JS_WMM_AB_ME')
translated_string='WMM und AfterBurner schließen sich gegenseitig aus. AfterBurner ist nun deaktiviert!';
else if(o == 'JS_NO_AB')
translated_string='AfterBurner ist im Repeater- oder Gateway (Hotspot)-Modus deaktiviert!';
}
else if(v=='JP')
{if(o=='JS_fieldblank')
translated_string ="フィールドは空にできません!!!";
else if(o=='JS_FirstTime')
translated_string="ASUS ワイヤレスルーターをお買い上げいただきましてありがとうございます。セットアップウィザードを直接起動しますか?";
else if(o=='JS_checkpass')
translated_string="2 つのパスワード文字列が一致しません!!!";
else if(o=='JS_duplicate')
translated_string="このエントリは表にありました!!!";
else if(o=='JS_notduplicate')
translated_string="このエントリは表にありませでした!!!";
else if(o=='JS_validrange')
translated_string="次の間の有効な値を入力してください";
else if(o=='JS_validmac')
translated_string="12 の 16 進法の文字を含む完全な MAC アドレスを入力してください。";
else if(o=='JS_validchar')
translated_string="文字が無効です!";
else if(o=='JS_validstrlen')
translated_string="このストリングは最短：";
else if(o=='JS_validstrlen2')
translated_string="。 00000000にリセットされます。";
else if(o=='JS_wanip_required')
translated_string="WAN IP が必要です!";
else if(o=='JS_wepkey')
translated_string="完全な WEP キーを入力してください。";
else if(o=='JS_wepkey_11')
translated_string="完全な WEP キーを入力してください。\n 10 の 16 進法の数字（0～9、a～f、 A～F）を入力できます。";
else if(o=='JS_wepkey_12')
translated_string="完全な WEP キーを入力してください。\n 26 の 16 進法の数字（0～9、a～f、 A～F）を入力できます。";
else if(o=='JS_wepkey_21')
translated_string="完全な WEP キーを入力してください。\n5 つの ASCII 文字を入力できます。";
else if(o=='JS_wepkey_22')
translated_string="完全な WEP キーを入力してください。\n13 の ASCII 文字を入力できます。";
else if(o=='JS_focus')
translated_string="焦点が合っていません!!!";
else if(o=='JS_itemlimit1')
translated_string="この表が許可するのは";
else if(o=='JS_itemlimit2')
translated_string="項目だけです!!!";
else if(o=='JS_validstr1')
translated_string="この文字列は次で始まることはできません";
else if(o=='JS_validstr2')
translated_string="この文字列は次を含むことはできません";
else if(o=='JS_validip')
translated_string=" は無効な IP アドレスです!";
else if(o=='JS_validport')
translated_string=" は無効なポート番号です!";
else if(o=='JS_validportrange')
translated_string=" ポート範囲が無効です!";
else if(o=='JS_fixchannel')
translated_string="WDS 用の固定チャンネルを設定してください!!!";
else if(o=='JS_ipblank')
translated_string="IP フィールドまたはサブネットマスクフィールドを空白のままにすると、接続タイプはAutomatic IP [自動 IP] として設定されます。";
else if(o=='JS_ipblank1')
translated_string="ローカル IP フィールドまたはポート範囲フィールドは空にできません!!!";
else if(o=='JS_FirmUpgrade')
translated_string="ファームウェアをアップデートしてプログラムするには約 30 秒掛かります。 「アップデート」は 2 回押さないでください。";
else if(o=='JS_WanLanAlert')
translated_string="DROP [廃棄]を選択して、フィルター表で一致したパケット以外の WAN から LAN へのパケットを廃棄します。 これは注意して使用してください。";
else if(o=='JS_LanWanAlert')
translated_string="DROP [廃棄]を選択して、フィルター表で一致したパケット以外の WAN から LAN へのパケットを廃棄します。 これは注意して使用してください。";
else if(o=='JS_wpapass')
translated_string="WPA 事前共有キーは 64 文字未満にします!!!";
else if(o=='JS_passzero')
translated_string="WPA 事前共有キーは 7 文字未満にします!!! このフィールを空白のままにすると、システムは 12345678 をあなたのパスフレーズとして割り当てます。";
else if(o=='JS_to')
translated_string=",";
else if(o == 'JS_RESET')
translated_string = "警告:\nすべての設定が失われます!\n続行しますか?";
else if(o == 'JS_DHCP1')
translated_string = "DHCP サーバーの設定は LAN の現在の IP アドレスと一致しません。 自動的に変更しますか?";
else if(o == 'JS_DHCP2')
translated_string = "DHCP サーバーの設定はワイヤレスファイアーウォールの現在の IP アドレスと一致しません。 自動的に変更しますか?";
else if(o == 'JS_nowpa')
translated_string = "このモードでは 802.1x のあるWPA/Radius はサポートされません!";
else if(o == 'JS_no8021X')
translated_string = "このモードでは 802.1x のあるRadius（WPA エンタープライズ/WPA2 エンタープライズなし）はサポートされません!";
else if(o == 'JS_GW_AP')
translated_string= 'WAN 接続タイプを "Automatic IP" [自動 IP] に設定した場合は、\nイーサネットポート経由でしか設定ページを開くことができません!';
else if(o == 'JS_AB_WMM_ME')
translated_string='AfterBurner [アフターバーナー] と WMM は相互に排他的です。 WMM は現在無効です!';
else if(o == 'JS_WMM_AB_ME')
translated_string='WMM とAfterBurner [アフターバーナー]は相互に排他的です。 AfterBurner [アフターバーナー] は現在無効です!';
else if(o == 'JS_NO_AB')
translated_string='Repeater [リピーター] モードまたは Gateway [ゲートウェイ] （ホットスポット）モードではア AfterBurner [アフターバーナー] は無効です!';
}
else if(v=='TR')
{if(o=='JS_fieldblank')
translated_string ="Alanlar boş olamaz!!!";
else if(o=='JS_FirstTime')
translated_string="ASUS Kablosuz Yönlendiricisi satın aldığınız için teşekkür ederiz, doğrudan Hızlı Kurulumu başlatmak ister misiniz?";
else if(o=='JS_checkpass')
translated_string="İki parola birbiri ile eşleşmiyor!!!";
else if(o=='JS_duplicate')
translated_string="Bu giriş tabloda var!!!";
else if(o=='JS_notduplicate')
translated_string="Bu giril tabloda yok!!!";
else if(o=='JS_validrange')
translated_string="Lütfen şunlar arasında bir değer girin";
else if(o=='JS_validmac')
translated_string="Lütfen 12 onaltılık harf içeren tam bir MAC adresi girin.";
else if(o=='JS_validchar')
translated_string="Geçersiz karakter!";
else if(o=='JS_validstrlen')
translated_string="Bu ifadenin minimum uzunluğu: ";
else if(o=='JS_validstrlen2')
translated_string=".00000000 olarak sıfırlandı.";
else if(o=='JS_wanip_required')
translated_string="WAN IP gereklidir!";
else if(o=='JS_wepkey')
translated_string="Lütfen tam WEP anahtarını girin.";
else if(o=='JS_wepkey_11')
translated_string="Lütfen tam WEP anahtarını girin.\n10 ONALTILI haneyi girebilirsiniz (0~9, a~f, A~F).";
else if(o=='JS_wepkey_12')
translated_string="Lütfen tam WEP anahtarını girin.\n26 ONALTILI haneyi girebilirsiniz (0~9, a~f, A~F).";
else if(o=='JS_wepkey_21')
translated_string="Lütfen tam WEP anahtarını girin.\n5 ASCII karakterini girebilirsiniz.";
else if(o=='JS_wepkey_22')
translated_string="Lütfen tam WEP anahtarını girin.\n13 ASCII karakterini girebilirsiniz.";
else if(o=='JS_focus')
translated_string="Odak dışı!!!";
else if(o=='JS_itemlimit1')
translated_string="bu tablo sadece şuna izin verir";
else if(o=='JS_itemlimit2')
translated_string="öğeler!!!";
else if(o=='JS_validstr1')
translated_string="Bu dizin şununla başlayamaz";
else if(o=='JS_validstr2')
translated_string="Bu dizin şunları içeremez";
else if(o=='JS_validip')
translated_string=" geçerli bir IP adresi değil!";
else if(o=='JS_validport')
translated_string=" geçerli bir bağlantı noktası numarası değil!";
else if(o=='JS_validportrange')
translated_string=" geçerli bir bağlantı noktası aralığı değil!";
else if(o=='JS_fixchannel')
translated_string="Lütfen WDS için sabit bir kanal ayarlayın!!!";
else if(o=='JS_ipblank')
translated_string="IP'den çıktığınızda, alt ağ maskesi veya varsayılan ağ geçidi alanları  boş olacaktır, bağlantı türü Otomatik IP olarak kaydedilecektir!";
else if(o=='JS_ipblank1')
translated_string="Yerel IP veya bağlantı noktası mesafe alanları boş olamaz!!!";
else if(o=='JS_FirmUpgrade')
translated_string="Bellenimi yüklemesi ve programlaması yaklaşık 30 saniye sürer. Lütfen 'Yükle'ye iki kez basmayın.";
else if(o=='JS_WanLanAlert')
translated_string="DROP seçimi, filtre tablosunda eşleşenler haricinde tüm WAN'ları LAN paketlerine düşürecektir. Lütfen dikkatlice kullanın.";
else if(o=='JS_LanWanAlert')
translated_string="DROP seçimi, filtre tablosunda eşleşenler haricinde tüm LAN'ları WAN paketlerine düşürecektir. Lütfen dikkatlice kullanın.";
else if(o=='JS_wpapass')
translated_string="WPA Ön paylaşımlı anahtarın uzunluğu 64 karakterden kısa olmalıdır!!!";
else if(o=='JS_passzero')
translated_string="WPA Ön paylaşımlı anahtarın uzunluğu 7 karakterden uzun olmalıdır!!! Eğer bu alanı boş bırakırsanız, sistem şifreni 12345678 olarak atayacaktır.";
else if(o=='JS_to')
translated_string=",";
else if(o == 'JS_RESET')
translated_string = "UYARI:\nTüm ayarlarınızı kaybedeceksiniz!\nDevam etsin mi etmesin mi?";
else if(o == 'JS_DHCP1')
translated_string = "DHCP sunucusunun ayarı LAN adresinin mevcut IP'si ile eşleşmiyor. Bunu otomatik olarak değiştirmek ister misiniz?";
else if(o == 'JS_DHCP2')
translated_string = "DHCP sunucusunun ayarı Kablosuz Güvenlik Duvarının mevcut IP adresi ile eşleşmiyor. Bunu otomatik olarak değiştirmek ister misiniz?";
else if(o == 'JS_nowpa')
translated_string = "802.1x ile WPA/Yarıçap bu modda desteklenmiyor!";
else if(o == 'JS_no8021X')
translated_string = "802.1x ile yarıçap (WPA-Enterprise/WPA2-Enterprise yok) bu modda desteklenmiyor!";
else if(o == 'JS_GW_AP')
translated_string= 'WAN Bağlantı türünü "Otomatik IP" olara ayarladığınızda\nyapılandırma sayfasına yalnızca Ethernet bağlantı noktasından girebilirsiniz!';
else if(o == 'JS_AB_WMM_ME')
translated_string='AfterBurner ve WMM karşılıklı olara özeldir. WMM şu anda engellendi!';
else if(o == 'JS_WMM_AB_ME')
translated_string='WMM ve AfterBurner karşılıklı olara özeldir. AfterBurner şu anda engellendi!';
else if(o == 'JS_NO_AB')
translated_string='AfterBurner, Yineleyici veya A Geçidi (Ortak Alan) modunda engellendi!';
}
else
{if(o=='JS_fieldblank')
translated_string ="Fields can't be blank!!!";
else if(o=='JS_FirstTime')
translated_string="Thanks for purchasing the ASUS Wireless Router, do you like to start Quick Setup directly?";
else if(o=='JS_checkpass')
translated_string="Two password strings mismatched !!!";
else if(o=='JS_duplicate')
translated_string="This entry has been in table!!!";
else if(o=='JS_notduplicate')
translated_string="This entry has not been in table!!!";
else if(o=='JS_validrange')
translated_string="Please enter a value between";
else if(o=='JS_validmac')
translated_string="Please enter the complete MAC address which contains 12 hexadecimal letters.";
else if(o=='JS_validchar')
translated_string="Invalid character!";
else if(o=='JS_validstrlen')
translated_string="The minimum length of this string: ";
else if(o=='JS_validstrlen2')
translated_string=". It is reset to 00000000.";
else if(o=='JS_wanip_required')
translated_string="WAN IP is required!";
else if(o=='JS_wepkey')
translated_string="Please enter the complete WEP key.";
else if(o=='JS_wepkey_11')
translated_string="Please enter the complete WEP key.\nYou can enter 10 HEX digits (0~9, a~f, A~F).";
else if(o=='JS_wepkey_12')
translated_string="Please enter the complete WEP key.\nYou can enter 26 HEX digits (0~9, a~f, A~F).";
else if(o=='JS_wepkey_21')
translated_string="Please enter the complete WEP key.\nYou can enter 5 ASCII characters.";
else if(o=='JS_wepkey_22')
translated_string="Please enter the complete WEP key.\nYou can enter 13 ASCII characters.";
else if(o=='JS_focus')
translated_string="Out of focus!!!";
else if(o=='JS_itemlimit1')
translated_string="This table only allow";
else if(o=='JS_itemlimit2')
translated_string="items!!!";
else if(o=='JS_validstr1')
translated_string="This string can not start with";
else if(o=='JS_validstr2')
translated_string="This string can not contain";
else if(o=='JS_validip')
translated_string=" is not a valid IP address!";
else if(o=='JS_validport')
translated_string="is not a valid port number!";
else if(o=='JS_validportrange')
translated_string="is not a valid port range!";
else if(o=='JS_fixchannel')
translated_string="Please set a fixed channel for WDS!!!";
else if(o=='JS_ipblank')
translated_string="As you leave IP, subnet mask or default gateway fields blank, connection type will be set as Automatic IP!";
else if(o=='JS_ipblank1')
translated_string="Local IP or port range fields can not be empty!!!";
else if(o=='JS_FirmUpgrade')
translated_string="It spends about 30 seconds to upload and program the firmware. Please do not press 'Upload' twice.";
else if(o=='JS_WanLanAlert')
translated_string="Selecting DROP will drop all WAN to LAN packets except for those matched in filter table. Please use it carefully.";
else if(o=='JS_LanWanAlert')
translated_string="Selecting DROP will drop all LAN to WAN packets except for those matched in filter table. Please use it carefully.";
else if(o=='JS_wpapass')
translated_string="WPA Pre-shared key should be less than 64 characters!!!";
else if(o=='JS_passzero')
translated_string="WPA Pre-shared key should be more than 7 characters!!! If you leave this field blank, system will assign 12345678 as your passphrase.";
else if(o=='JS_to')
translated_string=",";
else if(o == 'JS_RESET')
translated_string = "WARNING:\nAll your settings will be lost!\nProceed or not?";
else if(o == 'JS_DHCP1')
translated_string = "The setting of DHCP server does not match current IP address of LAN. Would you like to change it automatically?";
else if(o == 'JS_DHCP2')
translated_string = "The setting of DHCP server does not match current IP address of Wireless Firewall. Would you like to change it automatically?";
else if(o == 'JS_nowpa')
translated_string = "WPA/Radius with 802.1x is not supported under this mode!";
else if(o == 'JS_no8021X')
translated_string = "Radius with 802.1x (no WPA-Enterprise/WPA2-Enterprise) is not supported under this mode!";
else if(o == 'JS_GW_AP')
translated_string='You only can enter configuration page through the Ethernet port\nwhen setting WAN Connection Type as "Automatic IP"!';
else if(o == 'JS_AB_WMM_ME')
translated_string='AfterBurner and WMM are mutually exclusive. WMM is now disabled!';
else if(o == 'JS_WMM_AB_ME')
translated_string='WMM and AfterBurner are mutually exclusive. AfterBurner is now disabled!';
else if(o == 'JS_NO_AB')
translated_string='AfterBurner is disabled in Repeater or Gateway(Hotspot) mode!';
}
return translated_string;
}
