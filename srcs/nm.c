/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 17:44:07 by agrumbac          #+#    #+#             */
/*   Updated: 2018/05/10 02:40:58 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

/*
** sections_character_table:
** reset, read, write access to sections_character[256]
** reset -> knows that there is no 0th section, used as reset command
** read  -> has a secret read backdoor: when offset has FIRST_BIT_ON_64
** write -> can polymorph into t_section_manager to comply with iterate_sections
*/

static char		sections_character_table(const size_t offset)
{
	static char				sections_character[256];
	static uint8_t			sections = 0;
	char					*sectname;

	//reset state for next when offset == 0
	if (offset == 0)
	{
		ft_bzero(sections_character, 256);
		sections = 0;
	}
	//read character assuming offsets are always smaller than 1 << 63 ;)
	else if (FIRST_BIT_ON_64 & offset)
	{
		return (sections_character[offset & 0xff]);
	}
	//write character
	else
	{
		if (!(sectname = safe(offset, 16)))
			return (errors(ERR_FILE, "bad section name offset"));

		if (!ft_strncmp(sectname, "__text", 6))
			sections_character[++sections] = 't';
		else if (!ft_strncmp(sectname, "__data", 6))
			sections_character[++sections] = 'd';
		else if (!ft_strncmp(sectname, "__bss", 5))
			sections_character[++sections] = 'b';
		else
			sections_character[++sections] = 's';
	}
	return (BOOL_TRUE);
}

/*
** get_type character
*/

static char		get_type(const uint8_t n_type, const uint8_t n_sect, \
					uint64_t n_value)
{
	const int	n_type_field = N_TYPE & n_type;
	char		type = '?';

	//special types
	if (N_STAB & n_type)
		type = '-';
	if (N_PEXT & n_type)
		type = 'u';
	//n_type_field types
	if (n_type_field == N_UNDF)
		type = n_value ? 'c' : 'u';
	else if (n_type_field == N_ABS)
		type = 'a';
	else if (n_type_field == N_SECT)
	{
		if (!(type = sections_character_table(FIRST_BIT_ON_64 | n_sect)))
			return (errors(ERR_THROW, __func__));
	}
	else if (n_type_field == N_PBUD)
		type = 'u';
	else if (n_type_field == N_INDR)
		type = 'i';
	//if external set uppercase
	if (N_EXT & n_type)
		type = ft_toupper(type);

	return (type);
}

/*
** store symbol data
*/

static bool		store_values(const struct nlist *nlist, const uint64_t n_value, \
					const uint32_t stroff, const uint32_t strsize)
{
	uint32_t	str_offset = stroff + endian_4(nlist->n_un.n_strx);
	uint32_t	str_max_size = stroff + strsize - str_offset;
	char		*str = safe(str_offset, str_max_size);
	char		type = get_type(nlist->n_type, nlist->n_sect, n_value);

	// check if type is invalid
	if (!type)
		return (errors(ERR_THROW, __func__));
	// check if stringtable was invalid to begin with
	if (!safe(stroff, strsize))
		return (errors(ERR_FILE, "bad stringtable offset or size"));
	// check if str is not safe or not in stringtable
	if (!str || str_offset < stroff || str_offset > stroff + strsize)
	{
		str = "bad string offset";
		str_max_size = 17;
	}

	//TODO store values instead of printing
	if (n_value)
		ft_printf("%016lx %c %.*s\n", n_value, type, str_max_size, str);
	else
		ft_printf("                 %c %.*s\n", type, str_max_size, str);
	return (BOOL_TRUE);
}

/*
** t_lc_manager symtab_manager: manage_symtab, manage_symtab_64
*/

static bool		manage_symtab_32(const size_t offset)
{
	struct symtab_command	*sym;
	struct nlist			*nlist;
	uint32_t				nsyms;
	uint32_t				i;

	//retrieve pointers
	if (!(sym = safe(offset, sizeof(*sym))))
		return (errors(ERR_FILE, "bad symtab command offset"));
	nsyms = endian_4(sym->nsyms);
	if (!(nlist = safe(endian_4(sym->symoff), sizeof(*nlist) * nsyms)))
		return (errors(ERR_FILE, "bad symbol table offset or size"));

	//for each symbol
	i = 0;
	while (i < nsyms)
	{
		if (!(store_values(&nlist[i], endian_4(nlist[i].n_value), \
			endian_4(sym->stroff), endian_4(sym->strsize))))
			return (errors(ERR_THROW, __func__));
		i++;
	}
	//TODO sort stored values, print and free
	return (BOOL_TRUE);
}

static bool		manage_symtab_64(const size_t offset)
{
	struct symtab_command	*sym;
	struct nlist_64			*nlist;
	uint32_t				nsyms;
	uint32_t				i;

	//retrieve pointers
	if (!(sym = safe(offset, sizeof(*sym))))
		return (errors(ERR_FILE, "bad symtab command offset"));
	nsyms = endian_4(sym->nsyms);
	if (!(nlist = safe(endian_4(sym->symoff), sizeof(*nlist) * nsyms)))
		return (errors(ERR_FILE, "bad symbol table offset or size"));

	//for each symbol
	i = 0;
	while (i < nsyms)
	{
		if (!(store_values((struct nlist*)&nlist[i], endian_8(nlist[i].n_value), \
			endian_4(sym->stroff), endian_4(sym->strsize))))
			return (errors(ERR_THROW, __func__));
		i++;
	}
	//TODO sort stored values, print and free
	return (BOOL_TRUE);
}

/*
** -----------------------------------------------------------------------------
*/

/*
** t_lc_manager segment_manager: manage_segment, manage_segment_64
**   fills sections_character_table
*/

static bool		manage_segment_32(const size_t offset)
{
	if (!(iterate_sections(offset, NULL, NULL, \
		(t_section_manager)&sections_character_table)))
		return (errors(ERR_THROW, __func__));
	return (BOOL_TRUE);
}

static bool		manage_segment_64(const size_t offset)
{
	if (!(iterate_sections_64(offset, NULL, NULL, \
		(t_section_manager)&sections_character_table)))
		return (errors(ERR_THROW, __func__));
	return (BOOL_TRUE);
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
	sections_character_table(0);
	// fill sections table
	if (!(iterate_lc(is_64, lc_seg[is_64], segment_manager[is_64])))
		return (errors(ERR_THROW, __func__));
	// manage symtab
	if (!(iterate_lc(is_64, LC_SYMTAB, symtab_manager[is_64])))
		return (errors(ERR_THROW, __func__));

	return (BOOL_TRUE);
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
