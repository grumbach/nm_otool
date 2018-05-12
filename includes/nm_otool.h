/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm_otool.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 16:41:33 by agrumbac          #+#    #+#             */
/*   Updated: 2018/05/12 21:21:17 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NM_OTOOL_H
# define NM_OTOOL_H

/*
** ------------------------------- Includes ------------------------------------
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
** ------------------------------- Constants -----------------------------------
*/

# define ERR_NUMBER			4

# define ERR_SYS			0
# define ERR_USAGE			1
# define ERR_FILE			2
# define ERR_THROW			3

# define DEFAULT_TARGET		"a.out"

# define OTOOL_SECTION		"__text"
# define OTOOL_SEGMENT		"__TEXT"

# define ARCHIVE_MAGIC		0x72613C21

# define FIRST_BIT_ON_64	0x8000000000000000L

/*
** ------------------------------- Typedefs ------------------------------------
*/

typedef bool	(*t_fat_magic_retriever)(uint32_t, size_t, size_t*, uint32_t*);
typedef bool	(*t_gatherer)(const bool);
typedef bool	(*t_lc_manager)(const size_t);
typedef bool	(*t_section_manager)(const size_t);

typedef struct	s_safe_pointer
{
	void		*ptr;
	size_t		filesize;
	size_t		start_offset;
}				t_safe_pointer;

/*
** ------------------------------- Text Symbols --------------------------------
*/

/*
** Safe pointers management
*/

void			*safe(const uint64_t offset, const size_t size);
void			set_start_offset(size_t new_start_offset);
bool			read_file(const char *filename);
bool			free_file(void);

/*
** Endian management
*/

void			endian_little_mode(bool is_little_endian);
uint32_t		endian_4(uint32_t a);
uint64_t		endian_8(uint64_t a);

/*
** General functions
*/

bool			extract_macho(const char *filename, t_gatherer func);
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
