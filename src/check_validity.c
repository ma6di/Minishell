/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_validity.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrauh <nrauh@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 10:34:32 by nrauh             #+#    #+#             */
/*   Updated: 2024/11/28 19:14:24 by nrauh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// static int	is_operator(t_token_type type)
// {
// 	if (type == PIPE || type == APPEND || type == INPUT_REDIRECT
// 		|| type == LOGICAL_OR || type == HEREDOC || type == REDIRECT)
// 		return (1);
// 	return (0);
// }

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
		if ((curr->type == HEREDOC 
				|| curr->type == APPEND 
				|| curr->type == INPUT_REDIRECT 
				|| curr->type == REDIRECT) 
			&& curr->next 
			&& (curr->next->type == HEREDOC 
				|| curr->next->type == APPEND 
				|| curr->next->type == INPUT_REDIRECT 
				|| curr->next->type == REDIRECT))
		{
			display_error("syntax error near unexpected token", curr->value, head);
			return (NULL);
		}
		// if (is_operator(curr->type) && curr->prev && is_operator(curr->prev->type) && curr->prev->type != PIPE)
		// {
		// 	display_error("syntax error near unexpected token", curr->value, head);
		// 	return (NULL);
		// }
		// if ((is_operator(curr->type) && curr->next == NULL))
		// {
		// 	display_error("syntax error near unexpected token", "newline", head);
		// 	return (NULL);
		// }
		curr = curr->next;
	}
	return (head);
}
