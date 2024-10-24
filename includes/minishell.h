/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrauh <nrauh@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 16:23:19 by nrauh             #+#    #+#             */
/*   Updated: 2024/10/24 19:01:08 by nrauh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include "../libft/includes/libft.h"

# define WHITESPACE " "
# define OP_REDIRECT ">"
# define OP_INPUT_REDIRECT "<"
# define OP_APPEND ">>"
# define OP_HEREDOC "<<"
# define OP_PIPE "|"

typedef enum e_TokenType {
	COMMAND,
	REDIRECT,
	APPEND,
	HEREDOC,
	INPUT_REDIRECT,
	ARGUMENT,
	PIPE,
	STRING,
	FILENAME,
	ENV_VAR,
	SHELL_VAR
}	t_TokenType;

// think about best practice to name enums is it TokenType or token_type
typedef enum e_TokenState{
	STATE_GENERAL, 
	STATE_IN_QUOTE, 
	STATE_IN_DQUOTE,
	STATE_OPERATOR
}	t_TokenState;

typedef struct s_token {
	t_TokenType		type;
	char			*value;
	struct s_token	*prev;
	struct s_token	*next;
}					t_token;

void	lexer(char *input);

#endif
