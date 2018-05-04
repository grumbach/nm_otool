/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm_section_table.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/04 19:25:39 by agrumbac          #+#    #+#             */
/*   Updated: 2018/05/05 00:12:09 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"
#include "nm_section_table.h"

static inline t_section_table	*singleton()
{
	static t_section_table		section_table = {NULL, 0};

	return (&safe);
}

/*
** t_section_manager
*/

static bool					section_table_write(const size_t index, const char c)
{
	t_section_table			*section_table

	section_table = singleton();
	if (index >= section_table->size)// TODO this should not happen here..
		return (errors(ERR_FILE, "invalid section number"));
	section_table->str[index] = c;
	return (BOOL_TRUE);
}

static bool					section_table_read(const size_t index, char *c)
{
	t_section_table			*section_table

	section_table = singleton();
	if (index >= section_table->size)
		return (errors(ERR_FILE, "invalid section number"));

	*c = section_table->str[index];
	return (BOOL_TRUE);
}

static bool					section_table_constructor(const size_t total_sects)
{
	t_section_table			*section_table

	section_table = singleton();
	if (!(section_table->str = malloc(total_sects)))
		return (errors(ERR_SYS, "malloc failed"));
	section_table->size = total_sects;

	// set default value (other section : 's')
	ft_memset(section_table->str, 's', total_sects);

	return (BOOL_TRUE);
}

static void					section_table_destructor()
{
	t_section_table			*section_table

	section_table = singleton();
	free(section_table->ptr);
	section_table->ptr = NULL;
	section_table->size = 0;
}
