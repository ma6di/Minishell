# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nrauh <nrauh@student.42berlin.de>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/18 16:55:20 by nrauh             #+#    #+#              #
#    Updated: 2024/11/22 12:53:04 by nrauh            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = gcc
CFLAGS = -Wall -Wextra -Werror -g
RM = rm -rf

INCLUDES = -I includes/.
SRC_DIR = src/
SRC_EXEC_DIR = src_exec/

NAME = minishell
LIBFT = libft/libft.a

SRC = $(addprefix $(SRC_DIR), main.c lexer.c parse.c expand.c join_token.c \
	assign_types.c check_validity.c create_commands.c \
	helper/debugging.c helper/free_helper.c helper/token_list.c \
	helper/helper.c helper/parse_helper.c helper/command_list.c) \
	$(addprefix $(SRC_EXEC_DIR), exec.c exec_tools.c pwd.c export.c env.c echo.c \
	unset.c cd.c file_redir.c exec_external.c exit.c \
	signals.c heredoc.c pipe_redir.c expantion.c signals2.c export2.c \
	utilz.c exec_external2.c parent_reset.c ft_fprintf.c)

OBJ = $(SRC:.c=.o)

all: $(NAME)

libft/libft.a:
	make -C libft

$(NAME): $(OBJ) libft/libft.a
	$(CC) $(CFLAGS) $(OBJ) -g -Llibft -lft -L/usr/include -lreadline -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -g -Ilibft/includes -c $< -o $@

clean:
	make clean -C libft
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME) libft/libft.a

re: fclean all

.PHONY: all clean fclean re
