/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   endian.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/01 23:54:56 by agrumbac          #+#    #+#             */
/*   Updated: 2018/05/02 22:27:44 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "endian.h"
#include "nm_otool.h"

static inline bool	*singleton(void)
{
	static bool	little_endian = 0;

	return (&little_endian);
}

void				endian_little_mode(bool is_little_endian)
{
	bool			*endian;

	endian = singleton();
	*endian = is_little_endian;
}

uint32_t			endian_4(uint32_t a)
{
	return (*singleton() ? ENDIAN_4(a) : a);
}

uint64_t			endian_8(uint64_t a)
{
	return (*singleton() ? ENDIAN_8(a) : a);
}
