/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrauh <nrauh@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 16:22:04 by nrauh             #+#    #+#             */
/*   Updated: 2024/12/04 18:20:14 by nrauh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static t_main	*init_main(char **envp)
{
	t_main	*main;
	int		count;
	int		i;

	count = 0;
	i = 0;
	main = malloc(sizeof(t_main));
	while (envp && envp[count])
		count++;
	main->env_vars = ft_calloc((count + 1), sizeof(char *));
	main->env_vars[count] = NULL;
	if (!main->env_vars)
		exit (0);
	while (i < count)
	{
		main->env_vars[i] = ft_strdup(envp[i]);
		if (!main->env_vars[i])
		{
			free_two_dim(main->env_vars);
			exit (0);
		}
		i++;
	}
	//main->is_sleeping = false;
	main->command_list = NULL;
	main->exit_code = 0;
	main->heredoc_fork_permit = 0;
	main->should_exit = -1;
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
	int			exit_code;

	(void )		argc;
	(void )		argv;
	main = init_main(envp);
	commands = NULL;
	while (main->should_exit < 0)
	{
		set_signals_interactive();
		input = readline("Minishell% ");
		if (!input)
		{
			//printf("exit\n");
			//free_main(main);
			break ;
		}
		if (ft_strlen(input) > 0)
		{
			add_history(input);
			// if (ft_strncmp(input, "exit", ft_strlen(input)) == 0)
			// {
			// 	printf("exit\n");
			// 	rl_clear_history();
			// 	free_main(main);
			// 	free(input);
			// 	break ;
			// }
			if (g_sigint == 130)
				main->exit_code = 130;
			set_signals_noniteractive();
			commands = lexer(input, main->env_vars, &main);
			//printf("--------------- COMMANDS SET ---------------\n");
			main->command_list = commands;
		}
		if (commands)
		{
			exec_heredoc(main->command_list);
			set_signals_noniteractive();
			if (main->heredoc_fork_permit != -1)
			{
				execute_commands(&main);
			}
			main->is_sleeping = false;
			main->heredoc_fork_permit = 0;
			//printf("--------------- FREEING COMMANDS ---------------\n");
			remove_heredoc_file(main);
			//free_commands(&commands);
			free_commands(&(main->command_list));
			main->command_list = NULL;
			commands =NULL;
		}
		free(input);
	}
	printf("exit\n");
	exit_code = main->should_exit;
	rl_clear_history();
	main->command_list = NULL;
	//free_heredoc(commands);
	free_commands(&commands);
	free_main(main);
	//free(input);
	// commands = lexer(input, envp);
	// if (commands)
	// {
	// 	printf("----- FREEING COMMANDS -----\n");
	// 	free_commands(&commands);
	// }
	// return (0);
	exit(exit_code);
}
