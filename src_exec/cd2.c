/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd2.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcheragh <mcheragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 16:56:25 by mcheragh          #+#    #+#             */
/*   Updated: 2024/12/12 16:56:28 by mcheragh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	update_oldpwd(char ***env)
{
	char	cwd[MAX_PATH_LENGTH];

	if (!getcwd(cwd, sizeof(cwd)))
		return (ERROR);
	if (is_in_env(*env, "OLDPWD=") != -1)
	{
		if (cd_env_update("OLDPWD", cwd, *env) == -1)
			return (ERROR);
	}
	else if (env_add(env, ft_strjoin("OLDPWD=", cwd)) == -1)
		return (ERROR);
	return (SUCCESS);
}

const char	*get_home_path(char **env)
{
	int		i;
	int		len;

	len = ft_strlen("HOME=");
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], "HOME=", len) == 0)
			return (env[i] + len);
		i++;
	}
	return (NULL);
}

int	find_and_update_env(const char *key, char *env_entry, \
						char **env_vars, int key_len)
{
	int	i;

	i = 0;
	while (env_vars && env_vars[i])
	{
		if (ft_strncmp(env_vars[i], key, key_len) == 0 && \
			env_vars[i][key_len] == '=')
		{
			free(env_vars[i]);
			env_vars[i] = strdup(env_entry);
			return (1);
		}
		i++;
	}
	return (1);
}
