//NORM OK
#include "../includes/minishell.h"

void	fork_handler(t_command *cmd)
{
	if (is_special_builtin(cmd->command))
		return ;
	else
		cmd->pid = fork();
}

static void handle_signal(int status, t_command *cmd)
{
	int signal;

	signal = WTERMSIG(status);
	if (signal == SIGPIPE)
	{
		fprintf(stderr, "Minishell: Broken pipe\n");
		cmd->main->exit_code = 141;
	}
	else
		cmd->main->exit_code = 128 + signal;
}

void ft_wait(t_command *cmd, int original_std[2])
{
	int status;

	safe_close(&original_std[0]);
	safe_close(&original_std[1]);
	while (cmd)
	{
		if (cmd->pid >= 0)
		{
			while (waitpid(cmd->pid, &status, 0) == -1)
			{
				if (errno == EINTR)
					continue;
				cmd->main->exit_code = EXIT_FAILURE;
				return;
			}
			if (WIFEXITED(status))
				cmd->main->exit_code = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				handle_signal(status, cmd);
		}
		cmd = cmd->next;
	}
}

void	safe_close(int *fd)
{
	if (*fd != -1)
	{
		close(*fd);
		*fd = -1;
	}
}

void	handle_special_builtin(t_command **cmd)
{
	if (is_special_builtin((*cmd)->command) && !(type_redir_exist((*(cmd)), OUTFILE)))
	{
		if ((*cmd)->next && is_special_builtin((*cmd)->next->command))
		{
			if ((*cmd)->prev && !is_special_builtin((*cmd)->prev->command))
				safe_close(&(*cmd)->prev->pipe_fd[0]);
			*cmd = (*cmd)->next;
			(*cmd)->prev = NULL;
		}
	}
}
