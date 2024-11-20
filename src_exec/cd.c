//NORM OK
#include "../includes/minishell.h"

static int	env_update(const char *key, const char *value, char **env_vars)
{
	int		i;
	char	*env_entry;

	i = 0;
	env_entry = malloc(ft_strlen(key) + ft_strlen(value) + 2);
	if (!env_entry)
		return (-1);
	while (env_vars && env_vars[i])
	{
		if (ft_strncmp(env_vars[i], key, ft_strlen(key)) == 0 && \
			env_vars[i][ft_strlen(key)] == '=')
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

int	ft_cd(t_command *cmd, char **env)
{
	char	**args;
	char	cwd[MAX_PATH_LENGTH];

	args = cmd->args;
	if (cmd->io_fds->infile || !args[1] || !is_valid_path(args[1]))
		return (SUCCESS);
	if (update_oldpwd(env) == ERROR || chdir(args[1]) < 0)
	{
		cd_print_error(args[1]);
		return (CD_ERROR);
	}
	if (getcwd(cwd, sizeof(cwd)))
		env_update("PWD", cwd, env);
	return (CD_SUCCESS);
}
