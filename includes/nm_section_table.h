/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm_section_table.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/05 00:11:31 by agrumbac          #+#    #+#             */
/*   Updated: 2018/05/05 00:11:45 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NM_SECTION_TABLE_H
# define NM_SECTION_TABLE_H

typedef struct	s_section_table
{
	size_t		len;
	char		*str;
}				t_section_table;

static bool		section_table_write(const size_t index, const char c);
static bool		section_table_read(const size_t index, char *c);
static bool		section_table_constructor(const size_t total_sects);
static void		section_table_destructor(void);

#endif
