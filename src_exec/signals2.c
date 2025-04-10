/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcheragh <mcheragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 16:58:13 by mcheragh          #+#    #+#             */
/*   Updated: 2024/12/12 16:58:16 by mcheragh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

//HEREDOC//
static void	fake_exit(int signo)
{
	(void)signo;
	ft_child_exit(1);
}

void	set_signals_heredoc(void)
{
	signal(SIGINT, fake_exit);
	signal(SIGQUIT, SIG_IGN);
}

//SLEEP//

void	signal_reset_prompt_sleep(int signo)
{
	(void)signo;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
}

void	signal_quit_message(int signo)
{
	(void)signo;
	write(1, "Quit (core dumped)\n", 19);
}

void	set_signals_sleep_mode(void)
{
	signal(SIGQUIT, signal_quit_message);
	signal(SIGINT, signal_reset_prompt_sleep);
}
