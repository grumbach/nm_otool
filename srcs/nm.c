/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 17:44:07 by agrumbac          #+#    #+#             */
/*   Updated: 2018/05/05 00:43:39 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

/*
** t_lc_manager segment_manager: manage_segment, manage_segment_64
*/

static bool		manage_segment(const size_t offset)
{
	// same as 64
}

static bool		manage_segment_64(const size_t offset)
{
	// TODO count sections
	if (!(iterate_sections(offset, NULL, NULL, &section_table_write)))
		return (errors(ERR_THROW, "in _manage_segment"));

	// TODO fill section table
	if (!(iterate_sections(offset, NULL, NULL, &section_table_write)))
		return (errors(ERR_THROW, "in _manage_segment"));

	return (BOOL_TRUE);
}

/*
** t_lc_manager symtab_manager: manage_symtab, manage_symtab_64
*/

static bool		manage_symtab(const size_t offset)
{
	// same as 64
}

//------------TMP unsafe
char			get_type(const uint8_t n_type, const uint8_t n_sect, char *ptr)
{
	char		type = 'u';
	int			n_type_data = N_TYPE & n_type;

	if (N_STAB & n_type)
		type = '-';
	if (N_PEXT & n_type)
		type = 'u'; //TODO find correct letter for this
	if (n_type_data == N_UNDF)
		type = 'u';
	else if (n_type_data == N_ABS)
		type = 'a';
	else if (n_type_data == N_SECT)
		type = section_table_read(n_sect);
	else if (n_type_data == N_PBUD)
		type = 'u'; //TODO find correct letter for this
	else if (n_type_data == N_INDR)
		type = 'i';
	else if (n_type_data == N_INDR)//TODO find correct condition for this
		type = 'c';

	if (N_EXT & n_type) /* external to UPPERCASE */
		type = ft_toupper(type);
	return (type);
}
//------------TMP unsafe

static bool		manage_symtab_64(const size_t offset)
{
	struct symtab_command	*sym;
	char					*stringtable;
	struct nlist_64			*nlist;
	uint32_t				nsysms;
	uint32_t				i;

	sym = safe(sym->symoff, sizeof(*sym));

	nsysms = sym->nsysms;

	nlist = safe(sym->symoff, sizeof(*nlist) * nsysms);
	stringtable = safe(sym->stroff, sizeof(*stringtable));

	i = 0;
	//------------TMP unsafe
	while (i < nsysms)
	{
		char		type = get_type(nlist[i].n_type, nlist[i].n_sect, ptr);
		uint64_t	offset =  nlist[i].n_value;
		char		*str = stringtable + nlist[i].n_un.n_strx;

		if (offset)
			ft_printf("%016lx %c %s\n", offset, type, str);
		else
			ft_printf("                 %c %s\n", type, str);
		i++;
	}
	//------------TMP unsafe
}

/*
** t_gatherer nm_gatherer
*/

static bool		nm_gatherer(const bool is_64)
{
	static const uint32_t		lc_seg[2] = {LC_SEGMENT, LC_SEGMENT_64};
	static const t_lc_manager	segment_manager[2] =
	{
		&manage_segment,
		&manage_segment_64
	};
	static const t_lc_manager	symtab_manager[2] =
	{
		&manage_symtab,
		&manage_symtab_64
	};

	// enumerate sections
	iterate_lc(is_64, lc_seg[is_64], segment_manager[is_64]);

	// find symtab
	return (iterate_lc(is_64, LC_SYMTAB, symtab_manager[is_64]));
}

/*
** Muhahahahaahahahahaha >:D-
*/

int				main(int ac, char **av)
{
	if (ac < 2)
	{
		extract_macho(DEFAULT_TARGET, &nm_gatherer);
	}
	else if (*++av)
	{
		extract_macho(*av, &nm_gatherer);
		main(ac, av);
	}
	return (0);
}
