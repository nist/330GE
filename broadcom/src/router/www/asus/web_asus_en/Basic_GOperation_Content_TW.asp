<head>
<title><#ZVMODELVZ#> Web Manager</title>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<meta http-equiv="Pragma" content="no-cache">
<link rel="stylesheet" type="text/css" href="style.css" media="screen"></link>
<script language="JavaScript" type="text/javascript" src="overlib.js"></script>
<script language="JavaScript" type="text/javascript" src="general.js"></script>
<script language="JavaScript" type="text/javascript" src="quick.js"></script>
</head>
<body bgcolor="#FFFFFF" onLoad="loadQuick();wl_mode_ex_change(1);">
<div id="overDiv" style="position:absolute; visibility:hidden; z-index:1000;"></div>
<form method="GET" name="form" action="apply.cgi">
<!-- Table for the conntent page -->
<table width="666" border="0" cellpadding="0" cellspacing="0">
<input type="hidden" name="x_Mode" value="0">
<input type="hidden" name="current_page" value="Basic_GOperation_Content.asp">
<input type="hidden" name="next_page" value="Main_Operation3.asp">
<input type="hidden" name="next_host" value="">
<input type="hidden" name="sid_list" value="Layer3Forwarding;IPConnection;PPPConnection;WLANConfig11a;WLANConfig11b;LANHostConfig;FirewallConfig;">
<input type="hidden" name="action_mode" value="">
<input type="hidden" name="modified" value="0">
<input type="hidden" name="group_id" value="">
<input type="hidden" name="wan_proto" value="<% nvram_get_x("Layer3Forwarding","wan_proto"); %>">
<input type="hidden" name="wan_route_x" value="<% nvram_get_x("IPConnection","wan_route_x"); %>">
<input type="hidden" name="wan_nat_x" value="<% nvram_get_x("IPConnection","wan_nat_x"); %>">
<input type="hidden" name="wl_auth_mode" value="<% nvram_get_x("WLANConfig11b","wl_auth_mode"); %>">
<input type="hidden" name="wl_crypto" value="<% nvram_get_x("WLANConfig11b","wl_crypto"); %>">
<input type="hidden" name="wl_wep_x" value="<% nvram_get_x("WLANConfig11b","wl_wep_x"); %>">

<input type="hidden" name="wl_guest_auth_mode_1" value="<% nvram_get_x("WLANConfig11b","wl_guest_auth_mode_1"); %>">
<input type="hidden" name="wl_guest_crypto_1"    value="<% nvram_get_x("WLANConfig11b","wl_guest_crypto_1"); %>">
<input type="hidden" name="wl_guest_wep_x_1"     value="<% nvram_get_x("WLANConfig11b","wl_guest_wep_x_1"); %>">

<input type="hidden" name="wl_guest_ssid_1" value="">
<input type="hidden" name="wl_guest_wpa_psk_1" value="">
<input type="hidden" name="wl_guest_key1_1" value="">
<input type="hidden" name="wl_guest_key2_1" value="">
<input type="hidden" name="wl_guest_key3_1" value="">
<input type="hidden" name="wl_guest_key4_1" value="">
<input type="hidden" name="wl_guest_key_1"  value="">

<input type="hidden" name="wl_guest_enable" value="<% nvram_get_x("WLANConfig11b","wl_guest_enable"); %>">

<input type="hidden" name="lan_ipaddr" value="<% nvram_get_x("LANHostConfig","lan_ipaddr"); %>" readonly="1">
<input type="hidden" name="lan_netmask" value="<% nvram_get_x("LANHostConfig","lan_netmask"); %>" readonly="1">
<input type="hidden" name="lan1_ipaddr" value="<% nvram_get_x("LANHostConfig","lan1_ipaddr"); %>" readonly="1">
<input type="hidden" name="lan1_netmask" value="<% nvram_get_x("LANHostConfig","lan1_netmask"); %>" readonly="1">
<input type="hidden" name="dhcp_start" value="<% nvram_get_x("LANHostConfig","dhcp_start"); %>" readonly="1">
<input type="hidden" name="dhcp_end" value="<% nvram_get_x("LANHostConfig","dhcp_end"); %>" readonly="1">
<input type="hidden" name="preferred_lang" value="<% nvram_get_x("","preferred_lang"); %>">
<input type="hidden" name="wl_ssid2" value="<% nvram_get_x("WLANConfig11b","wl_ssid2"); %>">
<input type="hidden" name="wl_guest_ssid_2" value="<% nvram_get_x("WLANConfig11b","wl_guest_ssid_2"); %>">
<!--
<input type="hidden" name="mssid_ssid_x0" value="<% nvram_get_x("WLANConfig11b","mssid_ssid_x0"); %>">
<input type="hidden" name="mssid_ssid_2_x0" value="<% nvram_get_x("WLANConfig11b","mssid_ssid_2_x0"); %>">
-->
<input type="hidden" name="wl_wpa_mode" value="<% nvram_get_x("WLANConfig11b","wl_wpa_mode"); %>">

<tr>
<td>
<table width="666" border="1" cellpadding="0" cellspacing="0" bordercolor="E0E0E0">
<tr id="Country" class="content_header_tr">
<td class="content_header_td_title" colspan="2"><#BOP_title#></td>
</tr>
<tr class="content_section_header_tr">
<td class="content_section_header_td" colspan="2"><#BOP_time_title#></td>
</tr>
<tr>
<td class="content_desc_td" colspan="2" height="50">
<#BOP_time_desc#></td>
</tr>

