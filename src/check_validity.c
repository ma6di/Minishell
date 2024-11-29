/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_validity.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrauh <nrauh@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 10:34:32 by nrauh             #+#    #+#             */
/*   Updated: 2024/11/29 09:54:39 by nrauh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// bash operators that expect newline
// <<< <>

static int	is_herestring(t_token_type curr, t_token_type next)
{
	if (curr == HEREDOC && next == INPUT_REDIRECT)
		return (1);
	return (0);
}

static int	is_io_redirect(t_token_type curr, t_token_type next)
{
	if (curr == INPUT_REDIRECT && next == REDIRECT)
		return (1);
	return (0);
}

static int	is_operator(t_token_type type)
{
	if (type == APPEND || type == INPUT_REDIRECT
		|| type == HEREDOC || type == REDIRECT)
		return (1);
	return (0);
}

// static int	invalid_operator(t_token *curr)
// {
// 	if ((curr->type == HEREDOC 
// 			|| curr->type == APPEND 
// 			|| curr->type == INPUT_REDIRECT 
// 			|| curr->type == REDIRECT) 
// 		&& curr->next 
// 		&& (curr->next->type == HEREDOC 
// 			|| curr->next->type == APPEND 
// 			|| curr->next->type == INPUT_REDIRECT 
// 			|| curr->next->type == REDIRECT))
// 	{
// 		display_error("syntax error near unexpected token", curr->value, head);
// 		return (0);
// 	}
// 	return (1);
// }

t_token	**check_validity(t_token **head)
{
	t_token	*curr;

	if (!head || !(*head))
		return (NULL);
	curr = *head;
	if (curr->type == PIPE)
	{
		display_error("syntax error near unexpected token", curr->value, head);
		return (NULL);
	}
	while (curr)
	{
		if (curr->next && (is_herestring(curr->type, curr->next->type)
				|| is_io_redirect(curr->type, curr->next->type)))
		{
			if (!curr->next->next)
			{
				display_error("syntax error near unexpected token", 
					"newline", head);
				return (NULL);
			}
		}
		else if (is_operator(curr->type)
			&& curr->next && is_operator(curr->next->type))
		{
			display_error("syntax error near unexpected token", 
				curr->next->value, head);
			return (NULL);
		}
		curr = curr->next;
	}
	return (head);
}
