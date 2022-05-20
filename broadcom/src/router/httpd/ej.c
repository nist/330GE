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
 * Tiny Embedded JavaScript parser
 *
 * Copyright 2003, ASUSTek Inc.
 * All Rights Reserved.                
 *                                     
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of ASUSTek Inc.;   
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior      
 * written permission of ASUSTek Inc..                            
 *
 * $Id: ej.c,v 1.1.1.1 2006/04/25 06:09:26 jiahao_jhou Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>

#include <httpd.h>
#include "bcmnvram.h"

static char * get_arg(char *args, char **next);
static void call(char *func, FILE *stream);

/* Look for unquoted character within a string */
static char *
unqstrstr(char *haystack, char *needle)
{
	char *cur;
	int q;

	for (cur = haystack, q = 0;
	     cur < &haystack[strlen(haystack)] && !(!q && !strncmp(needle, cur, strlen(needle)));
	     cur++) {
		if (*cur == '"')
			q ? q-- : q++;
	}
	return (cur < &haystack[strlen(haystack)]) ? cur : NULL;
}

static char *
get_arg(char *args, char **next)
{
	char *arg, *end;

	/* Parse out arg, ... */
	if (!(end = unqstrstr(args, ","))) {
		end = args + strlen(args);
		*next = NULL;
	} else
		*next = end + 1;

	/* Skip whitespace and quotation marks on either end of arg */
	for (arg = args; isspace((int)*arg) || *arg == '"'; arg++);
	for (*end-- = '\0'; isspace((int)*end) || *end == '"'; end--)
		*end = '\0';

	return arg;
}

static void
call(char *func, FILE *stream)
{
	char *args, *end, *next;
	int argc;
	char * argv[16];
	struct ej_handler *handler;

	/* Parse out ( args ) */
	if (!(args = strchr(func, '(')))
		return;
	if (!(end = unqstrstr(func, ")")))
		return;
	*args++ = *end = '\0';

	/* Set up argv list */
	for (argc = 0; argc < 16 && args && *args; argc++, args = next) {
		if (!(argv[argc] = get_arg(args, &next)))
			break;
	}

	/* Call handler */
	for (handler = &ej_handlers[0]; handler->pattern; handler++) {
		if (strncmp(handler->pattern, func, strlen(handler->pattern)) == 0)
			handler->output(0, stream, argc, argv);
	}
}

