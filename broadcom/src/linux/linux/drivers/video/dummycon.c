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
 *  linux/drivers/video/dummycon.c -- A dummy console driver
 *
 *  To be used if there's no other console driver (e.g. for plain VGA text)
 *  available, usually until fbcon takes console over.
 */

#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/tty.h>
#include <linux/console.h>
#include <linux/console_struct.h>
#include <linux/vt_kern.h>
#include <linux/init.h>

/*
 *  Dummy console driver
 */

#if defined(__arm__)
#define DUMMY_COLUMNS	ORIG_VIDEO_COLS
#define DUMMY_ROWS	ORIG_VIDEO_LINES
#elif defined(__hppa__)
#define DUMMY_COLUMNS	80	
#define DUMMY_ROWS	25
#else
#define DUMMY_COLUMNS	80
#define DUMMY_ROWS	25
#endif

static const char *dummycon_startup(void)
{
    return "dummy device";
}

static void dummycon_init(struct vc_data *conp, int init)
{
    conp->vc_can_do_color = 1;
    if (init) {
	conp->vc_cols = DUMMY_COLUMNS;
	conp->vc_rows = DUMMY_ROWS;
    } else
	vc_resize_con(DUMMY_ROWS, DUMMY_COLUMNS, conp->vc_num);
}

static int dummycon_dummy(void)
{
    return 0;
}

#define DUMMY	(void *)dummycon_dummy

/*
 *  The console `switch' structure for the dummy console
 *
 *  Most of the operations are dummies.
 */

const struct consw dummy_con = {
    con_startup:	dummycon_startup,
    con_init:		dummycon_init,
    con_deinit:		DUMMY,
    con_clear:		DUMMY,
    con_putc:		DUMMY,
    con_putcs:		DUMMY,
    con_cursor:		DUMMY,
    con_scroll:		DUMMY,
    con_bmove:		DUMMY,
    con_switch:		DUMMY,
    con_blank:		DUMMY,
    con_font_op:	DUMMY,
    con_set_palette:	DUMMY,
    con_scrolldelta:	DUMMY,
};
