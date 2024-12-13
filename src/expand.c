/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrauh <nrauh@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 10:34:32 by nrauh             #+#    #+#             */
/*   Updated: 2024/12/13 14:03:59 by nrauh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// could be unified with the other is_operator???
static int	is_operator(t_token *token)
{
	if (token && token->value)
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
	}
	return (0);
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

static int	check_for_command(char *str, t_main *main)
{
	char	*path;
	char	*is_command;
	int		i;

	i = 0;
	while (str[i] && str[i] != ' ')
		i++;
	is_command = ft_substr(str, 0, i);
	path = get_command_path(is_command, main->env_vars);
	free(is_command);
	if (!path)
	{
		free(path);
		return (0);
	}
	free(path);
	return (1);
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
		tmp = check_equal(curr, tmp);
		free(curr->value);
		if (check_for_command(tmp, main))
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
