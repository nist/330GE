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
/* These structs are used by the system-use-sharing protocol, in which the
   Rock Ridge extensions are embedded.  It is quite possible that other
   extensions are present on the disk, and this is fine as long as they
   all use SUSP */

struct SU_SP{
  unsigned char magic[2];
  unsigned char skip;
} __attribute__((packed));

struct SU_CE{
  char extent[8];
  char offset[8];
  char size[8];
};

struct SU_ER{
  unsigned char len_id;
  unsigned char len_des;
  unsigned char len_src;
  unsigned char ext_ver;
  char data[0];
} __attribute__((packed));

struct RR_RR{
  char flags[1];
} __attribute__((packed));

struct RR_PX{
  char mode[8];
  char n_links[8];
  char uid[8];
  char gid[8];
};

struct RR_PN{
  char dev_high[8];
  char dev_low[8];
};


struct SL_component{
  unsigned char flags;
  unsigned char len;
  char text[0];
} __attribute__((packed));

struct RR_SL{
  unsigned char flags;
  struct SL_component link;
} __attribute__((packed));

struct RR_NM{
  unsigned char flags;
  char name[0];
} __attribute__((packed));

struct RR_CL{
  char location[8];
};

struct RR_PL{
  char location[8];
};

struct stamp{
  char time[7];
} __attribute__((packed));

struct RR_TF{
  char flags;
  struct stamp times[0];  /* Variable number of these beasts */
} __attribute__((packed));

/* Linux-specific extension for transparent decompression */
struct RR_ZF{
  char algorithm[2];
  char parms[2];
  char real_size[8];
};

/* These are the bits and their meanings for flags in the TF structure. */
#define TF_CREATE 1
#define TF_MODIFY 2
#define TF_ACCESS 4
#define TF_ATTRIBUTES 8
#define TF_BACKUP 16
#define TF_EXPIRATION 32
#define TF_EFFECTIVE 64
#define TF_LONG_FORM 128

struct rock_ridge{
  char signature[2];
  unsigned char len;
  unsigned char version;
  union{
    struct SU_SP SP;
    struct SU_CE CE;
    struct SU_ER ER;
    struct RR_RR RR;
    struct RR_PX PX;
    struct RR_PN PN;
    struct RR_SL SL;
    struct RR_NM NM;
    struct RR_CL CL;
    struct RR_PL PL;
    struct RR_TF TF;
    struct RR_ZF ZF;
  } u;
};

#define RR_PX 1   /* POSIX attributes */
#define RR_PN 2   /* POSIX devices */
#define RR_SL 4   /* Symbolic link */
#define RR_NM 8   /* Alternate Name */
#define RR_CL 16  /* Child link */
#define RR_PL 32  /* Parent link */
#define RR_RE 64  /* Relocation directory */
#define RR_TF 128 /* Timestamps */
