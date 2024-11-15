/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrauh <nrauh@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 16:22:04 by nrauh             #+#    #+#             */
/*   Updated: 2024/11/15 17:40:23 by nrauh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
// need to delete spaces when delimiter
// need to check what happens with env variables that are not found...
int	main(int argc, char **argv, char **envp)
{
	// echo $HOME>>myfile.txt | cat -flag catfile.txt|'p''w''d' |cat<<delimiter|echo '$NOEXPAND'
	char		*input;
	t_command	*commands;

	(void )		argc;
	(void )		argv;
	while (1)
	{
		input = readline("Minishell% ");
		if (!input)
		{
			printf("\n");
			break ;
		}
		if (ft_strlen(input) > 0)
		{
			add_history(input);
			if (ft_strncmp(input, "exit", ft_strlen(input)) == 0)
			{
				rl_clear_history();
				free(input);
				break ;
			}
			commands = lexer(input, envp);
		}
		if (commands)
		{
			printf("----- FREEING COMMANDS -----\n");
			free_commands(&commands);
		}
		free(input);
	}
	return (0);
}