<tr>
	<td class="content_header_td" onMouseOver="return overlib('<#BOP_time_zone_desc#>', LEFT);" onMouseOut="return nd();"><#BOP_time_zone_item#></td>
	<td class="content_input_td">
		<select name="time_zone" class="content_input_fd" onChange="return change_common(this, 'LANHostConfig', 'time_zone')">
			<option class="content_input_fd" value="UCT12" <% nvram_match_x("LANHostConfig","time_zone", "UCT12","selected"); %>			>(GMT-12:00) 國際換日線以西</option>
			<option class="content_input_fd" value="UCT11" <% nvram_match_x("LANHostConfig","time_zone", "UCT11","selected"); %>			>(GMT-11:00) 中途島，薩摩亞群島</option>
			<option class="content_input_fd" value="UCT10" <% nvram_match_x("LANHostConfig","time_zone", "UCT10","selected"); %>			>(GMT-10:00) 夏威夷</option>
			<option class="content_input_fd" value="NAST9NADT" <% nvram_match_x("LANHostConfig","time_zone", "NAST9NADT","selected"); %>		>(GMT-09:00) 阿拉斯加</option>
			<option class="content_input_fd" value="PST8PDT" <% nvram_match_x("LANHostConfig","time_zone", "PST8PDT","selected"); %>		>(GMT-08:00) 太平洋時間 (美國和加拿大)，提華納</option>
			<option class="content_input_fd" value="MST7MDT" <% nvram_match_x("LANHostConfig","time_zone", "MST7MDT","selected"); %>		>(GMT-07:00) 山區時間 (美國和加拿大)</option>
			<option class="content_input_fd" value="MST7" <% nvram_match_x("LANHostConfig","time_zone", "MST7","selected"); %>			>(GMT-07:00) 亞歷桑那</option>
			<option class="content_input_fd" value="MST7_1" <% nvram_match_x("LANHostConfig","time_zone", "MST7_1","selected"); %>		>(GMT-07:00) 齊驊華，拉帕茲，馬札特蘭</option>
			<option class="content_input_fd" value="CST6CDT_1" <% nvram_match_x("LANHostConfig","time_zone", "CST6CDT_1","selected"); %>		>(GMT-06:00) 中部時間 (美國和加拿大)</option>
			<option class="content_input_fd" value="CST6CDT_2" <% nvram_match_x("LANHostConfig","time_zone", "CST6CDT_2","selected"); %>		>(GMT-06:00) 薩克其萬(加拿大)</option>
			<option class="content_input_fd" value="CST6CDT_3" <% nvram_match_x("LANHostConfig","time_zone", "CST6CDT_3","selected"); %>		>(GMT-06:00) 瓜達拉加若，墨西哥城</option>
			<option class="content_input_fd" value="CST6CDT_3_1" <% nvram_match_x("LANHostConfig","time_zone", "CST6CDT_3_1","selected"); %>	>(GMT-06:00) 蒙特利</option>
			<option class="content_input_fd" value="UCT6" <% nvram_match_x("LANHostConfig","time_zone", "UCT6","selected"); %>			>(GMT-06:00) 中部時間 (美國和加拿大)</option>
			<option class="content_input_fd" value="EST5EDT" <% nvram_match_x("LANHostConfig","time_zone", "EST5EDT","selected"); %>		>(GMT-05:00) 東部時間 (美國和加拿大)</option>
			<option class="content_input_fd" value="UCT5_1" <% nvram_match_x("LANHostConfig","time_zone", "UCT5_1","selected"); %>			>(GMT-05:00) 大西洋時間 (加拿大)</option>
			<option class="content_input_fd" value="UCT5_2" <% nvram_match_x("LANHostConfig","time_zone", "UCT5_2","selected"); %>			>(GMT-05:00) 波哥大，利馬，基多</option>
			<option class="content_input_fd" value="AST4ADT" <% nvram_match_x("LANHostConfig","time_zone", "AST4ADT","selected"); %>		>(GMT-04:00) 大西洋時間 (加拿大)</option>
			<option class="content_input_fd" value="UCT4_1" <% nvram_match_x("LANHostConfig","time_zone", "UCT4_1","selected"); %>			>(GMT-04:00) 卡拉卡斯，拉帕茲</option>
			<option class="content_input_fd" value="UCT4_2" <% nvram_match_x("LANHostConfig","time_zone", "UCT4_2","selected"); %>			>(GMT-04:00) 聖地牙哥</option>
			<option class="content_input_fd" value="NST3.30" <% nvram_match_x("LANHostConfig","time_zone", "NST3.30","selected"); %>		>(GMT-03:30) 紐芬蘭</option>
			<option class="content_input_fd" value="EBST3EBDT_1" <% nvram_match_x("LANHostConfig","time_zone", "EBST3EBDT_1","selected"); %>	>(GMT-03:00) 巴西利亞</option>
			<option class="content_input_fd" value="UCT3" <% nvram_match_x("LANHostConfig","time_zone", "UCT3","selected"); %>			>(GMT-03:00) 布宜諾斯艾利斯，佐治敦</option>
			<option class="content_input_fd" value="EBST3EBDT_2" <% nvram_match_x("LANHostConfig","time_zone", "EBST3EBDT_2","selected"); %>	>(GMT-03:00) 格陵蘭</option>
			<option class="content_input_fd" value="NORO2" <% nvram_match_x("LANHostConfig","time_zone", "NORO2","selected"); %>			>(GMT-02:00) 大西洋中部</option>
			<option class="content_input_fd" value="EUT1EUTDST" <% nvram_match_x("LANHostConfig","time_zone", "EUT1EUTDST","selected"); %>		>(GMT-01:00) 亞速爾群島</option>
			<option class="content_input_fd" value="UCT1" <% nvram_match_x("LANHostConfig","time_zone", "UCT1","selected"); %>			>(GMT-01:00) 維德角群島</option>
			<option class="content_input_fd" value="GMT0BST_1" <% nvram_match_x("LANHostConfig","time_zone", "GMT0BST_1","selected"); %>		>(GMT) 格林威治時間：都柏林，愛丁堡，里斯本，倫敦</option>
			<option class="content_input_fd" value="GMT0BST_2" <% nvram_match_x("LANHostConfig","time_zone", "GMT0BST_2","selected"); %>		>(GMT) 卡薩布蘭卡，蒙羅維亞(賴比瑞亞)</option>
			<option class="content_input_fd" value="UCT-1_1" <% nvram_match_x("LANHostConfig","time_zone", "UCT-1_1","selected"); %>		>(GMT+01:00) 貝爾格勒，布拉提斯拉瓦，布達佩斯</option>
			<option class="content_input_fd" value="UCT-1_1_1" <% nvram_match_x("LANHostConfig","time_zone", "UCT-1_1_1","selected"); %>		>(GMT+01:00) 盧布亞那，布拉格</option>
			<option class="content_input_fd" value="UCT-1_2" <% nvram_match_x("LANHostConfig","time_zone", "UCT-1_2","selected"); %>		>(GMT+01:00) 塞拉耶佛，斯高彼亞，索非亞</option>
			<option class="content_input_fd" value="UCT-1_2_1" <% nvram_match_x("LANHostConfig","time_zone", "UCT-1_2_1","selected"); %>		>(GMT+01:00) 維爾紐斯，華沙，札格雷布</option>
			<option class="content_input_fd" value="MET-1METDST" <% nvram_match_x("LANHostConfig","time_zone", "MET-1METDST","selected"); %>	>(GMT+01:00) 布魯塞爾，哥本哈根</option>
			<option class="content_input_fd" value="MET-1METDST_1" <% nvram_match_x("LANHostConfig","time_zone", "MET-1METDST_1","selected"); %>	>(GMT+01:00) 馬德里，巴黎</option>
			<option class="content_input_fd" value="MEZ-1MESZ" <% nvram_match_x("LANHostConfig","time_zone", "MEZ-1MESZ","selected"); %>		>(GMT+01:00) 阿姆斯特丹，柏林，伯恩</option>
			<option class="content_input_fd" value="MEZ-1MESZ_1" <% nvram_match_x("LANHostConfig","time_zone", "MEZ-1MESZ_1","selected"); %>	>(GMT+01:00) 羅馬，斯德哥爾摩，維也納</option>
			<option class="content_input_fd" value="UCT-1_3" <% nvram_match_x("LANHostConfig","time_zone", "UCT-1_3","selected"); %>		>(GMT+01:00) 中西非</option>
			<option class="content_input_fd" value="UCT-2" <% nvram_match_x("LANHostConfig","time_zone", "UCT-2","selected"); %>		>(GMT+02:00) 布加勒斯特</option>
			<option class="content_input_fd" value="EST-2EDT" <% nvram_match_x("LANHostConfig","time_zone", "EST-2EDT","selected"); %>		>(GMT+02:00) 開羅</option>
			<option class="content_input_fd" value="UCT-2_1" <% nvram_match_x("LANHostConfig","time_zone", "UCT-2_1","selected"); %>		>(GMT+02:00) 赫爾辛基，里加，塔林</option>
			<option class="content_input_fd" value="UCT-2_2" <% nvram_match_x("LANHostConfig","time_zone", "UCT-2_2","selected"); %>		>(GMT+02:00) 雅典，伊斯坦堡，明斯克</option>
			<option class="content_input_fd" value="IST-2IDT" <% nvram_match_x("LANHostConfig","time_zone", "IST-2IDT","selected"); %>		>(GMT+02:00) 耶路撒冷</option>
			<option class="content_input_fd" value="SAST-2" <% nvram_match_x("LANHostConfig","time_zone", "SAST-2","selected"); %>			>(GMT+02:00) 哈拉雷，皮托里</option>
			<option class="content_input_fd" value="MST-3MDT" <% nvram_match_x("LANHostConfig","time_zone", "MST-3MDT","selected"); %>		>(GMT+03:00) 莫斯科，聖彼得堡</option>
			<option class="content_input_fd" value="MST-3MDT_1" <% nvram_match_x("LANHostConfig","time_zone", "MST-3MDT_1","selected"); %>		>(GMT+03:00) 伏爾加格勒</option>
			<option class="content_input_fd" value="UCT-3_1" <% nvram_match_x("LANHostConfig","time_zone", "UCT-3_1","selected"); %>		>(GMT+03:00) 科威特，里雅德</option>
			<option class="content_input_fd" value="UCT-3_2" <% nvram_match_x("LANHostConfig","time_zone", "UCT-3_2","selected"); %>		>(GMT+03:00) 奈洛比</option>
			<option class="content_input_fd" value="IST-3IDT" <% nvram_match_x("LANHostConfig","time_zone", "IST-3IDT","selected"); %>		>(GMT+03:00) 巴格達</option>
			<option class="content_input_fd" value="UCT-3.30" <% nvram_match_x("LANHostConfig","time_zone", "UCT-3.30","selected"); %>		>(GMT+03:30) 德黑蘭</option>
			<option class="content_input_fd" value="UCT-4_1" <% nvram_match_x("LANHostConfig","time_zone", "UCT-4_1","selected"); %>		>(GMT+04:00) 阿布達比，馬斯喀特</option>
			<option class="content_input_fd" value="UCT-4_2" <% nvram_match_x("LANHostConfig","time_zone", "UCT-4_2","selected"); %>		>(GMT+04:00) 巴庫，第比利斯，葉里溫</option>
			<option class="content_input_fd" value="UCT-4.30" <% nvram_match_x("LANHostConfig","time_zone", "UCT-4.30","selected"); %>		>(GMT+04:30) 喀布爾</option>
			<option class="content_input_fd" value="RFT-5RFTDST" <% nvram_match_x("LANHostConfig","time_zone", "RFT-5RFTDST","selected"); %>	>(GMT+05:00) 伊卡特林堡</option>
			<option class="content_input_fd" value="UCT-5" <% nvram_match_x("LANHostConfig","time_zone", "UCT-5","selected"); %>			>(GMT+05:00) 伊斯蘭馬巴德，克洛奇，塔什干</option>
			<option class="content_input_fd" value="UCT-5.30" <% nvram_match_x("LANHostConfig","time_zone", "UCT-5.30","selected"); %>		>(GMT+05:30) 加爾各答，辰內</option>
			<option class="content_input_fd" value="UCT-5.30_1" <% nvram_match_x("LANHostConfig","time_zone", "UCT-5.30_1","selected"); %>		>(GMT+05:30) 孟拜，新德里</option>
			<option class="content_input_fd" value="UCT-5.30" <% nvram_match_x("LANHostConfig","time_zone", "UCT-5.30","selected"); %>		>(GMT+05:30) 斯里哈亞華登尼普拉</option>
			<option class="content_input_fd" value="UCT-5.45" <% nvram_match_x("LANHostConfig","time_zone", "UCT-5.45","selected"); %>		>(GMT+05:45) 加德滿都</option>
			<option class="content_input_fd" value="UCT-6" <% nvram_match_x("LANHostConfig","time_zone", "UCT-6","selected"); %>			>(GMT+06:00) 阿斯塔那，達卡</option>
			<option class="content_input_fd" value="RFT-6RFTDST" <% nvram_match_x("LANHostConfig","time_zone", "RFT-6RFTDST","selected"); %>	>(GMT+06:00) 阿馬提，諾曼斯比爾斯科</option>
			<option class="content_input_fd" value="UCT-6.30" <% nvram_match_x("LANHostConfig","time_zone", "UCT-6.30","selected"); %>		>(GMT+06:30) 仰光</option>
			<option class="content_input_fd" value="UCT-7" <% nvram_match_x("LANHostConfig","time_zone", "UCT-7","selected"); %>			>(GMT+07:00) 曼谷，河內，雅加達</option>
			<option class="content_input_fd" value="RFT-7RFTDST" <% nvram_match_x("LANHostConfig","time_zone", "RFT-7RFTDST","selected"); %>	>(GMT+07:00) 克拉斯諾亞爾斯克</option>
			<option class="content_input_fd" value="CST-8" <% nvram_match_x("LANHostConfig","time_zone", "CST-8","selected"); %>			>(GMT+08:00) 北京，香港</option>
			<option class="content_input_fd" value="CST-8_1" <% nvram_match_x("LANHostConfig","time_zone", "CST-8_1","selected"); %>		>(GMT+08:00) 重慶，烏魯木齊</option>
			<option class="content_input_fd" value="SST-8" <% nvram_match_x("LANHostConfig","time_zone", "SST-8","selected"); %>			>(GMT+08:00) 吉隆坡，新加坡</option>
			<option class="content_input_fd" value="CCT-8" <% nvram_match_x("LANHostConfig","time_zone", "CCT-8","selected"); %>			>(GMT+08:00) 台北</option>
			<option class="content_input_fd" value="WAS-8WAD" <% nvram_match_x("LANHostConfig","time_zone", "WAS-8WAD","selected"); %>		>(GMT+08:00) 伯斯</option>
			<option class="content_input_fd" value="UCT-8" <% nvram_match_x("LANHostConfig","time_zone", "UCT-8","selected"); %>			>(GMT+08:00) 伊爾庫次克，烏蘭巴圖</option>
			<option class="content_input_fd" value="UCT-9_1" <% nvram_match_x("LANHostConfig","time_zone", "UCT-9_1","selected"); %>		>(GMT+09:00) 首爾</option>
			<option class="content_input_fd" value="JST" <% nvram_match_x("LANHostConfig","time_zone", "JST","selected"); %>			>(GMT+09:00) 大阪，北海道，東京</option>
			<option class="content_input_fd" value="UCT-9_2" <% nvram_match_x("LANHostConfig","time_zone", "UCT-9_2","selected"); %>		>(GMT+09:00) 亞庫茲克</option>
			<option class="content_input_fd" value="CST-9.30CDT" <% nvram_match_x("LANHostConfig","time_zone", "CST-9.30CDT","selected"); %>	>(GMT+09:30) 達爾文</option>
			<option class="content_input_fd" value="UCT-9.30" <% nvram_match_x("LANHostConfig","time_zone", "UCT-9.30","selected"); %>		>(GMT+09:30) 愛德蘭</option>
			<option class="content_input_fd" value="UCT-10_1" <% nvram_match_x("LANHostConfig","time_zone", "UCT-10_1","selected"); %>		>(GMT+10:00) 坎培拉，墨爾本，雪梨</option>
			<option class="content_input_fd" value="UCT-10_2" <% nvram_match_x("LANHostConfig","time_zone", "UCT-10_2","selected"); %>		>(GMT+10:00) 布里斯本</option>
			<option class="content_input_fd" value="TST-10TDT" <% nvram_match_x("LANHostConfig","time_zone", "TST-10TDT","selected"); %>		>(GMT+10:00) 霍巴特</option>
			<option class="content_input_fd" value="RFT-10RFTDST" <% nvram_match_x("LANHostConfig","time_zone", "RFT-10RFTDST","selected"); %>	>(GMT+10:00) 海參崴</option>
			<option class="content_input_fd" value="UCT-10_5" <% nvram_match_x("LANHostConfig","time_zone", "UCT-10_5","selected"); %>		>(GMT+10:00) 關島，莫爾斯貝港</option>
