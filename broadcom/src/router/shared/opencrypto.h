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
 * Broadcom Home Gateway Reference Design
 *
 * Copyright 2005, ASUSTek Inc.
 * All Rights Reserved.
 * 
 * THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 * $Id: opencrypto.h,v 1.1.1.1 2007/01/25 12:52:22 jiahao_jhou Exp $
 */

#ifndef _opencrypto_h
#define _opencrypto_h


#define AES_BLOCK_LEN   8


/*  AES-based keywrap function defined in RFC3394 */
int aes_wrap(size_t kl, uint8 *key, size_t il, uint8 *input, uint8 *output);
 
/* AES-based key unwrap function defined in RFC3394 */
int aes_unwrap(size_t kl, uint8 *key, size_t il, uint8 *input, uint8 *output);

/*Pseudo random function */
int fPRF(unsigned char *key, int key_len, unsigned char *prefix,
        int prefix_len, unsigned char *data, int data_len,
        unsigned char *output, int len);

/* hmac-sha1 keyed secure hash algorithm */
void hmac_sha1(unsigned char *text, int text_len, unsigned char *key,
               int key_len, unsigned char *digest);



#endif /*_opencrypto_h */
