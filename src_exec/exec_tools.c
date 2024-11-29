//NORM OK
#include "../includes/minishell.h"

void	fork_handler(t_command *cmd)
{
	if (is_special_builtin(cmd->command))
		return ;
	else
		cmd->pid = fork();
}

#include <sys/wait.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>

void ft_wait(t_command *cmd)
{
    int status;
    int signal;

    while (waitpid(cmd->pid, &status, 0) == -1)
    {
        if (errno == EINTR) // Interrupted by a signal, retry waitpid
            continue;
        else
        {
            cmd->main->exit_code = EXIT_FAILURE;
            return;
        }
    }

    if (WIFEXITED(status))
    {
        cmd->main->exit_code = WEXITSTATUS(status);
        return;
    }
    else if (WIFSIGNALED(status)) // Process terminated by a signal
    {
        signal = WTERMSIG(status);
        if (signal == SIGPIPE) // Check specifically for SIGPIPE
        {
            fprintf(stderr, "Error: Broken pipe detected (SIGPIPE).\n");
            cmd->main->exit_code = 141; // Conventionally 128 + SIGPIPE (13)
        }
        else
        {
            cmd->main->exit_code = 128 + signal;
        }
        return;
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
			// handle_heredoc(*cmd);
			*cmd = (*cmd)->next;
			(*cmd)->prev = NULL;
		}
	}
}
