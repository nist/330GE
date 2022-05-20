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
 * Write-once support for otp.
 *
 * Copyright 2006, ASUSTek Inc.
 * All Rights Reserved.                
 *                                     
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of ASUSTek Inc.;   
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior      
 * written permission of ASUSTek Inc..                            
 *
 * $Id: bcmotp.c,v 1.1.1.1 2007/01/25 12:52:12 jiahao_jhou Exp $
 */

#include <typedefs.h>
#include <bcmdefs.h>
#include <osl.h>
#include <bcmutils.h>
#include <sbutils.h>
#include <bcmendian.h>
#include <sbconfig.h>
#include <sbchipc.h>
#include <bcmotp.h>

#define OTP_MSG(x)

typedef struct _otpinfo {
	sb_t	*sbh;		/* Saved sb handle */
	uint	ccrev;		/* chipc revision */
	uint	size;		/* Size of otp in bytes */
	uint	hwprot;		/* Hardware protection bits */
	uint	signvalid;	/* Signature valid bits */
	int	boundary;	/* hw/sw boundary */
} otpinfo_t;

static otpinfo_t otpinfo;

static uint16 otproff(void *oh, chipcregs_t *cc, int woff);
#ifdef BCMNVRAMW
static int otp_write_word(void *oh, chipcregs_t *cc, int wn, uint16 data);
#endif /* BCMNVRAMW */

#define OTPP_TRIES	10000000	/* # of tries for OTPP */
#define OTP_TRIES	300		/* # of tries for OTP */

uint16
otpr(void *oh, chipcregs_t *cc, uint wn)
{
	otpinfo_t *oi = (otpinfo_t *)oh;
	osl_t *osh;
	uint16 *ptr;

	ASSERT(wn < ((((otpinfo_t *)oh)->size / 2) + OTP_LIM_OFF));
	ASSERT(cc);

	osh = sb_osh(oi->sbh);

	ptr = (uint16 *)((uchar *)cc + CC_OTP);
	return (R_REG(osh, &ptr[wn]));
}

static uint16
otproff(void *oh, chipcregs_t *cc, int woff)
{
	otpinfo_t *oi = (otpinfo_t *)oh;
	osl_t *osh;
	uint16 *ptr;

	ASSERT(woff >= (-((int)oi->size / 2)));
	ASSERT(woff < OTP_LIM_OFF);
	ASSERT(cc);

	osh = sb_osh(oi->sbh);

	ptr = (uint16 *)((uchar *)cc + CC_OTP);

	return (R_REG(osh, &ptr[(oi->size / 2) + woff]));
}

void *
otp_init(sb_t *sbh)
{
	uint idx;
	chipcregs_t *cc;
	otpinfo_t *oi;
	uint32 cap = 0;
	void *ret = NULL;
	osl_t *osh;

	oi = &otpinfo;
	bzero(oi, sizeof(otpinfo_t));

	idx = sb_coreidx(sbh);

	oi->sbh = sbh;
	osh = sb_osh(oi->sbh);

	/* Check for otp */
	if ((cc = sb_setcore(sbh, SB_CC, 0)) != NULL) {
		cap = R_REG(osh, &cc->capabilities);
		if ((cap & CC_CAP_OTPSIZE) == 0) {
			/* Nothing there */
			goto out;
		}

		oi->sbh = sbh;
		oi->ccrev = sb_chipcrev(sbh);

		/* As of right now, support only 4320a2 and 4311a1 */
		if ((oi->ccrev != 12) && (oi->ccrev != 17)) {
			goto out;
		}

		oi->size = 1 << (((cap & CC_CAP_OTPSIZE) >> CC_CAP_OTPSIZE_SHIFT)
			+ CC_CAP_OTPSIZE_BASE);
		oi->hwprot = (int)(R_REG(osh, &cc->otpstatus) & OTPS_PROTECT);
		oi->boundary = -1;

		if (oi->ccrev != 17) {
			if (otproff(oi, cc, OTP_HWSIGN_OFF) == OTP_SIGNATURE) {
				oi->signvalid |= OTP_HW_REGION;
				oi->boundary = otproff(oi, cc, OTP_BOUNDARY_OFF);
			}

			if (otproff(oi, cc, OTP_SWSIGN_OFF) == OTP_SIGNATURE)
				oi->signvalid |= OTP_SW_REGION;

			if (otproff(oi, cc, OTP_CIDSIGN_OFF) == OTP_SIGNATURE)
				oi->signvalid |= OTP_CID_REGION;
		}

		ret = (void *)oi;
	}

out:	/* All done */
	sb_setcoreidx(sbh, idx);

	return ret;
}

