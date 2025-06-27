# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aistok <aistok@student.42london.com>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/26 21:37:12 by aistok            #+#    #+#              #
#    Updated: 2025/06/26 21:37:15 by aistok           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = mlx_loop_end_test

SRC_DIR = src
OBJ_DIR = obj
INC_DIR = inc

LIBMLX_DIR = minilibx-linux
LIBMLX = ${LIBMLX_DIR}/libmlx.a

# TO GET THE ALL SOURCE FILES (TEMPORARILY), UNTIL THE PROJECT BECOMES MORE STABLE
TMP_SRC_DIRS = ${shell find $(SRC_DIR) -type d}
TMP_OBJ_DIRS = ${subst ${SRC_DIR},${OBJ_DIR},${TMP_SRC_DIRS}}
TMP_FILES = ${wildcard ${addsuffix /*, ${TMP_SRC_DIRS}}}
SRC = ${filter %.c, $(TMP_FILES)}

OBJS = ${SRC:${SRC_DIR}/%.c=${OBJ_DIR}/%.o}

CC = gcc -g
RM = rm -rf

CFLAGS = -Wall -Wextra -Werror -O0

all: ${NAME}

${NAME}: ${LIBMLX} ${OBJS}
	${CC} ${OBJS} -o $@ -I${INC_DIR} -L${LIBMLX_DIR} -lmlx -lXext -lX11 -lm 

${OBJ_DIR}/%.o:${SRC_DIR}/%.c | ${OBJ_DIR}
	${CC} ${CFLAGS} -I${INC_DIR} -I${LIBMLX_DIR} -c $< -o $@ 

${OBJ_DIR}:
	mkdir -p ${TMP_OBJ_DIRS}

${LIBMLX}: | ${LIBMLX_DIR}
	git submodule update --init ${LIBMLX_DIR}
	make -C ${LIBMLX_DIR}

${LIBMLX_DIR}:
	mkdir -p $@

clean:
	${RM} ${OBJ_DIR}
	make -C ${LIBMLX_DIR} clean

fclean: clean
	${RM} ${NAME}
	${RM} ${LIBMLX}

re: fclean all

.PHONY: all clean fclean re
