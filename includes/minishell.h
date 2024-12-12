/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcheragh <mcheragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 16:23:19 by nrauh             #+#    #+#             */
/*   Updated: 2024/12/12 19:07:35 by mcheragh         ###   ########.fr       */
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
# include <signal.h>
# include <dirent.h>
# include "../libft/includes/libft.h"
# include <sys/stat.h>
# include <stdarg.h>

# define WHITESPACE " "
# define OP_REDIRECT ">"
# define OP_INPUT_REDIRECT "<"
# define OP_APPEND ">>"
# define OP_HEREDOC "<<"
# define OP_PIPE "|"

# define MAX_PATH_LENGTH 4096
# define BUFF_SIZE 1024

extern int					g_sigint;

typedef struct s_fds		t_fds;
typedef struct s_token		t_token;
typedef struct s_command	t_command;
typedef struct s_heredoc	t_heredoc;
typedef struct s_operator	t_operator;

typedef enum e_return_codes
{
	ERROR = -1,
	SUCCESS = 0,
	CD_SUCCESS = 0,
	CD_ERROR = 1
}			t_return_codes;

typedef enum e_token_type
{
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
	INFILE,
	OUTFILE,
	APPENDFILE,
	ENV_VAR,
	HERESTRING,
	IO_REDIRECT,
	UNINITIALIZED,
	LOGICAL_AND,
}	t_token_type;

typedef enum e_token_state
{
	GENERAL,
	QUOTE,
	DQUOTE,
	EMPTY
}	t_state;

typedef struct s_main
{
	t_command	*command_list;
	char		**env_vars;
	int			exit_code;
	int			heredoc_fork_permit;
	int			should_exit;
}				t_main;

typedef struct s_token
{
	t_token_type	type;
	t_state			state;
	char			*value;
	struct s_token	*prev;
	struct s_token	*next;
}					t_token;

typedef struct s_command
{
	char				*command;
	char				**args;
	t_heredoc			**heredocs;
	t_operator			**operators;
	int					*pipe_fd;
	int					has_pipe;
	bool				pipe_created;
	pid_t				pid;
	pid_t				heredoc_pid;
	t_fds				*io_fds;
	struct s_command	*next;
	struct s_command	*prev;
	t_main				*main;
}						t_command;

typedef struct s_fds
{
	int		fd_in;
	int		fd_out;
	int		has_heredoc;
}			t_fds;

typedef struct s_heredoc
{
	char	*delimiter;
	int		should_expand;
	char	*line;
	char	*filename;
	char	*expanded_line;
	int		heredoc_fd;
}				t_heredoc;

typedef struct s_operator
{
	t_token_type	type;
	char			*filename;
}				t_operator;

// Lexing
t_command		*lexer(char *input, char **envp, t_main **main);

// Parsing
t_token			**parse(t_token **head, char *input);
void			handle_empty_str(char **buff, t_token **head, \
					char *str, char c);
char			*handle_state_dq(char **buff, t_token **head, \
					t_state *state, char *str);
char			*handle_state_q(char **buff, t_token **head, \
					t_state *state, char *str);
char			*handle_variable(char **buff, t_token **head, \
					t_state *state, char *str);
char			*add_to_buffer(char **buffer, char c);
int				is_operator_char(char c);
int				is_delimiter(char c);

// Expansion
t_token			**expand(t_token **head, char **envp, t_main *main);
t_token			*insert_new_token(t_token **curr, char **tmp_split, int i);
t_token			*value_is_cla(t_token *curr, char *value);
char			**split_cla(char *value);
char			*replace_exit_code_in_arg(const char *arg, t_main *main);
char			*generate_new_arg(const char *arg, const char *exit_code_str, \
								const char *pos);

// Freeing
void			free_tokens(t_token **head);
void			free_commands(t_command **head);
void			free_command_child(t_command **cmd);
void			free_main(t_main *main);
void			free_two_dim(char **env_keys);
void			free_three_dim(char ***filtered_envp);