<!--
			<option class="content_input_fd" value="UCT-10_6" <% nvram_match_x("LANHostConfig","time_zone", "UCT-10_6","selected"); %>		>(GMT+10:00) 坎培拉，墨爾本，雪梨</option>
-->
			<option class="content_input_fd" value="UCT-11" <% nvram_match_x("LANHostConfig","time_zone", "UCT-11","selected"); %>			>(GMT+11:00) 馬加丹，所羅門群島</option>
			<option class="content_input_fd" value="UCT-11_1" <% nvram_match_x("LANHostConfig","time_zone", "UCT-11_1","selected"); %>		>(GMT+11:00) 新卡倫多尼亞</option>
			<option class="content_input_fd" value="UCT-12" <% nvram_match_x("LANHostConfig","time_zone", "UCT-12","selected"); %>			>(GMT+12:00) 斐濟，勘察加，馬歇爾群島</option>
			<option class="content_input_fd" value="NZST-12NZDT" <% nvram_match_x("LANHostConfig","time_zone", "NZST-12NZDT","selected"); %>	>(GMT+12:00) 奧克蘭，威靈頓</option>
			<option class="content_input_fd" value="UCT-13" <% nvram_match_x("LANHostConfig","time_zone", "UCT-13","selected"); %>			>(GMT+13:00) 努瓜婁發</option>
		</select>
	</td>
