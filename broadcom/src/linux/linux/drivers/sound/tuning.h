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
#ifdef SEQUENCER_C

unsigned short semitone_tuning[24] = 
{
/*   0 */ 10000, 10595, 11225, 11892, 12599, 13348, 14142, 14983, 
/*   8 */ 15874, 16818, 17818, 18877, 20000, 21189, 22449, 23784, 
/*  16 */ 25198, 26697, 28284, 29966, 31748, 33636, 35636, 37755
};

unsigned short cent_tuning[100] =
{
/*   0 */ 10000, 10006, 10012, 10017, 10023, 10029, 10035, 10041, 
/*   8 */ 10046, 10052, 10058, 10064, 10070, 10075, 10081, 10087, 
/*  16 */ 10093, 10099, 10105, 10110, 10116, 10122, 10128, 10134, 
/*  24 */ 10140, 10145, 10151, 10157, 10163, 10169, 10175, 10181, 
/*  32 */ 10187, 10192, 10198, 10204, 10210, 10216, 10222, 10228, 
/*  40 */ 10234, 10240, 10246, 10251, 10257, 10263, 10269, 10275, 
/*  48 */ 10281, 10287, 10293, 10299, 10305, 10311, 10317, 10323, 
/*  56 */ 10329, 10335, 10341, 10347, 10353, 10359, 10365, 10371, 
/*  64 */ 10377, 10383, 10389, 10395, 10401, 10407, 10413, 10419, 
/*  72 */ 10425, 10431, 10437, 10443, 10449, 10455, 10461, 10467, 
/*  80 */ 10473, 10479, 10485, 10491, 10497, 10503, 10509, 10515, 
/*  88 */ 10521, 10528, 10534, 10540, 10546, 10552, 10558, 10564, 
/*  96 */ 10570, 10576, 10582, 10589
};
#else
extern unsigned short semitone_tuning[24];
extern unsigned short cent_tuning[100];
#endif
