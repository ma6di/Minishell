//NORM OK

#include "../includes/minishell.h"

static char	**tokenize_path(char *path_env)
{
	return (ft_split(path_env, ':'));
}

static char	*get_path_env(char **env_vars)
{
	int	i;

	i = 0;
	while (env_vars[i])
	{
		if (ft_strncmp(env_vars[i], "PATH=", 5) == 0)
			return (env_vars[i] + 5);
		i++;
	}
	return (NULL);
}

static char	*find_command_in_paths(const char *command, char **paths)
{
	int	i;

	i = 0;
	while (paths[i])
	{
		if (command_exists_in_dir(paths[i], command))
		{
			return (join_path_and_command(paths[i], command));
		}
		i++;
	}
	return (NULL);
}

char	*get_command_path(const char *command, char **env_vars)
{
	char	*path_env;
	char	**paths;
	char	*full_path;

	path_env = get_path_env(env_vars);
	if (!path_env)
		return (NULL);
	paths = tokenize_path(path_env);
	if (!paths)
		return (NULL);
	full_path = find_command_in_paths(command, paths);
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
