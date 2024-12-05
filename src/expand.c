/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrauh <nrauh@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 10:34:32 by nrauh             #+#    #+#             */
/*   Updated: 2024/12/05 16:32:14 by nrauh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char *replace_exit_code_in_arg(const char *arg, t_main *main)
{
	char	*pos;
	char	*new_arg;
	char	*exit_code_str;
	size_t	prefix_len;
	size_t	new_arg_len;

	exit_code_str = ft_itoa(main->exit_code); // Convert exit_code to string
	if (!exit_code_str)
		return (NULL); // Handle memory allocation failure

	pos = ft_strnstr(arg, "$?", ft_strlen(arg));
	if (!pos)
	{
		free(exit_code_str);
		return (ft_strdup(arg)); // No `$?`, return a copy of the original string
	}

	prefix_len = pos - arg; // Length of text before `$?`
	new_arg_len = prefix_len + ft_strlen(exit_code_str) + ft_strlen(pos + 2) + 1;

	new_arg = malloc(new_arg_len); // Allocate new string
	if (!new_arg)
	{
		free(exit_code_str);
		return (NULL); // Handle memory allocation failure
	}

	// Copy parts into the new string using `ft_strlcpy`
	ft_strlcpy(new_arg, arg, prefix_len + 1);								 // Copy prefix
	ft_strlcpy(new_arg + prefix_len, exit_code_str, ft_strlen(exit_code_str) + 1); // Append exit_code
	ft_strlcpy(new_arg + prefix_len + ft_strlen(exit_code_str), pos + 2, ft_strlen(pos + 2) + 1); // Append remaining string

	free(exit_code_str); // Clean up
	return (new_arg);
}


char	*get_value(char *env_key, char **envp)
{
	int		i;
	int		j;
	char	*value;

	i = 0;
	if (!envp || !env_key)
		return (ft_strdup(""));
	value = NULL;
	while (envp[i])
	{
		j = 0;
		while (envp[i][j] && envp[i][j] != '=' && envp[i][j] == env_key[j])
			j++;
		if (envp[i][j] == '=' && env_key[j] == '\0')
		{
			j++;
			value = ft_substr(envp[i], j, ft_strlen(envp[i]) - j);
		}
		i++;
	}
	if (value)
		return (value);
	return (ft_strdup(""));
}

char	**split_cli(char *value)
{
	char	**tmp_split;
	int		i;

	i = 0;
	while (value[i] != ' ')
		i++;
	if (value[i] == '\0' || value[i + 1] == '\0')
		return (NULL);
	tmp_split = malloc(4 * sizeof(char *));
	if (!tmp_split)
		return (NULL);
	tmp_split[0] = ft_substr(value, 0, i);
	tmp_split[1] = ft_strdup(" ");
	tmp_split[2] = ft_substr(value, i + 1, ft_strlen(value) - i);
	tmp_split[3] = NULL;
	return (tmp_split);
}

// increments curr by one (the new added token)
t_token	*value_is_cli(t_token *curr, char *value)
{
	char	**tmp_split;
	t_token	*new_token;
	t_token	*next;
	int		i;

	i = 0;
	tmp_split = split_cli(value);
	if (!tmp_split)
		return (NULL);
	while (tmp_split[i])
	{
		if (i == 0)
			curr->value = tmp_split[i];
		else
		{
			next = curr->next;
			new_token = malloc(sizeof(t_token));
			new_token->value = tmp_split[i];
			new_token->state = curr->state;
			new_token->type = UNINITIALIZED;
			curr->next = new_token;
			new_token->next = next;
			new_token->prev = curr;
			if (next)
				next->prev = new_token;
			curr = new_token;
		}
		i++;
	}
	free(tmp_split);
	return (new_token);
}

// t_token	**expand_keys(t_token **head, char **envp, t_main *main)
// {
// 	t_token	*curr;
// 	char	*tmp;

// 	curr = *head;
// 	// we go through the wile two times?????
// 	while (curr)
// 	{
// 		// double cause we already ask in the fn before
// 		if (curr->state != QUOTE)
// 		{
// 			tmp = replace_exit_code_in_arg(curr->value, main);
// 			free(curr->value);
// 			curr->value = tmp;
// 		}
// 		if (curr->state != QUOTE && curr->value[0] == '$'
// 			&& ft_strlen(curr->value) > 1
// 			&& !(curr != *head && ft_strncmp(curr->prev->value, "<<", 2) == 0))
// 		{

// 			tmp = get_value(curr->value + 1, envp);
// 			free(curr->value);
// 			if (ft_strchr(tmp, ' '))
// 			{
// 				curr = value_is_cli(curr, tmp);
// 				free(tmp);
// 			}
// 			else
// 				curr->value = tmp;
// 		}
// 		curr = curr->next;
// 	}
// 	return (head);
// }

static void	expand_keys(t_token **head, t_token *curr, char **envp, t_main *main)
{
	char	*tmp;

	tmp = replace_exit_code_in_arg(curr->value, main);
	free(curr->value);
	curr->value = tmp;
	if (curr->value[0] == '$' && ft_strlen(curr->value) > 1
		&& !(curr != *head && ft_strncmp(curr->prev->value, "<<", 2) == 0))
	{
		tmp = get_value(curr->value + 1, envp);
		free(curr->value);
		if (ft_strchr(tmp, ' '))
		{
			curr = value_is_cli(curr, tmp);
			free(tmp);
		}
		else
			curr->value = tmp;
	}
}

t_token	**expand(t_token **head, char **envp, t_main *main)
{
	t_token	*curr;

	if (!head || !(*head))
		return (NULL);
	curr = *head;
	while (curr)
	{
		if (curr->state != QUOTE)
			expand_keys(head, curr, envp, main);
		curr = curr->next;
	}
	return (head);
}
