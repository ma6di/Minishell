# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nrauh <nrauh@student.42berlin.de>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/18 16:55:20 by nrauh             #+#    #+#              #
#    Updated: 2024/12/13 14:09:51 by nrauh            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
CFLAGS = -Wall -Wextra -Werror -g
RM = rm -rf

INCLUDES = -I includes/.
SRC_DIR = src/
SRC_EXEC_DIR = src_exec/

NAME = minishell
LIBFT = libft/libft.a

SRC = $(addprefix $(SRC_DIR), main.c lexer.c parse.c parse2.c expand.c expand2.c expand3.c \
	join_token.c assign_types.c check_validity.c create_commands.c \
	handle_operators.c handle_heredocs.c main2.c\
	helper/free_helper.c helper/free_helper2.c helper/token_list.c \
	helper/parse_helper.c helper/command_list.c) \
	$(addprefix $(SRC_EXEC_DIR), exec.c exec_tools.c pwd.c export.c env.c echo.c \
	unset.c cd.c cd2.c file_redir.c exec_external.c exit.c  export2.c\
	signals.c heredoc.c pipe_redir.c heredoc_expantion.c signals2.c export3.c\
	utilz.c exec_external2.c parent_reset.c ft_fprintf.c heredoc_expantion2.c)

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
