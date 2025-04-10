/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrauh <nrauh@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 13:07:56 by nrauh             #+#    #+#             */
/*   Updated: 2024/12/05 10:41:02 by nrauh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// all functions that manipulate the token list
#include "../../includes/minishell.h"

// function to add token to the end of double linked list and connect it
void	add_token(t_token **head, t_token *new_token)
{
	t_token	*curr;

	if (!(*head) || !head)
	{
		*head = new_token;
		new_token->prev = NULL;
	}
	else
	{
		curr = *head;
		while (curr->next)
			curr = curr->next;
		curr->next = new_token;
		new_token->prev = curr;
	}
	new_token->next = NULL;
}

void	create_token(t_token **head, char *value, t_state state)
{
	t_token			*new_token;

	new_token = malloc(sizeof(t_token));
	new_token->value = value;
	new_token->state = state;
	new_token->type = UNINITIALIZED;
	new_token->next = NULL;
	new_token->prev = NULL;
	add_token(head, new_token);
}
