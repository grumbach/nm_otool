/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 17:44:48 by agrumbac          #+#    #+#             */
/*   Updated: 2018/05/11 07:36:20 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

bool				errors(const int err, const char *str)
{
	static const char	*msg[ERR_NUMBER] =
	{
		"Fatal Error: ",
		"Bad usage: ",
		"Bad file: ",
		"  -> in _"
	};

	ft_putstr_fd(msg[err], 2);
	ft_putendl_fd(str, 2);
	if (err == ERR_USAGE)
	{
		ft_putendl_fd("usage: ./ft_nm [flags ...] [file ...] ", 2);
		ft_putendl_fd("  -a -- Display  all symbol table entries", 2);
		ft_putendl_fd("  -g -- Display only global (external) symbols", 2);
		ft_putendl_fd("  -p -- Don't sort; display in symbol-table order", 2);
		ft_putendl_fd("  -u -- Display only undefined symbols", 2);
		ft_putendl_fd("  -U -- Don't display undefined symbols", 2);
		ft_putendl_fd("  -j -- Just display the symbol names", 2);
		ft_putendl_fd("  -r -- Sort in reverse order", 2);
		ft_putendl_fd("  -n -- Sort numerically rather than alphabetically", 2);
	}
	return (false);
}
