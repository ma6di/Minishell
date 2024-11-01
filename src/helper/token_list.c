/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrauh <nrauh@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 13:07:56 by nrauh             #+#    #+#             */
/*   Updated: 2024/11/01 13:41:58 by nrauh            ###   ########.fr       */
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

void	create_token(t_token **head, char *value, t_token_state state)
{
	t_token			*new_token;

	new_token = malloc(sizeof(t_token));
	new_token->value = value;
	//new_token->type = NULL;
	new_token->state = state;
	//new_token->type = get_token_type(value);
	add_token(head, new_token);
}

// happens later in expansion ...
t_token_type	get_token_type(char *value)
{
	if (ft_strncmp(value, "echo", 4) == 0
		|| ft_strncmp(value, "cd", 2) == 0
		|| ft_strncmp(value, "pwd", 3) == 0
		|| ft_strncmp(value, "export", 6) == 0
		|| ft_strncmp(value, "unset", 5) == 0
		|| ft_strncmp(value, "env", 3) == 0
		|| ft_strncmp(value, "exit", 4) == 0)
		return (COMMAND);
	else if (ft_strnstr(value, ".", ft_strlen(value)))
		return (FILENAME);
	else if (ft_strncmp(value, ">", 1) == 0)
		return (REDIRECT);
	else if (ft_strncmp(value, ">>", 2) == 0)
		return (APPEND);
	else if (ft_strncmp(value, "<", 1) == 0)
		return (INPUT_REDIRECT);
	else if (ft_strncmp(value, "<<", 2) == 0)
		return (HEREDOC);
	else if (ft_strncmp(value, "|", 1) == 0)
		return (PIPE);
	else
		return (ARGUMENT);
}
