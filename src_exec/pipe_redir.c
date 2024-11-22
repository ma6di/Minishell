//NORM OK
#include "../includes/minishell.h"

void	pipe_handler(t_command *cmd)
{
	if ((cmd->next && cmd->has_pipe && !is_special_builtin(cmd->command)) || \
		(cmd->next && cmd->has_pipe && is_special_builtin(cmd->command)))
	{
		if (pipe(cmd->pipe_fd) == -1)
		{
			perror("minishell: pipe failed");
			cmd->error_code = 1;
		}
		cmd->pipe_created = true;
	}
}

void	setup_pipe_redirections_parent(t_command *cmd)
{
	if (cmd->prev)
	{
		if (!is_special_builtin(cmd->prev->command))
			dup2_in(cmd->prev->pipe_fd);
	}
	if (cmd->next && !cmd->io_fds->outfile)
	{
		if (!is_special_builtin(cmd->next->command))
			dup2_out(cmd->pipe_fd);
	}
}

void	setup_pipe_redirections_child(t_command *cmd)
{
	if (cmd->prev)
	{
		if (cmd->prev->pipe_created && !cmd->io_fds->infile)
		{
			dup2_in(cmd->prev->pipe_fd);
			close(cmd->pipe_fd[1]);
		}
	}
	if (cmd->pipe_created && cmd->next && !cmd->io_fds->outfile)
	{
		dup2_out(cmd->pipe_fd);
		close(cmd->pipe_fd[0]);
	}
}

void	dup2_out(int *pipe_fd)
{
	if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
		perror("minishell: dup2 out pipe_fd[1] failed");
	safe_close(&pipe_fd[1]);
}

void	dup2_in( int *pipe_fd)
{
	if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
		perror("minishell: dup2 in pipe_fd[0] failed");
	safe_close(&pipe_fd[0]);
}
