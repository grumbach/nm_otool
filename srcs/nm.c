/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 17:44:07 by agrumbac          #+#    #+#             */
/*   Updated: 2018/04/29 22:03:04 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

/*
** t_lc_manager ...
*/

static bool		manage_symtab(const size_t offset)
{

}

static bool		manage_symtab_64(const size_t offset)
{

}

/*
** t_gatherer nm_gatherer
*/

static bool		nm_gatherer(const bool is_64)
{
	static const uint32_t		lc[2] = {LC_SEGMENT, LC_SEGMENT_64};
	static const t_lc_manager	manager[2] =
	{
		&manage_symtab,
		&manage_symtab_64
	};

	// enumerate sections, only sect
	iterate_lc(is_64, lc[is_64], );

	//
	return (iterate_lc(is_64, LC_SYMTAB, manager[is_64]));
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
