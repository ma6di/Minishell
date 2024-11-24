//NORM OK
#include "minishell.h"

static int	nb_args(char **args)
{
	int	size;

	size = 0;
	while (args[size])
		size++;
	return (size);
}

static void	ft_echo_print(char **args, t_command *cmd, int i)
{
	while (args[i])
	{
		if (strcmp(args[i], "$?") == 0)
			ft_putnbr_fd(cmd->main->exit_code, 1);
		else
		{
			ft_putstr_fd(args[i], 1);
			if (args[i + 1])
				ft_putstr_fd(" ", 1);
		}
		i++;
	}
}

int	ft_echo(t_command *cmd)
{
	char	**args;
	int		i;
	int		n_option;

	i = 1;
	n_option = 0;
	args = cmd->args;
	if (nb_args(args) > 1)
	{
		while (args[i] && ft_strncmp(args[i], "-n", (ft_strlen(args[i])+ft_strlen("-n"))) == 0)
		{
			n_option = 1;
			i++;
		}
		ft_echo_print(args, cmd, i);
	}
	if (!n_option)
		ft_putstr_fd("\n", 1);
	return (SUCCESS);
}
