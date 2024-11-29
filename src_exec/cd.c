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
			return (env[i] + len);
		i++;
	}
	return (NULL);
}

int	ft_cd(t_command *cmd, char **env)
{
	char		**args;
	const char	*path;
	char		cwd[MAX_PATH_LENGTH];

	args = cmd->args;

	if(type_redir_exist(cmd, INFILE))
		return (SUCCESS);
	if (!args[1])
	{
		path = get_home_path(env);
		if (!path)
		{
			ft_fprintf("Minishell: cd: HOME not set\n");
			return (1);
		}
	}
	else
	{
		if (!is_valid_path(args[1]))
		{
			ft_fprintf("Minishell: cd: invalid path\n");
			return (1);
		}
		path = args[1];
	}
	if (args[2])
	{
		ft_fprintf("Minishell: cd: too many arguments\n");
		return (1);
	}
	if (update_oldpwd(env) == ERROR || chdir(path) < 0)
	{
		cd_print_error(path);
		return (CD_ERROR);
	}
	if (getcwd(cwd, sizeof(cwd)))
		env_update("PWD", cwd, env);
	return (CD_SUCCESS);
}

