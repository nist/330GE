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
#ifndef _ASM_IA64_FPSWA_H
#define _ASM_IA64_FPSWA_H

/*
 * Floating-point Software Assist
 *
 * Copyright (C) 1999 Intel Corporation.
 * Copyright (C) 1999 Asit Mallick <asit.k.mallick@intel.com>
 * Copyright (C) 1999 Goutham Rao <goutham.rao@intel.com>
 */

typedef struct {
	/* 4 * 128 bits */
	unsigned long fp_lp[4*2];
} fp_state_low_preserved_t;

typedef struct {
	/* 10 * 128 bits */
	unsigned long fp_lv[10 * 2];
} fp_state_low_volatile_t;

typedef	struct {
	/* 16 * 128 bits */
	unsigned long fp_hp[16 * 2];
} fp_state_high_preserved_t;

typedef struct {
	/* 96 * 128 bits */
	unsigned long fp_hv[96 * 2];
} fp_state_high_volatile_t;

/**
 * floating point state to be passed to the FP emulation library by
 * the trap/fault handler
 */
typedef struct {
	unsigned long			bitmask_low64;
	unsigned long			bitmask_high64;
	fp_state_low_preserved_t	*fp_state_low_preserved;
	fp_state_low_volatile_t		*fp_state_low_volatile;
	fp_state_high_preserved_t	*fp_state_high_preserved;
	fp_state_high_volatile_t	*fp_state_high_volatile;
} fp_state_t;

typedef struct {
	unsigned long status;
	unsigned long err0;
	unsigned long err1;
	unsigned long err2;
} fpswa_ret_t;

/**
 * function header for the Floating Point software assist
 * library. This function is invoked by the Floating point software
 * assist trap/fault handler.
 */
typedef fpswa_ret_t (*efi_fpswa_t) (unsigned long trap_type, void *bundle, unsigned long *ipsr,
				    unsigned long *fsr, unsigned long *isr, unsigned long *preds,
				    unsigned long *ifs, fp_state_t *fp_state);

/**
 * This is the FPSWA library interface as defined by EFI.  We need to pass a 
 * pointer to the interface itself on a call to the assist library
 */
typedef struct {
	unsigned int	 revision;
	unsigned int	 reserved;
	efi_fpswa_t	 fpswa;
} fpswa_interface_t;

#endif /* _ASM_IA64_FPSWA_H */
