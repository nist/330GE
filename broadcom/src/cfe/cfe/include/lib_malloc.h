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
/*  *********************************************************************
    *  Broadcom Common Firmware Environment (CFE)
    *  
    *  Local memory manager			File: mempool.h
    *  
    *  This routine is used to manage memory allocated within the 
    *  firmware.  You give it a chunk of memory to manage, and then
    *  these routines manage suballocations from there.
    *  
    *  Author:  Mitch Lichtenberg (mpl@broadcom.com)
    *  
    *********************************************************************  
    *
    *  Copyright 2000,2001,2002,2003
    *  ASUSTek Inc.. All rights reserved.
    *  
    *  This software is furnished under license and may be used and 
    *  copied only in accordance with the following terms and 
    *  conditions.  Subject to these conditions, you may download, 
    *  copy, install, use, modify and distribute modified or unmodified 
    *  copies of this software in source and/or binary form.  No title 
    *  or ownership is transferred hereby.
    *  
    *  1) Any source code used, modified or distributed must reproduce 
    *     and retain this copyright notice and list of conditions 
    *     as they appear in the source file.
    *  
    *  2) No right is granted to use any trade name, trademark, or 
    *     logo of ASUSTek Inc..  The "Broadcom Corporation" 
    *     name may not be used to endorse or promote products derived 
    *     from this software without the prior written permission of 
    *     ASUSTek Inc..
    *  
    *  3) THIS SOFTWARE IS PROVIDED "AS-IS" AND ANY EXPRESS OR
    *     IMPLIED WARRANTIES, INCLUDING BUT NOT LIMITED TO, ANY IMPLIED
    *     WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR 
    *     PURPOSE, OR NON-INFRINGEMENT ARE DISCLAIMED. IN NO EVENT 
    *     SHALL BROADCOM BE LIABLE FOR ANY DAMAGES WHATSOEVER, AND IN 
    *     PARTICULAR, BROADCOM SHALL NOT BE LIABLE FOR DIRECT, INDIRECT,
    *     INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
    *     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
    *     GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
    *     BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY 
    *     OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR 
    *     TORT (INCLUDING NEGLIGENCE OR OTHERWISE), EVEN IF ADVISED OF 
    *     THE POSSIBILITY OF SUCH DAMAGE.
    ********************************************************************* */

#ifndef _LIB_MALLOC_H
#define _LIB_MALLOC_H

typedef struct memstats_s {
    int mem_totalbytes;
    int mem_allocbytes;
    int mem_freebytes;
    int mem_allocnodes;
    int mem_freenodes;
    int mem_largest;
} memstats_t;

typedef struct mempool_s mempool_t;
void kmeminit(mempool_t *pool,unsigned char *buffer,int length);
void kfree(mempool_t *pool,void *ptr);
void *kmalloc(mempool_t *pool,unsigned int size,unsigned int align);
int kmemchk(mempool_t *pool,int verbose);
extern mempool_t kmempool;
void *kmempoolbase(mempool_t *pool);
int kmempoolsize(mempool_t *pool);
int kmemstats(mempool_t *pool,memstats_t *stats);

#define KMEMINIT(buffer,length) kmeminit(&kmempool,(buffer),(length))
#define KMEMPOOLBASE() kmempoolbase(&kmempool)
#define KMEMPOOLSIZE() kmempoolsize(&kmempool)
#define KMALLOC(size,align) kmalloc(&kmempool,(size),(align))
#define KFREE(ptr) kfree(&kmempool,(ptr))
#define KMEMSTATS(s) kmemstats(&kmempool,(s))

#endif
