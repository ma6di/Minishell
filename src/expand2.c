/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcheragh <mcheragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 10:34:32 by nrauh             #+#    #+#             */
/*   Updated: 2024/12/12 17:11:29 by mcheragh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	**split_cla(char *value)
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

t_token	*insert_new_token(t_token **curr, char **tmp_split, int i)
{
	t_token	*new_token;
	t_token	*next;

	next = (*curr)->next;
	new_token = malloc(sizeof(t_token));
	new_token->value = tmp_split[i];
	new_token->state = (*curr)->state;
	new_token->type = UNINITIALIZED;
	(*curr)->next = new_token;
	new_token->next = next;
	new_token->prev = (*curr);
	if (next)
		next->prev = new_token;
	(*curr) = new_token;
	return (new_token);
}

t_token	*value_is_cla(t_token *curr, char *value)
{
	char	**tmp_split;
	t_token	*new_token;
	int		i;

	i = 0;
	tmp_split = split_cla(value);
	if (!tmp_split)
		return (NULL);
	while (tmp_split[i])
	{
		if (i == 0)
			curr->value = tmp_split[i];
		else
			new_token = insert_new_token(&curr, tmp_split, i);
		i++;
	}
	free(tmp_split);
	return (new_token);
}

char	*replace_exit_code_in_arg(const char *arg, t_main *main)
{
	char	*pos;
	char	*new_arg;
	char	*exit_code_str;

	exit_code_str = ft_itoa(main->exit_code);
	if (!exit_code_str)
		return (NULL);
	pos = ft_strnstr(arg, "$?", ft_strlen(arg));
	if (!pos)
	{
		free(exit_code_str);
		return (ft_strdup(arg));
	}
	new_arg = generate_new_arg(arg, exit_code_str, pos);
	free(exit_code_str);
	return (new_arg);
}