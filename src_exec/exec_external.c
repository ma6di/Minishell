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
		free(full_path);
		ft_fprintf("minishell: malloc failed");
		return (NULL);
	}
	ft_strlcpy(full_path, dir, dir_len + 1);
	full_path[dir_len] = '/';
	ft_strlcpy(full_path + dir_len + 1, command, cmd_len + 1);
	return (full_path);
}

static int	is_directory(const char *path)
{
	struct stat	path_stat;

	if (stat(path, &path_stat) == -1)
		return (0);
	return (S_ISDIR(path_stat.st_mode));
}

static int	error_code(int err_code, t_command *cmd)
{
	if (err_code == EACCES)
	{
		ft_fprintf("minishell: %s: Permission denied\n", cmd->command);
		return (126);
	}
	else if (err_code == ENOENT)
	{
		ft_fprintf("minishell: %s: No such file or directory\n", cmd->command);
		return (127);
	}
	else if (err_code == EISDIR)
	{
		ft_fprintf("Minishell: %s: Is a directory\n", cmd->command);
		return (126);
	}
	else
	{
		perror("Minishell");
		return (1);
	}
}

static int	exec_dir(t_command *cmd)
{
	if (access(cmd->command, F_OK) != 0)
	{
		ft_fprintf("Minishell: %s: No such file or directory\n", cmd->command);
		return (127);
	}
	if (is_directory(cmd->command))
	{
		ft_fprintf("Minishell: %s: Is a directory\n", cmd->command);
		return (126);
	}
	if (access(cmd->command, X_OK) != 0)
	{
		ft_fprintf("Minishell: %s: Permission denied\n", cmd->command);
		return (126);
	}
	if (execve(cmd->command, cmd->args, cmd->main->env_vars) == -1)
	{
		return (error_code(errno, cmd));
	}
	return (0);
}

int	exec_external(t_command *cmd, char **env_vars)
{
	char	*path;
	int		exec_result;

	if (!cmd || !cmd->command || !cmd->args)
	{
		ft_fprintf("Minishell: invalid command structure\n");
		return (1);
	}
	if (ft_strchr(cmd->command, '/'))
		return (exec_dir(cmd));
	path = get_command_path(cmd->command, env_vars);
	if (!path)
	{
		ft_fprintf("Minishell: %s: command not found\n", cmd->command);
		free(path);
		return (127);
	}
	exec_result = execve(path, cmd->args, env_vars);
	if (exec_result == -1)
	{
		free(path);
		return (error_code(errno, cmd));
	}
	return (0);
}
