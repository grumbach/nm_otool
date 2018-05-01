/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   otool.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/23 17:42:24 by agrumbac          #+#    #+#             */
/*   Updated: 2018/05/02 21:47:41 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

static void		hexdump_text(const char *text, uint64_t offset, uint64_t size)
{
	uint64_t			i;
	uint64_t			j;

	ft_printf("Contents of (%s,%s) section\n", OTOOL_SEGMENT, OTOOL_SECTION);
	i = 0;
	while (i < size)
	{
		ft_printf("%016llx\t", offset + i);
		j = 0;
		while (j < 0x10 && i + j < size)
			ft_printf("%02hhx ", text[i + j++]);
		ft_printf("\n");
		i += 0x10;
	}
}

/*
** t_section_manager
*/

static bool		print_section(const size_t offset, const uint32_t section_index)
{
	struct section		*sect;
	char				*text;

	if (!(sect = safe(offset, sizeof(*sect))))
		return (errors(ERR_FILE, "bad section offset"));
	if (!(text = safe(endian_4(sect->offset), endian_4(sect->size))))
		return (errors(ERR_FILE, "bad text offset"));

	hexdump_text(text, endian_4(sect->addr), endian_4(sect->size));
	return (BOOL_TRUE);
}

static bool		print_section_64(const size_t offset, \
					const uint32_t section_index)
{
	struct section_64	*sect;
	char				*text;

	if (!(sect = safe(offset, sizeof(*sect))))
		return (errors(ERR_FILE, "bad section offset"));
	if (!(text = safe(endian_4(sect->offset), endian_8(sect->size))))
		return (errors(ERR_FILE, "bad text offset"));

	hexdump_text(text, endian_8(sect->addr), endian_8(sect->size));
	return (BOOL_TRUE);
}

/*
** t_lc_manager manage_segment, manage_segment_64
*/

static bool		manage_segment(const size_t offset)
{
	if (!(iterate_sections(offset, OTOOL_SEGMENT, OTOOL_SECTION, \
		&print_section)))
		return (errors(ERR_THROW, "in _manage_segment"));
	return (BOOL_TRUE);
}

static bool		manage_segment_64(const size_t offset)
{
	if (!(iterate_sections_64(offset, OTOOL_SEGMENT, OTOOL_SECTION, \
		&print_section_64)))
		return (errors(ERR_THROW, "in _manage_segment_64"));
	return (BOOL_TRUE);
}

/*
** t_gatherer otool_gatherer
*/

static bool		otool_gatherer(const bool is_64)
{
	static const uint32_t		lc[2] = {LC_SEGMENT, LC_SEGMENT_64};
	static const t_lc_manager	manager[2] =
	{
		&manage_segment,
		&manage_segment_64
	};

	return iterate_lc(is_64, lc[is_64], manager[is_64]);
}

/*
** Muhahahahaahahahahaha >:D-
*/

int				main(int ac, char **av)
{
	if (ac < 2)
	{
		ft_printf("%s:\n", DEFAULT_TARGET);
		extract_macho(DEFAULT_TARGET, &otool_gatherer);
	}
	else if (*++av)
	{
		ft_printf("%s:\n", *av);
		extract_macho(*av, &otool_gatherer);
		main(ac, av);
	}
	return (0);
}
