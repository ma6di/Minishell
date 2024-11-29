/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrauh <nrauh@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 16:23:19 by nrauh             #+#    #+#             */
/*   Updated: 2024/11/29 11:31:43 by nrauh            ###   ########.fr       */
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
# include <sys/stat.h>


# define WHITESPACE " "
# define OP_REDIRECT ">"
# define OP_INPUT_REDIRECT "<"
# define OP_APPEND ">>"
# define OP_HEREDOC "<<"
# define OP_PIPE "|"

# define MAX_PATH_LENGTH 4096

# define BUFF_SIZE 1024

extern int	g_sigint_received;
extern int	g_pid;


typedef struct s_fds		t_fds;	// Forward declaration
typedef struct s_token		t_token;	// Forward declaration
typedef struct s_command	t_command;	// Forward declaration
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
	IO_REDIRECT
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
	char		**shell_vars;
	int			exit_code;
	bool		is_sleeping;
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
	char				*command; //cat
	char				**args;
	t_heredoc			**heredocs; // [t_heredoc, t_heredoc, NULL]
	t_operator			**operators; // [t_operator.....]
	int					nr_of_pipes;
	int					*pipe_fd;
	int					has_pipe;
	int					error_code;
	char				*error_message;
	char				*result_file;
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
	char	*infile; //heredoc.txt -> need to free the file before replacing it
	char	*outfile;
	char	*append_outfile;
	int		fd_in;
	int		fd_out;
	int		has_heredoc; // 2 nr_of_heredoc
	int		fd_err;
	int		is_stderr_redirected;
	int		in_duped;
	int		out_duped;
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
// heredoc.txt (in)  filename(in)
// . < < << > < >> < >> <<
// [file heredoc missing_file]

typedef struct s_operator
{
	t_token_type	type; // infile , outfile, append
	char			*filename; // custom
}				t_operator;


t_command		*lexer(char *input, char **envp, t_main **main);
t_token			**parse(t_token **head, char *input);
t_token			**expand(t_token **head, char **envp, t_main *main);
t_token			**expand_keys(t_token **head, char **envp, t_main *main);
void			free_tokens(t_token **head);
void			free_commands(t_command **head);
void			free_command_child(t_command **cmd);
void			free_main(t_main *main);
void			add_token(t_token **head, t_token *new_token);
void			create_token(t_token **head, char *value, t_state state);
t_token_type	get_token_type(char *value);
void			print_token_list(t_token **head);
void			print_cmd_list(t_command **head);
void			print_keys(char **env_keys);
void			print_key_val(char ***filtered_envp);
void			free_two_dim(char **env_keys);
void			free_three_dim(char ***filtered_envp);
int				is_whitespace(char c);
void			display_error(char *message, char *value, t_token **head);
t_token			**join_token(t_token **head);
t_token			**assign_types(t_token **head);
t_token			**check_validity(t_token **head);
char			*add_to_buffer(char **buffer, char c);
void			end_token(char **buffer, t_token **head, t_state state);
int				is_operator_char(char c);
int				is_delimiter(char c);
t_command		**create_commands(t_command **head_c, t_token **head_t, t_main **main);
void			init_empty_fds(t_command **new_cmd);
t_command		*init_empty_cmd(t_main **main);
t_command		*add_command(t_command **head, t_command *new_cmd);
char			*get_command_path(const char *command, char **env_vars);
int				execute_external(t_command *cmd, char **env_vars);
void			execute_commands(t_main **main);
void			exec_child(t_command *cmd, t_main **main, int original_stdout, int original_stdin);
int				is_builtin(char *command);
int				setup_file_redirections(t_command *cmd);
void			pipe_handler(t_command *cmd);
int				exec_builtin(t_command *cmd, t_main *main);
int				exec_external(t_command *cmd, char **env_vars);
void			setup_test_data(t_main *main, char **argv);
void			cleanup_commands(t_main *mian);
int				ft_unset(char **args, t_main *main);
int				ft_pwd(void);
int				is_in_env(char **env_vars, const char *args);
int				ft_export(char **args, t_main *main, t_command *cmd);
int				env_add(char ***env_vars, const char *value);
int				ft_exit(t_main *main);
int				ft_env(t_main *main, t_command *cmd);
int				ft_echo(t_command *cmd);
int				ft_cd(t_command *cmd, char **env);
void			safe_close(int *fd);
void			fork_handler(t_command *cmd);
void			ft_wait(t_command *cmd);
void			dup2_out( int *pipe_fd);
void			dup2_in(int *pipe_fd);
void			parent_pipe_close(t_command *cmd);
void			ft_fd_reset(t_command *cmd, int original_stdin, int original_stdout);
void			handle_special_builtin(t_command **cmd);
int				is_special_builtin(char *command);
void			setup_pipe_redirections_parent(t_command *cmd);
void			setup_pipe_redirections_child(t_command *cmd);
void			exec_heredoc(t_command *cmds);
void			remove_heredoc_file(t_main *main);
void			set_signals_interactive(void);
void			set_signals_heredoc(void);	
void			set_signals_child(void);
void			signal_quit_message(int signo);
void			set_signals_sleep_mode(void);
void			signal_reset_prompt_sleep(int signo);
void			set_signals_noniteractive(void);
char			*expand_variables_in_line(char *line, char **envp);
char			*get_env_name(const char *src);
void			cd_print_error(const char *arg);
int				command_exists_in_dir(const char *dir, const char *command);
char			*join_path_and_command(const char *dir, const char *command);
int 			exp_env_update(char **env_vars, int index, const char *value);
void			child_pipe_close(t_command *cmd);
void			is_it_cat(t_command *cmd);
int				type_redir_exist(t_command *cmd, t_token_type	type);
void			ft_fprintf(const char *format, ...);


#endif
