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
typedef int	FILE;
extern FILE *xmon_stdin, *xmon_stdout;
#define EOF	(-1)
#define stdin	xmon_stdin
#define stdout	xmon_stdout
#define printf	xmon_printf
#define fprintf	xmon_fprintf
#define fputs	xmon_fputs
#define fgets	xmon_fgets
#define putchar	xmon_putchar
#define getchar	xmon_getchar
#define putc	xmon_putc
#define getc	xmon_getc
#define fopen(n, m)	NULL
#define fflush(f)	do {} while (0)
#define fclose(f)	do {} while (0)
extern char *fgets(char *, int, void *);
extern void xmon_printf(const char *, ...);
extern void xmon_fprintf(void *, const char *, ...);
extern void xmon_sprintf(char *, const char *, ...);

#define perror(s)	printf("%s: no files!\n", (s))
