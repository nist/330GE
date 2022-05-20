<head>
<title><#ZVMODELVZ#> Web Manager</title>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<script language="JavaScript" type="text/javascript" src="/javascript/menu.js"></script>
<script language="JavaScript" type="text/javascript" src="/javascript/title.js"></script>
<script language="JavaScript" type="text/javascript" src="/javascript/utils.js"></script>
<link rel="stylesheet" type="text/css" href="style.css" media="screen"></link>
<script language="JavaScript" type="text/javascript" src="overlib.js"></script>
<script language="JavaScript" type="text/javascript" src="general.js"></script>
<input type="hidden" name="preferred_lang" value="<% nvram_get_x("","preferred_lang"); %>">
</head>  

<script language='javascript'>
<!--
    var qos_global_enable_x="<% nvram_get_x("PrinterStatus", "qos_global_enable"); %>";
    var qos_pshack="<% nvram_get_x("PrinterStatus", "qos_pshack_prio"); %>";            //Game
    var qos_shortpkt="<% nvram_get_x("PrinterStatus", "qos_shortpkt_prio"); %>"; 	    //Internet
    var qos_service_enable_x="0"; //FTP
    var qos_tos="<% nvram_get_x("PrinterStatus", "qos_tos_prio"); %>";		    //VOIP


function click_GameBtn()
{
    with(document.forms[0]){
    	if(qos_pshack_prio.value==0){
		qos_global_enable.value=1;
		qos_pshack_prio.value=1;		
		blocking('GameOn' ,true);
		blocking('GameOff' ,false);

                qos_shortpkt_prio.value=1;
                blocking('InternetOn' ,true);
                blocking('InternetOff' ,false);

	}
	else{
		qos_pshack_prio.value=0;		
		blocking('GameOn' ,false);
		blocking('GameOff' ,true);

                qos_shortpkt_prio.value=0;
                blocking('InternetOn' ,false);
                blocking('InternetOff' ,true);

		if(qos_shortpkt_prio.value==0 && qos_service_enable.value==0 && qos_tos_prio.value==0)
			qos_global_enable.value=0;
	}
	constructDynm();
    }
}
                                                                                                                                              
function click_InternetBtn()
{
    with(document.forms[0]){
        if(qos_shortpkt_prio.value==0){
                qos_global_enable.value=1;
                qos_shortpkt_prio.value=1;
                blocking('InternetOn' ,true);
                blocking('InternetOff' ,false);
		
		qos_pshack_prio.value=1;		
		blocking('GameOn' ,true);
		blocking('GameOff' ,false);
        }
        else{
                qos_shortpkt_prio.value=0;
                blocking('InternetOn' ,false);
                blocking('InternetOff' ,true);
		
		qos_pshack_prio.value=0;		
		blocking('GameOn' ,false);
		blocking('GameOff' ,true);
                
		if(qos_pshack_prio.value==0 && qos_service_enable.value==0 && qos_tos_prio.value==0)
                        qos_global_enable.value=0;
        }
        constructDynm();
    }
}
                                                                                                                                              
function click_ServerBtn()
{
    with(document.forms[0]){
    	if(qos_service_enable.value==0){
		qos_global_enable.value=1;
		qos_service_enable.value=1;		
		blocking('ServerOn' ,true);
		blocking('ServerOff' ,false);	
	}
	else{
		qos_service_enable.value=0;		
		blocking('ServerOn' ,false);
		blocking('ServerOff' ,true);	
                if(qos_pshack_prio.value==0 && qos_shortpkt_prio.value==0 && qos_tos_prio.value==0)
                        qos_global_enable.value=0;
	}
	constructDynm();
    }
}

function click_VOIPBtn()
{
    with(document.forms[0]){
        if(qos_tos_prio.value==0){
                qos_global_enable.value=1;
                qos_tos_prio.value=1;
                blocking('VOIPOn' ,true);
                blocking('VOIPOff' ,false);
        }
        else{
                qos_tos_prio.value=0;
                blocking('VOIPOn' ,false);
                blocking('VOIPOff' ,true);
                if(qos_pshack_prio.value==0 && qos_shortpkt_prio.value==0 && qos_service_enable.value==0)
                        qos_global_enable.value=0;
        }
        constructDynm();
    }
}
                                                                                                                                              
