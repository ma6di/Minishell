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
		// printf("pip created\n");
		cmd->pipe_created = true;
	}
}

void	setup_pipe_redirections_parent(t_command *cmd)
{
	if (cmd->prev)
	{
		if (!is_special_builtin(cmd->prev->command))
		{
			// printf("p dup in pre 0\n");
			dup2_in(cmd->prev->pipe_fd);
		}
	}
	if (cmd->next && !cmd->io_fds->outfile)
	{
		if (!is_special_builtin(cmd->next->command))
		{
			// printf("p dup out cur 1\n");
			dup2_out(cmd->pipe_fd);
		}
	}
}

void	setup_pipe_redirections_child(t_command *cmd)
{
	if (cmd->prev)
	{
		if (cmd->prev->pipe_created && !cmd->io_fds->infile)
		{
			// printf("c dup in pre 0\n");
			dup2_in(cmd->prev->pipe_fd);
		}
	}
	if (cmd->pipe_created && cmd->next && !cmd->io_fds->outfile)
	{
		// printf("c dup out cur 1\n");
		dup2_out(cmd->pipe_fd);
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
