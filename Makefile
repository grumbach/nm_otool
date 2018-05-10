# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/04/10 17:19:11 by agrumbac          #+#    #+#              #
#    Updated: 2018/05/10 21:13:59 by agrumbac         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

############################## BIN #############################################

NM_NAME = ft_nm

OTOOL_NAME = ft_otool

CC = gcc

CFLAGS = -Wall -Wextra -Werror -fsanitize=address,undefined

COMMON_SRC = errors.c safe.c extract_macho.c parse_macho.c endian.c

NM_SRC = nm.c nm_display.c nm_sections_character_table.c ${COMMON_SRC}

OTOOL_SRC = otool.c ${COMMON_SRC}

SRCDIR = srcs

OBJDIR = objs

LIB = -Llibft/ -lft

INCLUDES = -Ilibft/includes/ -Iincludes/

DEP = includes/nm_otool.h libft/libft.a

############################## COLORS ##########################################

BY = "\033[33;1m"
BR = "\033[31;1m"
BG = "\033[32;1m"
BB = "\033[34;1m"
BM = "\033[35;1m"
BC = "\033[36;1m"
Y = "\033[33m"
R = "\033[31m"
G = "\033[32m"
B = "\033[34m"
M = "\033[35m"
C = "\033[36m"
WT = "\033[37;1m"
W = "\033[0m""\033[32;1m"
WR = "\033[0m""\033[31;5m"
WY = "\033[0m""\033[33;5m"
X = "\033[0m"
UP = "\033[A"
CUT = "\033[K"

############################## RULES ###########################################

all: art ${NM_NAME} ${OTOOL_NAME}

libft/%:
	@[[ -d libft ]] || (echo ${M}Cloning"   "[libft]...${X} && git clone https://github.com/grumbach/libft &>/dev/null)
	@make -C libft

############################## NM ##############################################

NM_OBJ = $(addprefix ${OBJDIR}/, $(NM_SRC:.c=.o))

${NM_NAME}: ${NM_OBJ}
	@echo ${B}Compiling [${NM_NAME}]...${X}
	@${CC} ${CFLAGS} ${INCLUDES} ${LIB} -o $@ ${NM_OBJ}
	@echo ${G}Success"   "[${NM_NAME}]${X}

${OBJDIR}/%.o: ${SRCDIR}/%.c ${DEP}
	@echo ${Y}Compiling [$@]...${X}
	@/bin/mkdir -p ${OBJDIR}
	@${CC} ${CFLAGS} ${INCLUDES} -c -o $@ $<
	@printf ${UP}${CUT}

############################## OTOOL ###########################################

OTOOL_OBJ = $(addprefix ${OBJDIR}/, $(OTOOL_SRC:.c=.o))

${OTOOL_NAME}: ${OTOOL_OBJ}
	@echo ${B}Compiling [${OTOOL_NAME}]...${X}
	@${CC} ${CFLAGS} ${INCLUDES} ${LIB} -o $@ ${OTOOL_OBJ}
	@echo ${G}Success"   "[${OTOOL_NAME}]${X}

${OBJDIR}/%.o: ${SRCDIR}/%.c ${DEP}
	@echo ${Y}Compiling [$@]...${X}
	@/bin/mkdir -p ${OBJDIR}
	@${CC} ${CFLAGS} ${INCLUDES} -c -o $@ $<
	@printf ${UP}${CUT}

############################## GENERAL #########################################

clean:
	@echo ${R}Cleaning"  "[libft objs]...${X}
	@make -C libft/ clean
	@echo ${R}Cleaning"  "[objs]...${X}
	@/bin/rm -Rf ${OBJDIR}

fclean: clean
	@make -C libft/ fclean
	@echo ${R}Cleaning"  "[${NM_NAME}]...${X}
	@/bin/rm -f ${NM_NAME}
	@/bin/rm -Rf ${NM_NAME}.dSYM
	@echo ${R}Cleaning"  "[${OTOOL_NAME}]...${X}
	@/bin/rm -f ${OTOOL_NAME}
	@/bin/rm -Rf ${OTOOL_NAME}.dSYM

test: libft/libft.a
	@${CC} -g ${INCLUDES} -fsanitize=address,undefined ${LIB} \
	-I. -o ${NM_NAME} $(addprefix srcs/, ${NM_SRC})
	@${CC} -g ${INCLUDES} -fsanitize=address,undefined ${LIB} \
	-I. -o ${OTOOL_NAME} $(addprefix srcs/, ${OTOOL_SRC})

odiff: ft_otool
	@./ft_otool ft_otool > /tmp/ft_nmotool_diff
	@otool -t ft_otool > /tmp/ft_nmotool_daff
	@diff /tmp/ft_nmotool_diff /tmp/ft_nmotool_daff

ndiff: ft_nm
	@./ft_nm ft_nm > /tmp/ft_nmotool_diff
	@nm -p ft_nm > /tmp/ft_nmotool_daff
	@diff /tmp/ft_nmotool_diff /tmp/ft_nmotool_daff

diff: odiff ndiff

re: fclean all

build: libft ${NM_OBJ} ${OTOOL_OBJ}

############################## DECORATION ######################################

art:
	@echo ${BB}
	@echo "           __d"
	@echo "        _(___)"
	@echo "     _(______)"
	@echo "    (_______)"
	@echo ${BB}" ..."${BR}"/( "${WR}"00"${X}${BR}"  )\\ "${BB}"....."
	@echo ${BB}"...."${BR}"\\\\\\     //"${BB}"......."
	@echo ${X}

.PHONY: all clean fclean re art
