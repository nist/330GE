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
 * Routines to load into memory and execute stand-along program images using
 * ARCS PROM firmware.
 *
 * Copyright (C) 1996 David S. Miller (dm@engr.sgi.com)
 */
#include <linux/init.h>
#include <asm/sgialib.h>

LONG __init ArcLoad(CHAR *Path, ULONG TopAddr, ULONG *ExecAddr, ULONG *LowAddr)
{
	return ARC_CALL4(load, Path, TopAddr, ExecAddr, LowAddr);
}

LONG __init ArcInvoke(ULONG ExecAddr, ULONG StackAddr, ULONG Argc, CHAR *Argv[],
	CHAR *Envp[])
{
	return ARC_CALL5(invoke, ExecAddr, StackAddr, Argc, Argv, Envp);
}

LONG __init ArcExecute(CHAR *Path, LONG Argc, CHAR *Argv[], CHAR *Envp[])
{
	return ARC_CALL4(exec, Path, Argc, Argv, Envp);
}
