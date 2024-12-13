/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcheragh <mcheragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 16:57:06 by mcheragh          #+#    #+#             */
/*   Updated: 2024/12/12 17:00:44 by mcheragh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
			ft_fprintf("argument required\n");
			main->should_exit = 2;
			return (0);
		}
		str++;
	}
	return (1);
}

int	ft_exit(t_command *cmd)
{
	if (cmd->next || cmd->prev)
		return (0);
	if (!cmd->args[1])
	{
		cmd->main->should_exit = 0;
		return (0);
	}
	if (cmd->args[2])
	{
		ft_fprintf("Minishell: exit: too many arguments\n");
		return (1);
	}
	if (!is_numeric(cmd->main, cmd->args[1]))
		return (2);
	if (is_numeric(cmd->main, cmd->args[1]) && ft_atoi(cmd->args[1]) < 0)
	{
		cmd->main->should_exit = 256 + ft_atoi(cmd->args[1]);
		return (cmd->main->should_exit);
	}
	cmd->main->should_exit = ft_atoi(cmd->args[1]);
	return (cmd->main->should_exit);
}
