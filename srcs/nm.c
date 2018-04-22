/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 17:44:07 by agrumbac          #+#    #+#             */
/*   Updated: 2018/04/22 22:59:15 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

int			main(int ac, char **av)
{
	if (ac < 2)
		check_file_type(DEFAULT_TARGET);
	while (*++av)
	{
		check_file_type(*av);
	}
	return (0);
}
