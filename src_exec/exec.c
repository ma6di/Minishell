/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcheragh <mcheragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 16:56:59 by mcheragh          #+#    #+#             */
/*   Updated: 2024/12/12 16:57:01 by mcheragh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	setup_pipe_redirections(cmd);
	if (ft_strncmp(cmd->command, "cd", ft_strlen("cd") + cmd_len) == 0)
		status = ft_cd(cmd, &main->env_vars);
	else if (ft_strncmp(cmd->command, "export", ft_strlen("export") + \
		cmd_len) == 0)
		status = ft_export(cmd->args, main, cmd);
	else if (ft_strncmp(cmd->command, "unset", ft_strlen("unset") + \
		cmd_len) == 0)
		status = ft_unset(cmd->args, main);
	else if (ft_strncmp(cmd->command, "exit", ft_strlen("exit") + cmd_len) == 0)
		status = ft_exit(cmd);
	return (status);
}

void	exec_child(t_command *cmd, t_main **main, int original_std[2])
{
	int	exit_code;

	set_signals_child();
	if (cmd->pid == 0)
	{
		if (setup_file_redirections(cmd) == -1)
			ft_child_exit(1);
		setup_pipe_redirections(cmd);
		child_pipe_close(cmd, original_std);
		if (is_builtin(cmd->command))
			cmd->main->exit_code = exec_builtin(cmd, cmd->main);
		else
			cmd->main->exit_code = exec_external(cmd, (*main)->env_vars);
		ft_child_exit(cmd->main->exit_code);
		rl_clear_history();
		exit_code = (*main)->exit_code;
		free_commands(&(*main)->command_list);
		free_main(*main);
		cmd = NULL;
		*main = NULL;
		exit(exit_code);
	}
}

void	execute_commands(t_main **main)
{
	t_command	*cmd;
	t_command	*command;
	int			original_std[2];

	cmd = (*main)->command_list;
	command = (*main)->command_list;
	while (cmd)
	{
		original_std[1] = dup(STDOUT_FILENO);
		original_std[0] = dup(STDIN_FILENO);
		pipe_handler(cmd);
		fork_handler(cmd);
		if (is_special_builtin(cmd->command))
			(*main)->exit_code = exec_special_builtin(cmd, *main);
		else
			exec_child(cmd, main, original_std);
		std_fd_reset(cmd, original_std);
		parent_pipe_close(cmd);
		cmd = cmd->next;
	}
	ft_wait(command, original_std);
}
