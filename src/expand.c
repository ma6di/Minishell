/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcheragh <mcheragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 10:34:32 by nrauh             #+#    #+#             */
/*   Updated: 2024/12/12 17:11:44 by mcheragh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// could be unified with the other is_operator???
static int	is_operator(t_token *token)
{
	if (ft_strncmp(token->value, " ", ft_strlen(token->value) + 1) == 0
		|| ft_strncmp(token->value, "||", ft_strlen(token->value) + 2) == 0
		|| ft_strncmp(token->value, "|", ft_strlen(token->value) + 1) == 0
		|| ft_strncmp(token->value, ">>", ft_strlen(token->value) + 2) == 0
		|| ft_strncmp(token->value, "<", ft_strlen(token->value) + 1) == 0
		|| ft_strncmp(token->value, "<<", ft_strlen(token->value) + 2) == 0
		|| ft_strncmp(token->value, "<<<", ft_strlen(token->value) + 3) == 0
		|| ft_strncmp(token->value, "<>", ft_strlen(token->value) + 2) == 0
		|| ft_strncmp(token->value, ">", ft_strlen(token->value) + 1) == 0)
		return (1);
	return (0);
}

char	*generate_new_arg(const char *arg, const char *exit_code_str, \
								const char *pos)
{
	size_t	prefix_len;
	size_t	new_arg_len;
	char	*new_arg;

	prefix_len = pos - arg;
	new_arg_len = prefix_len + ft_strlen(exit_code_str) + \
					ft_strlen(pos + 2) + 1;
	new_arg = malloc(new_arg_len);
	if (!new_arg)
		return (NULL);
	ft_strlcpy(new_arg, arg, prefix_len + 1);
	ft_strlcpy(new_arg + prefix_len, exit_code_str, \
				ft_strlen(exit_code_str) + 1);
	ft_strlcpy(new_arg + prefix_len + ft_strlen(exit_code_str), pos + 2, \
				ft_strlen(pos + 2) + 1);
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

static void	exp_keys(t_token **head, t_token *curr, char **envp, t_main *main)
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
			curr = value_is_cla(curr, tmp);
			free(tmp);
		}
		else
			curr->value = tmp;
		if (is_operator(curr))
			curr->state = QUOTE;
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
			exp_keys(head, curr, envp, main);
		curr = curr->next;
	}
	return (head);
}
