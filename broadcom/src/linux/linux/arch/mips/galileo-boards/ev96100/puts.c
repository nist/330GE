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
 * Debug routines which directly access the uart.
 */

#include <linux/types.h>
#include <asm/galileo-boards/ev96100.h>


//#define SERIAL_BASE    EV96100_UART0_REGS_BASE
#define SERIAL_BASE    0xBD000020
#define NS16550_BASE   SERIAL_BASE

#define SERA_CMD       0x0D
#define SERA_DATA      0x08
//#define SERB_CMD       0x05
#define SERB_CMD       20
#define SERB_DATA      0x00
#define TX_BUSY        0x20

#define TIMEOUT    0xffff
#undef SLOW_DOWN

static const char digits[16] = "0123456789abcdef";
static volatile unsigned char * const com1 = (unsigned char *)SERIAL_BASE;


#ifdef SLOW_DOWN
static inline void slow_down()
{
    int k;
    for (k=0; k<10000; k++);
}
#else
#define slow_down()
#endif

void
putch(const unsigned char c)
{
    unsigned char ch;
    int i = 0;

    do {
        ch = com1[SERB_CMD];
        slow_down();
        i++;
        if (i>TIMEOUT) {
            break;
        }
    } while (0 == (ch & TX_BUSY));
    com1[SERB_DATA] = c;
}

void
putchar(const unsigned char c)
{
    unsigned char ch;
    int i = 0;

    do {
        ch = com1[SERB_CMD];
        slow_down();
        i++;
        if (i>TIMEOUT) {
            break;
        }
    } while (0 == (ch & TX_BUSY));
    com1[SERB_DATA] = c;
}

void
puts(unsigned char *cp)
{
    unsigned char ch;
    int i = 0;

    while (*cp) {
        do {
             ch = com1[SERB_CMD];
            slow_down();
            i++;
            if (i>TIMEOUT) {
                break;
            }
        } while (0 == (ch & TX_BUSY));
        com1[SERB_DATA] = *cp++;
    }
    putch('\r');
    putch('\n');
}

void
fputs(unsigned char *cp)
{
    unsigned char ch;
    int i = 0;

    while (*cp) {

        do {
             ch = com1[SERB_CMD];
             slow_down();
            i++;
            if (i>TIMEOUT) {
                break;
            }
        } while (0 == (ch & TX_BUSY));
        com1[SERB_DATA] = *cp++;
    }
}


void
put64(uint64_t ul)
{
    int cnt;
    unsigned ch;

    cnt = 16;            /* 16 nibbles in a 64 bit long */
    putch('0');
    putch('x');
    do {
        cnt--;
        ch = (unsigned char)(ul >> cnt * 4) & 0x0F;
                putch(digits[ch]);
    } while (cnt > 0);
}

void
put32(unsigned u)
{
    int cnt;
    unsigned ch;

    cnt = 8;            /* 8 nibbles in a 32 bit long */
    putch('0');
    putch('x');
    do {
        cnt--;
        ch = (unsigned char)(u >> cnt * 4) & 0x0F;
                putch(digits[ch]);
    } while (cnt > 0);
}
