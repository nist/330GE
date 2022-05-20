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
 * Copyright (C) 2002 Roman Zippel <zippel@linux-m68k.org>
 * Released under the terms of the GNU GPL v2.0.
 */

#ifndef LKC_H
#define LKC_H

#include "expr.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef LKC_DIRECT_LINK
#define P(name,type,arg)	extern type name arg
#else
#include "lkc_defs.h"
#define P(name,type,arg)	extern type (*name ## _p) arg
#endif
#include "lkc_proto.h"
#undef P

void symbol_end(char *help);
int zconfparse(void);
void zconfdump(FILE *out);

extern int zconfdebug;
void zconf_starthelp(void);
void zconf_initscan(const char *name);
void zconf_nextfile(const char *name);
int zconf_lineno(void);
char *zconf_curname(void);

/* confdata.c */
extern const char conf_def_filename[];
extern char conf_filename[];

char *conf_get_default_confname(void);

/* kconfig_load.c */
void kconfig_load(void);

/* menu.c */
void menu_init(void);
void menu_add_menu(void);
void menu_end_menu(void);
void menu_add_entry(struct symbol *sym);
void menu_end_entry(void);
struct property *create_prop(enum prop_type type);
void menu_add_dep(struct expr *dep);
struct property *menu_add_prop(int token, char *prompt, struct symbol *def, struct expr *dep);
void menu_finalize(struct menu *parent);
void menu_set_type(int type);
struct file *file_lookup(const char *name);
int file_write_dep(const char *name);

extern struct menu *current_entry;
extern struct menu *current_menu;

/* symbol.c */
void sym_init(void);
void sym_clear_all_valid(void);

static inline tristate sym_get_tristate_value(struct symbol *sym)
{
	return S_TRI(sym->curr);
}


static inline struct symbol *sym_get_choice_value(struct symbol *sym)
{
	return (struct symbol *)S_VAL(sym->curr);
}

static inline bool sym_set_choice_value(struct symbol *ch, struct symbol *chval)
{
	return sym_set_tristate_value(chval, yes);
}

static inline bool sym_is_choice(struct symbol *sym)
{
	return sym->flags & SYMBOL_CHOICE ? true : false;
}

static inline bool sym_is_choice_value(struct symbol *sym)
{
	return sym->flags & SYMBOL_CHOICEVAL ? true : false;
}

static inline bool sym_is_optional(struct symbol *sym)
{
	return sym->flags & SYMBOL_OPTIONAL ? true : false;
}

static inline bool sym_has_value(struct symbol *sym)
{
	//return S_VAL(sym->def) != NULL;
	return sym->flags & SYMBOL_NEW ? false : true;
}

#ifdef __cplusplus
}
#endif

#endif /* LKC_H */
