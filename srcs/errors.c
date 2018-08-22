/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 17:44:48 by agrumbac          #+#    #+#             */
/*   Updated: 2018/08/22 19:03:06 by agrumbac         ###   ########.fr       */
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

	ft_perr(msg[err]);
	ft_perr(str);
	ft_perr("\n");
	if (err == ERR_USAGE)
	{
		ft_perr("usage: ./ft_nm [flags ...] [file ...] \n");
		ft_perr("  -a -- Display  all symbol table entries\n");
		ft_perr("  -g -- Display only global (external) symbols\n");
		ft_perr("  -p -- Don't sort; display in symbol-table order\n");
		ft_perr("  -u -- Display only undefined symbols\n");
		ft_perr("  -U -- Don't display undefined symbols\n");
		ft_perr("  -j -- Just display the symbol names\n");
		ft_perr("  -r -- Sort in reverse order\n");
		ft_perr("  -n -- Sort numerically rather than alphabetically\n");
	}
	return (false);
}
