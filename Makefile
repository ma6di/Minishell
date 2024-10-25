# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nrauh <nrauh@student.42berlin.de>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/18 16:55:20 by nrauh             #+#    #+#              #
#    Updated: 2024/10/25 16:52:09 by nrauh            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
CFLAGS = -Wall -Wextra -Werror
RM = rm -rf

INCLUDES = -I includes/.
SRC_DIR = src/

NAME = minishell
LIBFT = libft/libft.a

SRC = $(addprefix $(SRC_DIR), main.c) \

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