</tr>
<tr>
<td class="content_input_td" colspan="2">
<table>
<tr>
<td width="500" height="100"></td><td>
<div align="center">
<font face="Arial"><input class=inputSubmit style="cursor:pointer;" onMouseOut="buttonOut(this)" onMouseOver="buttonOver(this)" onClick="toPrevTag('Country')" type="hidden" value="<#CTL_prev#>" name="action1"></font>&nbsp;&nbsp;
<font face="Arial"> <input class=inputSubmit style="cursor:pointer;" onMouseOut="buttonOut(this)" onMouseOver="buttonOver(this)" onClick="toTag('Country')" type="button" value="<#CTL_next#>" name="action"></font>
</div>
</td>
</tr>
<tr><td colspan="2" height="360"></td></tr>
</table>
</td>
</tr>
<tr id="Broadband" class="content_header_tr">
<td class="content_header_td_title" colspan="2"><#BOP_title#></td>
</tr>
<tr class="content_section_header_tr">
<td class="content_section_header_td" colspan="2"><#BOP_ctype_title#></td>
</tr>
<tr>
<td class="content_desc_td" colspan="2" height="50">
<#BOP_ctype_desc#></td>
</tr>
<tr> 
          <td class="content_header_td_less" onMouseOut="return nd();" colspan="2">          
          <p></p>
          <p><input type="radio" checked name="x_WANType" value="1" class="content_input_fd" onClick="changeWANType()"><#BOP_ctype_item1#></p>          
          <p><input type="radio" checked name="x_WANType" value="2" class="content_input_fd" onClick="changeWANType()"><#BOP_ctype_item2#></p>          
          <p><input type="radio" checked name="x_WANType" value="3" class="content_input_fd" onClick="changeWANType()"><#BOP_ctype_item3#></p>          
          <p><input type="radio" checked name="x_WANType" value="4" class="content_input_fd" onClick="changeWANType()"><#BOP_ctype_item4#></p>
          <p><input type="radio" checked name="x_WANType" value="5" class="content_input_fd" onClick="changeWANType()"><#BOP_ctype_item5#></p>
          <p></p>
          <p></p>
          </td>
