/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_macho.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/29 21:37:12 by agrumbac          #+#    #+#             */
/*   Updated: 2018/05/10 02:21:01 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

static bool		known_magic_retriever_64(uint32_t nfat_arch, size_t offset, \
					bool *is_little_endian, uint64_t *target_offset)
{
	struct fat_arch_64		*arch;
	uint32_t				*magic;

	// loop through architectures looking for known magic
	while (nfat_arch--)
	{
		//retrieve safe pointers
		if (!(arch = safe(offset, sizeof(*arch))))
			return (errors(ERR_FILE, "bad fat arch offset"));
		if (!(magic = safe(endian_8(arch->offset), sizeof(*magic))))
			return (errors(ERR_FILE, "bad fat arch magic offset"));

		// check for known magic
		if ((*is_little_endian = !(*magic == MH_MAGIC_64)) || *magic == MH_CIGAM_64)
			*target_offset = endian_8(arch->offset);
		else if (!target_offset && \
			((*is_little_endian = !(*magic == MH_MAGIC)) || *magic == MH_CIGAM))
			*target_offset = endian_8(arch->offset);
		offset += endian_8(arch->size);
	}
	return (BOOL_TRUE);
}

static bool		known_magic_retriever_32(uint32_t nfat_arch, size_t offset, \
					bool *is_little_endian, uint64_t *target_offset)
{
	struct fat_arch			*arch;
	uint32_t				*magic;

	// loop through architectures looking for known magic
	while (nfat_arch--)
	{
		//retrieve safe pointers
		if (!(arch = safe(offset, sizeof(*arch))))
			return (errors(ERR_FILE, "bad fat arch offset"));
		if (!(magic = safe(endian_4(arch->offset), sizeof(*magic))))
			return (errors(ERR_FILE, "bad fat arch magic offset"));

		// check for known magic
		if ((*is_little_endian = !(*magic == MH_MAGIC_64)) || *magic == MH_CIGAM_64)
			*target_offset = endian_4(arch->offset);
		else if (!target_offset && \
			((*is_little_endian = !(*magic == MH_MAGIC)) || *magic == MH_CIGAM))
			*target_offset = endian_4(arch->offset);
		offset += endian_4(arch->size);
	}
	return (BOOL_TRUE);
}

/*
** fat management, call a t_gatherer for the favorite known arch (_64 then _32)
*/

static bool		manage_fat(t_gatherer func_ptr, const bool is_64)
{
	static const t_fat_magic_retriever	find_the_magic[2] =
	{
		&known_magic_retriever_32,
		&known_magic_retriever_64
	};
	struct fat_header					*header;
	bool								is_little_endian;
	uint64_t							target_offset;

	//read header
	target_offset = 0;
	if (!(header = safe(0, sizeof(*header))))
		return (errors(ERR_FILE, "missing fat header"));

	//find the magic
	if (!find_the_magic[is_64](endian_4(header->nfat_arch), \
		sizeof(*header), &is_little_endian, &target_offset))
		return (errors(ERR_THROW, "in _manage_fat"));

	//do the mach-o parsing magic
	if (!target_offset)
		return (errors(ERR_FILE, "no known architectures found"));
	set_start_offset(target_offset);
	endian_little_mode(is_little_endian);
	return (func_ptr(BOOL_FALSE));
}

/*
** extract_macho and call a t_gatherer
*/

bool			extract_macho(const char *filename, t_gatherer func_ptr)
{
	uint32_t	*magic;
	bool		return_value;

	//map file
	if (!read_file(filename))
		return (errors(ERR_THROW, "in _read_file"));
	if (!(magic = safe(0, sizeof(uint32_t))))
		return (errors(ERR_FILE, "missing magic"));

	//detect endian
	endian_little_mode(*magic == FAT_CIGAM || *magic == FAT_CIGAM_64 || \
		*magic == MH_CIGAM || *magic == MH_CIGAM_64);

	//check magic
	if (*magic == ARCHIVE_MAGIC)
		return_value = 42;//TODO manage_archive(func_ptr, filename);
	else if (*magic == MH_MAGIC || *magic == MH_CIGAM)
		return_value = func_ptr(BOOL_FALSE);
	else if (*magic == MH_MAGIC_64 || *magic == MH_CIGAM_64)
		return_value = func_ptr(BOOL_TRUE);
	else if (*magic == FAT_MAGIC || *magic == FAT_CIGAM)
		return_value = manage_fat(func_ptr, BOOL_FALSE);
	else if (*magic == FAT_MAGIC_64 || *magic == FAT_CIGAM_64)
		return_value = manage_fat(func_ptr, BOOL_TRUE);
	else
		return_value = errors(ERR_FILE, "unknown file format");

	//unmap file
	if (!free_file())
		return (errors(ERR_THROW, "in _free_file"));
	return (return_value);
}
