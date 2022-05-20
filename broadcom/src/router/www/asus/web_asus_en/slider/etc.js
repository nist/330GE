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


function drawSliderBlock(){
	
	x = new getObj('sliderDiv');
	if ((document.all) || (document.getElementById)){
		x.obj.innerHTML = slider_table();
	}
	else{
		x.obj.document.write(slider_table());
		x.obj.document.close();
	}

	initSlider();
}

function loadSlider()
{	
	changePower(0);
	drawSliderBlock();
}


function slider_table()
{
	var result;
	var assignment_box;
	var disk_num=1;
	var num_disks_in_pane;
	var disk_array = ["A","B"];
	var space_array = ["100","100"];
	syncDisksInPool = true;
	maxSpaceAllowedInSync = 0.0;
	var isJbodPool = 1;
	
	result = "<table border=0 cellspacing=0 cellpadding=0><tr><td>";
	
	result += "<div id='sliderDiv" + disk_num + "'></div>\n";
		// the slider movement and gigabytes textbox value are kept in sync only if there is one disk per pane and it is a mirrored or striped pool

	//alert(stripe_count());
	//alert(mirror_count());
	result += "</td></tr></table>";
	return result;

}


 var max_slider_width = 18; // change it if needed

 function initSlider(){
 
 	var disk_num;
 	
 	sliderArray = new Array();
 	Bs_Objects = new Array(); // Have to initialize the internal array used by slider class as well
 	
 	var colorArray = new Array("red", "red","orange","green","magenta","yellow","turquoise");


	disk_num=1;
	
//	if (document.forms[0].regulation_domain.value == "30DE")
//		sliderArray[disk_num.toString(10)] = drawSlider(disk_num.toString(10),"sliderDiv" + disk_num, "red",1, 9, document.forms[0].wl_radio_power_x.value, 1, false); 
//	else
 		sliderArray[disk_num.toString(10)] = drawSlider(disk_num.toString(10),"sliderDiv" + disk_num, "red",1, 10, document.forms[0].wl_radio_power_x.value, 1, false); 
	return;
 
 }
 
 function drawSlider(sliderName,divName,color,minVal,maxVal,initVal,interval,isDisabled){
   
   if (arguments[0] != "")
   	mySlider = new Bs_Slider(sliderName);
   else
   	mySlider = new Bs_Slider();
   	
   mySlider.attachOnChange(bsSliderChange);
   mySlider.width         =  ( (maxVal / 2) * max_slider_width );
   //mySlider.width       = 121;
   mySlider.height        = 18;
   mySlider.minVal        = minVal;
   mySlider.maxVal        = maxVal;
   mySlider.valueInterval = interval;
   mySlider.arrowAmount   = interval;
   mySlider.valueDefault  = initVal;
   mySlider.imgDir   = 'slider/';
   //mySlider.setBackgroundImage('background.gif', 'no-repeat');
   mySlider.setBackgroundImage('background.gif', 'repeat');
   mySlider.setBackgroundImageLeft('background_left.gif', 7, 18);
   mySlider.setBackgroundImageRight('background_right.gif', 7, 18);
   mySlider.setSliderIcon('slider.gif', 13, 18);
   mySlider.setArrowIconLeft('arrowLeft.gif', 16, 16);
   mySlider.setArrowIconRight('arrowRight.gif', 16, 16);
   mySlider.useInputField = 0;
   //mySlider.styleValueFieldClass = 'sliderInput';
   mySlider.colorbar = new Object();
   mySlider.colorbar['color']           = color;
   mySlider.colorbar['height']          = 7;
   mySlider.colorbar['widthDifference'] = -14;
   mySlider.colorbar['offsetLeft']      = -3;
   mySlider.colorbar['offsetTop']       = 9;
   mySlider.setDisabled(isDisabled);
   mySlider.drawInto(divName);
   
   return mySlider;

 }
 
 function changePower(flag)
 {
 	var bpower, gpower, power_index;
 	
 	power_index=document.forms[0].wl_radio_power_x.value;
/*      	
      	if (power_index==12)
      	{	
      		bpower=23;
   		gpower=20;      	
      	}
      	else if(power_index==11)
      	{	
      		bpower=22;
   		gpower=19;
      	}
      	else
*/
      	if(power_index==10)
      	{	
      		bpower=21;
   		gpower=18;
      	}
      	else if(power_index==9)
      	{	
      		bpower=19;
   		gpower=19;
      	}
      	else
      	{
   		bpower=eval(power_index)*2+2;
   		gpower=eval(power_index)*2+2;
   	}	
   		 
   	document.forms[0].power_index_1.value = "B: " + bpower + " dbm / G: " + gpower + " dbm";
   	if (flag)
   	{	
   		window.top.pageChanged = 1;	
   	}	
 }
 
function bsSliderChange(sliderObj, val, newPos){       
   // if mirrored or striped and one disk per pane, make sure all disk sizes are made the same
   // update slider value and hidden field value here for the current field alone
   document.forms[0].wl_radio_power_x.value=sliderObj.getValue();
   changePower(1);   
}