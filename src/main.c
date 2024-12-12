/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrauh <nrauh@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 16:22:04 by nrauh             #+#    #+#             */
/*   Updated: 2024/12/12 18:58:22 by nrauh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	copy_env_vars(t_main *main, char **envp)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (envp && envp[count])
		count++;
	main->env_vars = ft_calloc((count + 1), sizeof(char *));
	if (!main->env_vars)
		exit(0);
	while (i < count)
	{
		main->env_vars[i] = ft_strdup(envp[i]);
		if (!main->env_vars[i])
		{
			free_two_dim(main->env_vars);
			exit(0);
		}
		i++;
	}
	main->env_vars[count] = NULL;
}

static void	process_commands(t_main *main, t_command *commands)
{
	exec_heredoc(commands);
	set_signals_noniteractive();
	if (main->heredoc_fork_permit != -1)
		execute_commands(&main);
	main->heredoc_fork_permit = 0;
	remove_heredoc_file(main);
	free_commands(&(main->command_list));
	main->command_list = NULL;
}

static void	handle_input(char *input, t_main *main)
{
	t_command	*commands;

	commands = NULL;
	if (ft_strlen(input) > 0)
	{
		add_history(input);
		if (g_sigint == 130)
			main->exit_code = 130;
		set_signals_noniteractive();
		commands = lexer(input, main->env_vars, &main);
		main->command_list = commands;
	}
	if (commands)
		process_commands(main, commands);
}

static void	cleanup_and_exit(t_main *main, int exit_code)
{
	printf("exit\n");
	rl_clear_history();
	free_commands(&main->command_list);
	free_main(main);
	exit(exit_code);
}

int	main(int argc, char **argv, char **envp)
{
	t_main	*main;
	char	*input;

	(void)argc;
	(void)argv;
	main = init_main(envp);
	while (main->should_exit < 0)
	{
		set_signals_interactive();
		input = readline("Minishell% ");
		if (!input)
			break ;
		handle_input(input, main);
		free(input);
	}
	cleanup_and_exit(main, main->should_exit);
}
