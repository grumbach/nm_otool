/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 17:44:48 by agrumbac          #+#    #+#             */
/*   Updated: 2018/04/22 23:00:57 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

void				errors(const int err, const char *str)
{
	static const char	*msg[3] =
	{
		"Error: ",
		"Bad usage: ",
		"Bad file: "
	};

	ft_putstr_fd(2, msg[err]);
	ft_putendl_fd(2, str);
}
