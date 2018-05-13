/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm_sections_character_table.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/10 21:04:50 by agrumbac          #+#    #+#             */
/*   Updated: 2018/05/13 17:03:09 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"
#include "nm_display.h"

/*
** nm_sections_character_table:
** reset, read, write access to sections_character[256]
** reset -> knows that there is no 0th section, used as reset command
** read  -> has a secret read backdoor: when offset has FIRST_BIT_ON_64
** write -> can polymorph into t_section_manager to comply with iterate_sections
*/

char		nm_sections_character_table(const size_t offset)
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
	return (true);
}

/*
** get_type character
*/

static char	get_type(const uint64_t n_value, const uint8_t n_type, \
				const uint8_t n_sect, const uint16_t n_desc)
{
	const int	n_type_field = N_TYPE & n_type;
	char		type = '?';

	if (N_STAB & n_type)
		type = '-';
	else if (n_type_field == N_UNDF)
		type = n_value ? 'c' : 'u';
	else if (n_type_field == N_ABS)
		type = 'a';
	else if (n_type_field == N_SECT && \
		!(type = nm_sections_character_table(FIRST_BIT_ON_64 | n_sect)))
		type = '?';
	else if (n_type_field == N_PBUD)
		type = 'u';
	else if (n_type_field == N_INDR)
		type = 'i';
	else if (n_desc & N_WEAK_REF)
		type = 'W';

	//if external set uppercase
	if (N_EXT & n_type)
		type = ft_toupper(type);

	return (type);
}

/*
** store symbol data
*/

static const char g_nm_invalid_string[18] = "bad string index";

void		nm_extract_values(const struct nlist *nlist, const uint64_t n_value,
				const struct symtab_command *sym, t_sym_sort *sorted_symbols)
{
	const uint32_t	stroff = endian_4(sym->stroff);
	const uint32_t	strsize = endian_4(sym->strsize);
	const uint32_t	str_offset = stroff + endian_4(nlist->n_un.n_strx);
	t_symbol		new_symbol;

	new_symbol.str_max_size = stroff + strsize - str_offset;
	new_symbol.string = safe(str_offset, new_symbol.str_max_size);
	new_symbol.type = get_type(n_value, nlist->n_type, nlist->n_sect, \
		endian_2(nlist->n_desc));
	new_symbol.offset = n_value;

	// check if str is not in stringtable
	if (str_offset < stroff || str_offset > stroff + strsize)
		new_symbol.string = (char *)g_nm_invalid_string;

	nm_store_value(sorted_symbols, &new_symbol);
}
