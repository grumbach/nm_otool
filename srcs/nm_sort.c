/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm_sort.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/11 00:01:33 by agrumbac          #+#    #+#             */
/*   Updated: 2018/05/13 21:21:32 by agrumbac         ###   ########.fr       */
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
	int		diff;

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
	static bool (*compare[4])(t_symbol *, t_symbol *) = {\
		&alphabetically, &reverse_alphabetically, \
		&numerically, &reverse_numerically};
	t_symbol	*swap;
	size_t		best;
	size_t		i;
	size_t		j;

	i = 0;
	while (i < sorted_symbols->nsyms_sort)
	{
		j = i;
		best = j;
		while (j < sorted_symbols->nsyms_sort)
		{
			if (compare[sort_type](sorted_symbols->symbols_sort[best], \
				sorted_symbols->symbols_sort[j]))
				best = j;
			j++;
		}
		swap = sorted_symbols->symbols_sort[best];
		sorted_symbols->symbols_sort[best] = sorted_symbols->symbols_sort[i];
		sorted_symbols->symbols_sort[i] = swap;
		i++;
	}
}
