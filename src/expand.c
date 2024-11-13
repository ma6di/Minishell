/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrauh <nrauh@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 10:34:32 by nrauh             #+#    #+#             */
/*   Updated: 2024/11/13 17:40:56 by nrauh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	count_vars(char *str)
{
	int	count;

	count = 0;
	while (*str)
	{
		if (*str == '$')
			count++;
		str++;
	}
	return (count);
}

char	***split_envp(char **envp)
{
	char	***envp_key_val;
	size_t	count;

	count = 0;
	while (envp[count])
		count++;
	envp_key_val = malloc((count + 1) * sizeof(char **));
	if (!envp_key_val)
		return (NULL);
	envp_key_val[count] = NULL;
	count = 0;
	while (envp[count])
	{
		envp_key_val[count] = ft_split(envp[count], '=');
		if (!envp_key_val[count])
			return (free_three_dim(envp_key_val), NULL);
		count++;
	}
	return (envp_key_val);
}

/*char	*get_value(char *env_key, char ***envp_key_val)
{
	int	i;

	i = 0;
	while (envp_key_val[i])
	{
		if (ft_strncmp(env_key, envp_key_val[i][0], ft_strlen(env_key)) == 0)
			return (ft_strdup(envp_key_val[i][1]));
		i++;
	}
	return (ft_strdup(""));
}*/

char	*get_value_two_dim(char *env_key, char **envp)
{
	int		i;
	int		j;
	char	*key;
	char	*value;

	i = 0;
	while (envp[i])
	{
		while (envp[i][j] != '=')
			add_to_buffer(&key, envp[i][j++]);
		j++;
		while (envp[i][j])
			add_to_buffer(&value, envp[i][j++]);
		if (ft_strncmp(env_key, key, ft_strlen(env_key)) == 0)
			return (value);
		i++;
	}
	return (ft_strdup(""));
}

t_token	**expand_keys_two_dim(t_token **head, char **envp)
{
	t_token	*curr;
	char	*tmp;

	curr = *head;
	while (curr)
	{
		if (curr->state != QUOTE && curr->value[0] == '$'
			&& ft_strlen(curr->value) > 1)
		{
			tmp = get_value_two_dim(curr->value + 1, envp);
			free(curr->value);
			curr->value = tmp;
		}
		curr = curr->next;
	}
	return (head);
}

/*t_token	**expand_keys(t_token **head, char ***envp_key_val)
{
	t_token	*curr;
	char	*tmp;

	curr = *head;
	while (curr)
	{
		if (curr->state != QUOTE && curr->value[0] == '$'
			&& ft_strlen(curr->value) > 1)
		{
			tmp = get_value(curr->value + 1, envp_key_val);
			free(curr->value);
			curr->value = tmp;
		}
		curr = curr->next;
	}
	return (head);
}*/

t_token	**expand(t_token **head, char **envp)
{
	t_token	*curr;
	//char	***envp_key_val;

	//envp_key_val = split_envp(envp);
	//if (!envp_key_val)
	//	return (free_three_dim(envp_key_val), NULL);
	curr = *head;
	while (curr)
	{
		if (curr->state != QUOTE)
			head = expand_keys_two_dim(head, envp);
		curr = curr->next;
	}
	//print_key_val(envp_key_val);
	//free_three_dim(envp_key_val);
	return (head);
}
