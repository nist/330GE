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
 * BK Id: SCCS/s.error_log.c 1.6 05/17/01 18:14:21 cort
 */
/*
 *  arch/ppc/kernel/error_log.c
 *  
 *  Copyright (c) 2000 Tilmann Bitterberg
 *  (tilmann@bitterberg.de)
 *
 *  Error processing of errors found by rtas even-scan routine
 *  which is done with every heartbeat. (chrp_setup.c)
 */

#include <linux/sched.h>

#include <asm/prom.h>

#include "error_log.h"

/* ****************************************************************** */
/* 
 * EVENT-SCAN
 * The whole stuff below here doesn't take any action when it found
 * an error, it just prints as much information as possible and 
 * then its up to the user to decide what to do.
 *
 * Returns 0 if no errors were found
 * Returns 1 if there may be more errors
 */
int ppc_rtas_errorlog_scan(void)
{
const char *_errlog_severity[] = {
#ifdef VERBOSE_ERRORS
	"No Error\n\t\
Should require no further information",
	"Event\n\t\
This is not really an error, it is an event. I use events\n\t\
to communicate with RTAS back and forth.",
	"Warning\n\t\
Indicates a non-state-losing error, either fully recovered\n\t\
by RTAS or not needing recovery. Ignore it.",
	"Error sync\n\t\
May only be fatal to a certain program or thread. Recovery\n\t\
and continuation is possible, if I only had a handler for\n\t\
this. Less serious",
	"Error\n\t\
Less serious, but still causing a loss of data and state.\n\t\
I can't tell you exactly what to do, You have to decide\n\t\
with help from the target and initiator field, what kind\n\t\
of further actions may take place.",
	"Fatal\n\t\
Represent a permanent hardware failure and I believe this\n\t\
affects my overall performance and behaviour. I would not\n\t\
attempt to continue normal operation."
#else
	"No Error",
	"Event",
	"Warning",
	"Error sync",
	"Error",
	"Fatal"
#endif /* VERBOSE_ERRORS */
};


const char *_errlog_extended[] = {
#ifdef VERBOSE_ERRORS
	"Not present\n\t\
Sad, the RTAS call didn't return an extended error log.",
	"Present\n\t\
The extended log is present and hopefully it contains a lot of\n\t\
useful information, which leads to the solution of the problem."
#else
	"Not present",
	"Present"
#endif /* VERBOSE_ERRORS */
};

const char *_errlog_initiator[] = { 
	"Unknown or not applicable",
	"CPU",
	"PCI",
	"ISA",
	"Memory",
	"Power management"
};

const char *_errlog_target[] = { 
	"Unknown or not applicable",
	"CPU",
	"PCI",
	"ISA",
	"Memory",
	"Power management"
};
	rtas_error_log error_log;
	char logdata[1024];
	int error;

	error = call_rtas ("event-scan", 4, 1, (unsigned long *)&error_log,
			INTERNAL_ERROR | EPOW_WARNING,
			0, __pa(logdata), 1024);

	if (error == 1) /* no errors found */
		return 0;

	if (error == -1) {
		printk(KERN_ERR "Unable to get errors. Do you a favor and throw this box away\n");
		return 0;
	}
	if (error_log.version != 1)
		printk(KERN_WARNING "Unknown version (%d), please implement me\n", 
				error_log.version);

	switch (error_log.disposition) {
		case DISP_FULLY_RECOVERED:
			/* there was an error, but everything is fine now */
			return 0;
		case DISP_NOT_RECOVERED:
			printk("We have a really serious Problem!\n");
		case DISP_LIMITED_RECOVERY:
			printk("Error classification\n");
			printk("Severity  : %s\n", 
					ppc_rtas_errorlog_check_severity (error_log));
			printk("Initiator : %s\n", 
					ppc_rtas_errorlog_check_initiator (error_log));
			printk("Target    : %s\n", 
					ppc_rtas_errorlog_check_target (error_log));
			printk("Type      : %s\n", 
					ppc_rtas_errorlog_check_type (error_log));
			printk("Ext. log  : %s\n", 
					ppc_rtas_errorlog_check_extended (error_log));
			if (error_log.extended)
				ppc_rtas_errorlog_disect_extended (logdata);
			return 1;	
		default:
			/* nothing */
			break;
	}
	return 0;
}
/* ****************************************************************** */
const char * ppc_rtas_errorlog_check_type (rtas_error_log error_log)
{
	const char *_errlog_type[] = {
		"unknown type",
		"too many tries failed",
		"TCE error",
		"RTAS device failed",
		"target timed out",
		"parity error on data",			/* 5 */
		"parity error on address",
		"parity error on external cache",
		"access to invalid address",
		"uncorrectable ECC error",
		"corrected ECC error"			/* 10 */
	};
	if (error_log.type == TYPE_EPOW) 
		return "EPOW"; 
	if (error_log.type >= TYPE_PMGM_POWER_SW_ON)
		return "PowerMGM Event (not handled right now)";
	return _errlog_type[error_log.type];
}

