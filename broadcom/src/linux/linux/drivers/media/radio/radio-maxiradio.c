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
 * Guillemot Maxi Radio FM 2000 PCI radio card driver for Linux 
 * (C) 2001 Dimitromanolakis Apostolos <apdim@grecian.net>
 *
 * Based in the radio Maestro PCI driver. Actually it uses the same chip
 * for radio but different pci controller.
 *
 * I didn't have any specs I reversed engineered the protocol from
 * the windows driver (radio.dll). 
 *
 * The card uses the TEA5757 chip that includes a search function but it
 * is useless as I haven't found any way to read back the frequency. If 
 * anybody does please mail me.
 *
 * For the pdf file see:
 * http://www.semiconductors.philips.com/pip/TEA5757H/V1
 *
 *
 * CHANGES:
 *   0.75b
 *     - better pci interface thanks to Francois Romieu <romieu@cogenit.fr>
 *
 *   0.75
 *     - tiding up
 *     - removed support for multiple devices as it didn't work anyway
 *
 * BUGS: 
 *   - card unmutes if you change frequency
 *
 */


#include <linux/module.h>
#include <linux/init.h>
#include <linux/ioport.h>
#include <linux/delay.h>
#include <linux/sched.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/semaphore.h>
#include <linux/pci.h>
#include <linux/videodev.h>

/* version 0.75      Sun Feb  4 22:51:27 EET 2001 */
#define DRIVER_VERSION	"0.75"

#ifndef PCI_VENDOR_ID_GUILLEMOT
#define PCI_VENDOR_ID_GUILLEMOT 0x5046
#endif

#ifndef PCI_DEVICE_ID_GUILLEMOT
#define PCI_DEVICE_ID_GUILLEMOT_MAXIRADIO 0x1001
#endif


/* TEA5757 pin mappings */
const int clk = 1, data = 2, wren = 4, mo_st = 8, power = 16 ;

static int radio_nr = -1;
MODULE_PARM(radio_nr, "i");


#define FREQ_LO		 50*16000
#define FREQ_HI		150*16000

#define FREQ_IF         171200 /* 10.7*16000   */
#define FREQ_STEP       200    /* 12.5*16      */

#define FREQ2BITS(x)	((( (unsigned int)(x)+FREQ_IF+(FREQ_STEP<<1))\
			/(FREQ_STEP<<2))<<2) /* (x==fmhz*16*1000) -> bits */

#define BITS2FREQ(x)	((x) * FREQ_STEP - FREQ_IF)


static int radio_open(struct video_device *, int);
static int radio_ioctl(struct video_device *, unsigned int, void *);
static void radio_close(struct video_device *);

static struct video_device maxiradio_radio=
{
	owner:		THIS_MODULE,
	name:		"Maxi Radio FM2000 radio",
	type:		VID_TYPE_TUNER,
	hardware:	VID_HARDWARE_SF16MI,
	open:		radio_open,
	close:		radio_close,
	ioctl:		radio_ioctl,
};

static struct radio_device
{
	__u16	io,	/* base of radio io */
		muted,	/* VIDEO_AUDIO_MUTE */
		stereo,	/* VIDEO_TUNER_STEREO_ON */	
		tuned;	/* signal strength (0 or 0xffff) */
		
	unsigned long freq;
	
	struct  semaphore lock;
} radio_unit = {0, 0, 0, 0, };


static void sleep_125ms(void)
{
	current->state = TASK_INTERRUPTIBLE;
	schedule_timeout(HZ >> 3);
}


static void outbit(unsigned long bit, __u16 io)
{
	if(bit != 0)
		{
			outb(  power|wren|data     ,io); udelay(4);
			outb(  power|wren|data|clk ,io); udelay(4);
			outb(  power|wren|data     ,io); udelay(4);
		}
	else	
		{
			outb(  power|wren          ,io); udelay(4);
			outb(  power|wren|clk      ,io); udelay(4);
			outb(  power|wren          ,io); udelay(4);
		}
}

