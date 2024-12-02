//NORM OK
#include "../includes/minishell.h"

int	ft_env(t_main *main, t_command *cmd)
{
	char	**env;

	env = main->env_vars;
	if (cmd->args[1])
	{
		ft_fprintf("Minishell: env: too many arguments\n");
		return (CD_ERROR);
	}
	if (!env || !*env)
		return (SUCCESS);
	while (*env)
	{
		if (*env && **env)
			ft_putendl_fd(*env, 1);
		env++;
	}
	return (SUCCESS);
}
