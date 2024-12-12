/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcheragh <mcheragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 16:57:17 by mcheragh          #+#    #+#             */
/*   Updated: 2024/12/12 16:57:19 by mcheragh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	compare_env_name(const char *var_name, const char *env_var)
{
	char	*env_name;
	int		result;

	env_name = get_env_name(env_var);
	if (!env_name)
		return (-1);
	result = ft_strncmp(var_name, env_name, ft_strlen(env_name) + \
			ft_strlen(var_name)) == 0;
	free(env_name);
	return (result);
}

int	is_in_env(char **env_vars, const char *args)
{
	char	*var_name;
	int		i;

	i = 0;
	var_name = get_env_name(args);
	if (!var_name)
		return (-1);
	while (env_vars && env_vars[i])
	{
		if (compare_env_name(var_name, env_vars[i]) == 1)
		{
			free(var_name);
			return (i);
		}
		else if (compare_env_name(var_name, env_vars[i]) == -1)
		{
			free(var_name);
			return (-1);
		}
		i++;
	}
	free(var_name);
	return (-1);
}

int	exp_env_update(char **env_vars, int index, const char *value)
{
	char	*new_value;

	new_value = ft_strdup(value);
	if (!new_value)
		return (-1);
	free(env_vars[index]);
	env_vars[index] = new_value;
	return (0);
}

char	*get_env_name(const char *src)
{
	size_t	i;
	char	*dest;

	i = 0;
	dest = malloc(BUFF_SIZE);
	if (!dest)
		return (NULL);
	while (src[i] && src[i] != '=' && i < BUFF_SIZE - 1)
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}
