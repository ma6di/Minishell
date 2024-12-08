//NORM OK
#include "../includes/minishell.h"

void	remove_heredoc_file(t_main *main)
{
	t_command	*cmd;

	cmd = main->command_list;
	while (cmd)
	{
		if (cmd->io_fds->has_heredoc)
			unlink("heredoc.txt");
		cmd = cmd->next;
	}
}

int	is_builtin(char *command)
{
	size_t	cmd_len;

	if (!command)
		return (0);
	cmd_len = ft_strlen(command);
	if (ft_strncmp(command, "echo", ft_strlen("echo") + cmd_len) == 0 || \
		ft_strncmp(command, "pwd", ft_strlen("pwd") + cmd_len) == 0 || \
		ft_strncmp(command, "env", ft_strlen("env") + cmd_len) == 0)
	{
		return (1);
	}
	return (0);
}

int	is_special_builtin(char *command)
{
	size_t	cmd_len;

	if (!command)
		return (0);
	cmd_len = ft_strlen(command);
	if (ft_strncmp(command, "cd", ft_strlen("cd") + cmd_len) == 0 || \
		ft_strncmp(command, "export", ft_strlen("export") + cmd_len) == 0 || \
		ft_strncmp(command, "unset", ft_strlen("unset") + cmd_len) == 0 || \
		ft_strncmp(command, "exit", ft_strlen("exit") + cmd_len) == 0)
	{
		return (1);
	}
	return (0);
}

int	type_redir_exist(t_command *cmd, t_token_type	type)
{
	int	i;

	i = 0;
	while (cmd->operators && cmd->operators[i])
	{
		if (cmd->operators[i]->type == type)
			return (1);
		i++;
	}
	return (0);
}

void	ft_child_exit(int exit_code)
{
	char	*new_prog;
	char	*args[2];
	char	*env[1];

	env[0] = NULL;
	if (exit_code == 1)
		new_prog = "/bin/false";
	else if (exit_code == 0)
		new_prog = "/bin/true";
	else
		return ;
	args[0] = new_prog;
	args[1] = NULL;
	if (execve(new_prog, args, env) == -1)
		perror("execve failed");
}
