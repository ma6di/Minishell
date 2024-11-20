/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_validity.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrauh <nrauh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 10:34:32 by nrauh             #+#    #+#             */
/*   Updated: 2024/11/09 04:03:44 by nrauh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	is_operator(t_token_type type)
{
	if (type == PIPE || type == APPEND || type == INPUT_REDIRECT
		|| type == LOGICAL_OR || type == HEREDOC || type == REDIRECT)
		return (1);
	return (0);
}

t_token	**check_validity(t_token **head)
{
	t_token	*curr;

	curr = *head;
	while (curr)
	{
		if (curr != *head
			&& is_operator(curr->type)
			&& is_operator(curr->prev->type))
			break ;
		curr = curr->next;
	}
	if (curr == NULL)
		return (head);
	display_error("parse error", head);
	return (head);
}