int
otp_status(void *oh)
{
	otpinfo_t *oi = (otpinfo_t *)oh;
	return ((int)(oi->hwprot | oi->signvalid));
}

int
otp_size(void *oh)
{
	otpinfo_t *oi = (otpinfo_t *)oh;
	return ((int)(oi->size));
}

#ifdef BCMNVRAMW

static int
otp_write_word(void *oh, chipcregs_t *cc, int wn, uint16 data)
{
	otpinfo_t *oi = (otpinfo_t *)oh;
	uint base, row, col, bit, i, j, k;
	uint32 pwait, init_pwait, otpc, otpp, pst, st;

#ifdef	OTP_FORCEFAIL
	OTP_MSG(("%s: [0x%x] = 0x%x\n", __FUNCTION__, wn * 2, data));
#endif /* OTP_FORCEFAIL */

	/* This is bit-at-a-time writing, future cores may do word-at-a-time */
	base = (wn * 16) + (wn / 4);
	if (oi->ccrev == 12) {
		otpc = 0x20000001;
		init_pwait = 0x00000200;
	} else {
		otpc = 0x20000000;
		init_pwait = 0x00004000;
	}
	for (i = 0; i < 16; i++) {
		pwait = init_pwait;
		bit = data & 1;
		row = (base + i) / 65;
		col = (base + i) % 65;
		otpp = OTPP_START |
			((bit << OTPP_VALUE_SHIFT) & OTPP_VALUE) |
			((row << OTPP_ROW_SHIFT) & OTPP_ROW_MASK) |
			(col & OTPP_COL_MASK);
		OTP_MSG(("row %d, col %d, val %d, otpc 0x%x, otpp 0x%x\n", row, col, bit,
		         otpc, otpp));
		j = 0;
		while (1) {
			j++;
			OTP_MSG(("  %d: pwait %d\n", j, (pwait >> 8)));
			W_REG(osh, &cc->otpcontrol, otpc | pwait);
			W_REG(osh, &cc->otpprog, otpp);
			pst = R_REG(osh, &cc->otpprog);
			for (k = 0; ((pst & OTPP_BUSY) == OTPP_BUSY) && (k < OTPP_TRIES); k++)
				pst = R_REG(osh, &cc->otpprog);
			if (k >= OTPP_TRIES) {
				OTP_MSG(("BUSY stuck: pst=0x%x, count=%d\n", pst, k));
				st = OTPS_PROGFAIL;
				break;
			}
			st = R_REG(osh, &cc->otpstatus);
			if (((st & OTPS_PROGFAIL) == 0) || (pwait == OTPC_PROGWAIT)) {
				break;
			} else {
				if ((oi->ccrev == 12) && (pwait >= 0x1000))
					pwait = (pwait << 3) & OTPC_PROGWAIT;
				else
					pwait = (pwait << 1) & OTPC_PROGWAIT;
				if (pwait == 0)
					pwait = OTPC_PROGWAIT;
			}
		}
		if (st & OTPS_PROGFAIL) {
			OTP_MSG(("After %d tries: otpc = 0x%x, otpp = 0x%x/0x%x, otps = 0x%x\n",
			       j, otpc | pwait, otpp, pst, st));
			OTP_MSG(("otp prog failed. wn=%d, bit=%d, ppret=%d, ret=%d\n",
			       wn, i, k, j));
			return 1;
		}
		data >>= 1;
	}
	return 0;
}