void
do_ej(char *path, FILE *stream)
{
	FILE *fp;
	int c;
	char pattern[1000], *asp = NULL, *func = NULL, *end = NULL;
	int len = 0;

	if (!(fp = fopen(path, "r")))
		return;

#ifdef WL550gE
        char *keyword = NULL, *name = NULL, *desc = NULL;
        const char *kw_mark1 = "<#", *kw_mark2 = "#>";
        int no_translate = 1;
        int translated_cnt = 0;
        kw_t kw;
        char pattern2[1000];
        char pattern3[1000];
        const char *PID_STR = "<#ZVMODELVZ#>";
        const char *PID_STR_TITLE = "ZVMODELVZ";
//	const char *REAL_PID_STR = "WL-999gE";
	const char *REAL_PID_STR = nvram_safe_get ("productid");
        char *p_PID_STR = NULL;

/*
        // Load dictionary file
        memset (&kw, 0, sizeof (kw));
        if (load_dictionary (nvram_safe_get ("preferred_lang"), &kw))   {
                no_translate = 0;
        }
*/

#ifdef TRANSLATE_ON_FLY
	char *lang;

        // Load dictionary file

        // If the router is restored to default, using browser's language setting to display ALL pages
        if (is_firsttime () && strcmp(nvram_safe_get ("preferred_lang"), "")==0 && Accept_Language[0] != '\0') {
                lang = Accept_Language;		
        } else {
                lang = nvram_safe_get ("preferred_lang");
        }

        if (load_dictionary (lang, &kw))        {
                no_translate = 0;
        }
#endif  //defined TRANSLATE_ON_FLY

#endif

	while ((c = getc(fp)) != EOF) {

		/* Add to pattern space */
		pattern[len++] = c;
		pattern[len] = '\0';
		if (len == (sizeof(pattern) - 1))
			goto release;


		/* Look for <% ... */
		if (!asp && !strncmp(pattern, "<%", len)) {
			if (len == 2)
				asp = pattern + 2;
			continue;
		}

		/* Look for ... %> */
		if (asp) {
			if (unqstrstr(asp, "%>")) {
				for (func = asp; func < &pattern[len]; func = end) {
					/* Skip initial whitespace */
					for (; isspace((int)*func); func++);
					if (!(end = unqstrstr(func, ";")))
						break;
					*end++ = '\0';

					/* Call function */
					call(func, stream);
				}
				asp = NULL;
				len = 0;
			}
			continue;
		}

//#if defined(ASUS_MIMO) && defined(TRANSLATE_ON_FLY)
#ifdef WL550gE
                if (no_translate)       {
                        goto release;
                }

                /* Look for kw_mark1 ... */
                if (!keyword && !strncmp(pattern, kw_mark1, len)) {
                        if (len == 2)
                                keyword = pattern + 2;
                        continue;
                }

                /* Look for ... kw_mark2 */
                if (keyword) {
                        if (strstr(keyword, kw_mark2)) {
                                for (name = keyword; name < &pattern[len]; name = end) {
                                        /* Skip initial whitespace */
                                        for (; isspace((int)*name); name++);
                                        if (!(end = strstr(name, kw_mark2)))
                                                break;
                                        *end++ = '=';           // '#' --> '=', search_desc() need '='
                                        *end++ = '\0';          // '>' --> '\0'

                                        translated_cnt++;

					if (!strncmp(PID_STR_TITLE, name, strlen(PID_STR_TITLE)-1))
					{
						fprintf (stream, "%s", REAL_PID_STR);
						continue;
					}
					else
                                       		desc = search_desc (&kw, name);

/*
					if (desc && (p_PID_STR=strstr(desc, PID_STR)))
					{
						memset(pattern2, 0, sizeof(pattern2));
						memcpy(pattern2, desc, p_PID_STR-desc);
						memcpy(pattern2+(p_PID_STR-desc), REAL_PID_STR, strlen(REAL_PID_STR));
						strcpy(pattern2+(p_PID_STR-desc)+strlen(REAL_PID_STR), p_PID_STR+strlen(PID_STR));
					}
*/

					if (desc)
					{
						while((p_PID_STR=strstr(desc, PID_STR)))
						{
							memset(pattern2, 0, sizeof(pattern2));
							memcpy(pattern2, desc, p_PID_STR-desc);
							memcpy(pattern2+(p_PID_STR-desc), REAL_PID_STR, strlen(REAL_PID_STR));
							strcpy(pattern2+(p_PID_STR-desc)+strlen(REAL_PID_STR), p_PID_STR+strlen(PID_STR));

							memset(pattern3, 0, sizeof(pattern3));
							strcpy(pattern3, pattern2);
							desc=pattern3;
						}
					}

                                        if (desc != NULL)       {
                                        	if (p_PID_STR)
                                        		fprintf (stream, "%s", pattern2);
                                        	else
                                                	fprintf (stream, "%s", desc);
                                        }
#if 0
                                        else {
                                                // Emphasize unknown keyword
                                                fprintf (stream, "<FONT SIZE=\"7\" COLOR=\"#FF0000\">%s</FONT>", name);
                                        }
#endif // #if 0
                                }
                                keyword = NULL;
                                len = 0;
                        }
                        continue;
                } /* if (keyword) */
#endif  // defined(ASUS_MIMO) && defined(TRANSLATE_ON_FLY)

	release:
		/* Release pattern space */
		fputs(pattern, stream);
		len = 0;
	}

//#if defined(ASUS_MIMO) && defined (TRANSLATE_ON_FLY)
#ifdef WL550gE
        if (no_translate == 0)  {
                release_dictionary (&kw);
        }
#endif  // defined(ASUS_MIMO) && defined (TRANSLATE_ON_FLY)

	fclose(fp);
}

int
ejArgs(int argc, char **argv, char *fmt, ...)
{
	va_list	ap;
	int arg;
	char *c;

	if (!argv)
		return 0;

	va_start(ap, fmt);
	for (arg = 0, c = fmt; c && *c && arg < argc;) {
		if (*c++ != '%')
			continue;
		switch (*c) {
		case 'd':
			*(va_arg(ap, int *)) = atoi(argv[arg]);
			break;
		case 's':
			*(va_arg(ap, char **)) = argv[arg];
			break;
		}
		arg++;
	}
	va_end(ap);

	return arg;
}
