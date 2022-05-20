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
    NetWinder Floating Point Emulator
    (c) Rebel.COM, 1998,1999

    Direct questions, comments to Scott Bambrough <scottb@netwinder.org>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include "fpa11.h"
#include "softfloat.h"
#include "fpopcode.h"

float32 float32_exp(float32 Fm);
float32 float32_ln(float32 Fm);
float32 float32_sin(float32 rFm);
float32 float32_cos(float32 rFm);
float32 float32_arcsin(float32 rFm);
float32 float32_arctan(float32 rFm);
float32 float32_log(float32 rFm);
float32 float32_tan(float32 rFm);
float32 float32_arccos(float32 rFm);
float32 float32_pow(float32 rFn,float32 rFm);
float32 float32_pol(float32 rFn,float32 rFm);

unsigned int SingleCPDO(const unsigned int opcode)
{
   FPA11 *fpa11 = GET_FPA11();
   float32 rFm, rFn;
   unsigned int Fd, Fm, Fn, nRc = 1;

   Fm = getFm(opcode);
   if (CONSTANT_FM(opcode))
   {
     rFm = getSingleConstant(Fm);
   }
   else
   {  
     switch (fpa11->fType[Fm])
     {
        case typeSingle:
          rFm = fpa11->fpreg[Fm].fSingle;
        break;
        
        default: return 0;
     }
   }

   if (!MONADIC_INSTRUCTION(opcode))
   {
      Fn = getFn(opcode);
      switch (fpa11->fType[Fn])
      {
        case typeSingle:
          rFn = fpa11->fpreg[Fn].fSingle;
        break;

        default: return 0;
      }
   }

   Fd = getFd(opcode);
   switch (opcode & MASK_ARITHMETIC_OPCODE)
   {
      /* dyadic opcodes */
      case ADF_CODE:
         fpa11->fpreg[Fd].fSingle = float32_add(rFn,rFm);
      break;

      case MUF_CODE:
      case FML_CODE:
        fpa11->fpreg[Fd].fSingle = float32_mul(rFn,rFm);
      break;

      case SUF_CODE:
         fpa11->fpreg[Fd].fSingle = float32_sub(rFn,rFm);
      break;

      case RSF_CODE:
         fpa11->fpreg[Fd].fSingle = float32_sub(rFm,rFn);
      break;

      case DVF_CODE:
      case FDV_CODE:
         fpa11->fpreg[Fd].fSingle = float32_div(rFn,rFm);
      break;

      case RDF_CODE:
      case FRD_CODE:
         fpa11->fpreg[Fd].fSingle = float32_div(rFm,rFn);
      break;


      case RMF_CODE:
         fpa11->fpreg[Fd].fSingle = float32_rem(rFn,rFm);
      break;


      /* monadic opcodes */
      case MVF_CODE:
         fpa11->fpreg[Fd].fSingle = rFm;
      break;

      case MNF_CODE:
         rFm ^= 0x80000000;
         fpa11->fpreg[Fd].fSingle = rFm;
      break;

      case ABS_CODE:
         rFm &= 0x7fffffff;
         fpa11->fpreg[Fd].fSingle = rFm;
      break;

      case RND_CODE:
      case URD_CODE:
         fpa11->fpreg[Fd].fSingle = float32_round_to_int(rFm);
      break;

      case SQT_CODE:
         fpa11->fpreg[Fd].fSingle = float32_sqrt(rFm);
      break;


      case NRM_CODE:
      break;
      
      default:
      {
        nRc = 0;
      }
   }

   if (0 != nRc) fpa11->fType[Fd] = typeSingle;
   return nRc;
}

