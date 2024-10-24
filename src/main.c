/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrauh <nrauh@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 16:22:04 by nrauh             #+#    #+#             */
/*   Updated: 2024/10/24 16:33:48 by nrauh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	(void)  argc;
	(void)  argv;
	(void)	envp;
	char	*input;

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
			lexer(input);
		}
		free(input);
	}
	return (0);
}
