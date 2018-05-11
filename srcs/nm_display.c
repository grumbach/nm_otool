/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm_display.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/10 03:08:08 by agrumbac          #+#    #+#             */
/*   Updated: 2018/05/11 05:12:25 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"
#include "nm_display.h"

static inline uint8_t	*singleton()
{
	static uint8_t		flags = 0;

	return (&flags);
}

bool					nm_set_flag(const char *av)
{
	static uint8_t		*flags;
	uint8_t				new_flags;

	new_flags = 0;
	new_flags |= !ft_strncmp(av, "-a", 2) * NM_FLAG_A(NM_TOGGLE)
		| !ft_strncmp(av, "-g", 2) * NM_FLAG_G(NM_TOGGLE)
		| !ft_strncmp(av, "-p", 2) * NM_FLAG_P(NM_TOGGLE)
		| !ft_strncmp(av, "-u", 2) * NM_FLAG_U(NM_TOGGLE)
		| !ft_strncmp(av, "-U", 2) * NM_FLAG_UU(NM_TOGGLE)
		| !ft_strncmp(av, "-j", 2) * NM_FLAG_J(NM_TOGGLE)
		| !ft_strncmp(av, "-r", 2) * NM_FLAG_R(NM_TOGGLE)
		| !ft_strncmp(av, "-n", 2) * NM_FLAG_N(NM_TOGGLE);

	if (!new_flags)
		return (errors(ERR_USAGE, av));

	flags = singleton();
	*flags ^= new_flags;
	return (true);
}

bool					nm_symbol_allocate(t_sym_sort *sorted_symbols, \
							const uint64_t nsyms)
{
	ft_bzero(sorted_symbols, sizeof(*sorted_symbols));
	if (nsyms)
	{
		sorted_symbols->symbols = ft_memalloc(nsyms * sizeof(t_symbol));
		if (!sorted_symbols->symbols)
			return (errors(ERR_SYS, "malloc failed"));
		sorted_symbols->symbols_sort = ft_memalloc(nsyms * sizeof(void *));
		if (!sorted_symbols->symbols_sort)
			return (errors(ERR_SYS, "malloc failed"));
	}
	return (true);
}

void					nm_store_value(t_sym_sort *sorted_symbols, \
							t_symbol *new_symbol)
{
	uint8_t				flags;

	flags = *singleton();
	if ((NM_FLAG_A(flags) || new_symbol->type != '-') && \
		!(NM_FLAG_G(flags) && !ft_isupper(new_symbol->type)) && \
		!(NM_FLAG_U(flags) && \
		(new_symbol->type != 'u' && new_symbol->type != 'U')) && \
		!(NM_FLAG_UU(flags) && \
		(new_symbol->type == 'u' || new_symbol->type == 'U')))
	{
		sorted_symbols->symbols[sorted_symbols->nsyms_sort] = *new_symbol;
		sorted_symbols->symbols_sort[sorted_symbols->nsyms_sort] = \
			&sorted_symbols->symbols[sorted_symbols->nsyms_sort];
		sorted_symbols->nsyms_sort++;
	}
}

void					nm_sort_print_free(t_sym_sort *sorted_symbols)
{
	uint8_t				flags;
	t_symbol			*curr;
	char				*str;
	size_t				i;

	//sort
	flags = *singleton();
	if (!NM_FLAG_P(flags))
		nm_selection_sort(sorted_symbols, \
			(!!NM_FLAG_R(flags) + (!!NM_FLAG_N(flags)) * 2));
	//print
	i = 0;
	while (i < sorted_symbols->nsyms_sort)
	{
		curr = sorted_symbols->symbols_sort[i];
		str = curr->string ? curr->string : "bad string index";

		//TODO _32 bits padding
		if (NM_FLAG_J(flags) || NM_FLAG_U(flags))
			ft_printf("%s\n", str);
		else if (curr->offset)
			ft_printf("%016lx %c %.*s\n", curr->offset, curr->type, \
				curr->str_max_size, str);
		else
			ft_printf("                 %c %.*s\n", curr->type, \
				curr->str_max_size, str);
		i++;
	}
	//free
	free(sorted_symbols->symbols);
	free(sorted_symbols->symbols_sort);
}
