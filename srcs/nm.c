/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 17:44:07 by agrumbac          #+#    #+#             */
/*   Updated: 2018/04/24 22:40:19 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

static bool		nm_gatherer(const bool is_64)
{
	return (errors(ERR_FILE, "you're nm'd"));//TODO actually manage smth
}

int				main(int ac, char **av)
{
	if (ac < 2)
	{
		extract_macho(DEFAULT_TARGET, &nm_gatherer);
	}
	while (*++av)
	{
		extract_macho(*av, &nm_gatherer);
	}
	return (0);
}
