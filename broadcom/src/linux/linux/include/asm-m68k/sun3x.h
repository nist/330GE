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
#ifndef SUN3X_H
#define SUN3X_H

/* hardware addresses */
#define SUN3X_IOMMU       0x60000000
#define SUN3X_ENAREG      0x61000000
#define SUN3X_INTREG      0x61001400
#define SUN3X_DIAGREG     0x61001800
#define SUN3X_ZS1         0x62000000
#define SUN3X_ZS2         0x62002000
#define SUN3X_LANCE       0x65002000
#define SUN3X_EEPROM      0x64000000
#define SUN3X_IDPROM      0x640007d8
#define SUN3X_VIDEO_BASE  0x50000000
#define SUN3X_VIDEO_P4ID  0x50300000
#define SUN3X_ESP_BASE	  0x66000000
#define SUN3X_ESP_DMA	  0x66001000
#define SUN3X_FDC         0x6e000000
#define SUN3X_FDC_FCR     0x6e000400
#define SUN3X_FDC_FVR     0x6e000800

/* some NVRAM addresses */
#define SUN3X_EEPROM_CONS 	(SUN3X_EEPROM + 0x1f)
#define SUN3X_EEPROM_PORTA	(SUN3X_EEPROM + 0x58)
#define SUN3X_EEPROM_PORTB	(SUN3X_EEPROM + 0x60)

#endif
