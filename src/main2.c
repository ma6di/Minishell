/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrauh <nrauh@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 16:22:04 by nrauh             #+#    #+#             */
/*   Updated: 2024/12/13 12:57:37 by nrauh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_main	*init_main(char **envp)
{
	t_main	*main;

	main = allocate_main();
	copy_env_vars(main, envp);
	return (main);
}

t_main	*allocate_main(void)
{
	t_main	*main;

	main = malloc(sizeof(t_main));
	if (!main)
		exit(0);
	main->command_list = NULL;
	main->exit_code = 0;
	main->heredoc_fork_permit = 0;
	main->should_exit = -1;
	return (main);
}
