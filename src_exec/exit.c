//NORM OK
#include "minishell.h"

// Helper function to check if a string is numeric
static int	is_numeric(const char *str)
{
	if (!str || *str == '\0')
		return (0);
	if (*str == '-' || *str == '+') // Handle optional sign
		str++;
	while (*str)
	{
		if (!ft_isalnum(*str) || !(*str >= '0' && *str <= '9')) // Must be a digit
			return (0);
		str++;
	}
	return (1);
}

void	ft_exit(t_main *main)
{
	t_command	*cmd;
	int			exit_code;

	cmd = main->command_list;
	//ft_putendl_fd("exit", 2);

	if (!cmd->args[1]) // No arguments
		exit(0);

	if (!is_numeric(cmd->args[1])) // Non-numeric argument
	{
		//ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(cmd->args[1], 2);
		ft_putendl_fd(": numeric argument required", 2);
		exit(2); // Exit with code 255 for non-numeric argument
	}

	if (cmd->args[2]) // More than one argument
	{
		main->exit_code = 1;
		ft_putendl_fd("minishell: exit: too many arguments", 2);
		return;
	}

	// Single numeric argument
	exit_code = atoi(cmd->args[1]);
	exit(exit_code);
}