/* expects the caller to disable interrupts before calling this routine */
int
otp_write_region(void *oh, int region, uint16 *data, uint wlen)
{
	otpinfo_t *oi = (otpinfo_t *)oh;
	uint32 st;
	uint wn, base = 0, lim;
	int ret;
	uint idx;
	chipcregs_t *cc;


	idx = sb_coreidx(oi->sbh);
	cc = sb_setcore(oi->sbh, SB_CC, 0);

	/* Run bist on chipc to get any unprogrammed bits into a known state */
	if (sb_corebist(oi->sbh) == 0)
		OTP_MSG(("%s: bist passed, otp is blank\n", __FUNCTION__));

	if (oi->ccrev != 17) {
		/* Check valid region */
		if ((region != OTP_HW_REGION) &&
		    (region != OTP_SW_REGION) &&
		    (region != OTP_CID_REGION)) {
			ret = -2;
			goto out;
		}

		/* Region already written? */
		st = oi->hwprot | oi-> signvalid;
		if ((st & region) != 0) {
			ret = -3;
			goto out;
		}

		/* HW and CID have to be written before SW */
		if ((st & OTP_SW_REGION) != 0) {
			ret = -4;
			goto out;
		}

		/* Bounds for the region */
		lim = (oi->size / 2) + OTP_SWLIM_OFF;
		if (region == OTP_HW_REGION) {
			base = 0;
		} else if (region == OTP_SW_REGION) {
			base = oi->boundary / 2;
		} else if (region == OTP_CID_REGION) {
			base = (oi->size / 2) + OTP_CID_OFF;
			lim = (oi->size / 2) + OTP_LIM_OFF;
		}
	} else {
		base = 0;
		lim = oi->size / 4;
	}
	if (wlen > (lim - base)) {
		ret = -5;
		goto out;
	}
	lim = base + wlen;


	/* Write the data */
	ret = -7;
	for (wn = base; wn < lim; wn++)
		if (oi->ccrev == 17) {
			uint werrs, rwn;

			rwn = 4 * wn;
			werrs = (otp_write_word(oh, cc, rwn++, *data) != 0) ? 1 : 0;
			werrs += (otp_write_word(oh, cc, rwn++, *data) != 0) ? 1 : 0;
			werrs += (otp_write_word(oh, cc, rwn, *data++) != 0) ? 1 : 0;
			if (werrs > 2)
				goto out;
		} else
			if (otp_write_word(oh, cc, wn, *data++) != 0)
				goto out;

	if (oi->ccrev != 17) {
		/* Done with the data, write the signature & boundary if needed */
		if (region == OTP_HW_REGION) {
			ret = -8;
			if (otp_write_word(oh, cc, (oi->size / 2) + OTP_BOUNDARY_OFF,
			                   lim * 2) != 0)
				goto out;
			ret = -9;
			if (otp_write_word(oh, cc, (oi->size / 2) + OTP_HWSIGN_OFF,
			                   OTP_SIGNATURE) != 0)
				goto out;
			oi->boundary = lim * 2;
			oi->signvalid |= OTP_HW_REGION;
		} else if (region == OTP_SW_REGION) {
			ret = -10;
			if (otp_write_word(oh, cc, (oi->size / 2) + OTP_SWSIGN_OFF,
			                   OTP_SIGNATURE) != 0)
				goto out;
			oi->signvalid |= OTP_SW_REGION;
		} else if (region == OTP_CID_REGION) {
			ret = -11;
			if (otp_write_word(oh, cc, (oi->size / 2) + OTP_CIDSIGN_OFF,
			                   OTP_SIGNATURE) != 0)
				goto out;
			oi->signvalid |= OTP_CID_REGION;
		}
	}
	ret = 0;
out:
	OTP_MSG(("bits written: %d, average (%d/%d): %d, max retry: %d, pp max: %d\n",
		st_n, st_s, st_n, st_n?(st_s / st_n):0, st_hwm, pp_hwm));

	sb_setcoreidx(oi->sbh, idx);

	return ret;
}

