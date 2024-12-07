#include "../includes/minishell.h"

void	parent_pipe_close(t_command *cmd)
{
	if (cmd->next && cmd->has_pipe)
		safe_close(&cmd->pipe_fd[1]);
	if (cmd->prev && cmd->prev->has_pipe)
		safe_close(&cmd->prev->pipe_fd[0]);
}

void	child_pipe_close(t_command *cmd, int original_std[2])
{
	if (cmd->pipe_fd[1])
		safe_close(&cmd->pipe_fd[1]);
	if (cmd->pipe_fd[0])
		safe_close(&cmd->pipe_fd[0]);
	safe_close(&original_std[0]);
	safe_close(&original_std[1]);
}

static void	stdin_reset(int original_stdin)
{
	if (dup2(original_stdin, STDIN_FILENO) == -1)
		ft_fprintf("Minishell: dup2 reset stdin failed\n");
	safe_close(&original_stdin);
}

static void	stdout_reset(int original_stdout)
{
	if (dup2(original_stdout, STDOUT_FILENO) == -1)
		ft_fprintf("Minishell: dup2 reset stdout failed\n");
	safe_close(&original_stdout);
}

void	std_fd_reset(t_command *cmd, int original_std[2])
{
	if (is_special_builtin(cmd->command))
	{
		stdin_reset(original_std[0]);
		stdout_reset(original_std[1]);
		safe_close(&original_std[0]);
		safe_close(&original_std[1]);
	}
}
