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
#ifndef __LINUX_PCI_GAMEPORT_H
#define __LINUX_PCI_GAMEPORT_H

/*
 *	Public interfaces for attaching a PCI gameport directly to the
 *	soundcard when it shares the same PCI ident
 */
 
#define PCIGAME_4DWAVE		0
#define PCIGAME_VORTEX		1
#define PCIGAME_VORTEX2		2


struct pcigame_data {
	int gcr;	/* Gameport control register */
	int legacy;	/* Legacy port location */
	int axes;	/* Axes start */
	int axsize;	/* Axis field size */
	int axmax;	/* Axis field max value */
	int adcmode;	/* Value to enable ADC mode in GCR */
};

struct pcigame {
	struct gameport gameport;
	struct pci_dev *dev;
        unsigned char *base;
	struct pcigame_data *data;
};


#if defined(CONFIG_INPUT_PCIGAME) || defined(CONFIG_INPUT_PCIGAME_MODULE)
extern struct pcigame *pcigame_attach(struct pci_dev *dev, int type);
extern void pcigame_detach(struct pcigame *game);
#else
#define pcigame_attach(a,b)	NULL
#define pcigame_detach(a)
#endif

#endif
