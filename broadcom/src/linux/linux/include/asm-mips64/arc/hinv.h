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
 * ARCS hardware/memory inventory/configuration and system ID definitions.
 */
#ifndef _ASM_ARC_HINV_H
#define _ASM_ARC_HINV_H

#include <asm/arc/types.h>

/* configuration query defines */
typedef enum configclass {
	SystemClass,
	ProcessorClass,
	CacheClass,
#ifndef	_NT_PROM
	MemoryClass,
	AdapterClass,
	ControllerClass,
	PeripheralClass
#else	/* _NT_PROM */
	AdapterClass,
	ControllerClass,
	PeripheralClass,
	MemoryClass
#endif	/* _NT_PROM */
} CONFIGCLASS;

typedef enum configtype {
	ARC,
	CPU,
	FPU,
	PrimaryICache,
	PrimaryDCache,
	SecondaryICache,
	SecondaryDCache,
	SecondaryCache,
#ifndef	_NT_PROM
	Memory,
#endif
	EISAAdapter,
	TCAdapter,
	SCSIAdapter,
	DTIAdapter,
	MultiFunctionAdapter,
	DiskController,
	TapeController,
	CDROMController,
	WORMController,
	SerialController,
	NetworkController,
	DisplayController,
	ParallelController,
	PointerController,
	KeyboardController,
	AudioController,
	OtherController,
	DiskPeripheral,
	FloppyDiskPeripheral,
	TapePeripheral,
	ModemPeripheral,
	MonitorPeripheral,
	PrinterPeripheral,
	PointerPeripheral,
	KeyboardPeripheral,
	TerminalPeripheral,
	LinePeripheral,
	NetworkPeripheral,
#ifdef	_NT_PROM
	Memory,
#endif
	OtherPeripheral,

	/* new stuff for IP30 */
	/* added without moving anything */
	/* except ANONYMOUS. */

	XTalkAdapter,
	PCIAdapter,
	GIOAdapter,
	TPUAdapter,

	Anonymous
} CONFIGTYPE;

typedef enum {
	Failed = 1,
	ReadOnly = 2,
	Removable = 4,
	ConsoleIn = 8,
	ConsoleOut = 16,
	Input = 32,
	Output = 64
} IDENTIFIERFLAG;

#ifndef NULL			    /* for GetChild(NULL); */
#define	NULL	0
#endif

union key_u {
	struct {
#ifdef	_MIPSEB
		unsigned char  c_bsize;		/* block size in lines */
		unsigned char  c_lsize;		/* line size in bytes/tag */
		unsigned short c_size;		/* cache size in 4K pages */
#else	/* _MIPSEL */
		unsigned short c_size;		/* cache size in 4K pages */
		unsigned char  c_lsize;		/* line size in bytes/tag */
		unsigned char  c_bsize;		/* block size in lines */
#endif	/* _MIPSEL */
	} cache;
	ULONG FullKey;
};

#if _MIPS_SIM == _ABI64
#define SGI_ARCS_VERS	64			/* sgi 64-bit version */
#define SGI_ARCS_REV	0			/* rev .00 */
#else
#define SGI_ARCS_VERS	1			/* first version */
#define SGI_ARCS_REV	10			/* rev .10, 3/04/92 */
#endif

typedef struct component {
	CONFIGCLASS	Class;
	CONFIGTYPE	Type;
	IDENTIFIERFLAG	Flags;
	USHORT		Version;
	USHORT		Revision;
	ULONG 		Key;
	ULONG		AffinityMask;
	ULONG		ConfigurationDataSize;
	ULONG		IdentifierLength;
	char		*Identifier;
} COMPONENT;

/* internal structure that holds pathname parsing data */
struct cfgdata {
	char *name;			/* full name */
	int minlen;			/* minimum length to match */
	CONFIGTYPE type;		/* type of token */
};

/* System ID */
typedef struct systemid {
	CHAR VendorId[8];
	CHAR ProductId[8];
} SYSTEMID;

/* memory query functions */
typedef enum memorytype {
	ExceptionBlock,
	SPBPage,			/* ARCS == SystemParameterBlock */
#ifndef	_NT_PROM
	FreeContiguous,
	FreeMemory,
	BadMemory,
	LoadedProgram,
	FirmwareTemporary,
	FirmwarePermanent
#else	/* _NT_PROM */
	FreeMemory,
	BadMemory,
	LoadedProgram,
	FirmwareTemporary,
	FirmwarePermanent,
	FreeContiguous
#endif	/* _NT_PROM */
} MEMORYTYPE;

typedef struct memorydescriptor {
	MEMORYTYPE	Type;
	LONG		BasePage;
	LONG		PageCount;
} MEMORYDESCRIPTOR;

#endif /* _ASM_ARC_HINV_H */
