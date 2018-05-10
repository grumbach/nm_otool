/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 17:44:07 by agrumbac          #+#    #+#             */
/*   Updated: 2018/05/10 23:07:18 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"
#include "nm_display.h"

/*
** t_lc_manager symtab_manager: manage_symtab, manage_symtab_64
*/

static bool		manage_symtab_32(const size_t offset)
{
	struct symtab_command	*sym;
	struct nlist			*nlist;
	t_sym_sort				sorted_symbols;
	uint32_t				nsyms;
	uint32_t				i;

	//retrieve pointers
	if (!(sym = safe(offset, sizeof(*sym))))
		return (errors(ERR_FILE, "bad symtab command offset"));
	nsyms = endian_4(sym->nsyms);
	if (!(nlist = safe(endian_4(sym->symoff), sizeof(*nlist) * nsyms)))
		return (errors(ERR_FILE, "bad symbol table offset or size"));
	// check if stringtable was invalid to begin with
	if (!safe(endian_4(sym->stroff), endian_4(sym->strsize)))
		return (errors(ERR_FILE, "bad stringtable offset or size"));
	// allocate memory for sorted_symbols
	if (!(nm_symbol_allocate(&sorted_symbols, nsyms)))
		return (errors(ERR_THROW, __func__));

	//for each symbol
	i = 0;
	while (i < nsyms)
	{
		nm_extract_values(&nlist[i], \
			endian_4(nlist[i].n_value), sym, &sorted_symbols);
		i++;
	}
	nm_sort_print_free(&sorted_symbols);
	return (true);
}

static bool		manage_symtab_64(const size_t offset)
{
	struct symtab_command	*sym;
	struct nlist_64			*nlist;
	t_sym_sort				sorted_symbols;
	uint32_t				nsyms;
	uint32_t				i;

	//retrieve pointers
	if (!(sym = safe(offset, sizeof(*sym))))
		return (errors(ERR_FILE, "bad symtab command offset"));
	nsyms = endian_4(sym->nsyms);
	if (!(nlist = safe(endian_4(sym->symoff), sizeof(*nlist) * nsyms)))
		return (errors(ERR_FILE, "bad symbol table offset or size"));
	// check if stringtable was invalid to begin with
	if (!safe(endian_4(sym->stroff), endian_4(sym->strsize)))
		return (errors(ERR_FILE, "bad stringtable offset or size"));
	// allocate memory for sorted_symbols
	if (!(nm_symbol_allocate(&sorted_symbols, nsyms)))
		return (errors(ERR_THROW, __func__));

	//for each symbol
	i = 0;
	while (i < nsyms)
	{
		nm_extract_values((struct nlist*)&nlist[i], \
			endian_8(nlist[i].n_value), sym, &sorted_symbols);
		i++;
	}
	nm_sort_print_free(&sorted_symbols);
	return (true);
}

/*
** -----------------------------------------------------------------------------
*/

/*
** t_lc_manager segment_manager: manage_segment, manage_segment_64
**   fills nm_sections_character_table
*/

static bool		manage_segment_32(const size_t offset)
{
	if (!(iterate_sections(offset, NULL, NULL, \
		(t_section_manager)&nm_sections_character_table)))
		return (errors(ERR_THROW, __func__));
	return (true);
}

static bool		manage_segment_64(const size_t offset)
{
	if (!(iterate_sections_64(offset, NULL, NULL, \
		(t_section_manager)&nm_sections_character_table)))
		return (errors(ERR_THROW, __func__));
	return (true);
}

/*
** t_gatherer nm_gatherer
*/

static bool		nm_gatherer(const bool is_64)
{
	static const uint32_t		lc_seg[2] = {LC_SEGMENT, LC_SEGMENT_64};
	static const t_lc_manager	segment_manager[2] =
	{
		&manage_segment_32,
		&manage_segment_64
	};
	static const t_lc_manager	symtab_manager[2] =
	{
		&manage_symtab_32,
		&manage_symtab_64
	};

	// reset section table
	nm_sections_character_table(0);
	// fill sections table
	if (!(iterate_lc(is_64, lc_seg[is_64], segment_manager[is_64])))
		return (errors(ERR_THROW, __func__));
	// manage symtab
	if (!(iterate_lc(is_64, LC_SYMTAB, symtab_manager[is_64])))
		return (errors(ERR_THROW, __func__));

	return (true);
}

/*
** Flags are toggled (on/off) for files in the order they are given:
**   ./ft_nm -p ft_ls -r -p -a ft_otool
** would apply -p for ft_ls, and only use -r and -a for ft_otool
** the flags are disabled on the second use (toggled on then off)
*/

int				main(int ac, char **av)
{
	if (ac < 2)
	{
		extract_macho(DEFAULT_TARGET, &nm_gatherer);
	}
	while (*++av)
	{
		if (**av == '-')
		{
			if (!nm_set_flag(*av))
				return (EXIT_FAILURE);
		}
		else
		{
			extract_macho(*av, &nm_gatherer);
		}
	}
	return (EXIT_SUCCESS);
}
