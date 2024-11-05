/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrauh <nrauh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 10:34:32 by nrauh             #+#    #+#             */
/*   Updated: 2024/11/05 14:18:41 by nrauh            ###   ########.fr       */
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

char	**get_keys(char **env_keys, int key_count, char *str)
{
	int		i;
	char	*start;
	size_t	len;

	i = 0;
	len = 0;
	while(i < key_count)
	{
		start = ft_strchr(str + len, '$') + 1;
		while (start[len] >= 65 && start[len] <= 90)
			len++;
		env_keys[i] = ft_substr(str, start - str, len);
		i++;
	}
	return (env_keys);
}

/*char	***filter_envp(char **envp, char **env_keys, int key_count)
{
	int		i;
	int		j;
	char	**tmp;
	char	***filtered_envp;

	i = 0;
	j = 0;
	filtered_envp = malloc((key_count + 1) * sizeof(char **));
	filtered_envp[key_count] = NULL;
	while (envp[i])
	{
		tmp = ft_split(envp[i], '=');
		while (env_keys[j])
		{
			if (tmp[0] == env_keys[j])
				filtered_envp[j] = tmp;
			j++;
		}
		j = 0;
		i++;
	}
	return (filtered_envp);
}*/

char	*get_value(char *env_key, char ***envp_key_val)
{
	int	i;

	i = 0;
	while (envp_key_val[i])
	{
		if (ft_strncmp(env_key, envp_key_val[i][0], ft_strlen(env_key)))
			return (envp_key_val[i][1]);
		i++;
	}
	return (strdup(""));
}

char	*expand_values(char **env_keys, char ***envp_key_val, int key_count, t_token *token)
{
	char	*joined;
	char	*tmp_value;
	int		start;
	int		i;

	i = 0;
	while (env_keys[i])
	{
		tmp_value = get_value(env_keys[i], envp_key_val);
		start = ft_strchr(token->value, '$');
		// counting the first part of the str and create substr
		// adding the value with strjoin
		// counting the next part of the str and create substr
		// if it is a env var adding the value with strjoin
		// adding the last bit of the string with strjoin
		// freeing accordingly ...
		//joined = ft_strjoin(ft_substr(token->value, start, ), tmp_value);
	}
	// strjoin it to the str before the $ index (0 - ?)
	// add the rest of the str after the last CAPS letter (? - END)
	// free the value
	// store joined str in the value
	// free previous curr->value
	free(token->value);
	return (joined);
}

char	***split_key_value(char **envp)
{
	char 	***envp_key_val;
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
			return (free_key_val(envp_key_val), NULL);
		count++;
	}
	return (envp_key_val);
}

t_token	**expand(t_token **head, char **envp)
{
	t_token	*curr;
	char	***envp_key_val;
	char	**env_keys;
	int		key_count;

	envp_key_val = split_key_value(envp);
	if (!envp_key_val)
		return (free_key_val(envp_key_val), NULL);
	print_key_val(envp_key_val);
	free_key_val(envp_key_val);
	curr = *head;
	while (curr)
	{
		if (curr->state != STATE_QUOTE)
		{
			key_count = count_vars(curr->value);
			env_keys = malloc((key_count + 1) * sizeof(char *));
			env_keys[key_count] = NULL;
			env_keys = get_keys(env_keys, key_count, curr->value);
			// should return updated list...
			curr->value = expand_values(env_keys, envp, key_count, curr);
			//printf("%s\n", value);
			print_keys(env_keys);
			free_keys(env_keys);
		}
		curr = curr->next;
	}
	return (head);
}
