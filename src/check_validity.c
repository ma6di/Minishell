/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_validity.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrauh <nrauh@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 10:34:32 by nrauh             #+#    #+#             */
/*   Updated: 2024/11/29 12:34:02 by nrauh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// bash operators that expect newline
// <<< <>

static int	is_operator(t_token_type type)
{
	if (type == APPEND || type == INPUT_REDIRECT || type == IO_REDIRECT
		|| type == HEREDOC || type == REDIRECT || type == HERESTRING)
		return (1);
	return (0);
}

static void	display_error_and_free(char *value, t_token **head)
{
	ft_fprintf("minishell: syntax error near unexpected token `%s'\n", value);
	free_tokens(head);
}

// Minishell% echo <<end >file >
// executes heredoc first, then shows syntax error....
// minishell: syntax error near unexpected token `newline'

t_token	**check_validity(t_token **head)
{
	t_token	*curr;

	if (!head || !(*head))
		return (NULL);
	curr = *head;
	if (curr->type == PIPE)
		return (display_error_and_free(curr->value, head), NULL);
	while (curr && curr->next)
	{
		if ((curr->type == HERESTRING || curr->type == IO_REDIRECT) 
			&& !is_operator(curr->next->type))
		{
			if (!curr->next->next)
				return (display_error_and_free("newline", head), NULL);
		}
		if ((is_operator(curr->type) && is_operator(curr->next->type))
			|| curr->next->type == LOGICAL_OR)
			return (display_error_and_free(curr->next->value, head), NULL);
		curr = curr->next;
	}
	if (is_operator(curr->type) || curr->type == HERESTRING 
		|| curr->type == IO_REDIRECT || curr->type == PIPE)
		return (display_error_and_free("newline", head), NULL);
	return (head);
}
