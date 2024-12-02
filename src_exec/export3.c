#include "../includes/minishell.h"

int	ft_export(char **args, t_main *main, t_command *cmd)
{
	int	error_ret;
	int	i;

	i = 0;
	error_ret = 0;
	if (cmd->has_pipe && cmd->next && cmd->args[1])
		return (0);
	else if (!args[1])
	{
		while (main->env_vars && main->env_vars[i])
		{
			if (main->env_vars[i] && main->env_vars[i][0] != '\0')
				ft_putendl_fd(main->env_vars[i], 1);
			i++;
		}
		return (0);
	}
	error_ret = ft_export_helper(args, 0, main);
	return (error_ret);
}
