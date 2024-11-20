//NORM OK
#include "../includes/minishell.h"

int	env_add(char ***env_vars, const char *value)
{
	int		count;
	char	**old_env;
	char	**new_env_vars;
	int		i;

	count = 0;
	old_env = *env_vars;
	while (old_env && old_env[count])
		count++;
	new_env_vars = malloc(sizeof(char *) * (count + 2));
	if (!new_env_vars)
		return (-1);
	i = 0;
	while (i < count)
	{
		new_env_vars[i] = old_env[i];
		i++;
	}
	new_env_vars[count] = ft_strdup(value);
	new_env_vars[count + 1] = NULL;
	*env_vars = new_env_vars;
	return (SUCCESS);
}

static int	is_in_env_helper(char *var_name, char *env_name )
{
	if (ft_strncmp(var_name, env_name, ft_strlen(var_name)) == 0)
	{
		if (env_name && var_name)
		{
			free(env_name);
			free(var_name);
		}
		return (1);
	}
	return (0);
}

int	is_in_env(char **env_vars, const char *args)
{
	char	*var_name;
	char	*env_name;
	int		i;

	i = 0;
	var_name = get_env_name(args);
	if (!var_name)
		return (0);
	while (env_vars && env_vars[i])
	{
		env_name = get_env_name(env_vars[i]);
		if (!env_name)
			return (0);
		if (is_in_env_helper(var_name, env_name))
			return (1);
		if (env_name)
			free(env_name);
		i++;
	}
	if (var_name)
		free(var_name);
	return (0);
}
