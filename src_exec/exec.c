//NORM OK
#include "../includes/minishell.h"

int	exec_builtin(t_command *cmd, t_main *main)
{
	int		status;
	size_t	cmd_len;

	status = 1;
	cmd_len = ft_strlen(cmd->command);
	if (ft_strncmp(cmd->command, "echo", ft_strlen("echo") + cmd_len) == 0)
		status = ft_echo(cmd);
	else if (ft_strncmp(cmd->command, "pwd", ft_strlen("pwd") + cmd_len) == 0)
		status = ft_pwd();
	else if (ft_strncmp(cmd->command, "env", ft_strlen("env") + cmd_len) == 0)
		status = ft_env(main, cmd);
	return (status);
}

int	exec_special_builtin(t_command *cmd, t_main *main)
{
	int		status;
	size_t	cmd_len;

	status = 1;
	cmd_len = ft_strlen(cmd->command);
	setup_file_redirections(cmd);
	setup_pipe_redirections_parent(cmd);
	if (ft_strncmp(cmd->command, "cd", ft_strlen("cd") + cmd_len) == 0)
		status = ft_cd(cmd, main->env_vars);
	else if (ft_strncmp(cmd->command, "export", ft_strlen("export") + \
		cmd_len) == 0)
		status = ft_export(cmd->args, main);
	else if (ft_strncmp(cmd->command, "unset", ft_strlen("unset") + \
		cmd_len) == 0)
		status = ft_unset(cmd->args, main);
	else if (ft_strncmp(cmd->command, "exit", ft_strlen("exit") + cmd_len) == 0)
		ft_exit(main);
	return (status);
}

void	exec_child(t_command *cmd, char **env)
{
	if (ft_strncmp(cmd->command, "sleep", ft_strlen("sleep") + \
		ft_strlen(cmd->command)) == 0)
		set_signals_sleep_mode();
	else
		set_signals_child();
	if (cmd->pid == 0)
	{
		if (ft_strncmp(cmd->command, "", ft_strlen(cmd->command)) == 0)
			exit (0);
		setup_file_redirections(cmd);
		setup_pipe_redirections_child(cmd);
		if (is_builtin(cmd->command))
			cmd->main->exit_code = exec_builtin(cmd, cmd->main);
		else
			cmd->main->exit_code = exec_external(cmd, env);
		exit(cmd->main->exit_code);
	}
	else
		ft_wait(cmd);
}

void	execute_commands(t_main *main)
{
	t_command	*cmd;
	int			original_stdout;
	int			original_stdin;

	cmd = main->command_list;
	original_stdout = dup(STDOUT_FILENO);
	original_stdin = dup(STDIN_FILENO);
	while (cmd)
	{
		handle_special_builtin(&cmd);
		pipe_handler(cmd);
		fork_handler(cmd);
		if (is_special_builtin(cmd->command))
			cmd->main->exit_code = exec_special_builtin(cmd, main);
		else
			exec_child(cmd, main->env_vars);
		parent_pipe_close(cmd);
		ft_fd_reset(cmd, original_stdin, original_stdout);
		cmd = cmd->next;
	}
}
