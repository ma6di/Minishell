//NORM OK
#include "../includes/minishell.h"

char	*join_path_and_command(const char *dir, const char *command)
{
	size_t	dir_len;
	size_t	cmd_len;
	char	*full_path;

	dir_len = ft_strlen(dir);
	cmd_len = ft_strlen(command);
	full_path = malloc(dir_len + cmd_len + 2);
	if (!full_path)
	{
		perror("minishell: malloc failed");
		return (NULL);
	}
	ft_strlcpy(full_path, dir, dir_len + 1);
	full_path[dir_len] = '/';
	ft_strlcpy(full_path + dir_len + 1, command, cmd_len + 1);
	return (full_path);
}

int	command_exists_in_dir(const char *dir, const char *command)
{
	char	*full_path;
	int		exists;

	full_path = join_path_and_command(dir, command);
	if (!full_path)
		return (0);
	exists = (access(full_path, X_OK) == 0);
	free(full_path);
	return (exists);
}

static void	cmd_not_found(t_command *cmd)
{
	write(2, "minishell: ", 11);
	write(2, cmd->command, ft_strlen(cmd->command));
	write(2, ": command not found\n", 20);
}

static int	error_code(int err_code)
{
	if (err_code == EACCES)
	{
		perror("minishell: permission denied");
		return (126);
	}
	else if (err_code == ENOENT)
	{
		perror("minishell");
		return (127);
	}
	else
	{
		perror("minishell: execution failed");
		return (1);
	}
}

int	exec_external(t_command *cmd, char **env_vars)
{
	char	*path;
	int		exec_result;

	if (!cmd || !cmd->command || !cmd->args)
	{
		write(1, "error\n", 6);
		fprintf(stderr, "minishell: invalid command structure\n");
		return (1);
	}
	path = get_command_path(cmd->command, env_vars);
	if (!path)
	{
		cmd_not_found(cmd);
		return (127);
	}
	exec_result = execve(path, cmd->args, env_vars);
	free(path);
	if (exec_result == -1 && cmd->io_fds->has_heredoc && cmd->command)
		return (error_code(errno));
	return (0);
}
