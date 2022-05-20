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
 *	pci_syscall.c
 *
 * For architectures where we want to allow direct access
 * to the PCI config stuff - it would probably be preferable
 * on PCs too, but there people just do it by hand with the
 * magic northbridge registers..
 */

#include <linux/sched.h>
#include <linux/errno.h>
#include <linux/pci.h>
#include <linux/smp_lock.h>
#include <asm/uaccess.h>


asmlinkage long
sys_pciconfig_read(unsigned long bus, unsigned long dfn,
		   unsigned long off, unsigned long len, void *buf)
{
	struct pci_dev *dev;
	u8 byte;
	u16 word;
	u32 dword;
	long err, cfg_ret;

	err = -EPERM;
	if (!capable(CAP_SYS_ADMIN))
		goto error;

	err = -ENODEV;
	dev = pci_find_slot(bus, dfn);
	if (!dev)
		goto error;

	lock_kernel();
	switch (len) {
	case 1:
		cfg_ret = pci_read_config_byte(dev, off, &byte);
		break;
	case 2:
		cfg_ret = pci_read_config_word(dev, off, &word);
		break;
	case 4:
		cfg_ret = pci_read_config_dword(dev, off, &dword);
		break;
	default:
		err = -EINVAL;
		unlock_kernel();
		goto error;
	};
	unlock_kernel();

	err = -EIO;
	if (cfg_ret != PCIBIOS_SUCCESSFUL)
		goto error;

	switch (len) {
	case 1:
		err = put_user(byte, (unsigned char *)buf);
		break;
	case 2:
		err = put_user(word, (unsigned short *)buf);
		break;
	case 4:
		err = put_user(dword, (unsigned int *)buf);
		break;
	};
	return err;

error:
	/* ??? XFree86 doesn't even check the return value.  They
	   just look for 0xffffffff in the output, since that's what
	   they get instead of a machine check on x86.  */
	switch (len) {
	case 1:
		put_user(-1, (unsigned char *)buf);
		break;
	case 2:
		put_user(-1, (unsigned short *)buf);
		break;
	case 4:
		put_user(-1, (unsigned int *)buf);
		break;
	};
	return err;
}

asmlinkage long
sys_pciconfig_write(unsigned long bus, unsigned long dfn,
		    unsigned long off, unsigned long len, void *buf)
{
	struct pci_dev *dev;
	u8 byte;
	u16 word;
	u32 dword;
	int err = 0;

	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;
	if (!pcibios_present())
		return -ENOSYS;

	dev = pci_find_slot(bus, dfn);
	if (!dev)
		return -ENODEV;

	lock_kernel();
	switch(len) {
	case 1:
		err = get_user(byte, (u8 *)buf);
		if (err)
			break;
		err = pci_write_config_byte(dev, off, byte);
		if (err != PCIBIOS_SUCCESSFUL)
			err = -EIO;
		break;

	case 2:
		err = get_user(word, (u16 *)buf);
		if (err)
			break;
		err = pci_write_config_word(dev, off, word);
		if (err != PCIBIOS_SUCCESSFUL)
			err = -EIO;
		break;

	case 4:
		err = get_user(dword, (u32 *)buf);
		if (err)
			break;
		err = pci_write_config_dword(dev, off, dword);
		if (err != PCIBIOS_SUCCESSFUL)
			err = -EIO;
		break;

	default:
		err = -EINVAL;
		break;
	};
	unlock_kernel();

	return err;
}
