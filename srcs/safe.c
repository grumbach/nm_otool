/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 16:40:47 by agrumbac          #+#    #+#             */
/*   Updated: 2018/04/25 23:17:13 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

static inline t_safe_pointer	*singleton(t_safe_pointer *new)
{
	static t_safe_pointer		safe;

	if (new)
		safe = *new;
	return (&safe);
}

/*
** safe()
** returns a safe pointer
** returns NULL if requested memory is out of range
*/

void							*safe(const uint64_t offset, const size_t size)
{
	t_safe_pointer				*safe;

	safe = singleton(NULL);
	return ((long)(safe->ptr + offset) * (offset + size < safe->filesize));
}

bool							read_file(const char *filename)
{
	int							fd;
	void						*ptr;
	struct stat					buf;
	t_safe_pointer				safe_pointer;

	if ((fd = open(filename, O_RDONLY)) < 0)
		return (errors(ERR_SYS, "No such file or directory."));
	if (fstat(fd, &buf) < 0)
		return (errors(ERR_USAGE, "fstat failed"));
	if ((ptr = mmap(0, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
		return (errors(ERR_SYS, "munmap failed"));
	if (close(fd))
		return (errors(ERR_SYS, "close failed"));

	safe_pointer = (t_safe_pointer){ptr, buf.st_size};
	return ((bool)singleton(&safe_pointer));
}
