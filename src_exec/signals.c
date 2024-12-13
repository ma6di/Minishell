/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrauh <nrauh@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 16:58:09 by mcheragh          #+#    #+#             */
/*   Updated: 2024/12/13 12:59:40 by nrauh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_sigint;

//CHILD//
static void	fake_exit(int signo)
{
	(void)signo;
	ft_child_exit(0);
}

void	set_signals_child(void)
{
	signal(SIGINT, signal_reset_prompt_sleep);
	signal(SIGQUIT, signal_quit_message);
	signal(SIGPIPE, fake_exit);
}

//PARENT//
void	signal_reset_prompt(int signo)
{
	(void)signo;
	g_sigint = 130;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	set_signals_interactive(void)
{
	signal(SIGINT, signal_reset_prompt);
	signal(SIGQUIT, SIG_IGN);
}

void	set_signals_noniteractive(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}
