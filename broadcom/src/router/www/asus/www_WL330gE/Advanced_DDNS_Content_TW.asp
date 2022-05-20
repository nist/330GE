<head>
<title><#ZVMODELVZ#> Web Manager</title>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<meta http-equiv="Pragma" content="no-cache">
<link rel="stylesheet" type="text/css" href="style.css" media="screen"></link>
<script language="JavaScript" type="text/javascript" src="overlib.js"></script>
<script language="JavaScript" type="text/javascript" src="general.js"></script>
</head>
<div id="overDiv" style="position:absolute; visibility:hidden; z-index:1000;"></div>
<body onLoad="load_body()" onunLoad="return unload_body();">
<form method="GET" name="form" action="apply.cgi">
<!-- Table for the conntent page -->
<table width="666" border="0" cellpadding="0" cellspacing="0">
<input type="hidden" name="current_page" value="Advanced_DDNS_Content.asp">
<input type="hidden" name="next_page" value="SaveRestart.asp">
<input type="hidden" name="next_host" value="">
<input type="hidden" name="sid_list" value="LANHostConfig;">
<input type="hidden" name="group_id" value="">
<input type="hidden" name="modified" value="0">
<input type="hidden" name="action_mode" value="">
<input type="hidden" name="first_time" value="">
<input type="hidden" name="action_script" value="">
<input type="hidden" name="preferred_lang" value="<% nvram_get_x("","preferred_lang"); %>">
<input type="hidden" name="ddns_passwd_x_org" value="<% nvram_char_to_ascii("LANHostConfig","ddns_passwd_x"); %>">
<input type="hidden" name="ddns_username_x_org" value="<% nvram_char_to_ascii("LANHostConfig","ddns_username_x"); %>">
<tr>
<td>
<table width="666" border="1" cellpadding="0" cellspacing="0" bordercolor="E0E0E0">
<tr class="content_header_tr">
<td class="content_header_td_title" colspan="2"><#t1IP#> - <#t2Misc#></td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#LANHostConfig_x_UPnPEnable_itemdesc#>', LEFT);" onMouseOut="return nd();"><#LANHostConfig_x_UPnPEnable_itemname#>
</td><td class="content_input_td"><input type="radio" value="1" name="upnp_enable" class="content_input_fd" onClick="return change_common_radio(this, 'LANHostConfig', 'upnp_enable', '1')" <% nvram_match_x("LANHostConfig","upnp_enable", "1", "checked"); %>><#RADIO_TEXT_YES#></input><input type="radio" value="0" name="upnp_enable" class="content_input_fd" onClick="return change_common_radio(this, 'LANHostConfig', 'upnp_enable', '0')" <% nvram_match_x("LANHostConfig","upnp_enable", "0", "checked"); %>><#RADIO_TEXT_NO#></input></td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#LANHostConfig_x_ServerLogEnable_itemdesc#>', LEFT);" onMouseOut="return nd();"><#LANHostConfig_x_ServerLogEnable_itemname#>
</td><td class="content_input_td"><input type="text" maxlength="15" class="content_input_fd" size="15" name="log_ipaddr" value="<% nvram_get_x("LANHostConfig","log_ipaddr"); %>" onChange="page_changed()" onBlur="return validate_ipaddr(this, 'log_ipaddr')" onKeyPress="return is_ipaddr(this)" onKeyUp="change_ipaddr(this)"></td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#LANHostConfig_x_TimeZone_itemdesc#>', LEFT);" onMouseOut="return nd();"><#LANHostConfig_x_TimeZone_itemname#></td>
<td class="content_input_td">
<select name="time_zone" class="content_input_fd" onChange="return change_common(this, 'LANHostConfig', 'time_zone')">
<option class="content_input_fd" value="UCT12" <% nvram_match_x("LANHostConfig","time_zone", "UCT12","selected"); %>>(GMT-12:00) 國際換日線以西</option>
<option class="content_input_fd" value="UCT11" <% nvram_match_x("LANHostConfig","time_zone", "UCT11","selected"); %>>(GMT-11:00) 中途島，薩摩亞群島</option>
<option class="content_input_fd" value="UCT10" <% nvram_match_x("LANHostConfig","time_zone", "UCT10","selected"); %>>(GMT-10:00) 夏威夷</option>
<option class="content_input_fd" value="NAST9NADT" <% nvram_match_x("LANHostConfig","time_zone", "NAST9NADT","selected"); %>>(GMT-09:00) 阿拉斯加</option>
<option class="content_input_fd" value="PST8PDT" <% nvram_match_x("LANHostConfig","time_zone", "PST8PDT","selected"); %>>(GMT-08:00) 太平洋時間 (美國和加拿大)，提華納</option>
<option class="content_input_fd" value="MST7MDT" <% nvram_match_x("LANHostConfig","time_zone", "MST7MDT","selected"); %>>(GMT-07:00) 山區時間 (美國和加拿大)</option>
<option class="content_input_fd" value="MST7" <% nvram_match_x("LANHostConfig","time_zone", "MST7","selected"); %>>(GMT-07:00) 亞歷桑那</option>
<option class="content_input_fd" value="MST7_1" <% nvram_match_x("LANHostConfig","time_zone", "MST7_1","selected"); %>>(GMT-07:00) 齊驊華，拉帕茲，馬札特蘭</option>
<option class="content_input_fd" value="CST6CDT_1" <% nvram_match_x("LANHostConfig","time_zone", "CST6CDT_1","selected"); %>>(GMT-06:00) 中部時間 (美國和加拿大)</option>
<option class="content_input_fd" value="CST6CDT_2" <% nvram_match_x("LANHostConfig","time_zone", "CST6CDT_2","selected"); %>>(GMT-06:00) 薩克其萬(加拿大)</option>
<option class="content_input_fd" value="CST6CDT_3" <% nvram_match_x("LANHostConfig","time_zone", "CST6CDT_3","selected"); %>>(GMT-06:00) 瓜達拉加若，墨西哥城</option>
<option class="content_input_fd" value="CST6CDT_3_1" <% nvram_match_x("LANHostConfig","time_zone", "CST6CDT_3_1","selected"); %>>(GMT-06:00) 蒙特利</option>
<option class="content_input_fd" value="UCT6" <% nvram_match_x("LANHostConfig","time_zone", "UCT6","selected"); %>>(GMT-06:00) 中部時間 (美國和加拿大)</option>
<option class="content_input_fd" value="EST5EDT" <% nvram_match_x("LANHostConfig","time_zone", "EST5EDT","selected"); %>>(GMT-05:00) 東部時間 (美國和加拿大)</option>
<option class="content_input_fd" value="UCT5_1" <% nvram_match_x("LANHostConfig","time_zone", "UCT5_1","selected"); %>>(GMT-05:00) 大西洋時間 (加拿大)</option>
<option class="content_input_fd" value="UCT5_2" <% nvram_match_x("LANHostConfig","time_zone", "UCT5_2","selected"); %>>(GMT-05:00) 波哥大，利馬，基多</option>
<option class="content_input_fd" value="AST4ADT" <% nvram_match_x("LANHostConfig","time_zone", "AST4ADT","selected"); %>>(GMT-04:00) 大西洋時間 (加拿大)</option>
<option class="content_input_fd" value="UCT4_1" <% nvram_match_x("LANHostConfig","time_zone", "UCT4_1","selected"); %>>(GMT-04:00) 卡拉卡斯，拉帕茲</option>
<option class="content_input_fd" value="UCT4_2" <% nvram_match_x("LANHostConfig","time_zone", "UCT4_2","selected"); %>>(GMT-04:00) 聖地牙哥</option>
<option class="content_input_fd" value="NST3.30" <% nvram_match_x("LANHostConfig","time_zone", "NST3.30","selected"); %>>(GMT-03:30) 紐芬蘭</option>
<option class="content_input_fd" value="EBST3EBDT_1" <% nvram_match_x("LANHostConfig","time_zone", "EBST3EBDT_1","selected"); %>>(GMT-03:00) 巴西利亞</option>
<option class="content_input_fd" value="UCT3" <% nvram_match_x("LANHostConfig","time_zone", "UCT3","selected"); %>>(GMT-03:00) 布宜諾斯艾利斯，佐治敦</option>
<option class="content_input_fd" value="EBST3EBDT_2" <% nvram_match_x("LANHostConfig","time_zone", "EBST3EBDT_2","selected"); %>>(GMT-03:00) 格陵蘭</option>
<option class="content_input_fd" value="NORO2" <% nvram_match_x("LANHostConfig","time_zone", "NORO2","selected"); %>>(GMT-02:00) 大西洋中部</option>
<option class="content_input_fd" value="EUT1EUTDST" <% nvram_match_x("LANHostConfig","time_zone", "EUT1EUTDST","selected"); %>>(GMT-01:00) 亞速爾群島</option>
<option class="content_input_fd" value="UCT1" <% nvram_match_x("LANHostConfig","time_zone", "UCT1","selected"); %>>(GMT-01:00) 維德角群島</option>
<option class="content_input_fd" value="GMT0BST_1" <% nvram_match_x("LANHostConfig","time_zone", "GMT0BST_1","selected"); %>>(GMT) 格林威治時間：都柏林，愛丁堡，里斯本，倫敦</option>
<option class="content_input_fd" value="GMT0BST_2" <% nvram_match_x("LANHostConfig","time_zone", "GMT0BST_2","selected"); %>>(GMT) 卡薩布蘭卡，蒙羅維亞(賴比瑞亞)</option>
<option class="content_input_fd" value="UCT-1_1" <% nvram_match_x("LANHostConfig","time_zone", "UCT-1_1","selected"); %>>(GMT+01:00) 貝爾格勒，布拉提斯拉瓦，布達佩斯</option>
<option class="content_input_fd" value="UCT-1_1_1" <% nvram_match_x("LANHostConfig","time_zone", "UCT-1_1_1","selected"); %>>(GMT+01:00) 盧布亞那，布拉格</option>
<option class="content_input_fd" value="UCT-1_2" <% nvram_match_x("LANHostConfig","time_zone", "UCT-1_2","selected"); %>>(GMT+01:00) 塞拉耶佛，斯高彼亞，索非亞</option>
<option class="content_input_fd" value="UCT-1_2_1" <% nvram_match_x("LANHostConfig","time_zone", "UCT-1_2_1","selected"); %>>(GMT+01:00) 維爾紐斯，華沙，札格雷布</option>
<option class="content_input_fd" value="MET-1METDST" <% nvram_match_x("LANHostConfig","time_zone", "MET-1METDST","selected"); %>>(GMT+01:00) 布魯塞爾，哥本哈根</option>
<option class="content_input_fd" value="MET-1METDST_1" <% nvram_match_x("LANHostConfig","time_zone", "MET-1METDST_1","selected"); %>>(GMT+01:00) 馬德里，巴黎</option>
<option class="content_input_fd" value="MEZ-1MESZ" <% nvram_match_x("LANHostConfig","time_zone", "MEZ-1MESZ","selected"); %>>(GMT+01:00) 阿姆斯特丹，柏林，伯恩</option>
<option class="content_input_fd" value="MEZ-1MESZ_1" <% nvram_match_x("LANHostConfig","time_zone", "MEZ-1MESZ_1","selected"); %>>(GMT+01:00) 羅馬，斯德哥爾摩，維也納</option>
<option class="content_input_fd" value="UCT-1_3" <% nvram_match_x("LANHostConfig","time_zone", "UCT-1_3","selected"); %>>(GMT+01:00) 中西非</option>
<option class="content_input_fd" value="UCT-2" <% nvram_match_x("LANHostConfig","time_zone", "UCT-2","selected"); %>>(GMT+02:00) 布加勒斯特</option>
<option class="content_input_fd" value="EST-2EDT" <% nvram_match_x("LANHostConfig","time_zone", "EST-2EDT","selected"); %>>(GMT+02:00) 開羅</option>
<option class="content_input_fd" value="UCT-2_1" <% nvram_match_x("LANHostConfig","time_zone", "UCT-2_1","selected"); %>>(GMT+02:00) 赫爾辛基，里加，塔林</option>
<option class="content_input_fd" value="UCT-2_2" <% nvram_match_x("LANHostConfig","time_zone", "UCT-2_2","selected"); %>>(GMT+02:00) 雅典，伊斯坦堡，明斯克</option>
<option class="content_input_fd" value="IST-2IDT" <% nvram_match_x("LANHostConfig","time_zone", "IST-2IDT","selected"); %>>(GMT+02:00) 耶路撒冷</option>
<option class="content_input_fd" value="SAST-2" <% nvram_match_x("LANHostConfig","time_zone", "SAST-2","selected"); %>>(GMT+02:00) 哈拉雷，皮托里</option>
<option class="content_input_fd" value="MST-3MDT" <% nvram_match_x("LANHostConfig","time_zone", "MST-3MDT","selected"); %>>(GMT+03:00) 莫斯科，聖彼得堡</option>
<option class="content_input_fd" value="MST-3MDT_1" <% nvram_match_x("LANHostConfig","time_zone", "MST-3MDT_1","selected"); %>>(GMT+03:00) 伏爾加格勒</option>
<option class="content_input_fd" value="UCT-3_1" <% nvram_match_x("LANHostConfig","time_zone", "UCT-3_1","selected"); %>>(GMT+03:00) 科威特，里雅德</option>
<option class="content_input_fd" value="UCT-3_2" <% nvram_match_x("LANHostConfig","time_zone", "UCT-3_2","selected"); %>>(GMT+03:00) 奈洛比</option>
<option class="content_input_fd" value="IST-3IDT" <% nvram_match_x("LANHostConfig","time_zone", "IST-3IDT","selected"); %>>(GMT+03:00) 巴格達</option>
<option class="content_input_fd" value="UCT-3.30" <% nvram_match_x("LANHostConfig","time_zone", "UCT-3.30","selected"); %>>(GMT+03:30) 德黑蘭</option>
<option class="content_input_fd" value="UCT-4_1" <% nvram_match_x("LANHostConfig","time_zone", "UCT-4_1","selected"); %>>(GMT+04:00) 阿布達比，馬斯喀特</option>
<option class="content_input_fd" value="UCT-4_2" <% nvram_match_x("LANHostConfig","time_zone", "UCT-4_2","selected"); %>>(GMT+04:00) 巴庫，第比利斯，葉里溫</option>
<option class="content_input_fd" value="UCT-4.30" <% nvram_match_x("LANHostConfig","time_zone", "UCT-4.30","selected"); %>>(GMT+04:30) 喀布爾</option>
<option class="content_input_fd" value="RFT-5RFTDST" <% nvram_match_x("LANHostConfig","time_zone", "RFT-5RFTDST","selected"); %>>(GMT+05:00) 伊卡特林堡</option>
<option class="content_input_fd" value="UCT-5" <% nvram_match_x("LANHostConfig","time_zone", "UCT-5","selected"); %>>(GMT+05:00) 伊斯蘭馬巴德，克洛奇，塔什干</option>
<option class="content_input_fd" value="UCT-5.30" <% nvram_match_x("LANHostConfig","time_zone", "UCT-5.30","selected"); %>>(GMT+05:30) 加爾各答，辰內</option>
<option class="content_input_fd" value="UCT-5.30_1" <% nvram_match_x("LANHostConfig","time_zone", "UCT-5.30_1","selected"); %>>(GMT+05:30) 孟拜，新德里</option>
<option class="content_input_fd" value="UCT-5.30" <% nvram_match_x("LANHostConfig","time_zone", "UCT-5.30","selected"); %>>(GMT+05:30) 斯里哈亞華登尼普拉</option>
<option class="content_input_fd" value="UCT-5.45" <% nvram_match_x("LANHostConfig","time_zone", "UCT-5.45","selected"); %>>(GMT+05:45) 加德滿都</option>
<option class="content_input_fd" value="UCT-6" <% nvram_match_x("LANHostConfig","time_zone", "UCT-6","selected"); %>>(GMT+06:00) 阿斯塔那，達卡</option>
<option class="content_input_fd" value="RFT-6RFTDST" <% nvram_match_x("LANHostConfig","time_zone", "RFT-6RFTDST","selected"); %>>(GMT+06:00) 阿馬提，諾曼斯比爾斯科</option>
<option class="content_input_fd" value="UCT-6.30" <% nvram_match_x("LANHostConfig","time_zone", "UCT-6.30","selected"); %>>(GMT+06:30) 仰光</option>
<option class="content_input_fd" value="UCT-7" <% nvram_match_x("LANHostConfig","time_zone", "UCT-7","selected"); %>>(GMT+07:00) 曼谷，河內，雅加達</option>
<option class="content_input_fd" value="RFT-7RFTDST" <% nvram_match_x("LANHostConfig","time_zone", "RFT-7RFTDST","selected"); %>>(GMT+07:00) 克拉斯諾亞爾斯克</option>
<option class="content_input_fd" value="CST-8" <% nvram_match_x("LANHostConfig","time_zone", "CST-8","selected"); %>>(GMT+08:00) 北京，香港</option>
<option class="content_input_fd" value="CST-8_1" <% nvram_match_x("LANHostConfig","time_zone", "CST-8_1","selected"); %>>(GMT+08:00) 重慶，烏魯木齊</option>
<option class="content_input_fd" value="SST-8" <% nvram_match_x("LANHostConfig","time_zone", "SST-8","selected"); %>>(GMT+08:00) 吉隆坡，新加坡</option>
<option class="content_input_fd" value="CCT-8" <% nvram_match_x("LANHostConfig","time_zone", "CCT-8","selected"); %>>(GMT+08:00) 台北</option>
<option class="content_input_fd" value="WAS-8WAD" <% nvram_match_x("LANHostConfig","time_zone", "WAS-8WAD","selected"); %>>(GMT+08:00) 伯斯</option>
<option class="content_input_fd" value="UCT-8" <% nvram_match_x("LANHostConfig","time_zone", "UCT-8","selected"); %>>(GMT+08:00) 伊爾庫次克，烏蘭巴圖</option>
<option class="content_input_fd" value="UCT-9_1" <% nvram_match_x("LANHostConfig","time_zone", "UCT-9_1","selected"); %>>(GMT+09:00) 首爾</option>
<option class="content_input_fd" value="JST" <% nvram_match_x("LANHostConfig","time_zone", "JST","selected"); %>>(GMT+09:00) 大阪，北海道，東京</option>
<option class="content_input_fd" value="UCT-9_2" <% nvram_match_x("LANHostConfig","time_zone", "UCT-9_2","selected"); %>>(GMT+09:00) 亞庫茲克</option>
<option class="content_input_fd" value="CST-9.30CDT" <% nvram_match_x("LANHostConfig","time_zone", "CST-9.30CDT","selected"); %>>(GMT+09:30) 達爾文</option>
<option class="content_input_fd" value="UCT-9.30" <% nvram_match_x("LANHostConfig","time_zone", "UCT-9.30","selected"); %>>(GMT+09:30) 愛德蘭</option>
<option class="content_input_fd" value="UCT-10_1" <% nvram_match_x("LANHostConfig","time_zone", "UCT-10_1","selected"); %>>(GMT+10:00) 坎培拉，墨爾本，雪梨</option>
<option class="content_input_fd" value="UCT-10_2" <% nvram_match_x("LANHostConfig","time_zone", "UCT-10_2","selected"); %>>(GMT+10:00) 布里斯本</option>
<option class="content_input_fd" value="TST-10TDT" <% nvram_match_x("LANHostConfig","time_zone", "TST-10TDT","selected"); %>>(GMT+10:00) 霍巴特</option>
<option class="content_input_fd" value="RFT-10RFTDST" <% nvram_match_x("LANHostConfig","time_zone", "RFT-10RFTDST","selected"); %>>(GMT+10:00) 海參崴</option>
<option class="content_input_fd" value="UCT-10_5" <% nvram_match_x("LANHostConfig","time_zone", "UCT-10_5","selected"); %>>(GMT+10:00) 關島，莫爾斯貝港</option>
<!--
<option class="content_input_fd" value="UCT-10_6" <% nvram_match_x("LANHostConfig","time_zone", "UCT-10_6","selected"); %>>(GMT+10:00) 坎培拉，墨爾本，雪梨</option>
-->
<option class="content_input_fd" value="UCT-11" <% nvram_match_x("LANHostConfig","time_zone", "UCT-11","selected"); %>>(GMT+11:00) 馬加丹，所羅門群島</option>
<option class="content_input_fd" value="UCT-11_1" <% nvram_match_x("LANHostConfig","time_zone", "UCT-11_1","selected"); %>>(GMT+11:00) 新卡倫多尼亞</option>
<option class="content_input_fd" value="UCT-12" <% nvram_match_x("LANHostConfig","time_zone", "UCT-12","selected"); %>>(GMT+12:00) 斐濟，勘察加，馬歇爾群島</option>
<option class="content_input_fd" value="NZST-12NZDT" <% nvram_match_x("LANHostConfig","time_zone", "NZST-12NZDT","selected"); %>>(GMT+12:00) 奧克蘭，威靈頓</option>
<option class="content_input_fd" value="UCT-13" <% nvram_match_x("LANHostConfig","time_zone", "UCT-13","selected"); %>>(GMT+13:00) 努瓜婁發</option>
</select>
</td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#LANHostConfig_x_NTPServer1_itemdesc#>', LEFT);" onMouseOut="return nd();"><#LANHostConfig_x_NTPServer1_itemname#>
</td><td class="content_input_td"><input type="text" maxlength="256" class="content_input_fd" size="32" name="ntp_server0" value="<% nvram_get_x("LANHostConfig","ntp_server0"); %>" onChange="page_changed()" onKeyPress="return is_string(this)" onBlur="validate_string(this)"><a href="javascript:openLink('x_NTPServer1')" class="content_input_link" name="x_NTPServer1_link"><#LANHostConfig_x_NTPServer1_linkname#>
</a></td>
</tr>
<tr class="content_section_header_tr">
<td class="content_section_header_td" colspan="2"><#LANHostConfig_x_DDNSEnable_sectionname#>
</td>
</tr>
<tr>
<td class="content_desc_td" colspan="2"><#LANHostConfig_x_DDNSEnable_sectiondesc#>
</td>
</tr>
<tr>
<td class="content_header_td"><#LANHostConfig_x_DDNSEnable_itemname#>
</td><td class="content_input_td"><input type="radio" value="1" name="ddns_enable_x" class="content_input_fd" onClick="return change_common_radio(this, 'LANHostConfig', 'ddns_enable_x', '1')" <% nvram_match_x("LANHostConfig","ddns_enable_x", "1", "checked"); %>><#RADIO_TEXT_YES#></input><input type="radio" value="0" name="ddns_enable_x" class="content_input_fd" onClick="return change_common_radio(this, 'LANHostConfig', 'ddns_enable_x', '0')" <% nvram_match_x("LANHostConfig","ddns_enable_x", "0", "checked"); %>><#RADIO_TEXT_NO#></input></td>
</tr>
<tr>
<td class="content_header_td"><#LANHostConfig_x_DDNSServer_itemname#>
</td><td class="content_input_td"><select name="ddns_server_x" class="content_input_fd" onChange="return change_common(this, 'LANHostConfig', 'ddns_server_x')"><option class="content_input_fd" value="WWW.DYNDNS.ORG" <% nvram_match_x("LANHostConfig","ddns_server_x", "WWW.DYNDNS.ORG","selected"); %>>WWW.DYNDNS.ORG</option>
<option class="content_input_fd" value="WWW.DYNDNS.ORG(CUSTOM)" <% nvram_match_x("LANHostConfig","ddns_server_x", "WWW.DYNDNS.ORG(CUSTOM)","selected"); %>>WWW.DYNDNS.ORG(CUSTOM)</option>
<option class="content_input_fd" value="WWW.DYNDNS.ORG(STATIC)" <% nvram_match_x("LANHostConfig","ddns_server_x", "WWW.DYNDNS.ORG(STATIC)","selected"); %>>WWW.DYNDNS.ORG(STATIC)</option>
<option class="content_input_fd" value="WWW.TZO.COM" <% nvram_match_x("LANHostConfig","ddns_server_x", "WWW.TZO.COM","selected"); %>>WWW.TZO.COM</option>
<option class="content_input_fd" value="WWW.ZONEEDIT.COM" <% nvram_match_x("LANHostConfig","ddns_server_x", "WWW.ZONEEDIT.COM","selected"); %>>WWW.ZONEEDIT.COM</option></select><a href="javascript:openLink('x_DDNSServer')" class="content_input_link" name="x_DDNSServer_link"><#LANHostConfig_x_DDNSServer_linkname#>
</a></td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#LANHostConfig_x_DDNSUserName_itemdesc#>', LEFT);" onMouseOut="return nd();"><#LANHostConfig_x_DDNSUserName_itemname#>
</td><td class="content_input_td"><input type="text" maxlength="32" class="content_input_fd" size="32" name="ddns_username_x" value="<% nvram_get_x("LANHostConfig","ddns_username_x"); %>" onChange="page_changed()" onKeyPress="return is_string(this)" onBlur="validate_string(this)"></td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#LANHostConfig_x_DDNSPassword_itemdesc#>', LEFT);" onMouseOut="return nd();"><#LANHostConfig_x_DDNSPassword_itemname#>
</td><td class="content_input_td"><input type="password" maxlength="64" class="content_input_fd" size="32" name="ddns_passwd_x" value="<% nvram_get_x("LANHostConfig","ddns_passwd_x"); %>" onChange="page_changed()" onKeyPress="return is_string(this)" onBlur="validate_string0(this)"></td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#LANHostConfig_x_DDNSHostNames_itemdesc#>', LEFT);" onMouseOut="return nd();"><#LANHostConfig_x_DDNSHostNames_itemname#>
</td><td class="content_input_td"><input type="text" maxlength="32" class="content_input_fd" size="32" name="ddns_hostname_x" value="<% nvram_get_x("LANHostConfig","ddns_hostname_x"); %>" onChange="page_changed()" onKeyPress="return is_string(this)" onBlur="validate_string(this)"></td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#LANHostConfig_x_DDNSWildcard_itemdesc#>', LEFT);" onMouseOut="return nd();"><#LANHostConfig_x_DDNSWildcard_itemname#>
</td><td class="content_input_td"><input type="radio" value="1" name="ddns_wildcard_x" class="content_input_fd" onClick="return change_common_radio(this, 'LANHostConfig', 'ddns_wildcard_x', '1')" <% nvram_match_x("LANHostConfig","ddns_wildcard_x", "1", "checked"); %>><#RADIO_TEXT_YES#></input><input type="radio" value="0" name="ddns_wildcard_x" class="content_input_fd" onClick="return change_common_radio(this, 'LANHostConfig', 'ddns_wildcard_x', '0')" <% nvram_match_x("LANHostConfig","ddns_wildcard_x", "0", "checked"); %>><#RADIO_TEXT_NO#></input></td>
</tr>
<tr>
<td class="content_header_td" onMouseOver="return overlib('<#LANHostConfig_x_DDNSStatus_itemdesc#>', LEFT);" onMouseOut="return nd();"><#LANHostConfig_x_DDNSStatus_itemname#>
</td><td class="content_input_td"><input type="hidden" maxlength="15" class="content_input_fd_ro" size="12" name="" value="<% nvram_get_f("ddns.log","DDNSStatus"); %>"><input type="submit" style="cursor:pointer;" maxlength="15" class="content_input_fd_ro" onClick="return onSubmitApply('ddnsclient')" size="12" name="LANHostConfig_x_DDNSStatus_button" value="<#LANHostConfig_x_DDNSStatus_buttonname#>"></td>
</tr>
</table>
</td>
</tr>
<tr>
<td>
<table width="666" border="2" cellpadding="0" cellspacing="0" bordercolor="E0E0E0"></table>
</td>
</tr>
<tr>
<td>
<table width="666" border="2" cellpadding="0" cellspacing="0" bordercolor="E0E0E0"></table>
</td>
</tr>
<tr>
<td>
<table width="666" border="2" cellpadding="0" cellspacing="0" bordercolor="E0E0E0"></table>
</td>
</tr>
<tr>
<td>
<table width="666" border="1" cellpadding="0" cellspacing="0" bordercolor="B0B0B0">
<tr bgcolor="#CCCCCC"><td colspan="3"><font face="arial" size="2"><b>&nbsp</b></font></td></tr>
<tr bgcolor="#FFFFFF">
<td id ="Confirm" height="25" width="34%">
<div align="center"><font face="Arial"> <input class=inputSubmit style="cursor:pointer;" onMouseOut=buttonOut(this) onMouseOver="buttonOver(this)" onClick="return onSubmitCtrl(this, ' Restore ');" type="submit" value=" <#CTL_restore#> " name="action"></font></div>
</td>
<td height="25" width="33%">
<div align="center"><font face="Arial"> <input class=inputSubmit style="cursor:pointer;" onMouseOut=buttonOut(this) onMouseOver="buttonOver(this)" onClick="return onSubmitCtrl(this, ' Finish ');" type="submit" value=" <#CTL_finish#> " name="action"></font></div>
</td>
<td height="25" width="33%">
<div align="center"><font face="Arial"> <input class=inputSubmit style="cursor:pointer;" onMouseOut=buttonOut(this) onMouseOver="buttonOver(this)" onClick="return onSubmitCtrl(this, ' Apply ');" type="submit" value=" <#CTL_apply#> " name="action"></font></div>
</td>
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
