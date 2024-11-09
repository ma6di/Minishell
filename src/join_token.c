/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join_token.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrauh <nrauh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 17:13:37 by nrauh             #+#    #+#             */
/*   Updated: 2024/11/09 06:44:14 by nrauh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	is_delimiter(t_token *token)
{
	if ((ft_strncmp(token->value, " ", ft_strlen(token->value)) == 0
		|| ft_strncmp(token->value, "||", ft_strlen(token->value)) == 0
		|| ft_strncmp(token->value, "|", ft_strlen(token->value)) == 0
		|| ft_strncmp(token->value, ">>", ft_strlen(token->value)) == 0
		|| ft_strncmp(token->value, "<", ft_strlen(token->value)) == 0
		|| ft_strncmp(token->value, "<<", ft_strlen(token->value)) == 0
		|| ft_strncmp(token->value, ">", ft_strlen(token->value)) == 0)
		&& token->state == GENERAL)
		return (1);
	return (0);
}

t_token	*remove_node(t_token *node, t_token **head)
{
	t_token	*next_node;

	next_node = node->next;
	// need to check here, node == *head or node->prev == *head?????
	if (node == *head)
	{
		*head = node->next;
		node->next->prev = *head;
	}
	else
	{
		node->prev->next = node->next;
		node->next->prev = node->prev;
	}
	free(node->value);
	free(node);
	return (next_node);
}

t_token	**join_token(t_token **head)
{
	t_token	*curr;
	char	*joined;

	curr = *head;
	while (curr && curr->next)
	{
		if (!is_delimiter(curr->next) && !is_delimiter(curr))
		{
			joined = ft_strjoin(curr->value, curr->next->value);
			free(curr->next->value);
			curr->next->value = joined;
			curr = remove_node(curr, head);
		}
		else if (ft_strncmp(curr->value, " ", ft_strlen(curr->value)) == 0 && curr->state == GENERAL)
			curr = remove_node(curr, head);
		else
			curr = curr->next;
	}
	return (head);
}
