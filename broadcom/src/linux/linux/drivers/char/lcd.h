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
/*
 * LED, LCD and Button panel driver for Cobalt
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1996, 1997 by Andrew Bose
 *
 * Linux kernel version history:
 *       March 2001: Ported from 2.0.34  by Liam Davies
 *
 */

// function headers

int dqpoll( volatile unsigned long, volatile unsigned char );
int timeout( volatile unsigned long );

#define LCD_CHARS_PER_LINE 40
#define FLASH_SIZE 524288
#define MAX_IDLE_TIME 120

struct lcd_display {
        unsigned long buttons;
        int size1;
        int size2;
        unsigned char line1[LCD_CHARS_PER_LINE];
        unsigned char line2[LCD_CHARS_PER_LINE];
        unsigned char cursor_address;
        unsigned char character;
        unsigned char leds;
        unsigned char *RomImage;
};



#define LCD_DRIVER	"Cobalt LCD Driver v2.10"

#define kLCD_IR		0xBF000000
#define kLCD_DR		0xBF000010
#define kGPI            0xBD000000
#define kLED		0xBC000000

#define kDD_R00         0x00
#define kDD_R01         0x27
#define kDD_R10         0x40
#define kDD_R11         0x67

#define kLCD_Addr       0x00000080

#define LCDTimeoutValue	0xfff


// Flash definitions AMD 29F040
#define kFlashBase      0xBFC00000

#define kFlash_Addr1    0x5555
#define kFlash_Addr2    0x2AAA
#define kFlash_Data1    0xAA
#define kFlash_Data2    0x55
#define kFlash_Prog     0xA0
#define kFlash_Erase3   0x80
#define kFlash_Erase6   0x10
#define kFlash_Read     0xF0

#define kFlash_ID       0x90
#define kFlash_VenAddr  0x00
#define kFlash_DevAddr  0x01
#define kFlash_VenID    0x01
#define kFlash_DevID    0xA4    // 29F040
//#define kFlash_DevID  0xAD    // 29F016


// Macros

#define LCDWriteData(x)	(*(volatile unsigned long *) kLCD_DR) = (x << 24)
#define LCDWriteInst(x)	(*(volatile unsigned long *) kLCD_IR) = (x << 24)

#define LCDReadData	(((*(volatile unsigned long *) kLCD_DR) >> 24))
#define LCDReadInst	(((*(volatile unsigned long *) kLCD_IR) >> 24))

#define GPIRead         (( (*(volatile unsigned long *) kGPI) >> 24))

#define LEDSet(x)	(*(volatile unsigned char *) kLED) = ((char)x)

#define WRITE_GAL(x,y)  (*((volatile unsigned long *) (0xB4000000 | (x)) ) =y)
#define BusyCheck()     while ((LCDReadInst & 0x80) == 0x80)

#define WRITE_FLASH(x,y)  (*((volatile unsigned char *) (kFlashBase | (x)) ) = y)
#define READ_FLASH(x)     *((volatile unsigned char *) (kFlashBase | (x)) )



/* 
 * Function command codes for io_ctl.
 */
#define LCD_On			1
#define LCD_Off			2
#define LCD_Clear		3
#define LCD_Reset		4
#define LCD_Cursor_Left		5
#define LCD_Cursor_Right	6
#define LCD_Disp_Left		7
#define LCD_Disp_Right		8
#define LCD_Get_Cursor		9
#define LCD_Set_Cursor		10
#define LCD_Home		11
#define LCD_Read		12		
#define LCD_Write		13	
#define LCD_Cursor_Off		14
#define LCD_Cursor_On		15
#define LCD_Get_Cursor_Pos	16
#define LCD_Set_Cursor_Pos	17
#define LCD_Blink_Off           18

#define LED_Set			40	
#define LED_Bit_Set		41
#define LED_Bit_Clear		42


//  Button defs
#define BUTTON_Read             50  

//  Flash command codes
#define FLASH_Erase		60
#define FLASH_Burn		61
#define FLASH_Read		62


// Ethernet LINK check hackaroo
#define LINK_Check              90
#define LINK_Check_2		91

//  Button patterns  _B - single layer lcd boards

#define BUTTON_NONE               0x3F
#define BUTTON_NONE_B             0xFE

#define BUTTON_Left               0x3B
#define BUTTON_Left_B             0xFA

#define BUTTON_Right              0x37
#define BUTTON_Right_B            0xDE

#define BUTTON_Up                 0x2F
#define BUTTON_Up_B               0xF6

#define BUTTON_Down               0x1F
#define BUTTON_Down_B             0xEE

#define BUTTON_Next               0x3D
#define BUTTON_Next_B             0x7E

#define BUTTON_Enter              0x3E
#define BUTTON_Enter_B            0xBE

#define BUTTON_Reset_B            0xFC


// debounce constants

#define BUTTON_SENSE            160000
#define BUTTON_DEBOUNCE		5000


//  Galileo register stuff

#define kGal_DevBank2Cfg        0x1466DB33
#define kGal_DevBank2PReg       0x464
#define kGal_DevBank3Cfg        0x146FDFFB
#define kGal_DevBank3PReg       0x468

// Network 

#define kIPADDR			1
#define kNETMASK		2
#define kGATEWAY		3	
#define kDNS			4

#define kClassA			5
#define kClassB			6
#define kClassC			7

