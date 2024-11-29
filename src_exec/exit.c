//NORM OK
#include "minishell.h"

static int	is_numeric(const char *str)
{
	if (!str || *str == '\0')
		return (0);
	if (*str == '-' || *str == '+')
		str++;
	while (*str)
	{
		if (!ft_isalnum(*str) || !(*str >= '0' && *str <= '9'))
			return (0);
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
	if (!is_numeric(cmd->args[1]))
	{
		ft_putstr_fd(cmd->args[1], 2);
		ft_putendl_fd(": numeric argument required", 2);
		main->should_exit = 2;
		return (2); 
	}
	if (cmd->args[2])
	{
		ft_putendl_fd("minishell: exit: too many arguments", 2);
		return (1);
	}
	if (is_numeric(cmd->args[1]))
	{
		if(ft_atoi(cmd->args[1]) < 0)
		{
			main->should_exit = 256 + ft_atoi(cmd->args[1]);
			return (main->should_exit);
		}
	}
	main->should_exit = ft_atoi(cmd->args[1]);
	return (main->should_exit);
}

