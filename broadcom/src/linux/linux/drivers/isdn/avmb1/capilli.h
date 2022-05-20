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
/* $Id: capilli.h,v 1.1.1.1 2007/01/25 12:51:54 jiahao_jhou Exp $
 * 
 * Kernel CAPI 2.0 Driver Interface for Linux
 * 
 * Copyright 1999 by Carsten Paeth <calle@calle.de>
 * 
 * This software may be used and distributed according to the terms
 * of the GNU General Public License, incorporated herein by reference.
 *
 */

#ifndef __CAPILLI_H__
#define __CAPILLI_H__

typedef struct capiloaddatapart {
	int user;		/* data in userspace ? */
	int len;
	unsigned char *data;
} capiloaddatapart;

typedef struct capiloaddata {
	capiloaddatapart firmware;
	capiloaddatapart configuration;
} capiloaddata;

typedef struct capicardparams {
	unsigned int port;
	unsigned irq;
	int cardtype;
	int cardnr;
	unsigned int membase;
} capicardparams;

struct capi_driver;

struct capi_ctr {
        struct capi_ctr *next;			/* next ctr of same driver */
        struct capi_driver *driver;
	int cnr;				/* controller number */
	char name[32];				/* name of controller */
	volatile unsigned short cardstate;	/* controller state */
	volatile int blocked;			/* output blocked */
	int traceflag;				/* capi trace */

	void *driverdata;			/* driver specific */

	/* filled before calling ready callback */
	__u8 manu[CAPI_MANUFACTURER_LEN];	/* CAPI_GET_MANUFACTURER */
	capi_version version;			/* CAPI_GET_VERSION */
	capi_profile profile;			/* CAPI_GET_PROFILE */
	__u8 serial[CAPI_SERIAL_LEN];		/* CAPI_GET_SERIAL */

	/* functions */
        void (*ready)(struct capi_ctr * card);
        void (*reseted)(struct capi_ctr * card);
        void (*suspend_output)(struct capi_ctr * card);
        void (*resume_output)(struct capi_ctr * card);
        void (*handle_capimsg)(struct capi_ctr * card,
			   	__u16 appl, struct sk_buff *skb);
	void (*appl_registered)(struct capi_ctr * card, __u16 appl);
	void (*appl_released)(struct capi_ctr * card, __u16 appl);

        void (*new_ncci)(struct capi_ctr * card,
			   	__u16 appl, __u32 ncci, __u32 winsize);
        void (*free_ncci)(struct capi_ctr * card, __u16 appl, __u32 ncci);

	/* management information for kcapi */

	unsigned long nrecvctlpkt;
	unsigned long nrecvdatapkt;
	unsigned long nsentctlpkt;
	unsigned long nsentdatapkt;

	struct proc_dir_entry *procent;
        char procfn[128];
};

struct capi_driver_interface {
    struct capi_ctr *(*attach_ctr)(struct capi_driver *driver, char *name, void *data);
    int (*detach_ctr)(struct capi_ctr *);
};

struct capi_driver {
     char name[32];				/* driver name */
     char revision[32];
     int (*load_firmware)(struct capi_ctr *, capiloaddata *);
     void (*reset_ctr)(struct capi_ctr *);
     void (*remove_ctr)(struct capi_ctr *);
     void (*register_appl)(struct capi_ctr *, __u16 appl,
						capi_register_params *);
     void (*release_appl)(struct capi_ctr *, __u16 appl);
     void (*send_message)(struct capi_ctr *, struct sk_buff *skb);

     char *(*procinfo)(struct capi_ctr *);
     int (*ctr_read_proc)(char *page, char **start, off_t off,
			       int count, int *eof, struct capi_ctr *card);
     int (*driver_read_proc)(char *page, char **start, off_t off,
		               int count, int *eof, struct capi_driver *driver);

     int (*add_card)(struct capi_driver *driver, capicardparams *data);

     /* intitialized by kcapi */
     struct capi_ctr	*controller;		/* list of controllers */
     struct capi_driver *next;
     int ncontroller;
     struct proc_dir_entry *procent;
     char procfn[128];
};

struct capi_driver_interface *attach_capi_driver(struct capi_driver *driver);
void detach_capi_driver(struct capi_driver *driver);

#endif				/* __CAPILLI_H__ */