</tr>
<tr>
<td class="content_input_td" colspan="2">
<table>
<tr>
<td width="444" height="100"></td><td>
<div align="center">
<font face="Arial"><input class=inputSubmit style="cursor:pointer;" onMouseOut="buttonOut(this)" onMouseOver="buttonOver(this)" onClick="toPrevTag('Broadband')" type="button" value="<#CTL_prev#>" name="action"></font>&nbsp;&nbsp;
<font face="Arial"> <input class=inputSubmit style="cursor:pointer;" onMouseOut="buttonOut(this)" onMouseOver="buttonOver(this)" onClick="toTag('Broadband')" type="button" value="<#CTL_next#>" name="action"></font>
</div>
</td>
</tr>
<tr><td colspan="2" height="360"></td></tr>
</table>
</td>
</tr>
<tr id="PPPoE" class="content_header_tr">
<td class="content_header_td_title" colspan="2"><#BOP_title#></td>
</tr>
<tr class="content_section_header_tr">
<td  class="content_section_header_td" colspan="2"><#BOP_account_title#></td>
</tr>
<tr>
<td class="content_desc_td" colspan="2" height="50"><#BOP_account_desc#></td>
</tr>
<tr>
<td class="content_header_td_less"><#BOP_account_user_item#></td><td class="content_input_td"><input type="text" maxlength="64" size="32" name="wan_pppoe_username" class="content_input_fd" value="<% nvram_get_x("PPPConnection","wan_pppoe_username"); %>" onChange="page_changed()" onKeyPress="return is_string(this)" onBlur="validate_string(this)"></td>
</tr>
<tr>
<td class="content_header_td_less"><#BOP_account_pass_item#></td><td class="content_input_td"><input type="password" maxlength="64" size="32" name="wan_pppoe_passwd" class="content_input_fd" value="<% nvram_get_x("PPPConnection","wan_pppoe_passwd"); %>" onChange="page_changed()" onKeyPress="return is_string(this)" onBlur="validate_string0(this)"></td>
</tr>
<tr>
<td class="content_input_td" colspan="2">
<table>
<tr>
<td width="444" height="100"></td>
<td>
<div align="center">
<font face="Arial"><input class=inputSubmit style="cursor:pointer;" onMouseOut="buttonOut(this)" onMouseOver="buttonOver(this)" onClick="toPrevTag('PPPoE')" type="button" value="<#CTL_prev#>" name="action"></font>&nbsp;&nbsp;
<font face="Arial"><input class=inputSubmit style="cursor:pointer;" onMouseOut="buttonOut(this)" onMouseOver="buttonOver(this)" onClick="toTag('PPPoE')" type="button" value="<#CTL_next#>" name="action"></font>
</div>
</td>
</tr>
<tr><td colspan="2" height="360"></td></tr>
</table>
</td>
</tr>
<tr id="MacHost" class="content_header_tr">
<td class="content_header_td_title" colspan="2"><#BOP_title#></td>
</tr>
<tr class="content_section_header_tr">
<td  class="content_section_header_td" colspan="2"><#BOP_isp_title#></td>
</tr>
<tr>
<td class="content_desc_td" colspan="2" height="50"><#BOP_isp_desc#></td>
</tr>
<tr>
<td class="content_header_td_less" onMouseOver="return overlib('<#BOP_isp_host_desc#>', LEFT);" onMouseOut="return nd();"><#BOP_isp_host_item#></td><td class="content_input_td"><input type="text" maxlength="32" size="32" name="wan_hostname" class="content_input_fd" value="<% nvram_get_x("PPPConnection","wan_hostname"); %>" onChange="page_changed()" onBlur="validate_string2(this)" onKeyPress="return is_string(this)"></td>
</tr>
<tr>
<td class="content_header_td_less" onMouseOver="return overlib('<#BOP_isp_mac_desc#>', LEFT);" onMouseOut="return nd();"><#BOP_isp_mac_item#></td><td class="content_input_td"><input type="text" maxlength="12" size="12" name="wan_hwaddr_x" class="content_input_fd" value="<% nvram_get_x("PPPConnection","wan_hwaddr_x"); %>" onChange="page_changed()" onBlur="return validate_hwaddr(this)" onKeyPress="return is_hwaddr()"></td>
</tr>
<tr>
<td class="content_header_td_less" onMouseOver="return overlib('<#BOP_isp_heart_desc#>', LEFT);" onMouseOut="return nd();"><#BOP_isp_heart_item#></td><td class="content_input_td"><input type="text" maxlength="32" size="32" name="wan_heartbeat_x" class="content_input_fd" value="<% nvram_get_x("PPPConnection","wan_heartbeat_x"); %>" onChange="page_changed()" onBlur="validate_string3(this)" onKeyPress="return is_string(this)"></td>
</tr>
<tr>
<td class="content_input_td_less" colspan="2">
<table>
<tr>
<td width="444" height="100"></td>
<td>
<div align="center">
<font face="Arial"><input class=inputSubmit style="cursor:pointer;" onMouseOut="buttonOut(this)" onMouseOver="buttonOver(this)" onClick="toPrevTag('MacHost')" type="button" value="<#CTL_prev#>" name="action"></font>&nbsp;&nbsp;
<font face="Arial"><input class=inputSubmit style="cursor:pointer;" onMouseOut="buttonOut(this)" onMouseOver="buttonOver(this)" onClick="toTag('MacHost')" type="button" value="<#CTL_next#>" name="action"></font>
</div>
</td>
</tr>
<tr><td colspan="2" height="360"></td></tr>
</table>
</td>
</tr>
<tr id="WANSetting" class="content_header_tr">
<td class="content_header_td_title" colspan="2"><#BOP_title#></td>
</tr>
<tr class="content_section_header_tr">
<td class="content_section_header_td" colspan="2"><#BOP_wan_title#></td>
</tr>
<tr>
<td class="content_desc_td" colspan="2" height="50"><#BOP_wan_desc#></td>
</tr>
<tr>
<td class="content_header_td_less"><#BOP_wan_dhcp_item#></td><td class="content_input_td"><input type="radio" value="1" name="x_DHCPClient" class="content_input_fd" onClick="changeDHCPClient()">Yes</input><input type="radio" value="0" name="x_DHCPClient" class="content_input_fd" onClick="changeDHCPClient()">No</input></td>
</tr>
<tr>
<td class="content_header_td_less" onMouseOver="return overlib('<#BOP_wan_ip_desc#>', LEFT);" onMouseOut="return nd();"><#BOP_wan_ip_item#></td><td class="content_input_td"><input type="text" maxlength="15" size="15" name="wan_ipaddr" class="content_input_fd" value="<% nvram_get_x("IPConnection","wan_ipaddr"); %>" onChange="page_changed()" onBlur="return validate_ipaddr(this, 'wan_ipaddr')" onKeyPress="return is_ipaddr(this)"></td>
</tr>
<tr>
<td class="content_header_td_less"><#BOP_wan_sb_item#></td><td class="content_input_td"><input type="text" maxlength="15" size="15" name="wan_netmask" class="content_input_fd" value="<% nvram_get_x("IPConnection","wan_netmask"); %>" onChange="page_changed()" onBlur="return validate_ipaddr(this, 'wan_netmask')" onKeyPress="return is_ipaddr(this)"></td>
</tr>
<tr>
<td class="content_header_td_less"><#BOP_wan_gw_item#></td><td class="content_input_td"><input type="text" maxlength="15" size="15" name="wan_gateway" class="content_input_fd" value="<% nvram_get_x("IPConnection","wan_gateway"); %>" onChange="page_changed()" onBlur="return validate_ipaddr(this, 'wan_gateway')" onKeyPress="return is_ipaddr(this)"></td>
</tr>
<tr>
<td class="content_header_td_less"><#BOP_wan_dns_item#></td><td class="content_input_td"><input type="radio" value="1" name="wan_dnsenable_x" class="content_input_fd" onClick="changeDNSServer()" <% nvram_match_x("IPConnection","wan_dnsenable_x", "1", "checked"); %>>Yes</input><input type="radio" value="0" name="wan_dnsenable_x" class="content_input_fd" onClick="changeDNSServer()" <% nvram_match_x("IPConnection","wan_dnsenable_x", "0", "checked"); %>>No</input></td>
</tr>
<tr>
<td class="content_header_td_less"><#BOP_wan_dns1_item#></td><td class="content_input_td"><input type="text" maxlength="15" size="15" name="wan_dns1_x" class="content_input_fd" value="<% nvram_get_x("IPConnection","wan_dns1_x"); %>" onChange="page_changed()" onBlur="return validate_ipaddr(this)" onKeyPress="return is_ipaddr(this)"></td>
</tr>
<tr>
<td class="content_header_td_less"><#BOP_wan_dns2_item#></td><td class="content_input_td"><input type="text" maxlength="15" size="15" name="wan_dns2_x" class="content_input_fd" value="<% nvram_get_x("IPConnection","wan_dns2_x"); %>" onChange="page_changed()" onBlur="return validate_ipaddr(this)" onKeyPress="return is_ipaddr(this)"></td>
</tr>
<tr>
<td class="content_input_td" colspan="2">
<table>
<tr>
<td width="444" height="100"></td><td>
<div align="center">
<font face="Arial"><input class=inputSubmit style="cursor:pointer;" onMouseOut="buttonOut(this)" onMouseOver="buttonOver(this)" onClick="toPrevTag('WANSetting')" type="button" value="<#CTL_prev#>" name="action"></font>&nbsp;&nbsp;
<font face="Arial"><input class=inputSubmit style="cursor:pointer;" onMouseOut="buttonOut(this)" onMouseOver="buttonOver(this)" onClick="toTag('WANSetting')" type="button" value="<#CTL_next#>" name="action"></font>
</div>
</td>
</tr>
<tr><td colspan="2" height="360"></td></tr>
</table>
</td>
</tr>

