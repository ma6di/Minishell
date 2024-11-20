//NORM OK
#include "minishell.h"

static size_t	env_size(const char *env)
{
	size_t	i;

	i = 0;
	while (env[i] && env[i] != '=')
		i++;
	return (i);
}

static void	free_env_var(char **env_vars, size_t index)
{
	free(env_vars[index]);
	while (env_vars[index])
	{
		env_vars[index] = env_vars[index + 1];
		index++;
	}
}

int	ft_unset(char **args, t_main *main)
{
	size_t	i;
	size_t	j;

	i = 1;
	j = 0;
	while (args[i])
	{
		while (main->env_vars[j])
		{
			if (strncmp(args[i], main->env_vars[j], \
				env_size(main->env_vars[j])) == 0)
			{
				free_env_var(main->env_vars, j);
				break ;
			}
			j++;
		}
		i++;
	}
	return (SUCCESS);
}
