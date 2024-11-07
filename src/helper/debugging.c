/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debugging.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrauh <nrauh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 18:09:29 by nrauh             #+#    #+#             */
/*   Updated: 2024/11/07 12:24:54 by nrauh            ###   ########.fr       */
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
		printf("state %d\n", curr->state);
		/*if (curr->type == STRING)
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
			printf("Argument\n");*/
		printf("value length: %zu\n", ft_strlen(curr->value));
		printf("----------\n");
		printf(" <-    -> \n");
		curr = curr->next;
	}
}

void	print_keys(char **env_keys)
{
	int	i;

	i = 0;
	while (env_keys[i])
	{
		printf("i: %i - %s\n", i, env_keys[i]);
		i++;
	}
	printf("---------\n");
}

void	print_key_val(char ***envp_key_val)
{
	int	i;

	i = 0;
	while (envp_key_val[i])
	{
		printf("pointer %p i %d \n", envp_key_val[i], i);
		printf("key: %s | %p\nvalue: %s | %p\n",
				envp_key_val[i][0], envp_key_val[i][0],
				envp_key_val[i][1], envp_key_val[i][1]);
		printf("----------\n");
		i++;
	}
}