static void turn_power(__u16 io, int p)
{
	if(p != 0) outb(power, io); else outb(0,io);
}


static void set_freq(__u16 io, __u32 data)
{
	unsigned long int si;
	int bl;
	
	/* TEA5757 shift register bits (see pdf) */

	outbit(0,io); // 24  search 
	outbit(1,io); // 23  search up/down
	
	outbit(0,io); // 22  stereo/mono

	outbit(0,io); // 21  band
	outbit(0,io); // 20  band (only 00=FM works I think)

	outbit(0,io); // 19  port ?
	outbit(0,io); // 18  port ?
	
	outbit(0,io); // 17  search level
	outbit(0,io); // 16  search level
 
	si = 0x8000;
	for(bl = 1; bl <= 16 ; bl++) { outbit(data & si,io); si >>=1; }
	
	outb(power,io);
}

static int get_stereo(__u16 io)
{	
	outb(power,io); udelay(4);
	return !(inb(io) & mo_st);
}

static int get_tune(__u16 io)
{	
	outb(power+clk,io); udelay(4);
	return !(inb(io) & mo_st);
}


inline static int radio_function(struct video_device *dev, 
				 unsigned int cmd, void *arg)
{
	struct radio_device *card=dev->priv;
	switch(cmd) {
		case VIDIOCGCAP: {
			struct video_capability v;
			
			strcpy(v.name, "Maxi Radio FM2000 radio");
			v.type=VID_TYPE_TUNER;
			v.channels=v.audios=1;
			v.maxwidth=v.maxheight=v.minwidth=v.minheight=0;
			
			if(copy_to_user(arg,&v,sizeof(v)))
				return -EFAULT;
				
			return 0;
		}
		case VIDIOCGTUNER: {
			struct video_tuner v;
			
			if(copy_from_user(&v, arg,sizeof(v))!=0)
				return -EFAULT;
				
			if(v.tuner)
				return -EINVAL;
				
			card->stereo = 0xffff * get_stereo(card->io);
			card->tuned = 0xffff * get_tune(card->io);
			
			v.flags = VIDEO_TUNER_LOW | card->stereo;
			v.signal = card->tuned;
			
			strcpy(v.name, "FM");
			
			v.rangelow = FREQ_LO;
			v.rangehigh = FREQ_HI;
			v.mode = VIDEO_MODE_AUTO;
			
			if(copy_to_user(arg,&v, sizeof(v)))
				return -EFAULT;
				
		  return 0;
		}
		case VIDIOCSTUNER: {
			struct video_tuner v;
			
			if(copy_from_user(&v, arg, sizeof(v)))
				return -EFAULT;
				
			if(v.tuner!=0)
				return -EINVAL;
				
			return 0;
		}
		case VIDIOCGFREQ: {
			unsigned long tmp=card->freq;
			
			if(copy_to_user(arg, &tmp, sizeof(tmp)))
				return -EFAULT;
				
			return 0;
		}
		
		case VIDIOCSFREQ: {
			unsigned long tmp;
			
			if(copy_from_user(&tmp, arg, sizeof(tmp)))
				return -EFAULT;
				
			if ( tmp<FREQ_LO || tmp>FREQ_HI )
				return -EINVAL;
				
			card->freq = tmp;
			
			set_freq(card->io, FREQ2BITS(card->freq));
			sleep_125ms();

			return 0;
		}
		case VIDIOCGAUDIO: {	
			struct video_audio v;
			strcpy(v.name, "Radio");
			v.audio=v.volume=v.bass=v.treble=v.balance=v.step=0;
			v.flags=VIDEO_AUDIO_MUTABLE | card->muted;
			v.mode=VIDEO_SOUND_STEREO;
			if(copy_to_user(arg,&v, sizeof(v)))
				return -EFAULT;
			return 0;		
		}
		
		case VIDIOCSAUDIO: {
			struct video_audio v;
			
			if(copy_from_user(&v, arg, sizeof(v)))
				return -EFAULT;
				
			if(v.audio)
				return -EINVAL;
				
			
			card->muted = v.flags & VIDEO_AUDIO_MUTE;
				
			if(card->muted)
				turn_power(card->io, 0);
			else
				set_freq(card->io, FREQ2BITS(card->freq));
				
			return 0;
		}
		
		case VIDIOCGUNIT: {
			struct video_unit v;
			v.video=VIDEO_NO_UNIT;
			v.vbi=VIDEO_NO_UNIT;
			v.radio=dev->minor;
			v.audio=0;
			v.teletext=VIDEO_NO_UNIT;
			if(copy_to_user(arg, &v, sizeof(v)))
				return -EFAULT;
			return 0;		
		}
		default: return -ENOIOCTLCMD;
	}
}

