/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_validity.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrauh <nrauh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 10:34:32 by nrauh             #+#    #+#             */
/*   Updated: 2024/12/11 14:35:46 by nrauh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	is_operator(t_token_type type, t_state state)
{
	if (state == GENERAL && (type == APPEND || type == INPUT_REDIRECT
			|| type == IO_REDIRECT || type == HEREDOC || type == REDIRECT
			|| type == HERESTRING))
		return (1);
	return (0);
}

static void	display_error_and_free(char *value, t_token **head)
{
	ft_fprintf("minishell: syntax error near unexpected token `%s'\n", value);
	free_tokens(head);
}

t_token	**check_validity(t_token **head)
{
	t_token	*curr;

	if (!head || !(*head))
		return (NULL);
	curr = *head;
	if (curr->type == PIPE || curr->type == LOGICAL_OR
		|| curr->type == LOGICAL_AND)
		return (display_error_and_free(curr->value, head), NULL);
	while (curr && curr->next)
	{
		if ((curr->type == HERESTRING || curr->type == IO_REDIRECT)
			&& !is_operator(curr->next->type, curr->next->state)
			&& !curr->next->next)
		{
			if (!curr->next->next)
				return (display_error_and_free("newline", head), NULL);
		}
		if ((is_operator(curr->type, curr->state)
				&& (is_operator(curr->next->type, curr->next->state)))
			|| (curr->type == PIPE && curr->next->type == PIPE))
			return (display_error_and_free(curr->next->value, head), NULL);
		curr = curr->next;
	}
	if (is_operator(curr->type, curr->state) || curr->type == HERESTRING
		|| curr->type == IO_REDIRECT || curr->type == PIPE)
		return (display_error_and_free("newline", head), NULL);
	return (head);
}
