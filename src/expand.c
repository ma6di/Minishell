/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrauh <nrauh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 10:34:32 by nrauh             #+#    #+#             */
/*   Updated: 2024/11/06 15:42:21 by nrauh            ###   ########.fr       */
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

char	*get_value(char *env_key, char ***envp_key_val)
{
	int	i;

	i = 0;
	while (envp_key_val[i])
	{
		if (ft_strncmp(env_key, envp_key_val[i][0], ft_strlen(env_key)))
			return (ft_strdup(envp_key_val[i][1]));
		i++;
	}
	return (ft_strdup(""));
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

int is_end(char c)
{
  if (c == '\'' || c == ' ' || c == '$' || c == '"')
    return (1);
  return (0);
}

int	count_words(char *str)
{
	int	count;

	count = 0;
	while (*str)
	{
		if (*str != '$')
		{
			count++;
			while (*str && *str != '$')
				str++;
		}
		if (*str == '$')
		{
			count++;
			str++;
			while (*str && !is_end(*str))
				str++;
		}
		if (*str == '"')
			str++;
	}
	return (count);
}

/*char	*expand_keys(int key_count, char **split_keys, char *str)
{
	// goal is to split a string to pieces
	// "first" "$KEY" "second" "$key"
	// or "first" "$KEY" "$KEY"
	int		i;
	char	*start;
	size_t	len;
	char	**split;

	i = 0;
	len = 0;
	while(i < key_count)
	{
		start = ft_strchr(str + len, '$') + 1;
		while (start[len] >= 65 && start[len] <= 90)
			len++;
		split_keys[i] = ft_substr(str, start - str, len);
		i++;
	}
	return (split_keys);
}*/

char	**expand_keys(char **split, char ***envp_key_val)
{
	// search through split
	// if [0] == '$' search envp_key_val
	// get_value store in tmp
	// free the previous value (key)
	// set tmp to current
	int		i;
	char	*tmp;

	i = 0;
	while (split[i])
	{
		if (split[i][0] == '$')
		{
			tmp = get_value(split[i], envp_key_val);
			free(split[i]);
			split[i] = tmp;
		}
		i++;
	}
	return (split);
}

char	**split_keys(int key_count, char **split, char *str)
{
	int		end;
	int		i;

	end = 0;
	i = 0;
	while(i < key_count)
	{
		while (str[end])
		{
			if (str[end] != '$')
			{
				while (str[end] && str[end] != '$')
					end++;
				split[i] = ft_substr(str, 0, end);
				i++;
				str = str + end;
				end = 0;
			}
			if (str[end] == '$')
			{
				end++;
				while (str[end] && !is_end(str[end]))
					end++;
				split[i] = ft_substr(str, 0, end);
				i++;
				str = str + end;
				end = 0;
			}
			if (*str == '"')
				end++;
		}
	}
	return (split);
}

t_token	**expand(t_token **head, char **envp)
{
	t_token	*curr;
	char	***envp_key_val;
	char	**split;
	int		key_count;

	envp_key_val = split_key_value(envp);
	if (!envp_key_val)
		return (free_key_val(envp_key_val), NULL);
	curr = *head;
	while (curr)
	{
		if (curr->state == STATE_DQUOTE)
		{
			key_count = count_words(curr->value);
			printf("key count%d\n", key_count);
			split = malloc((key_count + 1) * sizeof(char *));
			split[key_count] = NULL;
			split = split_keys(key_count, split, curr->value);
			split = expand_keys(split, envp_key_val);
			// expansion could happen in the get_keys function (cause we work with start/end of key)
			// then we save the extra **array for the keys
			//curr->value = expand_values(env_keys, envp, key_count, curr);
			print_keys(split);
			free_keys(split);
		}
		curr = curr->next;
	}
	//print_key_val(envp_key_val);
	free_key_val(envp_key_val);
	return (head);
}