static int radio_ioctl(struct video_device *dev, unsigned int cmd, void *arg)
{
	struct radio_device *card=dev->priv;
	int ret;
	down(&card->lock);
	ret = radio_function(dev, cmd, arg);
	up(&card->lock);
	return ret;
}

static int radio_open(struct video_device *dev, int flags)
{
	return 0;
}

static void radio_close(struct video_device *dev)
{
}

MODULE_AUTHOR("Dimitromanolakis Apostolos, apdim@grecian.net");
MODULE_DESCRIPTION("Radio driver for the Guillemot Maxi Radio FM2000 radio.");
MODULE_LICENSE("GPL");


EXPORT_NO_SYMBOLS;

static int __devinit maxiradio_init_one(struct pci_dev *pdev, const struct pci_device_id *ent)
{
	if(!request_region(pci_resource_start(pdev, 0),
	                   pci_resource_len(pdev, 0), "Maxi Radio FM 2000")) {
	        printk(KERN_ERR "radio-maxiradio: can't reserve I/O ports\n");
	        goto err_out;
	}

	if (pci_enable_device(pdev))
	        goto err_out_free_region;

	radio_unit.io = pci_resource_start(pdev, 0);
	init_MUTEX(&radio_unit.lock);
	maxiradio_radio.priv = &radio_unit;

	if(video_register_device(&maxiradio_radio, VFL_TYPE_RADIO, radio_nr)==-1) {
	        printk("radio-maxiradio: can't register device!");
	        goto err_out_free_region;
	}

	printk(KERN_INFO "radio-maxiradio: version "
	       DRIVER_VERSION
	       " time "
	       __TIME__ "  "
	       __DATE__
	       "\n");

	printk(KERN_INFO "radio-maxiradio: found Guillemot MAXI Radio device (io = 0x%x)\n",
	       radio_unit.io);
	return 0;

err_out_free_region:
	release_region(pci_resource_start(pdev, 0), pci_resource_len(pdev, 0));
err_out:
	return -ENODEV;
}

static void __devexit maxiradio_remove_one(struct pci_dev *pdev)
{
	video_unregister_device(&maxiradio_radio);
	release_region(pci_resource_start(pdev, 0), pci_resource_len(pdev, 0));
}

static struct pci_device_id maxiradio_pci_tbl[] __devinitdata = {
	{ PCI_VENDOR_ID_GUILLEMOT, PCI_DEVICE_ID_GUILLEMOT_MAXIRADIO,
		PCI_ANY_ID, PCI_ANY_ID, },
	{ 0,}
};

MODULE_DEVICE_TABLE(pci, maxiradio_pci_tbl);

static struct pci_driver maxiradio_driver = {
	name:		"radio-maxiradio",
	id_table:	maxiradio_pci_tbl,
	probe:		maxiradio_init_one,
	remove:		__devexit_p(maxiradio_remove_one),
};

int __init maxiradio_radio_init(void)
{
	return pci_module_init(&maxiradio_driver);
}

void __exit maxiradio_radio_exit(void)
{
	pci_unregister_driver(&maxiradio_driver);
}

module_init(maxiradio_radio_init);
module_exit(maxiradio_radio_exit);
