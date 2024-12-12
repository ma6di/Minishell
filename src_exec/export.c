/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrauh <nrauh@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 16:57:11 by mcheragh          #+#    #+#             */
/*   Updated: 2024/12/12 18:24:19 by nrauh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	is_valid_env(const char *arg)
{
	const char	*equal_pos;
	size_t		name_len;
	size_t		i;

	if (!arg)
		return (0);
	equal_pos = strchr(arg, '=');
	if (equal_pos)
		name_len = (size_t)(equal_pos - arg);
	else
		return (1);
	if (!ft_isalpha(arg[0]) && arg[0] != '_')
		return (0);
	i = 1;
	while (i < name_len)
	{
		if (!(ft_isalnum(arg[i]) || arg[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

static int	print_error(int error, const char *arg)
{
	int	i;

	i = 0;
	ft_fprintf("Minishell: export: '");
	while (arg[i] && (arg[i] != '=' || error == -3))
	{
		ft_putchar_fd(arg[i], 2);
		i++;
	}
	if (error == -1)
		ft_fprintf("': not valid in this context\n");
	else if (error == 0 || error == -3)
		ft_fprintf("': not a valid identifier\n");
	return (-1);
}

static int	check_and_process_arg(char *arg, int *error_ret)
{
	*error_ret = is_valid_env(arg);
	if (arg[0] == '=')
		*error_ret = -3;
	if (*error_ret <= 0)
	{
		print_error(*error_ret, arg);
		return (-1);
	}
	return (0);
}

static int	process_env_variable(t_main *main, char *arg)
{
	int	env_index;

	env_index = is_in_env(main->env_vars, arg);
	if (env_index >= 0)
	{
		if (exp_env_update(main->env_vars, env_index, arg) == -1)
		{
			ft_fprintf("Minishell: failed to update variable");
			return (-1);
		}
	}
	else
	{
		if (env_add(&main->env_vars, arg) == -1)
		{
			ft_fprintf("Minishell: failed to add variable");
			return (-1);
		}
	}
	return (0);
}

int	ft_export_helper(char **args, int error_ret, t_main *main)
{
	int		i;
	int		error_found;

	i = 1;
	error_found = 0;
	while (args[i])
	{
		if (check_and_process_arg(args[i], &error_ret) == -1)
		{
			error_found = 1;
			i++;
			continue ;
		}
		if (!strchr(args[i], '='))
		{
			i++;
			continue ;
		}
		if (process_env_variable(main, args[i]) == -1)
			break ;
		i++;
	}
	return (error_found);
}
