/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcheragh <mcheragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 16:56:34 by mcheragh          #+#    #+#             */
/*   Updated: 2024/12/12 16:56:35 by mcheragh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	is_valid_n_option(const char *arg)
{
	int	i;

	i = 1;
	if (arg[0] != '-' || arg[1] != 'n')
		return (0);
	while (arg[++i])
	{
		if (arg[i] != 'n')
			return (0);
	}
	return (1);
}

static void	print_echo_output(char **args, int start, int suppress_newline)
{
	int	i;

	i = start;
	while (args[i])
	{
		ft_putstr_fd(args[i], 1);
		if (args[i + 1])
			ft_putstr_fd(" ", 1);
		i++;
	}
	if (!suppress_newline)
		ft_putstr_fd("\n", 1);
}

int	ft_echo(t_command *cmd)
{
	char	**args;
	int		i;
	int		suppress_newline;

	args = cmd->args;
	i = 1;
	suppress_newline = 0;
	while (args[i] && is_valid_n_option(args[i]))
	{
		suppress_newline = 1;
		i++;
	}
	print_echo_output(args, i, suppress_newline);
	return (SUCCESS);
}
