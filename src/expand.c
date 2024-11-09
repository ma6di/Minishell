/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrauh <nrauh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 10:34:32 by nrauh             #+#    #+#             */
/*   Updated: 2024/11/09 07:15:20 by nrauh            ###   ########.fr       */
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
			return (free_three_dim(envp_key_val), NULL);
		count++;
	}
	return (envp_key_val);
}

// int is_end(char c)
// {
// 	if (c == '\'' || c == ' ' || c == '$')
// 		return (1);
// 	return (0);
// }

// int	count_words(char *str)
// {
// 	int	count;

// 	count = 0;
// 	while (*str)
// 	{
// 		if (*str != '$')
// 		{
// 			count++;
// 			while (*str && *str != '$')
// 				str++;
// 		}
// 		if (*str == '$')
// 		{
// 			count++;
// 			str++;
// 			while (*str && !is_end(*str))
// 				str++;
// 		}
// 	}
// 	return (count);
// }

char	*get_value(char *env_key, char ***envp_key_val)
{
	int	i;

	i = 0;
	while (envp_key_val[i])
	{
		if (ft_strncmp(env_key, envp_key_val[i][0], ft_strlen(env_key)) == 0)
			return (ft_strdup(envp_key_val[i][1]));
		i++;
	}
	return (ft_strdup("NOTHING"));
}

/*char	**expand_keys(char **split, char ***envp_key_val)
{
	int				i;
	int				j;
	char			*tmp;

	i = 0;
	while (split[i])
	{
		j = 0;
		while (split[i][j])
		{
			if (split[i][j] == '$')
			{
				tmp = get_value(split[i] + 1, envp_key_val);
				free(split[i]);
				split[i] = tmp;
				break ;
			}
			j++;
		}
		i++;
	}
	return (split);
}*/

// char	**split_arg(int key_count, char **split, char *str)
// {
// 	int		end;
// 	int		i;

// 	end = 0;
// 	i = 0;
// 	while(i < key_count)
// 	{
// 		while (str[end])
// 		{
// 			if (str[end] != '$')
// 			{
// 				while (str[end] && str[end] != '$')
// 					end++;
// 				split[i] = ft_substr(str, 0, end);
// 				i++;
// 				str = str + end;
// 				end = 0;
// 			}
// 			if (str[end] == '$')
// 			{
// 				end++;
// 				while (str[end] && !is_end(str[end]))
// 					end++;
// 				split[i] = ft_substr(str, 0, end);
// 				i++;
// 				str = str + end;
// 				end = 0;
// 			}
// 		}
// 	}
// 	return (split);
// }

// char	*join_keys(char **split)
// {
// 	char	*joined;
// 	size_t	i;

// 	i = 0;
// 	joined = split[0];
// 	while (split[i])
// 	{
// 		if (split[i + 1])
// 		{
// 			joined = ft_strjoin(split[i], split[i + 1]);
// 			free(split[i + 1]);
// 			split[i + 1] = joined;
// 		}
// 		i++;
// 	}
// 	return (ft_strdup(joined));
// }

t_token **expand_keys(t_token **head, char ***envp_key_val)
{
	t_token	*curr;
	char	*tmp;

	curr = *head;
	while (curr)
	{
			if (curr->state != QUOTE && curr->value[0] == '$' && ft_strlen(curr->value) > 1)
			{
				tmp = get_value(curr->value + 1, envp_key_val);
				free(curr->value);
				curr->value = tmp;
			}
		curr = curr->next;
	}
	return (head);
}

t_token	**expand(t_token **head, char **envp)
{
	t_token	*curr;
	char	***envp_key_val;
	// char	**split;
	// int		key_count;

	envp_key_val = split_envp(envp);
	if (!envp_key_val)
		return (free_three_dim(envp_key_val), NULL);
	curr = *head;
	while (curr)
	{
		if (curr->state != QUOTE)
		{
			//key_count = count_words(curr->value);
			//split = malloc((key_count + 1) * sizeof(char *));
			//split[key_count] = NULL;
			//split = split_arg(key_count, split, curr->value);
			head = expand_keys(head, envp_key_val);
			//print_keys(split);
			//free(curr->value);
			//curr->value = join_keys(split);
			//free_two_dim(split);
		}
		curr = curr->next;
	}
	//print_key_val(envp_key_val);
	free_three_dim(envp_key_val);
	return (head);
}
