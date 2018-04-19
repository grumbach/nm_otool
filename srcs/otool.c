/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   otool.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/10 18:02:31 by agrumbac          #+#    #+#             */
/*   Updated: 2018/04/19 07:23:25 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "otool.h"

int				errors(const int err, const char *str)
{
	if (err == 1)
		ft_printf("%s", str);
	return (EXIT_FAILURE);
}

void			gracefully_print_text(char *text, uint64_t offset, uint64_t size)
{
	uint64_t i = 0;
	while (i < size)
	{
		// ft_printf("%016llx\t", offset + i);//TODO figure out missing 0x100000000
		ft_printf("00000001%08llx\t", offset + i);
		for (uint64_t j = 0; (j < 16 && i + j < size); j++)
		{
			ft_printf("%02hhx ", (char)text[i + j]);
		}
		ft_printf("\n");
		i += 16;
	}
}

void			segment_extract_text(struct segment_command_64 *seg, char *ptr)
{
	if (ft_strncmp(seg->segname, "__TEXT", 6))
		return ;

	struct section_64 *sect = (struct section_64 *)(seg + 1);

	int nsects = seg->nsects;
	for (size_t i = 0; i < nsects; i++)
	{
		if (!ft_strncmp(sect->sectname, "__text", 6))
		{
			ft_printf("Contents of (%s,%s) section\n", sect->segname, sect->sectname);
			gracefully_print_text(ptr + sect->offset, sect->offset, sect->size);
		}
		sect++;//this is a pointer
	}
}

void			otool(char *ptr)
{
	struct mach_header_64 *macho = (struct mach_header *)ptr;
	struct load_command *lc = (struct load_command *)(ptr + sizeof(*macho));

	int ncmds = macho->ncmds;
	for (int i = 0; i < ncmds; i++)
	{
		if (lc->cmd == LC_SEGMENT_64)
			segment_extract_text((struct section_64 *)lc, ptr);
		lc = (char *)lc + lc->cmdsize;
	}
}

int				main(int ac, char **av)
{
	int			fd;
	char		*ptr;
	struct stat	buf;

	if (ac != 2)
		return (errors(ERR_USAGE, "bad usage"));
	if ((fd = open(av[1], O_RDONLY)) < 0)
		return (errors(ERR_SYS, "open failed"));
	if (fstat(fd, &buf) < 0)
		return (errors(ERR_USAGE, "fstat failed"));
	if ((ptr = mmap(0, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
		return (errors(ERR_SYS, "munmap failed"));

	ft_printf("%s:\n", av[1]);
	otool(ptr);

	if (munmap(ptr, buf.st_size) < 0)
		return (errors(ERR_SYS, "munmap failed"));
	return (EXIT_SUCCESS);
}
