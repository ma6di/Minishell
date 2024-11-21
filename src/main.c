/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrauh <nrauh@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 16:22:04 by nrauh             #+#    #+#             */
/*   Updated: 2024/11/21 18:04:24 by nrauh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static t_main	*init_main(char **envp)
{
	t_main	*main;
	int		count;

	count = 0;
	main = malloc(sizeof(t_main));
	while (envp && envp[count])
		count++;
	main->env_vars = malloc(sizeof(char *) * (count + 1));
	if (!main->env_vars)
		exit (0);
	for (int i = 0; i < count; i++)
		main->env_vars[i] = ft_strdup(envp[i]);
	main->env_vars[count] = NULL;
	//main->is_sleeping = false;
	main->command_list = NULL;
	main->exit_code = 0;
	main->heredoc_fork_permit = 0;
	return (main);
}

#include "../includes/minishell.h"
// need to delete spaces when delimiter
// need to check what happens with env variables that are not found...
int	main(int argc, char **argv, char **envp)
{
	char		*input;
	t_command	*commands;
	t_main		*main;

	main = init_main(envp);

	(void )		argc;
	(void )		argv;
	while (1)
	{
		g_sigint_received = 0;
		set_signals_interactive();
		input = readline("Minishell% ");
		if (!input)
		{
			printf("exit\n");
			free_main(main);
			break ;
		}
		if (ft_strlen(input) > 0)
		{
			add_history(input);
			if (ft_strncmp(input, "exit", ft_strlen(input)) == 0)
			{
				printf("exit\n");
				rl_clear_history();
				free_main(main);
				free(input);
				break ;
			}
			if(g_sigint_received == 130)
				main->exit_code = 130;
			commands = lexer(input, main->env_vars, &main);
			//printf("--------------- COMMANDS SET ---------------\n");
			main->command_list = commands;
		}
		if (main->command_list)
		{
			g_sigint_received = 1;
			exec_heredoc(main->command_list);
			set_signals_noniteractive();
			if (g_sigint_received)
				execute_commands(&main);
			main->is_sleeping = false;
			main->heredoc_fork_permit = 0;
			printf("--------------- FREEING COMMANDS ---------------\n");
			free_commands(&commands);
		}
		free(input);
	}
	// commands = lexer(input, envp);
	// if (commands)
	// {
	// 	printf("----- FREEING COMMANDS -----\n");
	// 	free_commands(&commands);
	// }
	return (0);
}
