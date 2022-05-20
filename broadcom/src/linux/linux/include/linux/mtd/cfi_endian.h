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
 * $Id: cfi_endian.h,v 1.1.1.1 2007/01/25 12:52:07 jiahao_jhou Exp $
 *
 */

#include <asm/byteorder.h>

#ifndef CONFIG_MTD_CFI_ADV_OPTIONS

#define CFI_HOST_ENDIAN

#else

#ifdef CONFIG_MTD_CFI_NOSWAP
#define CFI_HOST_ENDIAN
#endif

#ifdef CONFIG_MTD_CFI_LE_BYTE_SWAP
#define CFI_LITTLE_ENDIAN
#endif

#ifdef CONFIG_MTD_CFI_BE_BYTE_SWAP
#define CFI_BIG_ENDIAN
#endif

#endif

#if defined(CFI_LITTLE_ENDIAN)
#define cpu_to_cfi8(x) (x)
#define cfi8_to_cpu(x) (x)
#define cpu_to_cfi16(x) cpu_to_le16(x)
#define cpu_to_cfi32(x) cpu_to_le32(x)
#define cfi16_to_cpu(x) le16_to_cpu(x)
#define cfi32_to_cpu(x) le32_to_cpu(x)
#elif defined(CFI_BIG_ENDIAN)
#define cpu_to_cfi8(x) (x)
#define cfi8_to_cpu(x) (x)
#define cpu_to_cfi16(x) cpu_to_be16(x)
#define cpu_to_cfi32(x) cpu_to_be32(x)
#define cfi16_to_cpu(x) be16_to_cpu(x)
#define cfi32_to_cpu(x) be32_to_cpu(x)
#elif defined(CFI_HOST_ENDIAN)
#define cpu_to_cfi8(x) (x)
#define cfi8_to_cpu(x) (x)
#define cpu_to_cfi16(x) (x)
#define cpu_to_cfi32(x) (x)
#define cfi16_to_cpu(x) (x)
#define cfi32_to_cpu(x) (x)
#else
#error No CFI endianness defined
#endif