<tr  id="Wireless"  class="content_header_tr">
<td class="content_header_td_title" colspan="2"><#BOP_title#></td>
</tr>
<tr class="content_section_header_tr">
<td class="content_section_header_td" colspan="2"><#BOP_wlan_title#></td>
</tr>
<tr>
<td class="content_desc_td" colspan="2" height="50"><#BOP_wlan_desc#></td>
</tr>
<tr>
<td class="content_header_td_less" onMouseOver="return overlib('<#BOP_wlan_ssid_desc#>', LEFT);" onMouseOut="return nd();"><#BOP_wlan_ssid_item#></td><td class="content_input_td"><input type="text" maxlength="32" size="32" name="wl_ssid" class="content_input_fd" value="<% nvram_get_x("WLANConfig11b","wl_ssid"); %>" onChange="page_changed()" onKeyPress="return is_string(this)" onBlur="validate_string4(this)"></td>
</tr>
<tr>
<td class="content_header_td_less" onMouseOver="return overlib('<#BOP_wlan_sec_desc#>', LEFT);" onMouseOut="return nd();"><#BOP_wlan_sec_item#></td>
<td class="content_input_td">
 <select name="SecurityLevel" class="content_input_fd" onChange="return change_security(this, 'WLANConfig11b', 0)">
     <option value="0">Low(Open System)</option>
     <option value="1">Medium(WEP-64bits)</option>
     <option value="2">Medium(WEP-128bits)</option>
     <option value="3">High(WPA-Personal)</option>
 </select>    
