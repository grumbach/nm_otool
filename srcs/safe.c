/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 16:40:47 by agrumbac          #+#    #+#             */
/*   Updated: 2018/05/09 20:56:56 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

static inline t_safe_pointer	*singleton()
{
	static t_safe_pointer		safe = {NULL, 0, 0};

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

	safe = singleton();
	return ((void *) \
		((size_t)(safe->ptr + safe->start_offset + offset) * \
		(safe->start_offset + offset + size <= safe->filesize)));
}

void							set_start_offset(size_t new_start_offset)
{
	t_safe_pointer				*safe;

	safe = singleton();
	safe->start_offset = new_start_offset;
}

bool							read_file(const char *filename)
{
	int							fd;
	void						*ptr;
	struct stat					buf;
	t_safe_pointer				*safe;

	if ((fd = open(filename, O_RDONLY)) < 0)
		return (errors(ERR_SYS, "no such file or directory"));
	if (fstat(fd, &buf) < 0)
		return (errors(ERR_USAGE, "fstat failed"));
	if (buf.st_mode & S_IFDIR)
		return (errors(ERR_USAGE, "can't parse directories"));
	if ((ptr = mmap(0, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
		return (errors(ERR_SYS, "mmap failed"));
	if (close(fd))
		return (errors(ERR_SYS, "close failed"));

	safe = singleton();
	safe->ptr = ptr;
	safe->filesize = buf.st_size;
	safe->start_offset = 0;
	return (BOOL_TRUE);
}

bool							free_file(void)
{
	t_safe_pointer				*safe;

	safe = singleton();
	if (munmap(safe->ptr, safe->filesize))
		return (errors(ERR_SYS, "munmap failed"));
	return (BOOL_TRUE);
}
