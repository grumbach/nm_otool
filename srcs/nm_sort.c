/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm_sort.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/11 00:01:33 by agrumbac          #+#    #+#             */
/*   Updated: 2018/05/11 04:50:40 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"
#include "nm_display.h"

static bool	alphabetically(t_symbol *best_match, t_symbol *other)
{
	int diff;

	diff = ft_strncmp(best_match->string, other->string, \
		best_match->str_max_size < other->str_max_size ? \
		best_match->str_max_size : other->str_max_size);
	if (!diff)
		return (best_match->offset > other->offset);
	return (0 < diff);
}

static bool	numerically(t_symbol *best_match, t_symbol *other)
{
	if (best_match->offset == other->offset)
		return (alphabetically(best_match, other));
	return (best_match->offset > other->offset);
}

static bool	reverse_alphabetically(t_symbol *best_match, t_symbol *other)
{
	int diff;

	diff = ft_strncmp(best_match->string, other->string, \
		best_match->str_max_size < other->str_max_size ? \
		best_match->str_max_size : other->str_max_size);
	if (!diff)
		return (best_match->offset < other->offset);
	return (0 > diff);
}

static bool	reverse_numerically(t_symbol *best_match, t_symbol *other)
{
	if (best_match->offset == other->offset)
		return (reverse_alphabetically(best_match, other));
	return (best_match->offset < other->offset);
}

void		nm_selection_sort(t_sym_sort *sorted_symbols, uint8_t sort_type)
{
	static bool 		(*compare[4])(t_symbol *, t_symbol *) =
	{
		&alphabetically, &reverse_alphabetically,
		&numerically, &reverse_numerically
	};
	t_symbol	**symbols_ptr = sorted_symbols->symbols_sort;
	t_symbol	*swap;
	size_t		best_match;
	size_t		i;
	size_t		j;

	i = 0;
	while (i < sorted_symbols->nsyms_sort)
	{
		j = i;
		best_match = j;
		while (j < sorted_symbols->nsyms_sort)
		{
			if (compare[sort_type](symbols_ptr[best_match], symbols_ptr[j]))
				best_match = j;
			j++;
		}
		swap = symbols_ptr[best_match];
		symbols_ptr[best_match] = symbols_ptr[i];
		symbols_ptr[i] = swap;
		i++;
	}
}
