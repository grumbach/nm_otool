/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/10 18:02:19 by agrumbac          #+#    #+#             */
/*   Updated: 2018/04/19 16:13:04 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

int				errors(const int err, const char *str)
{
	if (err == 1)
		ft_printf("%s", str);
	return (EXIT_FAILURE);
}

char			what_section(const uint8_t n_sect, char *ptr)
{
	struct mach_header_64 *macho = (struct mach_header_64 *)ptr;
	struct load_command *lc = (struct load_command *)(ptr + sizeof(*macho));
	struct section_64 *sect;

	int ncmds = macho->ncmds;
	uint8_t sect_number = 0;
	for (int i = 0; i < ncmds; i++)
	{
		if (lc->cmd == LC_SEGMENT_64)
		{
			struct segment_command_64 *seg = lc;
			sect = (struct section_64 *)(seg + 1);

			int nsects = seg->nsects;
			for (size_t j = 0; j < nsects && sect_number < n_sect; j++)
			{
				sect_number++;
				if (sect_number == n_sect)
					break;
				sect++;//this is a pointer

			}
			if (sect_number == n_sect)
				break;
		}
		lc = (char *)lc + lc->cmdsize;
	}
	if (!ft_strncmp(sect->sectname, "__text", 6))
		return ('t');
	else if (!ft_strncmp(sect->sectname, "__data", 6))
		return ('d');
	else if (!ft_strncmp(sect->sectname, "__bss", 5))
		return ('b');
	else
		return ('s');
}

char			get_type(const uint8_t n_type, const uint8_t n_sect, char *ptr)
{
	char		type = 'u';
	int			n_type_data = N_TYPE & n_type;

	if (N_STAB & n_type)
		type = '-';
	if (N_PEXT & n_type)
		type = 'u'; //TODO find correct letter for this
	if (n_type_data == N_UNDF)
		type = 'u';
	else if (n_type_data == N_ABS)
		type = 'a';
	else if (n_type_data == N_SECT)
		type = what_section(n_sect, ptr);
	else if (n_type_data == N_PBUD)
		type = 'u'; //TODO find correct letter for this
	else if (n_type_data == N_INDR)
		type = 'i';

	//TODO how about 'C' ?????

	if (N_EXT & n_type) /* external to UPPERCASE */
		type = ft_toupper(type);
	return (type);
}

static void			print_output(int nsysms, int symoff, int stroff, char *ptr)
{
	char			*stringtable;
	struct nlist_64	*array;
	int				i;

	array = (void *)ptr + symoff;
	stringtable = (void *)ptr + stroff;
	i = 0;

	//for each symbol table entries
	while (i  < nsysms)
	{
		char		type = get_type(array[i].n_type, array[i].n_sect, ptr);
		uint64_t	offset =  array[i].n_value;
		char		*str = stringtable + array[i].n_un.n_strx;

		if (offset)
			ft_printf("%016lx %c %s\n", offset, type, str);
		else
			ft_printf("                 %c %s\n", type, str);
		i++;
	}
}

void			handle_64(char *ptr)
{
	int						ncmds;
	int						i;
	struct mach_header_64	*header;
	struct load_command		*lc;
	struct symtab_command	*sym;

	header = (struct mach_header_64 *)ptr;
	ncmds = header->ncmds;
	i = 0;
	lc = (void *)ptr + sizeof(*header);

	// for each load commands
	while (i < ncmds)
	{
		if (lc->cmd == LC_SYMTAB)
		{
			sym = (struct symtab_command *) lc;
			print_output(sym->nsyms, sym->symoff, sym->stroff, ptr);
			break;
		}
		lc = (void *)lc + lc->cmdsize;
		i++;
	}
}

void			nm(char *ptr)
{
	int			magic_number;

	magic_number = *(int *) ptr;

	if (magic_number == MH_MAGIC_64)//TODO how about MH_CIGAM_64?
	{
		handle_64(ptr);
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

	nm(ptr);

	if (munmap(ptr, buf.st_size) < 0)
		return (errors(ERR_SYS, "munmap failed"));
	return (EXIT_SUCCESS);
}
