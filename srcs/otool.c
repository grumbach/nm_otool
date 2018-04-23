/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   otool.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/23 17:42:24 by agrumbac          #+#    #+#             */
/*   Updated: 2018/04/24 00:01:23 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

static bool		otool(void)
{
	return (errors(ERR_FILE, "you're otooled"));//TODO actually manage smth
}

int				main(int ac, char **av)
{
	if (ac < 2)
	{
		extract_macho(DEFAULT_TARGET, &otool);
	}
	while (*++av)
	{
		extract_macho(*av, &otool);
	}
	return (0);
}
