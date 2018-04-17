/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/10 18:02:19 by agrumbac          #+#    #+#             */
/*   Updated: 2018/04/17 20:14:35 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

int				errors(const int err, const char *str)
{
	if (err == 1)
		ft_printf("%s", str);
	return (EXIT_FAILURE);
}


char			get_type(const uint8_t n_type)
{
	// int			type;
	char		type = 'u'; /*local as default */

    //
	// if (N_STAB & n_type) /* debugging entry */
	// 	type = '-';
	// if (N_PEXT & n_type) /* private external */
	// 	type = 'u'; //TODO find letter for this
    //
    //
    //
	// /////////// ---------TMP
	// char ntyp[42];
	// ft_memset(ntyp, 'S', 42);
	// ntyp[0] = 'U';
	// ntyp[N_TYPE & N_UNDF] = 'U';
	// ntyp[N_TYPE & N_ABS] = 'A';
	// ntyp[N_TYPE & N_SECT] = 'T';
	// ntyp[N_TYPE & N_PBUD] = 'c';
	// ntyp[N_TYPE & N_INDR] = 'I';
	// // ntyp[N_PEXT | ] = '';
	// /////////// ---------TMP

	// ntyp[array[i].n_type]



	// if (N_EXT & n_type) /* external to UPERCASE */
	// 	type = ft_toupper(type);
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
		char		type = get_type(array[i].n_type);
		uint64_t	offset =  array[i].n_value;
		char		*str = stringtable + array[i].n_un.n_strx;

		ft_printf("===(n_type)%d, (n_sect)%d, (n_desc)%d\t===", array[i].n_type, array[i].n_sect, array[i].n_desc);

		ft_printf("%016lx %c %s\n", offset, type, str);
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
