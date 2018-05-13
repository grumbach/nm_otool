/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_macho.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/23 22:30:11 by agrumbac          #+#    #+#             */
/*   Updated: 2018/05/13 21:26:05 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

/*
** iterate_lc()
** iterate through the load_commands looking for target
** calls lc_manager for each found load_command
*/

bool			iterate_lc(const bool is_64, const uint32_t target, \
					t_lc_manager func_ptr)
{
	static const size_t		mach_size[2] = {
		sizeof(struct mach_header),
		sizeof(struct mach_header_64)
	};
	struct mach_header		*macho;
	struct load_command		*lc;
	uint32_t				ncmds;
	size_t					offset;

	offset = mach_size[is_64];
	if (!(macho = safe(0, sizeof(*macho))))
		return (errors(ERR_FILE, "bad macho header offset"));
	if (!(lc = safe(offset, sizeof(*lc))))
		return (errors(ERR_FILE, "bad load command offset"));
	ncmds = endian_4(macho->ncmds);
	while (ncmds--)
	{
		if (endian_4(lc->cmd) == target && !func_ptr(offset))
			return (errors(ERR_THROW, __func__));
		offset += endian_4(lc->cmdsize);
		if (!(lc = safe(offset, sizeof(*lc))))
			return (errors(ERR_FILE, "bad load command offset"));
	}
	return (true);
}

/*
** iterate_sections_64() and iterate_sections()
** iterate through the sections looking for target_segment and target_section
** a NULL value for target_segment or target_section means all
** calls section_manager for each found section
*/

bool			iterate_sections(const size_t start_offset, \
					const char *target_segment, const char *target_section, \
					t_section_manager func_ptr)
{
	struct segment_command		*seg;
	struct section				*sect;
	size_t						offset;
	uint32_t					nsects;

	if (!(seg = safe(start_offset, sizeof(*seg))))
		return (errors(ERR_FILE, "bad segment offset"));
	offset = start_offset + sizeof(*seg);
	if (!(sect = safe(offset, sizeof(*sect))))
		return (errors(ERR_FILE, "bad section offset"));
	nsects = endian_4(seg->nsects);
	while (nsects--)
	{
		if ((!target_section || !ft_strncmp(sect->sectname, target_section, 16))
		&& (!target_segment || !ft_strncmp(sect->segname, target_segment, 16))
		&& !func_ptr(offset))
			return (errors(ERR_THROW, __func__));
		offset += sizeof(*sect);
		if (!(sect = safe(offset, sizeof(*sect))))
			return (errors(ERR_FILE, "bad section offset"));
	}
	return (true);
}

bool			iterate_sections_64(const size_t start_offset, \
					const char *target_segment, const char *target_section, \
					t_section_manager func_ptr)
{
	struct segment_command_64	*seg;
	struct section_64			*sect;
	size_t						offset;
	uint32_t					nsects;

	if (!(seg = safe(start_offset, sizeof(*seg))))
		return (errors(ERR_FILE, "bad segment offset"));
	offset = start_offset + sizeof(*seg);
	if (!(sect = safe(offset, sizeof(*sect))))
		return (errors(ERR_FILE, "bad section offset"));
	nsects = endian_4(seg->nsects);
	while (nsects--)
	{
		if ((!target_section || !ft_strncmp(sect->sectname, target_section, 16))
		&& (!target_segment || !ft_strncmp(sect->segname, target_segment, 16))
		&& !func_ptr(offset))
			return (errors(ERR_THROW, __func__));
		offset += sizeof(*sect);
		if (!(sect = safe(offset, sizeof(*sect))))
			return (errors(ERR_FILE, "bad section offset"));
	}
	return (true);
}