function ShowNote(i)
{
	if(i==1)
	{
		blocking('NO', true);
		blocking('GAME', false);	
		blocking('INTERNET', false);
		blocking('SERVER', false);
		blocking('VOIP', false);
	}
	else if(i==3)
	{
		blocking('NO', false);
		blocking('GAME', true);	
		blocking('INTERNET', false);
		blocking('SERVER', false);
		blocking('VOIP', false);
	}
	else if(i==4)
	{
		blocking('NO', false);
		blocking('GAME', false);	
		blocking('INTERNET', true);
		blocking('SERVER', false);
		blocking('VOIP', false);
	}
	else if(i==5)
	{
		blocking('NO', false);
		blocking('GAME', false);	
		blocking('INTERNET', false);
		blocking('SERVER', true);
		blocking('VOIP', false);
	}
	else if(i==6)
	{
		blocking('NO', false);
		blocking('GAME', false);	
		blocking('INTERNET', false);
		blocking('SERVER', false);
		blocking('VOIP', true);
	}
}

function blocking(nr,show)
{
        if (document.getElementById)
        {
                var vista = (show) ? 'block' : 'none';
                document.getElementById(nr).style.display = vista;
        }
        else if (document.layers)
        {
                var current = (show) ? 'block' : 'none';
                document.layers[nr].display = current;
        }
        else if (document.all)
        {
                var current = (show) ? 'block' : 'none';
                document.all[nr].style.display = current;
        }
}

function frmload()
{
    with(document.forms[0]){
	qos_global_enable.vlue=qos_global_enable_x;
	qos_shortpkt_prio.value=qos_shortpkt;
	qos_pshack_prio.value=qos_pshack;
	qos_tos_prio.value=qos_tos;
	qos_service_enable.value=qos_service_enable_x;
	
	if(qos_pshack_prio.value==0) {
		blocking('GameOn' ,false);
		blocking('GameOff' ,true);
	}
	else {
		blocking('GameOn' ,true);
		blocking('GameOff' ,false);
	}
        if(qos_shortpkt_prio.value==0) {
                blocking('InternetOn' ,false);
                blocking('InternetOff' ,true);
        }
        else {
                blocking('InternetOn' ,true);
                blocking('InternetOff' ,false);
        }
/*
	if(qos_service_enable.value==0) {
		blocking('ServerOn' ,false);
		blocking('ServerOff' ,true);
	}
	else {
		blocking('ServerOn' ,true);
		blocking('ServerOff' ,false);
	}
*/
        if(qos_tos_prio.value==0) {
                blocking('VOIPOn' ,false);
                blocking('VOIPOff' ,true);
        }
        else {
                blocking('VOIPOn' ,true);
                blocking('VOIPOff' ,false);
        }
	ShowNote(1);
	constructDynm();
    }
}
                                                                                                                                              
function getObj(name)
{
        if (document.getElementById)
        {
                this.obj = document.getElementById(name);
                this.style = document.getElementById(name).style;
        }
        else if (document.all)
        {
                this.obj = document.all[name];
                this.style = document.all[name].style;
        }
        else if (document.layers)
        {
                this.obj = document.layers[name];
                this.style = document.layers[name];
        }
}
              
