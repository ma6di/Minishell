//NORM OK
#include "minishell.h"

void	ft_exit(t_main *main)
{
	t_command	*cmd;

	cmd = main->command_list;
	if (cmd->args[0] && cmd->args[1])
	{
		main->exit_code = 1;
		ft_putendl_fd("exit", 2);
		ft_putendl_fd("minishell: exit: too many arguments", 2);
	}
	else
	{
		ft_putstr_fd("exit ", 2);
		main->exit_code = 0;
	}
}
