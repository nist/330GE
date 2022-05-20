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
/* stdbool.h for GNU.  */
#ifndef __STDBOOL_H__
#define __STDBOOL_H__	1

/* The type `bool' must promote to `int' or `unsigned int'.  The constants
   `true' and `false' must have the value 0 and 1 respectively.  */
typedef enum
  {
    false = 0,
    true = 1
  } bool;

/* The names `true' and `false' must also be made available as macros.  */
#define false	false
#define true	true

/* Signal that all the definitions are present.  */
#define __bool_true_false_are_defined	1

#endif	/* stdbool.h */
