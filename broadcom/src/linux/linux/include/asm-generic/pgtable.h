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
#ifndef _ASM_GENERIC_PGTABLE_H
#define _ASM_GENERIC_PGTABLE_H

static inline int ptep_test_and_clear_young(pte_t *ptep)
{
	pte_t pte = *ptep;
	if (!pte_young(pte))
		return 0;
	set_pte(ptep, pte_mkold(pte));
	return 1;
}

static inline int ptep_test_and_clear_dirty(pte_t *ptep)
{
	pte_t pte = *ptep;
	if (!pte_dirty(pte))
		return 0;
	set_pte(ptep, pte_mkclean(pte));
	return 1;
}

static inline pte_t ptep_get_and_clear(pte_t *ptep)
{
	pte_t pte = *ptep;
	pte_clear(ptep);
	return pte;
}

static inline void ptep_set_wrprotect(pte_t *ptep)
{
	pte_t old_pte = *ptep;
	set_pte(ptep, pte_wrprotect(old_pte));
}

static inline void ptep_mkdirty(pte_t *ptep)
{
	pte_t old_pte = *ptep;
	set_pte(ptep, pte_mkdirty(old_pte));
}

#define pte_same(A,B)	(pte_val(A) == pte_val(B))

#endif /* _ASM_GENERIC_PGTABLE_H */
