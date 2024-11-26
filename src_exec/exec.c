//NORM OK
#include "../includes/minishell.h"

int	exec_builtin(t_command *cmd, t_main *main)
{
	int		status;
	size_t	cmd_len;

	status = 0;
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

	status = 0;
	cmd_len = ft_strlen(cmd->command);
	if (setup_file_redirections(cmd) == -1)
		return (1);
	setup_pipe_redirections_parent(cmd);
	if (ft_strncmp(cmd->command, "cd", ft_strlen("cd") + cmd_len) == 0)
	{
		status = ft_cd(cmd, main->env_vars);
	}
	else if (ft_strncmp(cmd->command, "export", ft_strlen("export") + \
		cmd_len) == 0)
	{
		status = ft_export(cmd->args, main, cmd);
	}
	else if (ft_strncmp(cmd->command, "unset", ft_strlen("unset") + \
		cmd_len) == 0)
	{
		status = ft_unset(cmd->args, main);
	}
	else if (ft_strncmp(cmd->command, "exit", ft_strlen("exit") + cmd_len) == 0)
		status = ft_exit(main);
	return (status);
}

void	exec_child(t_command *cmd, t_main **main, int original_stdout, int original_stdin)
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
		if (setup_file_redirections(cmd) == -1)
			exit (1);
		setup_pipe_redirections_child(cmd);
		//is_it_cat(cmd);
		if (is_builtin(cmd->command))
		{
			cmd->main->exit_code = exec_builtin(cmd, cmd->main);
			safe_close(&original_stdin);
			safe_close(&original_stdout);
		}
		else
		{
			cmd->main->exit_code = exec_external(cmd, (*main)->env_vars);
		}
		child_pipe_close(cmd);
		// printf("command %s\n", cmd->command);
		free_command_child(&cmd);
		free(cmd);
		int	exit_code = (*main)->exit_code;
		free_main((*main));
		exit(exit_code);
	}
}

void	execute_commands(t_main **main)
{
	t_command	*cmd;
	t_command	*command;
	int			original_stdout;
	int			original_stdin;

	cmd = (*main)->command_list;
	command = (*main)->command_list;
	original_stdout = dup(STDOUT_FILENO);
	original_stdin = dup(STDIN_FILENO);
	while (cmd)
	{
		//is_it_cat(cmd);
		handle_special_builtin(&cmd);
		pipe_handler(cmd);
		fork_handler(cmd);
		if (is_special_builtin(cmd->command))
			(*main)->exit_code = exec_special_builtin(cmd, *main);
		else
			exec_child(cmd, main, original_stdin, original_stdout);
		ft_fd_reset(cmd, original_stdin, original_stdout);
		parent_pipe_close(cmd);
		cmd = cmd->next;
	}
	while(command)
	{
		if(command->pid >= 0)
		{
			ft_wait(command);
		}
		command = command->next;
	}
	safe_close(&original_stdin);
	safe_close(&original_stdout);
}