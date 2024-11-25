/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrauh <nrauh@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 10:34:32 by nrauh             #+#    #+#             */
/*   Updated: 2024/11/22 12:01:33 by nrauh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*int	count_vars(char *str)
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
}*/

/*char	***split_envp(char **envp)
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
}*/

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

static char *replace_exit_code_in_arg(const char *arg, t_main *main)
{
    char    *pos;
    char    *new_arg;
    char    *exit_code_str;
    size_t  prefix_len;
    size_t  new_arg_len;

    exit_code_str = ft_itoa(main->exit_code); // Convert exit_code to string
    if (!exit_code_str)
        return (NULL); // Handle memory allocation failure

    pos = ft_strnstr(arg, "$?", ft_strlen(arg));
    if (!pos)
        return (ft_strdup(arg)); // No `$?`, return a copy of the original string

    prefix_len = pos - arg; // Length of text before `$?`
    new_arg_len = prefix_len + ft_strlen(exit_code_str) + ft_strlen(pos + 2) + 1;

    new_arg = malloc(new_arg_len); // Allocate new string
    if (!new_arg)
    {
        free(exit_code_str);
        return (NULL); // Handle memory allocation failure
    }

    // Copy parts into the new string using `ft_strlcpy`
    ft_strlcpy(new_arg, arg, prefix_len + 1);                                 // Copy prefix
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
		while (envp[i][j] && envp[i][j] != '='
			&& envp[i][j] == env_key[j])
			j++;
		if (envp[i][j++] == '=')
			value = ft_substr(envp[i], j, ft_strlen(envp[i]) - j);
		i++;
	}
	if (value)
		return (value);
	return (ft_strdup(""));
}

t_token	**expand_keys(t_token **head, char **envp, t_main *main)
{
	t_token	*curr;
	char	*tmp;

	curr = *head;
	while (curr)
	{
		if (curr->state != QUOTE)
		{
			tmp = replace_exit_code_in_arg(curr->value, main);
			free(curr->value);
			curr->value = tmp;
		}
		if (curr->state != QUOTE && curr->value[0] == '$'
			&& ft_strlen(curr->value) > 1
			&& !(curr != *head && ft_strncmp(curr->prev->value, "<<", 2) == 0))
		{

			tmp = get_value(curr->value + 1, envp);
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

t_token	**expand(t_token **head, char **envp, t_main *main)
{
	t_token	*curr;
	//char	***envp_key_val;

	//envp_key_val = split_envp(envp);
	//if (!envp_key_val)
	//	return (free_three_dim(envp_key_val), NULL);
	if (!head || !(*head))
		return (NULL);
	curr = *head;
	while (curr)
	{
		if (curr->state != QUOTE)
			head = expand_keys(head, envp, main);
		curr = curr->next;
	}
	//print_key_val(envp_key_val);
	//free_three_dim(envp_key_val);
	return (head);
}
