/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   archive.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/12 21:31:43 by agrumbac          #+#    #+#             */
/*   Updated: 2018/05/13 19:53:54 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "archive.h"

static bool     loop_archive_sorted(t_archive_header *header, \
					t_archive_symtab *symtab_array)
{
	const uint32_t		base_offset = sizeof(*header) - sizeof(uint32_t);
	uint64_t			i;
	t_archive_symtab	tmp;
	void				*src;

	if (!(src = safe(base_offset, header->symbol_tab_size)))
		return (errors(ERR_FILE, "bad archive symtab offset"));
	ft_memcpy(symtab_array, src, header->symbol_tab_size);
	i = 0;
	while (i + 1 < header->symbol_tab_size / sizeof(t_archive_symtab))
	{
		if (symtab_array[i].sym_offset > symtab_array[i + 1].sym_offset)
		{
			tmp = symtab_array[i];
			symtab_array[i] = symtab_array[i + 1];
			symtab_array[i + 1] = tmp;
			i = 0;
		}
		else
			++i;
	}
	return (true);
}

static bool     parse_object_header(t_gatherer func_ptr, uint32_t offset, \
					const char *filename, bool is_little_endian)
{
	t_object_header		*obj_header;
	uint32_t			*magic;

	if (!(obj_header = safe(offset, sizeof(*obj_header))))
		return (errors(ERR_FILE, "bad object header offset"));
	offset += sizeof(*obj_header);
	ft_printf("\n%s(%.20s):\n", filename, obj_header->long_name);
	if (!(magic = safe(offset, sizeof(*magic))))
		return (errors(ERR_FILE, "bad object magic offset"));
	endian_little_mode(*magic == MH_CIGAM_64 || *magic == MH_CIGAM);
	set_start_offset(offset);
	func_ptr(*magic == MH_MAGIC_64 || *magic == MH_CIGAM_64);
	set_start_offset(0);
	endian_little_mode(is_little_endian);
	return (true);
}

/*
** archive manager: calls a t_gatherer for each object found
*/

bool            manage_archive(t_gatherer func_ptr, const char *filename)
{
	t_archive_header	*header;
	t_archive_symtab	*symtab_arr;
	uint32_t			i;
	bool				success;

	ft_printf("Archive : %s\n", filename);
	if (!(header = safe(0, sizeof(*header))))
		return (errors(ERR_FILE, "missing archive header"));
	if (!(symtab_arr = malloc(header->symbol_tab_size)))
		return (errors(ERR_SYS, "malloc failed"));
	success = loop_archive_sorted(header, symtab_arr);
	i = 0;
	while (success && i < header->symbol_tab_size / sizeof(t_archive_symtab))
	{
		success = parse_object_header(func_ptr, symtab_arr[i].obj_offset, \
			filename, (header->magic == ARCHIVE_CIGAM));
		i++;
	}
	free(symtab_arr);
	return (success || errors(ERR_THROW, __func__));
}
