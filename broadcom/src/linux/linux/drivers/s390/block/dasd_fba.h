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

#ifndef DASD_FBA_H
#define DASD_FBA_H

typedef
    struct DE_fba_data_t {
	struct {
		unsigned char perm:2;	/* Permissions on this extent */
		unsigned char zero:2;	/* Must be zero */
		unsigned char da:1;	/* usually zero */
		unsigned char diag:1;	/* allow diagnose */
		unsigned char zero2:2;	/* zero */
	} __attribute__ ((packed)) mask;
	__u8 zero;		/* Must be zero */
	__u16 blk_size;		/* Blocksize */
	__u32 ext_loc;		/* Extent locator */
	__u32 ext_beg;		/* logical number of block 0 in extent */
	__u32 ext_end;		/* logocal number of last block in extent */
} __attribute__ ((packed))

    DE_fba_data_t;

typedef
    struct LO_fba_data_t {
	struct {
		unsigned char zero:4;
		unsigned char cmd:4;
	} __attribute__ ((packed)) operation;
	__u8 auxiliary;
	__u16 blk_ct;
	__u32 blk_nr;
} __attribute__ ((packed))

    LO_fba_data_t;

typedef
    struct dasd_fba_characteristics_t {
	union {
		__u8 c;
		struct {
			unsigned char reserved:1;
			unsigned char overrunnable:1;
			unsigned char burst_byte:1;
			unsigned char data_chain:1;
			unsigned char zeros:4;
		} __attribute__ ((packed)) bits;
	} __attribute__ ((packed)) mode;
	union {
		__u8 c;
		struct {
			unsigned char zero0:1;
			unsigned char removable:1;
			unsigned char shared:1;
			unsigned char zero1:1;
			unsigned char mam:1;
			unsigned char zeros:3;
		} __attribute__ ((packed)) bits;
	} __attribute__ ((packed)) features;
	__u8 dev_class;
	__u8 unit_type;
	__u16 blk_size;
	__u32 blk_per_cycl;
	__u32 blk_per_bound;
	__u32 blk_bdsa;
	__u32 reserved0;
	__u16 reserved1;
	__u16 blk_ce;
	__u32 reserved2;
	__u16 reserved3;
} __attribute__ ((packed))

    dasd_fba_characteristics_t;

int dasd_fba_init (void);
void dasd_fba_cleanup (void);
#endif				/* DASD_FBA_H */
