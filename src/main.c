/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrauh <nrauh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 16:22:04 by nrauh             #+#    #+#             */
/*   Updated: 2024/11/09 09:43:40 by nrauh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
// need to delete spaces when delimiter
// need to check what happens with env variables that are not found...
int	main(int argc, char **argv, char **envp)
{
	char		*input = "echo 'p''w''d' |$ >$FIRST$SECOND \"$USER$HELLO $HOME'$USER\"'$NOT_EXPAND' -l >>|| grep \"wo'rd\"";

	(void )		argc;
	(void )		argv;
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
	printf("%s\n", input);
	lexer(input, envp);
	return (0);
}
