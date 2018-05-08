/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 17:44:07 by agrumbac          #+#    #+#             */
/*   Updated: 2018/05/08 19:38:40 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

/*
** t_section_manager (-> polymorphs into): sections_character_table
**   fills, reads and resets section_table
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
	else if (offset & 0x8000000000000000L)
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

static char		get_type(const uint8_t n_type, const uint8_t n_sect)
{
	const int	n_type_field = N_TYPE & n_type;
	char		type = ' ';// TODO check default type letter

	//special types
	if (N_STAB & n_type)
		type = '-';
	if (N_PEXT & n_type)
		type = 'u'; //TODO find correct letter for private external symbol bit

	//n_type_field types
	if (n_type_field == N_UNDF)
		type = 'u';
	else if (n_type_field == N_ABS)
		type = 'a';
	else if (n_type_field == N_SECT)
	{//TODO clean 0x8000000000000000 and make for _32
		if (!(type = sections_character_table(0x8000000000000000L | n_sect)))
			return (errors(ERR_THROW, "in _get_type"));
	}
	else if (n_type_field == N_PBUD)
		type = 'u'; //TODO find correct letter for prebound undefined (def in dylib)
	else if (n_type_field == N_INDR)
		type = 'i';

	//if external set uppercase
	if (N_EXT & n_type)
		type = ft_toupper(type);

	return (type);
}

/*
** t_lc_manager symtab_manager: manage_symtab, manage_symtab_64
*/

static bool		manage_symtab_32(const size_t offset)
{
	// same as 64
}

//TODO endian management for this
static bool		manage_symtab_64(const size_t offset)
{
	struct symtab_command	*sym;
	char					*stringtable;
	struct nlist_64			*nlist;
	uint32_t				nsyms;
	uint32_t				i;

	//retrieve pointers
	if (!(sym = safe(offset, sizeof(*sym))))
		return (errors(ERR_FILE, "bad symtab command offset"));
	nsyms = sym->nsyms;
	if (!(nlist = safe(sym->symoff, sizeof(*nlist) * nsyms)))
		return (errors(ERR_FILE, "bad symbol table offset"));
	if (!(stringtable = safe(sym->stroff, sym->strsize-1)))//TODO - -1
		return (errors(ERR_FILE, "bad stringtable offset or size"));

	//for each symbol
	i = 0;
	while (i < nsyms)
	{
		//collect data
		char				type = get_type(nlist[i].n_type, nlist[i].n_sect);
		uint64_t			offset =  nlist[i].n_value;
		char				*str = stringtable + nlist[i].n_un.n_strx;
		size_t				str_max_size = sym->strsize - (size_t)(str - sym->stroff);

		//check for bad string offsets
		// if ((size_t)str < sym->stroff || (size_t)str - sym->stroff > sym->strsize)
			// return (errors(ERR_FILE, "bad symbol string offset"));
		//check for invalid types
		// if (!type)
			// return (errors(ERR_THROW, "in _manage_symtab_64"));

		//TODO store values instead of printing
		if (offset)
			ft_printf("%016lx %c %.*s\n", offset, type, str_max_size, str);
		else
			ft_printf("                 %c %.*s\n", type, str_max_size, str);
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
		return (errors(ERR_THROW, "in _manage_segment"));
	return (BOOL_TRUE);
}

static bool		manage_segment_64(const size_t offset)
{
	if (!(iterate_sections_64(offset, NULL, NULL, \
		(t_section_manager)&sections_character_table)))
		return (errors(ERR_THROW, "in _manage_segment"));
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
		return (errors(ERR_THROW, "in _nm_gatherer"));
	// manage symtab
	if (!(iterate_lc(is_64, LC_SYMTAB, symtab_manager[is_64])))
		return (errors(ERR_THROW, "in _nm_gatherer"));

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
