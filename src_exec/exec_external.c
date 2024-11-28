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

static void	is_a_dir(t_command *cmd)
{
	write(2, "minishell: ", 11);
	write(2, cmd->command, ft_strlen(cmd->command));
	write(2, ": Is a directory\n", 17);
}

static void	no_such_dir(t_command *cmd)
{
	write(2, "minishell: ", 11);
	write(2, cmd->command, ft_strlen(cmd->command));
	write(2, ": No such file or directory\n", 28);
}

static int is_directory(const char *path)
{
    struct stat path_stat;

    if (stat(path, &path_stat) == -1)
        return (0); // If stat fails, it's not accessible or doesn't exist
    return (S_ISDIR(path_stat.st_mode)); // Check if it's a directory
}

static int error_code(int err_code, t_command *cmd)
{
    if (err_code == EACCES)
    {
        printf("minishell: %s: Permission denied\n", cmd->command);
        return (126);
    }
    else if (err_code == ENOENT)
    {
        no_such_dir(cmd);
        return (127);  // Command not found error code
    }
    else if (err_code == EISDIR)
    {
       	is_a_dir(cmd);
        return (126);  // Directory execution error code
    }
    else
    {
        perror("minishell");
        return (1);  // General execution error code
    }
}

int exec_external(t_command *cmd, char **env_vars)
{
    char    *path;
    int     exec_result;

    if (!cmd || !cmd->command || !cmd->args)
    {
        fprintf(stderr, "minishell: invalid command structure\n");
        return (1);
    }

    // Check if command contains '/' (indicating direct path)
    if (ft_strchr(cmd->command, '/'))
    {
        // Check if the command exists and is executable
        if (access(cmd->command, F_OK) != 0)
        {
            no_such_dir(cmd); // No such file or directory
            return (127);
        }
        if (is_directory(cmd->command))
        {
            is_a_dir(cmd); // Is a directory
            return (126);
        }
        if (access(cmd->command, X_OK) != 0)
        {
            fprintf(stderr, "minishell: %s: Permission denied\n", cmd->command);
            return (126);
        }

        // Attempt to execute the direct path
        exec_result = execve(cmd->command, cmd->args, env_vars);
        if (exec_result == -1)
        {
            return (error_code(errno, cmd)); // Handle specific errors
        }
        return (0);
    }

    // If no '/' in the command, resolve using PATH
    path = get_command_path(cmd->command, env_vars);
    if (!path)
    {
        cmd_not_found(cmd); // Command not found in PATH
        return (127);
    }

    // Attempt to execute the resolved path
    exec_result = execve(path, cmd->args, env_vars);
    free(path);
    if (exec_result == -1)
    {
        return (error_code(errno, cmd)); // Handle specific errors
    }

    return (0);
}
