/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_helper2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrauh <nrauh@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 17:38:29 by nrauh             #+#    #+#             */
/*   Updated: 2024/12/12 17:49:14 by nrauh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_tokens(t_token **head)
{
	t_token	*curr;
	t_token	*next;

	curr = *head;
	while (curr)
	{
		next = curr->next;
		free(curr->value);
		curr->value = NULL;
		free(curr);
		curr = next;
	}
	*head = NULL;
}

void	free_main(t_main *main)
{
	int	i;

	i = 0;
	if (!main)
		return ;
	if (main->env_vars)
	{
		while (main->env_vars[i])
		{
			free(main->env_vars[i]);
			main->env_vars[i] = NULL;
			i++;
		}
		free(main->env_vars);
		main->env_vars = NULL;
	}
	free(main);
}