/* expects the caller to disable interrupts before calling this routine */
int
otp_nvwrite(void *oh, uint16 *data, uint wlen)
{
	otpinfo_t *oi = (otpinfo_t *)oh;
	uint32 st;
	uint16 crc, clen, *p, hdr[2];
	uint wn, base = 0, lim;
	int err, gerr = 0;
	uint idx;
	chipcregs_t *cc;


	/* Run bist on chipc to get any unprogrammed bits into a known state */
	if (sb_corebist(oi->sbh) == 0)
		OTP_MSG(("%s: bist passed, otp is blank\n", __FUNCTION__));

	/* otp already written? */
	st = oi->hwprot | oi-> signvalid;
	if ((st & (OTP_HW_REGION | OTP_SW_REGION)) == (OTP_HW_REGION | OTP_SW_REGION))
		return BCME_EPERM;

	/* save the orig core */
	idx = sb_coreidx(oi->sbh);
	cc = sb_setcore(oi->sbh, SB_CC, 0);

	/* Bounds for the region */
	lim = (oi->size / 2) + OTP_SWLIM_OFF;
	base = 0;

	/* Look for possible chunks from the end down */
	wn = lim;
	while (wn > 0) {
		wn--;
		if (otpr(oh, cc, wn) == OTP_MAGIC) {
			base = wn + (otpr(oh, cc, wn + 1) / 2);
			break;
		}
	}
	if (base == 0) {
		OTP_MSG(("Unprogrammed otp\n"));
	} else {
		OTP_MSG(("Found some chunks, skipping to 0x%x\n", base * 2));
	}
	if ((wlen + 3) > (lim - base)) {
		err =  BCME_NORESOURCE;
		goto out;
	}


	/* Prepare the header and crc */
	hdr[0] = OTP_MAGIC;
	hdr[1] = (wlen + 3) * 2;
	crc = hndcrc16((uint8 *)hdr, sizeof(hdr), CRC16_INIT_VALUE);
	crc = hndcrc16((uint8 *)data, wlen * 2, crc);
	crc = ~crc;

	do {
		p = data;
		wn = base + 2;
		lim = base + wlen + 2;

		OTP_MSG(("writing chunk, 0x%x bytes @ 0x%x-0x%x\n", wlen * 2,
		         base * 2, (lim + 1) * 2));

		/* Write the header */
		err = otp_write_word(oh, cc, base, hdr[0]);

		/* Write the data */
		while (wn < lim) {
			err += otp_write_word(oh, cc, wn++, *p++);

			/* If there has been an error, close this chunk */
			if (err != 0) {
				OTP_MSG(("closing early @ 0x%x\n", wn * 2));
				break;
			}
		}

		/* If we wrote the whole chunk, write the crc */
		if (wn == lim) {
			OTP_MSG(("  whole chunk written, crc = 0x%x\n", crc));
			err += otp_write_word(oh, cc, wn++, crc);
			clen = hdr[1];
		} else {
			/* If there was an error adjust the count to point to
			 * the word after the error so we can start the next
			 * chunk there.
			 */
			clen = (wn - base) * 2;
			OTP_MSG(("  partial chunk written, chunk len = 0x%x\n", clen));
		}
		/* And now write the chunk length */
		err += otp_write_word(oh, cc, base + 1, clen);

		if (base == 0) {
			/* Write the signature and boundary if this is the HW region,
			 * but don't report failure if either of these 2 writes fail.
			 */
			if (otp_write_word(oh, cc, (oi->size / 2) + OTP_BOUNDARY_OFF, wn * 2) == 0)
				gerr += otp_write_word(oh, cc, (oi->size / 2) + OTP_HWSIGN_OFF,
				                       OTP_SIGNATURE);
			else
				gerr++;
			oi->boundary = wn * 2;
			oi->signvalid |= OTP_HW_REGION;
		}

		if (err != 0) {
			gerr += err;
			/* Errors, do it all over again if there is space left */
			if ((wlen + 3) <= ((oi->size / 2) + OTP_SWLIM_OFF - wn)) {
				base = wn;
				lim = base + wlen + 2;
				OTP_MSG(("Programming errors, retry @ 0x%x\n", wn * 2));
			} else {
				OTP_MSG(("Programming errors, no space left ( 0x%x)\n", wn * 2));
				break;
			}
		}
	} while (err != 0);

	OTP_MSG(("bits written: %d, average (%d/%d): %d, max retry: %d, pp max: %d\n",
	       st_n, st_s, st_n, st_s / st_n, st_hwm, pp_hwm));

	if (gerr != 0)
		OTP_MSG(("programming %s after %d errors\n", (err == 0) ? "succedded" : "failed",
		         gerr));
out:
	/* done */
	sb_setcoreidx(oi->sbh, idx);

	if (err)
		return BCME_ERROR;
	else
		return 0;
}
#endif /* BCMNVRAMW */

