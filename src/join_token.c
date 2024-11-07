/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join_token.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrauh <nrauh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 17:13:37 by nrauh             #+#    #+#             */
/*   Updated: 2024/11/07 14:42:23 by nrauh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_token	**join_token(t_token **head)
{
	t_token	*curr;
	char	*joined;

	curr = *head;
	while (curr && ft_strncmp(curr->value, " ", ft_strlen(curr->value) == 1))
	{
		joined = ft_strjoin(curr->value, curr->next->value);
		free(curr->next->value);
		curr->next->value = joined;
		curr = curr->next;
	}
	return (head);
}
