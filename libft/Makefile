# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nrauh <nrauh@student.42berlin.de>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/22 13:09:14 by nrauh             #+#    #+#              #
#    Updated: 2024/07/12 17:43:52 by nrauh            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#VARIABLES
NAME = libft.a

#DIRCTORIES
SRCS_DIR = srcs
HEADER_DIR = includes
OBJS_DIR = objs
LIBFT_DIR = libft
FT_PRINTF_DIR = ft_printf
GNL_DIR = gnl

#COMMANDS
CC = cc
CFLAGS = -Wall -Wextra -Werror
RM = rm -rf
INCLUDES = -I $(HEADER_DIR)

#SRCS FILES

SRCS_FLS = 	$(LIBFT_FLS)\
			$(FT_PRINTF_FLS)\
			$(GNL_FLS)\

LIBFT_FLS = \
	$(addprefix $(LIBFT_DIR)/, \
	ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c ft_isprint.c ft_strlen.c \
	ft_memset.c ft_bzero.c ft_memcpy.c ft_memmove.c ft_strlcpy.c ft_strlcat.c \
	ft_toupper.c ft_tolower.c ft_strchr.c ft_strrchr.c ft_strncmp.c ft_memchr.c \
	ft_memcmp.c ft_strnstr.c ft_atoi.c ft_calloc.c ft_strdup.c ft_substr.c \
	ft_strjoin.c ft_strtrim.c ft_split.c ft_itoa.c ft_strmapi.c ft_striteri.c \
	ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c \
	ft_lstnew.c ft_lstadd_back.c ft_lstadd_front.c ft_lstsize.c ft_lstlast.c \
	ft_lstdelone.c ft_lstclear.c ft_lstiter.c ft_lstmap.c) \

FT_PRINTF_FLS = \
	$(addprefix $(FT_PRINTF_DIR)/, \
	ft_printf.c ft_print_number.c ft_print_string.c ft_print_hex.c ft_print_pointer.c \
	ft_putchar.c ft_putnbr.c ft_putstr.c ft_putnbr_ui.c ft_count_digits.c) \

GNL_FLS = \
	$(addprefix $(GNL_DIR)/, \
	get_next_line_utils.c get_next_line.c) \

#FILE_PATHS
SRCS = $(addprefix $(SRCS_DIR)/, $(SRCS_FLS))
OBJS = $(patsubst $(SRCS_DIR)/%.c, $(OBJS_DIR)/%.o, $(SRCS))
DEPS = $(OBJS:.o=.d)

all: $(NAME)

-include $(DEPS)
$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c | $(OBJS_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJS_DIR):
	mkdir -p $(OBJS_DIR)
	mkdir -p $(OBJS_DIR)/$(LIBFT_DIR)
	mkdir -p $(OBJS_DIR)/$(FT_PRINTF_DIR)
	mkdir -p $(OBJS_DIR)/$(GNL_DIR)

$(NAME): $(OBJS)
	ar rcs $@ $^
	ranlib $@

clean: 
	$(RM) $(OBJS_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

#RULES
.PHONY: all bonus clean fclean re
