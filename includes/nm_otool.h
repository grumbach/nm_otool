/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm_otool.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 16:41:33 by agrumbac          #+#    #+#             */
/*   Updated: 2018/04/23 23:58:39 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NM_OTOOL_H
# define NM_OTOOL_H

/*
** ------------------------------- includes ------------------------------------
*/

#include <sys/mman.h>
#include <mach-o/loader.h>
#include <mach-o/nlist.h>
#include <mach-o/fat.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>

#include "libft.h"

/*
** ------------------------------- error types ---------------------------------
*/

# define ERR_NUMBER			4

# define ERR_SYS			0
# define ERR_USAGE			1
# define ERR_FILE			2
# define ERR_THROW			3

# define DEFAULT_TARGET		"a.out"

# define ARCHIVE_MAGIC		0x213C6172

/*
** ------------------------------- typedefs ------------------------------------
*/

typedef int		bool;

typedef struct	s_safe_pointer
{
	void		*ptr;
	size_t		filesize;
}				t_safe_pointer;

/*
** ------------------------------- Text symbols --------------------------------
*/

bool			read_file(const char *filename);
bool			extract_macho(const char *filename, bool (*gatherer)(void));

void			*safe(const uint64_t offset, const size_t size);
bool			errors(const int err, const char *str);

#endif
