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
    Copyright 2001, ASUSTek Inc.
    All Rights Reserved.
    
    This is UNPUBLISHED PROPRIETARY SOURCE CODE of ASUSTek Inc.;
    the contents of this file may not be disclosed to third parties, copied or
    duplicated in any form, in whole or in part, without the prior written
    permission of ASUSTek Inc..
*/
/*
 * NVRAM variable manipulation
 *
 * Copyright (C) 2001 ASUSTek Inc.
 *
 * $Id: common.h,v 1.1.1.1 2006/04/25 06:09:26 jiahao_jhou Exp $
 */

#ifndef _COMMON_H_
#define _COMMON_H_

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

struct variable {
	char *name;
	char *longname;
	int (*validate)(char *value, struct variable *v);
	char **argv;	
	int nullok;
	int event;
};

struct action
{
      char *name;
      char **in_name;
      char **in_var;
      char **out_name;
      char **out_var; 
      char **group_tag;     
      int (*callback)(void);
};

struct svcLink
{
      char *serviceId;
      char *serviceType;
      struct variable *variables;
      struct action *actions;
};

#define ARGV(args...) ((char *[]) { args, NULL })


#ifdef __cplusplus
extern "C" {
#endif

/* API export for UPnP function */
void InitVariables(void);
int LookupServiceId(char *serviceId);
char *GetServiceId(int sid);
struct action *GetActions(int sid);
struct variable *GetVariables(int sid);
char *GetServiceType(int sid);
struct action *CheckActions(int sid, char *name);
int CheckVariables(int sid, char *name, char *var);
char *GetVariable(int sid, char *name);
void SetVariable(int sid, char *name, char *value);

struct variable *LookupGroupVariables(int sid, char *groupName);
int CheckGroupVariables(int sid, struct variable *gvs, char *name, char *var);
void SetGroupVariable(int sid, struct variable *gvs, char *name, char *value, char *action);
char *GetGroupVariable(int sid, struct variable *gvs, char *name);


#ifdef __cplusplus
}
#endif

#endif /* _COMMON_H_ */
