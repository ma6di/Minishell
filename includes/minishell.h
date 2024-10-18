/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrauh <nrauh@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 16:23:19 by nrauh             #+#    #+#             */
/*   Updated: 2024/10/18 16:59:54 by nrauh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include "../libft/includes/libft.h"

typedef enum e_TokenType {
	COMMAND,
	REDIRECT,
	ARGUMENT,
	PIPE,
	STRING,
	FILENAME,
	ENV_VAR,
	SHELL_VAR
}	t_TokenType;

typedef struct s_token {
	t_TokenType		type;
	char			*value;
	struct s_token	*prev;
	struct s_token	*next;
}					t_token;

#endif
