/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrauh <nrauh@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 16:23:19 by nrauh             #+#    #+#             */
/*   Updated: 2024/10/26 11:59:58 by nrauh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
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

typedef enum e_TokenState{
	STATE_GENERAL, 
	STATE_IN_QUOTE, 
	STATE_IN_DQUOTE,
	STATE_OPERATOR
}	t_TokenState;

typedef struct s_fds
{
	char	*infile;
	char	*outfile;
	char	*append_outfile;
	int		fd_in;
	int		fd_out;
	int		fd_err;
	int		is_stderr_redirected;
}			t_fds;

typedef struct s_command
{
	char				*command;
	char				**args;
	char				*heredoc_delimiter;
	char				*heredoc_content;
	int					nr_of_pipes; // do we still need this?
	int					*pipe_fd;
	int					has_pipe;
	t_fds				*io_fds;
	int					error_code;
	char				*error_message;
	char				*result_file;
	pid_t				pid;
	struct s_command	*next;
	struct s_command	*prev;
}						t_command;

typedef struct s_token {
	t_TokenType		type;
	char			*value;
	struct s_token	*prev;
	struct s_token	*next;
}					t_token;

typedef struct s_main
{
	t_command	**command_list;
	t_token		**token_list;
	char		**env_vars;
	char		**shell_vars;
	int			exit_code;
}				t_main;

void		lexer(char *input);
void		print_token_list(t_token **head);
void		free_tokens(t_token **head);
void		add_token(t_token **head, t_token *new_token);
void		create_token(t_token **head, char *value);
t_TokenType	get_token_type(char *value);

#endif
