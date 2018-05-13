/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_macho.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/29 21:37:12 by agrumbac          #+#    #+#             */
/*   Updated: 2018/05/13 19:53:48 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "archive.h"

static bool		known_magic_retriever_64(uint32_t nfat_arch, size_t offset, \
					size_t *target_offset, uint32_t *magic)
{
	struct fat_arch_64		*arch;
	uint32_t				*magic_ptr;

	// loop through architectures looking for known magic
	while (nfat_arch--)
	{
		//retrieve safe pointers
		if (!(arch = safe(offset, sizeof(*arch))))
			return (errors(ERR_FILE, "bad fat arch offset"));
		if (!(magic_ptr = safe(endian_8(arch->offset), sizeof(*magic_ptr))))
			return (errors(ERR_FILE, "bad fat arch magic offset"));

		// check for known magic
		if (*magic_ptr == MH_CIGAM_64 || *magic_ptr == MH_MAGIC_64)
			*target_offset = endian_8(arch->offset);
		else if (!(*target_offset) && (*magic_ptr == MH_CIGAM || \
			*magic_ptr == MH_MAGIC))
			*target_offset = endian_8(arch->offset);
		offset += sizeof(*arch);
	}
	if (!(*magic = *(uint32_t*)safe(*target_offset, sizeof(*magic))))
		return (errors(ERR_FILE, "bad fat arch magic offset"));
	return (true);
}

static bool		known_magic_retriever_32(uint32_t nfat_arch, size_t offset, \
					size_t *target_offset, uint32_t *magic)
{
	struct fat_arch			*arch;
	uint32_t				*magic_ptr;

	// loop through architectures looking for known magic
	while (nfat_arch--)
	{
		//retrieve safe pointers
		if (!(arch = safe(offset, sizeof(*arch))))
			return (errors(ERR_FILE, "bad fat arch offset"));
		if (!(magic_ptr = safe(endian_4(arch->offset), sizeof(*magic_ptr))))
			return (errors(ERR_FILE, "bad fat arch magic offset"));

		// check for known magic
		if (*magic_ptr == MH_CIGAM_64 || *magic_ptr == MH_MAGIC_64)
			*target_offset = endian_4(arch->offset);
		else if (!(*target_offset) && (*magic_ptr == MH_CIGAM || \
			*magic_ptr == MH_MAGIC))
			*target_offset = endian_4(arch->offset);
		offset += sizeof(*arch);
	}
	if (!(*magic = *(uint32_t*)safe(*target_offset, sizeof(*magic))))
		return (errors(ERR_FILE, "bad fat arch magic offset"));
	return (true);
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
	size_t								target_offset;
	uint32_t							magic;

	//read header
	target_offset = 0;
	if (!(header = safe(0, sizeof(*header))))
		return (errors(ERR_FILE, "missing fat header"));

	//find the magic
	if (!find_the_magic[is_64](endian_4(header->nfat_arch), \
		sizeof(*header), &target_offset, &magic))
		return (errors(ERR_THROW, __func__));

	//do the mach-o parsing magic
	if (!target_offset)
		return (errors(ERR_FILE, "no known architectures found"));
	set_start_offset(target_offset);
	endian_little_mode(magic == MH_CIGAM || magic == MH_CIGAM_64);
	return (func_ptr(magic == MH_MAGIC_64 || magic == MH_CIGAM_64));
}

/*
** extract_macho and call a t_gatherer
*/

bool			extract_macho(const char *filename, t_gatherer func_ptr)
{
	uint32_t	magic;
	bool		return_value;

	//map file
	if (!read_file(filename))
		return (errors(ERR_THROW, __func__));
	if (!(magic = *(uint32_t*)safe(0, sizeof(magic))))
		return (errors(ERR_FILE, "missing magic"));

	//detect endian
	endian_little_mode(magic == FAT_CIGAM || magic == FAT_CIGAM_64 || \
		magic == MH_CIGAM || magic == MH_CIGAM_64 || magic == ARCHIVE_CIGAM);

	//check magic
	if (magic == ARCHIVE_MAGIC || magic == ARCHIVE_CIGAM)
		return_value = manage_archive(func_ptr, filename);
	else if (magic == MH_MAGIC || magic == MH_CIGAM)
		return_value = func_ptr(false);
	else if (magic == MH_MAGIC_64 || magic == MH_CIGAM_64)
		return_value = func_ptr(true);
	else if (magic == FAT_MAGIC || magic == FAT_CIGAM)
		return_value = manage_fat(func_ptr, false);
	else if (magic == FAT_MAGIC_64 || magic == FAT_CIGAM_64)
		return_value = manage_fat(func_ptr, true);
	else
		return_value = errors(ERR_FILE, "unknown file format");

	//unmap file
	if (!free_file())
		return (errors(ERR_THROW, __func__));
	return (return_value);
}
