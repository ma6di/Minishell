/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrauh <nrauh@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 16:23:19 by nrauh             #+#    #+#             */
/*   Updated: 2024/11/14 15:31:03 by nrauh            ###   ########.fr       */
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
# include <sys/wait.h>
# include <errno.h>
# include <string.h>
# include <limits.h> 
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <signal.h>
# include <unistd.h>
# include <dirent.h>
# include "../libft/includes/libft.h"

# define WHITESPACE " "
# define OP_REDIRECT ">"
# define OP_INPUT_REDIRECT "<"
# define OP_APPEND ">>"
# define OP_HEREDOC "<<"
# define OP_PIPE "|"

# define MAX_PATH_LENGTH PATH_MAX

# define BUFF_SIZE 1024

typedef struct s_fds		t_fds;	// Forward declaration
typedef struct s_token		t_token;	// Forward declaration
typedef struct s_command	t_command;	// Forward declaration

typedef enum e_return_codes
{
	ERROR = -1,
	SUCCESS = 0,
	CD_SUCCESS = 0,
	CD_ERROR = 1
}			t_return_codes;

typedef enum e_token_type {
	COMMAND,
	REDIRECT,
	APPEND,
	HEREDOC,
	HEREDOC_DELIMITER,
	INPUT_REDIRECT,
	PIPE,
	LOGICAL_OR,
	ARGUMENT,
	STRING,
	FILENAME,
	ENV_VAR,
	SHELL_VAR
}	t_token_type;

typedef enum e_token_state {
	GENERAL,
	QUOTE,
	DQUOTE
}	t_token_state;


typedef struct s_main
{
	t_command	**command_list;
	t_token		**token_list;
	char		**env_vars;
	char		**shell_vars;
	int			exit_code;
	int			should_exit;
	bool		is_sleeping;
}				t_main;

typedef struct s_token {
	t_token_type	type;
	t_token_state	state;
	char			*value;
	struct s_token	*prev;
	struct s_token	*next;
}					t_token;

typedef struct s_command
{
	char				*command;
	char				**args; // ["cmd", "arg1", ...]
	char				*heredoc_delimiter;
	char				*heredoc_content;
	int					nr_of_pipes; // do we still need this?
	int					*pipe_fd;
	int					has_pipe;
	t_fds				*io_fds;
	int					error_code;
	char				*error_message;
	char				*result_file;
	bool				pipe_created;
	pid_t				pid;
	t_main				*main;
	struct s_command	*next;
	struct s_command	*prev;
}						t_command;

typedef struct s_fds
{
	char	*infile;
	char	*outfile;
	char	*append_outfile;
	int		fd_in;
	int		fd_out;
	int		has_heredoc;
	int		fd_err;
	int		is_stderr_redirected;
	int		in_duped;
	int		out_duped;
}			t_fds;

void			lexer(char *input, char **envp);
t_token			**parse(t_token **head, char *input);
t_token			**expand(t_token **head, char **envp);
void			free_tokens(t_token **head);
void			free_commands(t_command **head);
void			add_token(t_token **head, t_token *new_token);
void			create_token(t_token **head, char *value, t_token_state state);
t_token_type	get_token_type(char *value);
void			print_token_list(t_token **head);
void			print_cmd_list(t_command **head);
void			print_keys(char **env_keys);
void			print_key_val(char ***filtered_envp);
void			free_two_dim(char **env_keys);
void			free_three_dim(char ***filtered_envp);
int				is_whitespace(char c);
int				is_lower(char c);
int				is_upper(char c);
void			display_error(char *message, t_token **head);
t_token			**join_token(t_token **head);
t_token			**assign_types(t_token **head);
t_token			**check_validity(t_token **head);
char			*add_to_buffer(char **buffer, char c);
t_command		**create_commands(t_token **head_t);

#endif