#if	defined(WLTEST)
static uint16
read_otp_bit(void *oh, chipcregs_t *cc, uint idx)
{
	uint k, row, col;
	uint32 otpp, st;
	osl_t *osh;
	otpinfo_t *oi = (otpinfo_t *)oh;

	osh = sb_osh(oi->sbh);
	row = idx / 65;
	col = idx % 65;

	otpp = OTPP_START | OTPP_READ |
	        ((row << OTPP_ROW_SHIFT) & OTPP_ROW_MASK) |
	        (col & OTPP_COL_MASK);

	OTP_MSG(("%s: idx = %d, row = %d, col = %d, otpp = 0x%x", __FUNCTION__,
	         idx, row, col, otpp));

	W_REG(osh, &cc->otpprog, otpp);
	st = R_REG(osh, &cc->otpprog);
	for (k = 0; ((st & OTPP_BUSY) == OTPP_BUSY) && (k < OTPP_TRIES); k++)
		st = R_REG(osh, &cc->otpprog);

	if (k >= OTPP_TRIES) {
		OTP_MSG(("\n%s: BUSY stuck: st=0x%x, count=%d\n", __FUNCTION__, st, k));
		return 0xffff;
	}
	if (st & OTPP_READERR) {
		OTP_MSG(("\n%s: Could not read OTP bit %d\n", __FUNCTION__, idx));
		return 0xffff;
	}
	st = (st & OTPP_VALUE) >> OTPP_VALUE_SHIFT;
	OTP_MSG((" => %d\n", st));
	return (uint16)st;
}

static uint16
otpr16(void *oh, chipcregs_t *cc, uint wn)
{
	uint base, i;
	uint16 val, bit;

	base = (wn * 16) + (wn / 4);
	val = 0;
	for (i = 0; i < 16; i++) {
		if ((bit = read_otp_bit(oh, cc, base + i)) == 0xffff)
			break;
		val = val | (bit << i);
	}
	if (i < 16)
		val = 0xaaaa;
	return val;
}

int
otp_dump(void *oh, int arg, char *buf, uint size)
{
	otpinfo_t *oi = (otpinfo_t *)oh;
	chipcregs_t *cc;
	uint idx, i, count, lil;
	uint16 val;
	struct bcmstrbuf b;

	idx = sb_coreidx(oi->sbh);
	cc = sb_setcore(oi->sbh, SB_CC, 0);

	if (arg >= 16) {
		arg -= 16;
	} else {
		/* Run bist on chipc to get any unprogrammed bits into a known state */
		if (sb_corebist(oi->sbh) == 0)
			OTP_MSG(("%s: bist passed, otp is blank\n", __FUNCTION__));
	}

	if (arg == 2) {
		count = 66 * 4;
		lil = 3;
	} else {
		count = (oi->size / 2) + OTP_LIM_OFF;
		lil = 7;
	}

	OTP_MSG(("%s: arg %d, size %d, words %d\n", __FUNCTION__, arg, size, count));
	bcm_binit(&b, buf, size);
	for (i = 0; i < count; i++) {
		if ((i & lil) == 0)
			bcm_bprintf(&b, "0x%04x:", 2 * i);

		if (arg == 0)
			val = otpr(oh, cc, i);
		else
			val = otpr16(oh, cc, i);
		bcm_bprintf(&b, " 0x%04x", val);
		if ((i & lil) == lil) {
			if (arg == 2) {
				bcm_bprintf(&b, " %d\n",
				              read_otp_bit(oh, cc, ((i / 4) * 65) + 64) & 1);
			} else {
				bcm_bprintf(&b, "\n");
			}
		}
	}
	if ((i & lil) != lil)
		bcm_bprintf(&b, "\n");

	OTP_MSG(("%s: returning %d, left %d, wn %d\n",
		__FUNCTION__, (int)(b.buf - b.origbuf), b.size, i));

	sb_setcoreidx(oi->sbh, idx);

	return ((int)(b.buf - b.origbuf));
}
#endif	
