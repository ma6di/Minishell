//NORM OK
#include "../includes/minishell.h"

static int env_update(const char *key, const char *value, char **env_vars)
{
	int		i;
	char	*env_entry;
	int		key_len;
	int		value_len;
	int		total_len;

	i = 0;
	key_len = ft_strlen(key);
	value_len = ft_strlen(value);
	total_len = key_len + value_len + 2;
	env_entry = malloc(total_len);
	if (!env_entry)
		return (-1);
	ft_memcpy(env_entry, key, key_len);
	env_entry[key_len] = '=';
	ft_memcpy(env_entry + key_len + 1, value, value_len + 1);
	while (env_vars && env_vars[i])
	{
		if (ft_strncmp(env_vars[i], key, key_len) == 0 && \
			env_vars[i][key_len] == '=')
		{
			free(env_vars[i]);
			env_vars[i] = env_entry;
			return (SUCCESS);
		}
		i++;
	}
	free(env_entry);
	return (0);
}

static int	update_oldpwd(char **env)
{
	char	cwd[MAX_PATH_LENGTH];

	if (!getcwd(cwd, sizeof(cwd)))
		return (ERROR);
	if (is_in_env(env, "OLDPWD="))
	{
		if (env_update("OLDPWD", cwd, env) == -1)
			return (ERROR);
	}
	else if (env_add(&env, ft_strjoin("OLDPWD=", cwd)) == -1)
		return (ERROR);
	return (SUCCESS);
}

static int	is_valid_path(const char *path)
{
	return (path && (path[0] == '/' || path[0] != '-'));
}

static const char *get_home_path(char **env)
{
	int		i;
	int		len;

	len = ft_strlen("HOME=");
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], "HOME=", len) == 0)
			return (env[i] + len); // Return the value of HOME
		i++;
	}
	return (NULL); // Return NULL if HOME is not found
}

int	ft_cd(t_command *cmd, char **env)
{
	char		**args;
	const char	*path;
	char		cwd[MAX_PATH_LENGTH];

	args = cmd->args;
	if (cmd->io_fds->infile) // Ignore infile commands
		return (SUCCESS);
	if (!args[1]) // No arguments provided
	{
		path = get_home_path(env);
		if (!path)
		{
			write(2, "Minishell: cd: HOME not set\n", 28);
			return (1);
		}
	}
	else
	{
		if (!is_valid_path(args[1])) // Validate the argument
		{
			write(2, "Minishell: cd: invalid path\n", 28);
			return (1);
		}
		path = args[1];
	}
	if (args[2])
	{
		write(2, "Minishell: cd: too many arguments\n", 34);
		return (1);
	}
	if (update_oldpwd(env) == ERROR || chdir(path) < 0) // Attempt to change directory
	{
		cd_print_error(path);
		return (CD_ERROR);
	}
	if (getcwd(cwd, sizeof(cwd))) // Update the current working directory
		env_update("PWD", cwd, env);
	return (CD_SUCCESS);
}

