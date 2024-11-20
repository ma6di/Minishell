//NORM OK
#include "../includes/minishell.h"

///HEREDOC/////HEREDOC/////HEREDOC/////HEREDOC/////HEREDOC/////HEREDOC//

static void	signal_interrupt_heredoc(int signo)
{
	(void)signo;
	g_sigint_received = 0;
	write(1, "\n", 1);
    rl_replace_line("", 0);
    rl_done = 1;
    rl_on_new_line();
	rl_redisplay();
}

void	set_signals_heredoc(void)
{
	signal(SIGINT, signal_interrupt_heredoc);
}

//SLEEP////SLEEP////SLEEP////SLEEP////SLEEP////SLEEP//

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