</td>
</tr>
<tr>
<td class="content_header_td_less" onMouseOver="return overlib('<#BOP_wlan_pass_desc#>', LEFT);" onMouseOut="return nd();"><#BOP_wlan_pass_item#></td><td class="content_input_td"><script language="JavaScript" type="text/javascript" src="phrase.js"></script><script language="JavaScript" type="text/javascript" src="md5.js"></script><input type="text" maxlength="63" size="32" name="wl_wpa_psk" class="content_input_fd"  value="<% nvram_get_x("WLANConfig11b","wl_wpa_psk"); %>" onKeyUp="return is_wlphrase_q('WLANConfig11b', this)" onChange="return is_wlphrase_q('WLANConfig11b',this)" onBlur="return validate_wlphrase_q('WLANConfig11b', this)"></td>
</tr>
<tr>
<td class="content_header_td_less"><#BOP_wlan_wep1_item#></td><td class="content_input_td"><input type="text" maxlength="32" size="32" name="wl_key1" class="content_input_fd" value="<% nvram_get_x("WLANConfig11b","wl_key1"); %>" onBlur="return validate_wlkey(this, 'WLANConfig11b')" onKeyPress="return is_wlkey(this, 'WLANConfig11b')" onKeyUp="return change_wlkey(this, 'WLANConfig11b')"></td>
</tr>				   
<tr>
<td class="content_header_td_less"><#BOP_wlan_wep2_item#></td><td class="content_input_td"><input type="text" maxlength="32" size="32" name="wl_key2" class="content_input_fd" value="<% nvram_get_x("WLANConfig11b","wl_key2"); %>" onBlur="return validate_wlkey(this, 'WLANConfig11b')" onKeyPress="return is_wlkey(this, 'WLANConfig11b')" onKeyUp="return change_wlkey(this, 'WLANConfig11b')"></td>
</tr>
<tr>
<td class="content_header_td_less"><#BOP_wlan_wep3_item#></td><td class="content_input_td"><input type="text" maxlength="32" size="32" name="wl_key3" class="content_input_fd" value="<% nvram_get_x("WLANConfig11b","wl_key3"); %>" onBlur="return validate_wlkey(this, 'WLANConfig11b')" onKeyPress="return is_wlkey(this, 'WLANConfig11b')" onKeyUp="return change_wlkey(this, 'WLANConfig11b')"></td>
</tr>
<tr>
<td class="content_header_td_less"><#BOP_wlan_wep4_item#></td><td class="content_input_td"><input type="text" maxlength="32" size="32" name="wl_key4" class="content_input_fd" value="<% nvram_get_x("WLANConfig11b","wl_key4"); %>" onBlur="return validate_wlkey(this, 'WLANConfig11b')" onKeyPress="return is_wlkey(this, 'WLANConfig11b')" onKeyUp="return change_wlkey(this, 'WLANConfig11b')"></td>
</tr>
<tr>
<td class="content_header_td_less"><#BOP_wlan_defkey_item#></td><td class="content_input_td"><select name="wl_key" class="content_input_fd" onChange="return change_common(this, 'WLANConfig11b', 'wl_key')"><option value="1" <% nvram_match_x("WLANConfig11b","wl_key", "1","selected"); %>>1</option>
			<option value="2" <% nvram_match_x("WLANConfig11b","wl_key", "2","selected"); %>>2</option>
			<option value="3" <% nvram_match_x("WLANConfig11b","wl_key", "3","selected"); %>>3</option>
			<option value="4" <% nvram_match_x("WLANConfig11b","wl_key", "4","selected"); %>>4</option></select></td>
