/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrauh <nrauh@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 16:22:04 by nrauh             #+#    #+#             */
/*   Updated: 2024/11/01 13:35:44 by nrauh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char		*input;

	(void )		argc;
	(void )		argv;
	while (1)
	{
		input = readline("PROMPT% ");
		if (!input)
		{
			printf("\n");
			break ;
		}
		if (ft_strlen(input) > 0)
		{
			add_history(input);
			lexer(input, envp);
		}
		free(input);
	}
	return (0);
}
