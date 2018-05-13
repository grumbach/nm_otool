/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   otool.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/23 17:42:24 by agrumbac          #+#    #+#             */
/*   Updated: 2018/05/13 21:31:00 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "otool_display.h"

/*
** t_lc_manager manage_segment, manage_segment_64
*/

static bool		manage_segment(const size_t offset)
{
	if (!(iterate_sections(offset, OTOOL_SEGMENT, OTOOL_SECTION, \
		&print_section)))
		return (errors(ERR_THROW, __func__));
	return (true);
}

static bool		manage_segment_64(const size_t offset)
{
	if (!(iterate_sections_64(offset, OTOOL_SEGMENT, OTOOL_SECTION, \
		&print_section_64)))
		return (errors(ERR_THROW, __func__));
	return (true);
}

/*
** t_gatherer otool_gatherer
*/

static bool		otool_gatherer(const bool is_64)
{
	static const uint32_t		lc[2] = {LC_SEGMENT, LC_SEGMENT_64};
	static const t_lc_manager	manager[2] = {
		&manage_segment,
		&manage_segment_64
	};

	return (iterate_lc(is_64, lc[is_64], manager[is_64]));
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
