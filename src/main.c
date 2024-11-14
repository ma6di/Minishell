/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrauh <nrauh@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 16:22:04 by nrauh             #+#    #+#             */
/*   Updated: 2024/11/14 17:58:13 by nrauh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
// need to delete spaces when delimiter
// need to check what happens with env variables that are not found...
int	main(int argc, char **argv, char **envp)
{
	char		*input = "echo hello | echo $PATH";

	(void )		argc;
	(void )		argv;
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
