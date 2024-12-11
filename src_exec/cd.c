//NORM OK
#include "../includes/minishell.h"

int	cd_env_update(const char *key, const char *value, char **env_vars)
{
	int		key_len;
	int		value_len;
	int		total_len;
	char	*env_entry;

	key_len = ft_strlen(key);
	value_len = ft_strlen(value);
	total_len = key_len + value_len + 2;
	env_entry = malloc(total_len);
	if (!env_entry)
		return (-1);
	ft_memcpy(env_entry, key, key_len);
	env_entry[key_len] = '=';
	ft_memcpy(env_entry + key_len + 1, value, value_len + 1);
	if (find_and_update_env(key, env_entry, env_vars, key_len))
		return (SUCCESS);
	free(env_entry);
	return (0);
}

static int	is_valid_path(const char *path)
{
	return (path && (path[0] == '/' || path[0] != '-'));
}

static const char	*get_cd_path(char **args, char **env)
{
	const char	*path;

	if (!args[1])
	{
		path = get_home_path(env);
		if (!path)
		{
			ft_fprintf("Minishell: cd: HOME not set\n");
			return (NULL);
		}
	}
	else
	{
		if (!is_valid_path(args[1]))
		{
			ft_fprintf("Minishell: cd: invalid path\n");
			return (NULL);
		}
		path = args[1];
	}
	return (path);
}

static int	handle_cd_execution(const char *path, char ***env)
{
	char	cwd[MAX_PATH_LENGTH];
	if (update_oldpwd(env) == ERROR || chdir(path) < 0)
	{
		ft_fprintf("Minishell: cd: %s: %s\n", path, strerror(errno));
		return (CD_ERROR);
	}
	if (getcwd(cwd, sizeof(cwd)))
		cd_env_update("PWD", cwd, *env);
	return (CD_SUCCESS);
}

int	ft_cd(t_command *cmd, char ***env)
{
	int i;
	const char	*path;

	i = 0;
	while(cmd->args[i])
		i++;
	if (type_redir_exist(cmd, INFILE))
		return (SUCCESS);
	if (i > 2)
	{
		ft_fprintf("Minishell: cd: too many arguments\n");
		return (1);
	}
	path = get_cd_path(cmd->args, *env);
	if (!path)
		return (1);
	return (handle_cd_execution(path, env));
}
 