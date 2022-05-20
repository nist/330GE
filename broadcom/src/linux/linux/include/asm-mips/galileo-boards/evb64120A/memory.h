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
/* Memory.h - Memory mappings and remapping functions declarations */

/* Copyright - Galileo technology. */

#ifndef __INCmemoryh
#define __INCmemoryh

/* includes */

#include "core.h"

/* defines */

#define DONT_MODIFY     0xffffffff
#define PARITY_SUPPORT  0x40000000

#define _8BIT           0x00000000
#define _16BIT          0x00010000
#define _32BIT          0x00020000
#define _64BIT          0x00030000

/* typedefs */

typedef enum __memBank{BANK0,BANK1,BANK2,BANK3} MEMORY_BANK;
typedef enum __device{DEVICE0,DEVICE1,DEVICE2,DEVICE3,BOOT_DEVICE} DEVICE;


unsigned int   getMemoryBankBaseAddress(MEMORY_BANK bank);
unsigned int   getDeviceBaseAddress(DEVICE device);
unsigned int   getMemoryBankSize(MEMORY_BANK bank);
unsigned int   getDeviceSize(DEVICE device);
unsigned int   getDeviceWidth(DEVICE device);

bool mapMemoryBanks0and1(unsigned int bank0Base,unsigned int bank0Length,
                         unsigned int bank1Base,unsigned int bank1Length);
bool mapMemoryBanks2and3(unsigned int bank2Base,unsigned int bank2Length,
                         unsigned int bank3Base,unsigned int bank3Length);
bool mapDevices0_1and2MemorySpace(unsigned int device0Base,
                                  unsigned int device0Length,
                                  unsigned int device1Base,
                                  unsigned int device1Length,
                                  unsigned int device2Base,
                                  unsigned int device2Length);
bool mapDevices3andBootMemorySpace(unsigned int device3Base,
                                   unsigned int device3Length,
                                   unsigned int bootDeviceBase,
                                   unsigned int bootDeviceLength);
bool mapInternalRegistersMemorySpace(unsigned int internalRegBase);
bool modifyDeviceParameters(DEVICE device,unsigned int TurnOff,
                            unsigned int AccToFirst,unsigned int AccToNext,
                            unsigned int ALEtoWr, unsigned int WrActive,
                            unsigned int WrHigh,unsigned int Width,
                            bool ParitySupport);

bool remapAddress(unsigned int remapReg, unsigned int remapValue);
#endif /* __INCmemoryh */

