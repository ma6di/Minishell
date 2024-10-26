/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debugging.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrauh <nrauh@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 18:09:29 by nrauh             #+#    #+#             */
/*   Updated: 2024/10/26 12:05:33 by nrauh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	print_token_list(t_token **head)
{
	t_token	*curr;
	int		i;

	i = 0;
	curr = *head;
	while (curr)
	{
		i++;
		printf("----------\n");
		printf("#%d\n%s\n", i, curr->value);
		if (curr->type == STRING)
			printf("String\n");
		else if (curr->type == COMMAND)
			printf("Command\n");
		else if (curr->type == PIPE)
			printf("Pipe\n");
		else if (curr->type == REDIRECT)
			printf("Redirect\n");
		else if (curr->type == FILENAME)
			printf("Filename\n");
		else if (curr->type == ARGUMENT)
			printf("Argument\n");
		printf("value length: %zu\n", ft_strlen(curr->value));
		printf("----------\n");
		printf(" <-    -> \n");
		curr = curr->next;
	}
}


