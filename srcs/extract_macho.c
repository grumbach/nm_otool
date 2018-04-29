/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_macho.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/29 21:37:12 by agrumbac          #+#    #+#             */
/*   Updated: 2018/04/29 22:04:30 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

/*
** fat managers, call a t_gatherer for the default arch (_64 then _32)
*/

static bool		manage_fat(t_gatherer func_ptr)//TODO not operationnal
{
	struct fat_header		*header;
	struct fat_arch			*arch;
	uint32_t				nfat_arch;
	size_t					offset;

	if (!(header = safe(0, sizeof(*header))))
		return (errors(ERR_FILE, "missing fat header"));
	offset = sizeof(*header);
	nfat_arch = header->nfat_arch;
	while (nfat_arch--)
	{
		if (!(arch = safe(offset, sizeof(*arch))))
			return (errors(ERR_FILE, "bad fat arch offset"));
		func_ptr(BOOL_FALSE);//TODO call for the right one (currently calling all!)
		offset += arch->size;
	}
	return (BOOL_TRUE);
}

static bool		manage_fat_64(t_gatherer func_ptr)//TODO not operationnal
{
	struct fat_header		*header;
	struct fat_arch_64		*arch;
	uint32_t				nfat_arch;
	size_t					offset;

	if (!(header = safe(0, sizeof(*header))))
		return (errors(ERR_FILE, "missing fat header"));
	offset = sizeof(*header);
	nfat_arch = header->nfat_arch;
	while (nfat_arch--)
	{
		if (!(arch = safe(offset, sizeof(*arch))))
			return (errors(ERR_FILE, "bad fat arch offset"));
		func_ptr(BOOL_TRUE);//TODO call for the right one (currently calling all!)
		offset += arch->size;
	}
	return (BOOL_TRUE);
}

/*
** archive manager: calls a t_gatherer for each object found
*/

static bool		manage_archive(t_gatherer func_ptr)
{
	return (errors(ERR_FILE, "you're arch"));//TODO actually manage smth
}

/*
** extract_macho and call a t_gatherer
*/

bool			extract_macho(const char *filename, t_gatherer func_ptr)
{
	uint32_t	*magic;

	if (!read_file(filename))
		return (errors(ERR_THROW, "in _read_file"));
	if (!(magic = safe(0, sizeof(uint32_t))))
		return (errors(ERR_FILE, "missing the magic"));

	if (*magic == ARCHIVE_MAGIC)
		return (manage_archive(func_ptr));

	ft_printf("%s:\n", filename);

	if (*magic == MH_MAGIC || *magic == MH_CIGAM)
		return (func_ptr(BOOL_FALSE));
	if (*magic == MH_MAGIC_64 || *magic == MH_CIGAM_64)
		return (func_ptr(BOOL_TRUE));
	if (*magic == FAT_MAGIC || *magic == FAT_CIGAM)
		return (manage_fat(func_ptr));
	if (*magic == FAT_MAGIC_64 || *magic == FAT_CIGAM_64)
		return (manage_fat_64(func_ptr));

	return (errors(ERR_FILE, "unknown file format"));

	//TODO munmap(safe(0, 0));
}
