/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcheragh <mcheragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 16:57:24 by mcheragh          #+#    #+#             */
/*   Updated: 2024/12/12 18:43:40 by mcheragh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

char	**allocate_new_env(char **old_env, int count)
{
	char	**new_env_vars;
	int		i;

	new_env_vars = malloc(sizeof(char *) * (count + 2));
	if (!new_env_vars)
		return (NULL);
	i = 0;
	while (i < count)
	{
		new_env_vars[i] = old_env[i];
		i++;
	}
	new_env_vars[count + 1] = NULL;
	return (new_env_vars);
}

int	env_add(char ***env_vars, const char *value)
{
	int		count;
	char	**old_env;
	char	**new_env_vars;

	old_env = *env_vars;
	count = 0;
	while (old_env && old_env[count])
		count++;
	new_env_vars = allocate_new_env(old_env, count);
	if (!new_env_vars)
		return (-1);
	new_env_vars[count] = ft_strdup(value);
	if (!new_env_vars[count])
	{
		free(new_env_vars);
		return (-1);
	}
	free(old_env);
	*env_vars = new_env_vars;
	return (0);
}
