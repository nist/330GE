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
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * ARC firmware interface.
 *
 * Copyright (C) 1994, 1995, 1996, 1999 Ralf Baechle
 * Copyright (C) 1999 Silicon Graphics, Inc.
 */
#include <linux/init.h>

#include <asm/arc/types.h>
#include <asm/sgialib.h>

LONG __init
ArcGetDirectoryEntry(ULONG FileID, struct linux_vdirent *Buffer,
                     ULONG N, ULONG *Count)
{
	return ARC_CALL4(get_vdirent, FileID, Buffer, N, Count);
}

LONG __init
ArcOpen(CHAR *Path, enum linux_omode OpenMode, ULONG *FileID)
{
	return ARC_CALL3(open, Path, OpenMode, FileID);
}

LONG __init
ArcClose(ULONG FileID)
{
	return ARC_CALL1(close, FileID);
}

LONG __init
ArcRead(ULONG FileID, VOID *Buffer, ULONG N, ULONG *Count)
{
	return ARC_CALL4(read, FileID, Buffer, N, Count);
}

LONG __init
ArcGetReadStatus(ULONG FileID)
{
	return ARC_CALL1(get_rstatus, FileID);
}

LONG __init
ArcWrite(ULONG FileID, PVOID Buffer, ULONG N, PULONG Count)
{
	return ARC_CALL4(write, FileID, Buffer, N, Count);
}

LONG __init
ArcSeek(ULONG FileID, struct linux_bigint *Position, enum linux_seekmode SeekMode)
{
	return ARC_CALL3(seek, FileID, Position, SeekMode);
}

LONG __init
ArcMount(char *name, enum linux_mountops op)
{
	return ARC_CALL2(mount, name, op);
}

LONG __init
ArcGetFileInformation(ULONG FileID, struct linux_finfo *Information)
{
	return ARC_CALL2(get_finfo, FileID, Information);
}

LONG __init ArcSetFileInformation(ULONG FileID, ULONG AttributeFlags,
                                  ULONG AttributeMask)
{
	return ARC_CALL3(set_finfo, FileID, AttributeFlags, AttributeMask);
}
