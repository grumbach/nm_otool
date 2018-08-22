/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 16:40:47 by agrumbac          #+#    #+#             */
/*   Updated: 2018/08/22 18:57:49 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

static t_safe_pointer			safe_pointer = {NULL, 0, 0};

/*
** safe()
** returns a safe pointer
** returns NULL if requested memory is out of range
*/

void							*safe(const uint64_t offset, const size_t size)
{
	return ((void *) \
		((size_t)(safe_pointer.ptr + safe_pointer.start_offset + offset) * \
		(safe_pointer.start_offset + offset + size <= safe_pointer.filesize)));
}

void							set_start_offset(size_t new_start_offset)
{
	safe_pointer.start_offset = new_start_offset;
}

bool							read_file(const char *filename)
{
	int							fd;
	void						*ptr;
	struct stat					buf;

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

	safe_pointer.ptr = ptr;
	safe_pointer.filesize = buf.st_size;
	safe_pointer.start_offset = 0;
	return (true);
}

bool							free_file(void)
{
	if (munmap(safe_pointer.ptr, safe_pointer.filesize))
		return (errors(ERR_SYS, "munmap failed"));
	return (true);
}
