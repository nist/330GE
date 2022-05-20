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
 * NVRAM variable manipulation (direct mapped flash)
 *
 * Copyright 2006, ASUSTek Inc.
 * All Rights Reserved.
 * 
 * THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 *
 * $Id: nvram_rw.c,v 1.1.1.1 2007/01/25 12:52:12 jiahao_jhou Exp $
 */

#include <typedefs.h>
#include <bcmdefs.h>
#include <osl.h>
#include <bcmutils.h>
#include <sbutils.h>
#include <mipsinc.h>
#include <bcmnvram.h>
#include <bcmendian.h>
#include <flashutl.h>
#include <sbconfig.h>
#include <sbchipc.h>

struct nvram_tuple * _nvram_realloc(struct nvram_tuple *t, const char *name, const char *value);
void  _nvram_free(struct nvram_tuple *t);
int  _nvram_read(void *buf);

extern char * _nvram_get(const char *name);
extern int _nvram_set(const char *name, const char *value);
extern int _nvram_unset(const char *name);
extern int _nvram_getall(char *buf, int count);
extern int _nvram_commit(struct nvram_header *header);
extern int _nvram_init(void *sb);
extern void _nvram_exit(void);

static struct nvram_header *nvram_header = NULL;
static 	ulong flash_base = 0;

#define NVRAM_LOCK()	do {} while (0)
#define NVRAM_UNLOCK()	do {} while (0)

/* Convenience */
#define KB * 1024
#define MB * 1024 * 1024

char *
BCMINITFN(nvram_get)(const char *name)
{
	char *value;

	NVRAM_LOCK();
	value = _nvram_get(name);
	NVRAM_UNLOCK();

	return value;
}

int
BCMINITFN(nvram_getall)(char *buf, int count)
{
	int ret;

	NVRAM_LOCK();
	ret = _nvram_getall(buf, count);
	NVRAM_UNLOCK();

	return ret;
}

int
BCMINITFN(nvram_set)(const char *name, const char *value)
{
	int ret;

	NVRAM_LOCK();
	ret = _nvram_set(name, value);
	NVRAM_UNLOCK();

	return ret;
}

int
BCMINITFN(nvram_unset)(const char *name)
{
	int ret;

	NVRAM_LOCK();
	ret = _nvram_unset(name);
	NVRAM_UNLOCK();

	return ret;
}

int
BCMINITFN(nvram_resetgpio_init)(void *sb)
{
	char *value;
	int gpio;
	sb_t *sbh;

	sbh = (sb_t *)sb;

	value = nvram_get("reset_gpio");
	if (!value)
		return -1;

	gpio = (int) bcm_atoi(value);
	if (gpio > 7)
		return -1;

	/* Setup GPIO input */
	sb_gpioouten(sbh, ((uint32) 1 << gpio), 0, GPIO_DRV_PRIORITY);

	return gpio;
}

static bool
BCMINITFN(nvram_reset)(sb_t *sbh)
{
	int gpio;
	uint msec;

	if ((gpio = nvram_resetgpio_init((void *)sbh)) < 0)
		return FALSE;

	/* GPIO reset is asserted low */
	for (msec = 0; msec < 5000; msec++) {
		if (sb_gpioin(sbh) & ((uint32) 1 << gpio))
			return FALSE;
		OSL_DELAY(1000);
	}

	return TRUE;
}

extern unsigned char embedded_nvram[];

static struct nvram_header *
BCMINITFN(find_nvram)(bool embonly, bool *isemb)
{
	struct nvram_header *nvh;
	uint32 off, lim;


	if (!embonly) {
		*isemb = FALSE;
		if (flash_base == SB_FLASH1)
			lim = SB_FLASH1_SZ;
		else
			lim = SB_FLASH2_SZ;
		off = FLASH_MIN;
		while (off <= lim) {
			nvh = (struct nvram_header *)KSEG1ADDR(flash_base + off - NVRAM_SPACE);
			if (nvh->magic == NVRAM_MAGIC)
				/* if (nvram_calc_crc(nvh) == (uint8) nvh->crc_ver_init) */{
					return (nvh);
				}
			off <<= 1;
		};
	}

	/* Now check embedded nvram */
	*isemb = TRUE;
	nvh = (struct nvram_header *)KSEG1ADDR(flash_base + (4 * 1024));
	if (nvh->magic == NVRAM_MAGIC)
/*		if (nvram_calc_crc(nvh) == (uint8) nvh->crc_ver_init) */{
			return (nvh);
		}
	nvh = (struct nvram_header *)KSEG1ADDR(flash_base + 1024);
	if (nvh->magic == NVRAM_MAGIC)
/*		if (nvram_calc_crc(nvh) == (uint8) nvh->crc_ver_init) */{
			return (nvh);
		}
#ifdef _CFE_
	nvh = (struct nvram_header *)embedded_nvram;
	if (nvh->magic == NVRAM_MAGIC)
		return (nvh);
#endif
	printf("find_nvram: no nvram found\n");
	return (NULL);
}

