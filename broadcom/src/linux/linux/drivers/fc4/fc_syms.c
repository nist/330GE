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
 * We should not even be trying to compile this if we are not doing
 * a module.
 */
#define __NO_VERSION__
#include <linux/config.h>
#include <linux/module.h>

#ifdef CONFIG_MODULES

#include <linux/sched.h>
#include <linux/types.h>
#include <linux/string.h>
#include <linux/kernel.h>

#include "fcp_impl.h"

EXPORT_SYMBOL(fcp_init);
EXPORT_SYMBOL(fcp_release);
EXPORT_SYMBOL(fcp_queue_empty);
EXPORT_SYMBOL(fcp_receive_solicited);
EXPORT_SYMBOL(fc_channels);
EXPORT_SYMBOL(fcp_state_change);
EXPORT_SYMBOL(fc_do_plogi);
EXPORT_SYMBOL(fc_do_prli);

/* SCSI stuff */
EXPORT_SYMBOL(fcp_scsi_queuecommand);
EXPORT_SYMBOL(fcp_old_abort);
EXPORT_SYMBOL(fcp_scsi_abort);
EXPORT_SYMBOL(fcp_scsi_dev_reset);
EXPORT_SYMBOL(fcp_scsi_bus_reset);
EXPORT_SYMBOL(fcp_scsi_host_reset);

#endif /* CONFIG_MODULES */
