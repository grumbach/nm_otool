/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm_otool.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 16:41:33 by agrumbac          #+#    #+#             */
/*   Updated: 2018/04/28 19:25:12 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NM_OTOOL_H
# define NM_OTOOL_H

/*
** ------------------------------- includes ------------------------------------
*/

# include <sys/mman.h>
# include <mach-o/loader.h>
# include <mach-o/nlist.h>
# include <mach-o/fat.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <stdlib.h>
# include <stdbool.h>

# include "libft.h"

/*
** ------------------------------- error types ---------------------------------
*/

# define BOOL_TRUE			1
# define BOOL_FALSE			0

# define ERR_NUMBER			4

# define ERR_SYS			0
# define ERR_USAGE			1
# define ERR_FILE			2
# define ERR_THROW			3

# define DEFAULT_TARGET		"a.out"

# define OTOOL_SECTION		"__text"
# define OTOOL_SEGMENT		"__TEXT"

# define ARCHIVE_MAGIC		0x72613C21

/*
** ------------------------------- typedefs ------------------------------------
*/

typedef bool	(*t_gatherer)(const bool is_64);
typedef bool	(*t_lc_manager)(const size_t offset);
typedef bool	(*t_section_manager)(const size_t offset, \
					const uint32_t section_index);

typedef struct	s_safe_pointer
{
	void		*ptr;
	size_t		filesize;
}				t_safe_pointer;

/*
** ------------------------------- Text symbols --------------------------------
*/

bool			read_file(const char *filename);
bool			extract_macho(const char *filename, t_gatherer func);

void			*safe(const uint64_t offset, const size_t size);
bool			errors(const int err, const char *str);

/*
** Mach-O iterator functions
*/

bool			iterate_lc(const bool is_64, const uint32_t target, \
					t_lc_manager func);
bool			iterate_sections_64(const size_t start_offset, \
					const char *target_segment, const char *target_section, \
					t_section_manager func_ptr);
bool			iterate_sections(const size_t start_offset, \
					const char *target_segment, const char *target_section, \
					t_section_manager func_ptr);

#endif