function paint_backup_progress(i)
{
    with(document.forms[0]){                                                                                                                                              
        var result = "";
        var punit, progress;
	var total = parseInt(qos_shortpkt_prio.value) + parseInt(qos_pshack_prio.value) + parseInt(qos_tos_prio.value) + parseInt(qos_service_enable.value);

	if( i==1 ) { //Game
	    if (qos_pshack_prio.value==1)
	    	progress=12/total;
	    else
	    	progress=1;
	}
	if( i==2 ) { //Internet
	    if (qos_shortpkt_prio.value==1)
	    	progress=12/total;
	    else
	    	progress=1;
	}
	if( i==3 ) { //FTP
		progress=0;
/*
	    if (qos_service_enable.value==1)
	    	progress=12/total;
	    else
	    	progress=1;
*/
	}
	if( i==4 ) { //VOIP
	    if (qos_tos_prio.value==1)
	    	progress=12/total;
	    else
	    	progress=1;
	}
		
        result += '<table  border="0"  width="360" border="0" cellpadding="0" cellspacing="0">';
                                                                                                                                              
        result += '<tr><td><table  border="0" border="0" cellpadding="0" cellspacing="0"><tr height="20">';
        for(punit = 0; punit < progress; punit++){
                result += '<td width="30" bgcolor="#00cc00"></td>';
        }
        result += '</tr></table></td>';
        result += "</tr></table>";

        return result;
    }
}
                                                                                                                                
function constructDynm(){
        var w,x,y,z;
                                                                                                                                              
        w = new getObj("backup_progress_div1");
                                                                                                                                              
        if((document.all) || (document.getElementById)){
                w.obj.innerHTML = paint_backup_progress(1);
        } else{
                w.obj.document.write(paint_backup_progress(1));
                w.obj.document.close();
        }
        
        x = new getObj("backup_progress_div2");
                                                                                                                                             
        if((document.all) || (document.getElementById)){
                x.obj.innerHTML = paint_backup_progress(2);
        } else{
                x.obj.document.write(paint_backup_progress(2));
                x.obj.document.close();
        }
        
        y = new getObj("backup_progress_div3");
                                                                                                                                              
        if((document.all) || (document.getElementById)){
                y.obj.innerHTML = paint_backup_progress(3);
        } else{
                y.obj.document.write(paint_backup_progress(3));
                y.obj.document.close();
        }
        
        z = new getObj("backup_progress_div4");
                                                                                                                                              
        if((document.all) || (document.getElementById)){
                z.obj.innerHTML = paint_backup_progress(4);
        } else{
                z.obj.document.write(paint_backup_progress(4));
                z.obj.document.close();
        }
}
-->
</script>
<style ="text/css">
<!--
body {
    color: #000000;
    background-color: #FFFFFF;
}
.ModeText {
	font-size:13px;
	font-weight:bold;
	TEXT-DECORATION: none;
	font-family: Arial, Helvetica, sans-serif;
}

