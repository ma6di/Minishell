/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_validity.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrauh <nrauh@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 10:34:32 by nrauh             #+#    #+#             */
/*   Updated: 2024/11/21 17:42:20 by nrauh            ###   ########.fr       */
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

	if (!head || !(*head))
		return (NULL);
	curr = *head;
	while (curr && curr->next)
		curr = curr->next;
	if (is_operator(curr->type) && curr->next == NULL)
	{
		display_error("syntax error near unexpected token `newline'", head);
		return (NULL);
	}
	return (head);
}
