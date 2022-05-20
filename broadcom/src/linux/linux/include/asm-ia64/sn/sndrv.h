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
#ifndef _ASM_IA64_SN_SNDRV_H
#define _ASM_IA64_SN_SNDRV_H

/* ioctl commands */
#define SNDRV_GET_ROUTERINFO		1
#define SNDRV_GET_INFOSIZE		2
#define SNDRV_GET_HUBINFO		3
#define SNDRV_GET_FLASHLOGSIZE		4
#define SNDRV_SET_FLASHSYNC		5
#define SNDRV_GET_FLASHLOGDATA		6
#define SNDRV_GET_FLASHLOGALL		7

#define SNDRV_SET_HISTOGRAM_TYPE	14

#define SNDRV_ELSC_COMMAND		19
#define	SNDRV_CLEAR_LOG			20
#define	SNDRV_INIT_LOG			21
#define	SNDRV_GET_PIMM_PSC		22
#define SNDRV_SET_PARTITION		23
#define SNDRV_GET_PARTITION		24

/* see synergy_perf_ioctl() */
#define SNDRV_GET_SYNERGY_VERSION	30
#define SNDRV_GET_SYNERGY_STATUS	31
#define SNDRV_GET_SYNERGYINFO		32
#define SNDRV_SYNERGY_APPEND		33
#define SNDRV_SYNERGY_ENABLE		34
#define SNDRV_SYNERGY_FREQ		35

/* Devices */
#define SNDRV_UKNOWN_DEVICE		-1
#define SNDRV_ROUTER_DEVICE		1
#define SNDRV_HUB_DEVICE		2
#define SNDRV_ELSC_NVRAM_DEVICE		3
#define SNDRV_ELSC_CONTROLLER_DEVICE	4
#define SNDRV_SYSCTL_SUBCH		5
#define SNDRV_SYNERGY_DEVICE		6

#endif /* _ASM_IA64_SN_SNDRV_H */