int
BCMINITFN(nvram_init)(void *sb)
{
	uint idx;
	bool isemb;
	int ret;
	sb_t *sbh;

	sbh = (sb_t *)sb;


	idx = sb_coreidx(sbh);
	if (sb_setcore(sbh, SB_CC, 0) != NULL) {
		flash_base = SB_FLASH2;
		sb_setcoreidx(sbh, idx);
	} else
		flash_base = SB_FLASH1;

	/* Temporarily initialize with embedded NVRAM */
	nvram_header = find_nvram(TRUE, &isemb);
	ret = _nvram_init(sb);
	if (ret == 0) {
		/* Restore defaults from embedded NVRAM if button held down */
		if (nvram_reset(sbh)) {
			return 1;
		}
		_nvram_exit();
	}

	/* Find NVRAM */
	nvram_header = find_nvram(FALSE, &isemb);
	ret = _nvram_init(sb);
	if (ret == 0) {
		/* Restore defaults if embedded NVRAM used */
		if (nvram_header && isemb) {
			ret = 1;
		}
	}

	return ret;
}

void
BCMINITFN(nvram_exit)(void *sb)
{
	sb_t *sbh;

	sbh = (sb_t *)sb;

	_nvram_exit();
}

int
BCMINITFN(_nvram_read)(void *buf)
{
	uint32 *src, *dst;
	uint i;

	if (!nvram_header)
		return -19; /* -ENODEV */

	src = (uint32 *) nvram_header;
	dst = (uint32 *) buf;

	for (i = 0; i < sizeof(struct nvram_header); i += 4)
		*dst++ = *src++;

	for (; i < nvram_header->len && i < NVRAM_SPACE; i += 4)
		*dst++ = ltoh32(*src++);

	return 0;
}

struct nvram_tuple *
BCMINITFN(_nvram_realloc)(struct nvram_tuple *t, const char *name, const char *value)
{
	if (!(t = MALLOC(NULL, sizeof(struct nvram_tuple) + strlen(name) + 1 +
	                 strlen(value) + 1))) {
		printf("_nvram_realloc: our of memory\n");
		return NULL;
	}

	/* Copy name */
	t->name = (char *) &t[1];
	strcpy(t->name, name);

	/* Copy value */
	t->value = t->name + strlen(name) + 1;
	strcpy(t->value, value);

	return t;
}

void
BCMINITFN(_nvram_free)(struct nvram_tuple *t)
{
	if (t)
		MFREE(NULL, t, sizeof(struct nvram_tuple) + strlen(t->name) + 1 +
		      strlen(t->value) + 1);
}

int
BCMINITFN(nvram_commit)(void)
{
	struct nvram_header *header;
	int ret;
	uint32 *src, *dst;
	uint i;

	if (!(header = (struct nvram_header *) MALLOC(NULL, NVRAM_SPACE))) {
		printf("nvram_commit: out of memory\n");
		return -12; /* -ENOMEM */
	}

	NVRAM_LOCK();

	/* Regenerate NVRAM */
	ret = _nvram_commit(header);
	if (ret)
		goto done;

	src = (uint32 *) &header[1];
	dst = src;

	for (i = sizeof(struct nvram_header); i < header->len && i < NVRAM_SPACE; i += 4)
		*dst++ = htol32(*src++);

#ifdef _CFE_
	if ((ret = cfe_open("flash0.nvram")) >= 0) {
		cfe_writeblk(ret, 0, (unsigned char *) header, header->len);
		cfe_close(ret);
	}
#else
	if (sysFlashInit(NULL) == 0) {
		/* set/write invalid MAGIC # (in case writing image fails/is interrupted)
			 write the NVRAM image to flash(with invalid magic)
			 set/write valid MAGIC #
		*/
		header->magic = NVRAM_CLEAR_MAGIC;
		nvWriteChars((unsigned char *)&header->magic, sizeof(header->magic));

		header->magic = NVRAM_INVALID_MAGIC;
		nvWrite((unsigned short *) header, NVRAM_SPACE);

		header->magic = NVRAM_MAGIC;
		nvWriteChars((unsigned char *)&header->magic, sizeof(header->magic));
	}
#endif /* ifdef _CFE_ */

done:
	NVRAM_UNLOCK();
	MFREE(NULL, header, NVRAM_SPACE);
	return ret;
}