//NORM OK
#include "../includes/minishell.h"

int	g_pid;

//CHILD//

void	set_signals_child(void)
{
	signal(SIGINT, signal_reset_prompt_sleep);
	signal(SIGQUIT, signal_quit_message);
}

//PARENT//

void	signal_reset_prompt(int signo)
{
	(void)signo;
	g_pid = 130;
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
