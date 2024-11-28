//NORM OK
#include "../includes/minishell.h"

void	parent_pipe_close(t_command *cmd)
{
	if (!is_special_builtin(cmd->command))
	{
		if (cmd->next && cmd->has_pipe)
		{
			// printf("NS p close cur 1\n");
			safe_close(&cmd->pipe_fd[1]);
		}
		if (cmd->prev && cmd->prev->has_pipe)
		{
			// printf("NS p close pre 0\n");
			safe_close(&cmd->prev->pipe_fd[0]);
		}
	}
	if (is_special_builtin(cmd->command))
	{
		if (cmd->prev)
		{
			if (!is_special_builtin(cmd->prev->command))
			{
				// printf("S p close pre 0\n");
				safe_close(&cmd->prev->pipe_fd[0]);
			}
		}
		if (cmd->next)
		{
			if (!is_special_builtin(cmd->next->command))
			{
				// printf("S p close cur 1\n");
				safe_close(&cmd->pipe_fd[1]);
			}
		}
	}
}

void	child_pipe_close(t_command *cmd)
{
	// if (cmd->prev)
	// {
	// 	if (cmd->prev->pipe_created && !cmd->io_fds->infile)
	// 	{
	// 		safe_close(&cmd->prev->pipe_fd[0]);
	// 	}
	// }
	// if (cmd->pipe_created && cmd->next && !cmd->io_fds->outfile)
	// {
	// 	safe_close(&cmd->pipe_fd[1]);
	// }
	if(cmd->pipe_fd[1])
		safe_close(&cmd->pipe_fd[1]);
	if(cmd->pipe_fd[0])
		safe_close(&cmd->pipe_fd[0]);
}

static void	stdin_reset(int original_stdin)
{
	dup2(original_stdin, STDIN_FILENO);
	safe_close(&original_stdin);
}

static void	stdout_reset(int original_stdout)
{
	dup2(original_stdout, STDOUT_FILENO);
	safe_close(&original_stdout);
}

void	ft_fd_reset(t_command *cmd, int original_stdin, int original_stdout)
{
	if (is_special_builtin(cmd->command))
	{
		if (cmd->prev)
		{
			if (!is_special_builtin(cmd->prev->command))
				stdin_reset(original_stdin);
		}
		if (cmd->next)
		{
			if (!is_special_builtin(cmd->next->command))
				stdout_reset(original_stdout);
		}
	}
}
