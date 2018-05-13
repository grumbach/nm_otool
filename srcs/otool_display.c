/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   otool_display.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/13 21:28:43 by agrumbac          #+#    #+#             */
/*   Updated: 2018/05/13 21:30:10 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

static void		hexdump_text(const char *text, uint64_t offset, uint64_t size)
{
	uint64_t			i;
	uint64_t			j;

	ft_printf("Contents of (%s,%s) section\n", OTOOL_SEGMENT, OTOOL_SECTION);
	i = 0;
	while (i < size)
	{
		ft_printf("%016llx\t", offset + i);
		j = 0;
		while (j < 0x10 && i + j < size)
			ft_printf("%02hhx ", text[i + j++]);
		ft_printf("\n");
		i += 0x10;
	}
}

/*
** t_section_manager
*/

bool			print_section(const size_t offset)
{
	struct section		*sect;
	char				*text;

	if (!(sect = safe(offset, sizeof(*sect))))
		return (errors(ERR_FILE, "bad section offset"));
	if (!(text = safe(endian_4(sect->offset), endian_4(sect->size))))
		return (errors(ERR_FILE, "bad text offset"));
	hexdump_text(text, endian_4(sect->addr), endian_4(sect->size));
	return (true);
}

bool			print_section_64(const size_t offset)
{
	struct section_64	*sect;
	char				*text;

	if (!(sect = safe(offset, sizeof(*sect))))
		return (errors(ERR_FILE, "bad section offset"));
	if (!(text = safe(endian_4(sect->offset), endian_8(sect->size))))
		return (errors(ERR_FILE, "bad text offset"));
	hexdump_text(text, endian_8(sect->addr), endian_8(sect->size));
	return (true);
}
