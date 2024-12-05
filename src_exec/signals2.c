//NORM OK
#include "../includes/minishell.h"

//HEREDOC//
static void fake_exit()
{
	char *new_prog = "/bin/false";
	char *args[] = {"false", NULL};
	char *env[] = {NULL};
	if(execve(new_prog, args, env) == -1)
		perror("true failed");
}

void	set_signals_heredoc()
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
