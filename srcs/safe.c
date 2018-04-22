/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 16:40:47 by agrumbac          #+#    #+#             */
/*   Updated: 2018/04/22 19:02:01 by agrumbac         ###   ########.fr       */
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

void							*safe(const uint64_t offset, const size_t size)
{
	const t_safe_pointer		*safe = singleton(NULL);

	return (safe->ptr + offset * (offset + size < safe->filesize));
}

void							*read_file(const char *file)
{
	int			fd;
	void		*ptr;
	struct stat	buf;

	if ((fd = open(file, O_RDONLY)) < 0)
		return (errors(ERR_SYS, "No such file or directory."));
	if (fstat(fd, &buf) < 0)
		return (errors(ERR_USAGE, "fstat failed"));
	if ((ptr = mmap(0, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
		return (errors(ERR_SYS, "munmap failed"));
	if (close(fd))
		return (errors(ERR_SYS, "close failed"));
	return (singleton(ptr));
}