// Tokenizing
void			create_token(t_token **head, char *value, t_state state);
void			add_token(t_token **head, t_token *new_token);
void			end_token(char **buffer, t_token **head, t_state state);
t_token_type	get_token_type(char *value);
t_token			**join_token(t_token **head);
t_token			**assign_types(t_token **head);
t_token			**check_validity(t_token **head);

// Create Commands
t_command		**create_commands(t_command **head_c, t_token **head_t, \
									t_main **main);
t_command		*init_empty_cmd(t_main **main);
t_command		*add_command(t_command **head, t_command *new_cmd);
void			handle_heredoc(t_command **cmd, t_token *curr);
void			handle_operators(t_command **cmd, t_token *curr, \
								t_token **head_t);
void			handle_argument(t_command **cmd, char *value, t_state state);

// Debugging Helpers
void			print_token_list(t_token **head);
void			print_cmd_list(t_command **head);
void			print_keys(char **env_keys);
void			print_key_val(char ***filtered_envp);

//Execution
void			execute_commands(t_main **main);
void			exec_child(t_command *cmd, t_main **main, int original_std[2]);
int				exec_builtin(t_command *cmd, t_main *main);
int				exec_external(t_command *cmd, char **env_vars);
char			*get_command_path(const char *command, char **env_vars);
char			*join_path_and_command(const char *dir, const char *command);
int				command_exists_in_dir(const char *dir, const char *command);
void			ft_wait(t_command *cmd, int original_std[2]);
void			handle_special_builtin(t_command **cmd);

//Pipe
void			pipe_handler(t_command *cmd);
void			dup2_out( int *pipe_fd);
void			dup2_in(int *pipe_fd);
void			child_pipe_close(t_command *cmd, int original_std[2]);
void			parent_pipe_close(t_command *cmd);
void			setup_pipe_redirections(t_command *cmd);

int				is_builtin(char *command);
//File redirection
int				setup_file_redirections(t_command *cmd);
int				type_redir_exist(t_command *cmd, t_token_type	type);

//External commands
int				ft_unset(char **args, t_main *main);
int				ft_pwd(void);
int				ft_exit(t_command *cmd);
int				ft_env(t_main *main, t_command *cmd);
int				ft_echo(t_command *cmd);

//export//
int				ft_export(char **args, t_main *main, t_command *cmd);
int				is_in_env(char **env_vars, const char *args);
int				ft_export_helper(char **args, int error_ret, t_main *main);
int				exp_env_update(char **env_vars, int index, const char *value);
int				env_add(char ***env_vars, const char *value);
char			*get_env_name(const char *src);
	//cd//
int				ft_cd(t_command *cmd, char ***env);
int				cd_env_update(const char *key, const char *value, \
								char **env_vars);
int				find_and_update_env(const char *key, char *env_entry, \
									char **env_vars, int key_len);
int				update_oldpwd(char ***env);
const char		*get_home_path(char **env);

//Fork
void			fork_handler(t_command *cmd);

//Utilz
void			safe_close(int *fd);
void			std_fd_reset(t_command *cmd, int original_std[2]);
int				is_special_builtin(char *command);
void			ft_fprintf(const char *format, ...);
void			ft_child_exit(int exit_code);

//Heredoc
void			exec_heredoc(t_command *cmds);
void			free_heredoc(t_command *cmd);
void			remove_heredoc_file(t_main *main);
char			*expand_variables_in_line(char *line, char **envp, \
					t_main *main);
char			*process_variable(char **res, char *start, char **envp, \
					t_main *main);
char			*heredoc_get_value(const char *var_name, char **envp);
void			append_text(char **res, char *text, size_t len);

//Signal
void			set_signals_interactive(void);
void			set_signals_heredoc(void);
void			set_signals_child(void);
void			signal_quit_message(int signo);
void			set_signals_sleep_mode(void);
void			signal_reset_prompt_sleep(int signo);
void			set_signals_noniteractive(void);

//Main
t_main			*init_main(char **envp);
t_main			*allocate_main(void);
void			copy_env_vars(t_main *main, char **envp);

#endif
