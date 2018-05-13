/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   otool_display.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/13 21:29:37 by agrumbac          #+#    #+#             */
/*   Updated: 2018/05/13 21:30:36 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OTOOL_DISPLAY_H
# define OTOOL_DISPLAY_H

# include "nm_otool.h"

bool			print_section(const size_t offset);
bool			print_section_64(const size_t offset);

#endif
