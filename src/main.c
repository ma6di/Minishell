/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrauh <nrauh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 16:22:04 by nrauh             #+#    #+#             */
/*   Updated: 2024/11/07 02:43:54 by nrauh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char		*input = "he'll'o $USER$HOME'hello''hi'";

	(void )		argc;
	(void )		argv;
	(void )		envp;
	//char *test[3] = {"HOME=path", "USER=username", NULL};

	/*while (1)
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
	}*/
	lexer(input, envp);
	return (0);
}