</tr>
<tr>
	<td class="content_header_td" onMouseOver="return overlib('<#WLANConfig11b_x_ExtendMode_itemdesc#>', LEFT);" onMouseOut="return nd();"><#WLANConfig11b_x_ExtendMode_itemname#></td>
	<td class="content_input_td">
		<select name="wl_mode_ex" class="content_input_fd" onChange="return change_common(this, 'WLANConfig11b', 'wl_mode_ex')">
			<option class="content_input_fd" value="ap" <% nvram_match_x("WLANConfig11b","wl_mode_ex", "ap","selected"); %>>AP</option>
			<option class="content_input_fd" value="re" <% nvram_match_x("WLANConfig11b","wl_mode_ex", "re","selected"); %>>Repeater</option>
		</select>
	</td>
</tr>
</table>
</td>
</tr>

<tr>
<td>		
<table width="666" border="1" cellpadding="0" cellspacing="0" bordercolor="B0B0B0">
<tr bgcolor="#CCCCCC"><td colspan="3"><font face="arial" size="2"><b>&nbsp</b></font></td></tr>
<tr bgcolor="#FFFFFF">  
   <td height="25" width="34%">  
   </td>
   <td height="25" width="33%">  
   </td>
   <td height="25" width="33%">  
   <div align="center">
   <font face="Arial"><input class=inputSubmit style="cursor:pointer;" onMouseOut="buttonOut(this)" onMouseOver="buttonOver(this)" onClick="toPrevTag('Wireless')" type="button" value="<#CTL_prev#>" name="action"></font>&nbsp;&nbsp;&nbsp;&nbsp;
   <font face="Arial"><input class=inputSubmit style="cursor:pointer;" onMouseOut="buttonOut(this)" onMouseOver="buttonOver(this)" type="submit" value="<#CTL_finish#>" name="action" onClick="return saveQuick(this);"></font></div>
   </td>
</tr>
</table>
</td>
</tr>

<tr>
<td>
<table width="666" border="1" cellpadding="0" cellspacing="0" bordercolor="B0B0B0">
<tr>
    <td colspan="2" width="666" height="25" bgcolor="#FFBB00"></td> 
</tr>                   
</table>
</td>
</tr>
<tr><td colspan="2" height="240"></td></tr>
</table>
</form>

