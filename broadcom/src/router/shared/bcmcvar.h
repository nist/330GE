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
 * Broadcom Web Page Configuration Variables
 *
 * Copyright 2005, ASUSTek Inc.
 * All Rights Reserved.
 * 
 * THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 * $Id: bcmcvar.h,v 1.1.1.1 2007/01/25 12:52:22 jiahao_jhou Exp $
 */

#ifndef _bcmcvar_h
#define _bcmcvar_h

#define POST_BUF_SIZE	10000
#define WEBS_BUF_SIZE	5000
#define MAX_STA_COUNT	256
#define NVRAM_BUFSIZE	100

#define websBufferInit(wp) {webs_buf = malloc(WEBS_BUF_SIZE); webs_buf_offset = 0;}
#define websBufferWrite(wp, fmt, args...) {webs_buf_offset += sprintf(webs_buf+webs_buf_offset, fmt, ## args);} 
#define websBufferFlush(wp) {webs_buf[webs_buf_offset] = '\0'; fprintf(wp, webs_buf); fflush(wp); free(webs_buf); webs_buf = NULL;}

#define ARGV(args...) ((char *[]) { args, NULL })
#define XSTR(s) STR(s)
#define STR(s) #s

struct variable {
	char *name;
	char *longname;
	char *prefix;
	void (*validate)(webs_t wp, char *value, struct variable *v, char *);
	char **argv;
	int nullok;
	int ezc_flags;
};

int variables_arraysize(void);

/* NVRAM macros and defines */

/* Uplad constraints for NVRAM file*/
struct UPLOAD_CONSTRAINTS {
				char name[32];			/*name of constraint */
				char *(*get)(const char *);	/*get method for the constraint*/
				char altval[32];		/*if primary match fail use this subject to modifiers below*/  
				int  flags;			/*match modifier flags */
			  };
			  
typedef struct UPLOAD_CONSTRAINTS upload_constraints;

/* Upload constraint modifier flags */
#define	NVRAM_CONS_INT_GT		0x01	/*Convert to integer and match if filevar > imagevar */
#define NVRAM_CONS_INT_LT		0x02	/*Convert to integer and match if filevar < imagevar */
#define NVRAM_CONS_OPTIONAL 		0x04	/*Match if present in file*/
#define NVRAM_CONS_ALT_MATCH		0x08	/*Do an exact match if altvar is specified ignored otherwise*/
#define NVRAM_CONS_AND_MORE		0x10	/*There are more AND match criteria to follow for the given var*/
#define NVRAM_CONS_OR_MORE		0x20	/*There are more OR match criteria to follow for the given var*/
#define NVRAM_CONS_PARTIAL_MATCH	0x40	/*Altval contains the partial patch string */

/* default initializer for constraint variables */
#define NULL_STR		{'\0'}

#define NVRAM_CONSTRAINT_VARS	{ 	{ "boardtype",	nvram_get,	NULL_STR,	0				},\
					{ "boardrev" ,	nvram_get,	NULL_STR,	0				},\
					{ "boardflags",	nvram_get,	NULL_STR,	0				},\
					{ "os_name" ,	nvram_get,	NULL_STR,	0				},\
					{ "os_version",	osversion_get,	"INTERNAL",	NVRAM_CONS_PARTIAL_MATCH	},\
					{ NULL_STR,	NULL,		NULL_STR,	0				}\
				}
/*
 * NVRAM Validation control flags. 
 * Also directs handling of the hodge podge of 
 * NVRAM variable formats
 *
*/
#define NVRAM_ENCRYPT	 	0x10000000 /* Encrypt variable prior to saving NVRAM to file */ 
#define NVRAM_MP		0x20000000 /* NVRAM Variable is both single and multi instance eg wanXX_unit */
#define NVRAM_WL_MULTI		0x40000000 /* Multi instance wireless variable wlXX_name*/
#define NVRAM_WAN_MULTI		0x80000000 /* Multi instance wan variable wanXX_name */
#define NVRAM__IGNORE		0x01000000 /* Don't save or restore to NVRAM */
#define NVRAM_VLAN_MULTI	0x02000000 /* Multi instance VLAN variable vlanXXname */
#define NVRAM_GENERIC_MULTI	0x04000000 /* Port forward multi instance variables nameXX */
#define NVRAM_IGNORE		0x08000000 /* Skip NVRAM processing ie no save to file or validate */
#define NVRAM_MI		0x00100000 /* Multi instance NVRAM variable */
#define WEB_IGNORE		0x00200000 /* Ignore during web validation */

/* Markers for HTTP post buffer. Assumes HTTP multipart type encoding */
#define NVRAM_BEGIN_WEBFILE		"nvfile"
#define NVRAM_END_WEBFILE		'\0'

#define NVRAM_MAX_NETIF		8	/* Maximum number of suppoters NVRAM interfaces (wan,vlan,wl) of each type */
#define NVRAM_MAX_STRINGSIZE	256
#define NVRAM_MAX_LINES		2*NVRAM_SPACE/NVRAM_MAX_STRINGSIZE

#define NVRAM_CHECKSUM_FILLER	"NVRAMTemporaryChecksumFiller"
#define NVRAM_CHECKSUM_LINENUM	1 	/* Line number where checksum is located starting from zero */

#define NVRAM_PASSPHRASE	{0xdb,0xca,0xfe,0xde,0xbb,0xde,0xad,0xbe,0xef,0x00}

#define NVRAM_ENCTAG		':'

#define NVRAM_SALTSIZE		8
#define NVRAM_HASHSIZE		20
#define NVRAM_FILECHKSUM_SIZE	NVRAM_SALTSIZE + NVRAM_HASHSIZE
#define NVRAM_SHA1BUFSIZE	80
#define NVRAM_FILEKEYSIZE	32


/*To expand the header, 
*	1)Make entry in the NVRAM_FILEHEADER define
*  	2)Add line reference below if needed
*/
#define NVRAM_FILEHEADER	{	"NumVariables",	"Checksum" ,	\
					NULL_STR				\
				}
				
/*Line references to each of the header elements */
#define NVRAM_LINECOUNT_LINENUM	0
#define NVRAM_CHECKSUM_LINENUM	1 				

/*  Number of lines the header occupies. */
#define NVRAM_HEADER_LINECOUNT(hdr)	( sizeof( (hdr) ) / sizeof ( (hdr)[0] ) - 1 )				

#endif /* _bcmcvar_h */