//NORM OK
#include "minishell.h"

static int	is_numeric(t_main *main, char *str)
{
	if (!str || *str == '\0')
		return (0);
	if (*str == '-' || *str == '+')
		str++;
	while (*str)
	{
		if (!ft_isalnum(*str) || !(*str >= '0' && *str <= '9'))
		{
			ft_fprintf("Minishell: exit: %s: numeric ", str);
			ft_fprintf("argument ./m./required\n");
			main->should_exit = 2;
			return (0);
		}
		str++;
	}
	return (1);
}

int	ft_exit(t_main *main)
{
	t_command	*cmd;

	cmd = main->command_list;
	if (!cmd->args[1])
	{
		main->should_exit = 0;
		return (0);
	}
	if (cmd->args[2])
	{
		ft_fprintf("Minishell: exit: too many arguments\n");
		return (1);
	}
	if (!is_numeric(main, cmd->args[1]))
		return (2);
	if (is_numeric(main, cmd->args[1]) && ft_atoi(cmd->args[1]) < 0)
	{
		main->should_exit = 256 + ft_atoi(cmd->args[1]);
		return (main->should_exit);
	}
	main->should_exit = ft_atoi(cmd->args[1]);
	return (main->should_exit);
}
