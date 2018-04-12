/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/10 18:02:19 by agrumbac          #+#    #+#             */
/*   Updated: 2018/04/12 20:35:37 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

int				errors(const int err, const char *str)
{
	if (err == 1)
		ft_printf("%s", str);
	return (EXIT_FAILURE);
}

void			print_output(int nsysms, int symoff, int stroff, char *ptr)
{
	int				i;
	char			*stringtable;
	struct nlist_64	*array;
	/////////// ---------TMP
	char ntyp[42];
	ft_memset(ntyp, 'S', 42);
	ntyp[N_TYPE & N_UNDF] = 'U';
	ntyp[N_TYPE & N_ABS] = 'A';
	ntyp[N_TYPE & N_SECT] = 'T';
	ntyp[N_TYPE & N_PBUD] = 'c';
	ntyp[N_TYPE & N_INDR] = 'I';
	// ntyp[N_PEXT | ] = '';
	/////////// ---------TMP

	array = (void *)ptr + symoff;
	stringtable = (void *)ptr + stroff;
	i = 0;

	//for each symbol table entries
	while (i  < nsysms)
	{
		ft_printf("===%x, %d, %d===", array[i].n_type, array[i].n_sect, array[i].n_desc);
		ft_printf("%016x %c %s\n", array[i].n_value, ntyp[array[i].n_type], stringtable + array[i].n_un.n_strx);
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