.title {font-size: 12px; font-weight: bold; TEXT-DECORATION: none; font-family: Arial, Helvetica, sans-serif; color: #FFFFFF; }
-->
</style>

<div id="overDiv" style="position:absolute; visibility:hidden; z-index:1000;"></div>    
<body onLoad="frmload();load_body()" onunLoad="return unload_body();">
<form method="GET" name="form" action="apply.cgi">
<!-- Table for the conntent page -->	    
<table  width="666" border="0" cellpadding="0" cellspacing="0">     	      
    	
<input type="hidden" name="current_page" value="Advanced_QOS_Content.asp"><input type="hidden" name="next_page" value="Advanced_QOSUserSpec_Content.asp"><input type="hidden" name="next_host" value=""><input type="hidden" name="sid_list" value="PrinterStatus;"><input type="hidden" name="group_id" value=""><input type="hidden" name="modified" value="0"><input type="hidden" name="action_mode" value=""><input type="hidden" name="first_time" value=""><input type="hidden" name="action_script" value=""><input type="hidden" name="preferred_lang" value="<% nvram_get_x("","preferred_lang"); %>">
<input type='hidden' name='qos_global_enable' value="<% nvram_get_x('PrinterStatus', 'qos_global_enable'); %>">
<input type='hidden' name='qos_pshack_prio'> <!-- Game -->
<input type='hidden' name='qos_shortpkt_prio'><!-- Internet -->
<input type='hidden' name='qos_service_enable' value="0"><!-- FTP -->
<input type='hidden' name='qos_tos_prio'><!-- VOIP -->
<tr>
<td>
<table width="666" border="1" cellpadding="0" cellspacing="0" bordercolor="E0E0E0">
<tr class="content_header_tr">
<td class="content_header_td_title" colspan="2"><#BM_title_BOD#></td>
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
<table width="666" border="0" cellpadding="0" cellspacing="0" bordercolor="E0E0E0"></table>
</td>
</tr>
<tr>
<td>
<table width="666" border="0" cellpadding="0" cellspacing="0" bordercolor="E0E0E0">

<table class="content_list_table" width="640" border="0" cellspacing="0" cellpadding="0">
<tr>
<td colspan="3" height="10"></td>
</tr>
<tr>
<td colspan="3">
<div align="center">
     <table width="452" height="86" border="0" cellpadding="0" cellspacing="0">
      <tr>
      <td width="90"></td>
      <td width="88" height="86" align="center" valign="middle" background="graph/game_botton_bg.gif"
          onMouseOver="ShowNote(3);" onMouseOut="ShowNote(1);" onclick="click_GameBtn(); NSKey();"; STYLE="cursor:hand;cursor:pointer;">
      <table width="88" border="0" cellspacing="0" cellpadding="0">
        <tr>
          <th width="88" height="43" scope="col">&nbsp;</th>
        </tr>
        <tr>
          <td width="88" height="43" align="center">
            <div id='GameOn' style='display:none'>
          	<font color='#FFFF00' class="ModeText"><#BM_Gaming#><br/><#BM_Blaster#></font></a>
	    </div>
	    <div id='GameOff' style='display:none'>
          	<font color='#000066' class="ModeText"><#BM_Gaming#><br/><#BM_Blaster#></font></a>
	    </div>
          </td>
        </tr>
      </table>
      </td>
      <td width="88" height="86" align="center" valign="middle">&nbsp;</td>
      <td width="88" height="86" align="center" valign="middle" background="graph/download_botton_bg.gif"
          onMouseOver="ShowNote(4);" onMouseOut="ShowNote(1);" onclick="click_InternetBtn(); NSKey();" STYLE="cursor:hand;cursor:pointer;">
      <table width="88" border="0" cellspacing="0" cellpadding="0">
        <tr>
          <th width="88" height="43" scope="col">&nbsp;</th>
        </tr>
        <tr>
          <td width="88" height="43" align="center">
            <div id='InternetOn' style='display:none'>
          	<font color='#FFFF00' class="ModeText"><#BM_Internet#><br/><#BM_Application#></font></a>
	    </div>
	    <div id='InternetOff' style='display:none'>
          	<font color='#000066' class="ModeText"><#BM_Internet#><br/><#BM_Application#></font></a>
	    </div>
          </td>
        </tr>
      </table>
      </td>

      <td width="88" height="86" align="center" valign="middle">&nbsp;</td>
      <td width="88" height="86" align="center" valign="middle" background="graph/video_botton_bg.gif" onMouseOver="ShowNote(6);" onMouseOut="ShowNote(1);" onclick="click_VOIPBtn(); NSKey();" STYLE="cursor:hand;cursor:pointer;">
      <table width="88" border="0" cellspacing="0" cellpadding="0">
        <tr>
          <th width="88" height="43" scope="col">&nbsp;</th>
        </tr>
        <tr>
          <td width="88" height="43" align="center">
            <div id='VOIPOn' style='display:none'>
          	<font color='#FFFF3C' class="ModeText"><#BM_VOIP/Video#><br/><#BM_Streaming#></font></a>
	    </div>
	    <div id='VOIPOff' style='display:none'>
          	<font color='#000066' class="ModeText"><#BM_VOIP/Video#><br/><#BM_Streaming#></font></a>
	    </div>
          </td>
        </tr>
      </table>
      </td>

      <td width="1" height="86" align="center" valign="middle">&nbsp;</td>
      <td width="1" height="86" align="center" valign="middle">
      <table width="1" border="0" cellspacing="0" cellpadding="0">
        <tr>
          <th width="1" height="43" scope="col">&nbsp;</th>
        </tr>
        <tr>
          <td width="1" height="43" align="center">
            <div id='ServerOn' style='display:none'>
          	<font color='#FFFF00' class="ModeText"><#BM_FTP#><br/><#BM_Server#></font></a>
	    </div>
	    <div id='ServerOff' style='display:none'>
          	<font color='#000066' class="ModeText"><#BM_FTP#><br/><#BM_Server#></font></a>
	    </div>
          </td>
        </tr>
      </table>
      </td>

    </tr>
    </table><br>
  <table border="0" >
  <tr height="70"><td valign='top' width="510">
  <div id='NO' style='display:none'>&nbsp;</div>
  <div id='GAME' style='display:none'><#BM_desc1#></div>
  <div id='INTERNET' style='display:none'><#BM_desc2#></div>
  <div id='SERVER' style='display:none'><#BM_desc3#></div>
  <div id='VOIP' style='display:none'><#BM_desc4#></div>
  </tr></td>
  </table>

    <table border="0" cellpadding="0" cellspacing="0">
        <tr>
                <td width="150"></td>
                <td width="50"></td>
                <td width="260" class="content_header_td" align="center"><#BM_Prioroty#></td>
                <td width="50"></td>
        </tr>
        <tr>
                <td></td>
                <td align="center">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<b><font face="Arial" size="2"><#BM_low#></font></b></td>
		<td></td>
		<td align="left"><b><font face="Arial" size="2"><#BM_high#></font></b></td>
        </tr>
      
    </table>
    <table border="0" cellpadding="0" cellspacing="0">
        <tr>
		<td width='150' align="right" class="ModeText"><b><#BM_GB#> &nbsp;</b><td>
                <td><div id='backup_progress_div1'></div></td>
        </tr>
        <tr height="20"><td></td></tr>
        <tr>
                <td align="right" class="ModeText"><b><#BM_IA#> &nbsp;</b><td>
                <td><div id='backup_progress_div2'></div></td>
        </tr>
        <tr height="20"><td></td></tr>
        <tr>
                <td align="right" class="ModeText"><b><#BM_VVS#> &nbsp;</b><td>
                <td><div id='backup_progress_div4'></div></td>
        </tr>
        <tr height="20"><td></td></tr>
        <tr>
                <td align="right" class="ModeText"><td>
                <td><div id='backup_progress_div3'></div></td>
        </tr>
    </table>
</div>
</td>
</tr>
<tr>
<td colspan="3" height="10"></td>
</tr>
</table>
</table>
</td>
</tr>
<tr>
<td>
</td>
</tr>
<tr>
<td>		
<table width="666" border="1" cellpadding="0" cellspacing="0" bordercolor="B0B0B0">
<tr bgcolor="#CCCCCC"><td colspan="3"><font face="arial" size="2"><b>&nbsp</b></font></td></tr>
<tr bgcolor="#FFFFFF">  
   <td id ="Confirm" height="25" width="34%">  
   <div align="center"><font face="Arial"> <input class=inputSubmit onMouseOut=buttonOut(this) onMouseOver="buttonOver(this)" onClick="return onSubmitCtrl(this, ' Restore ');" type="submit" value=" <#CTL_restore#> " name="action"></font></div> 
   </td>  
   <td height="25" width="33%">  
   <div align="center"><font face="Arial"> <input class=inputSubmit onMouseOut=buttonOut(this) onMouseOver="buttonOver(this)" onClick="return onSubmitCtrl(this, ' Finish ');" type="submit" value=" <#CTL_finish#> " name="action"></font></div> 
   </td>
   <td height="25" width="33%">  
   <div align="center"><font face="Arial"> <input class=inputSubmit onMouseOut=buttonOut(this) onMouseOver="buttonOver(this)" onClick="return onSubmitCtrl(this, ' Apply ');" type="submit" value=" <#CTL_apply#> " name="action"></font></div> 
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
