//NORM OK
#include "../includes/minishell.h"

static int	setup_input_redirection(t_fds *io_fds, t_command *cmd)
{
	if (io_fds->infile && !is_special_builtin(cmd->command))
	{
		io_fds->fd_in = open(io_fds->infile, O_RDONLY);
		if (io_fds->fd_in == -1)
		{
			perror("minishell: failed to open input file");
			return (-1);
		}
		if (dup2(io_fds->fd_in, STDIN_FILENO) == -1)
		{
			perror("minishell: dup2 input redirection failed");
			close(io_fds->fd_in);
			return (-1);
		}
		close(io_fds->fd_in);
	}
	return (0);
}

static int	setup_output_redirection(t_fds *io_fds)
{
	if (io_fds->outfile)
	{
		io_fds->fd_out = open(io_fds->outfile, \
			O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (io_fds->fd_out == -1)
		{
			perror("minishell: failed to open output file");
			return (-1);
		}
		if (dup2(io_fds->fd_out, STDOUT_FILENO) == -1)
		{
			perror("minishell: dup2 output redirection failed");
			close(io_fds->fd_out);
			return (-1);
		}
		close(io_fds->fd_out);
	}
	return (0);
}

static int	setup_append_redirection(t_fds *io_fds)
{
	if (io_fds->append_outfile)
	{
		io_fds->fd_out = open(io_fds->append_outfile, \
			O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (io_fds->fd_out == -1)
		{
			perror("minishell: failed to open append output file");
			return (-1);
		}
		if (dup2(io_fds->fd_out, STDOUT_FILENO) == -1)
		{
			perror("minishell: dup2 append output redirection failed");
			close(io_fds->fd_out);
			return (-1);
		}
		close(io_fds->fd_out);
	}
	return (0);
}

static int	setup_error_redirection(t_fds *io_fds)
{
	if (io_fds->is_stderr_redirected)
	{
		io_fds->fd_err = open("error.log", O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (io_fds->fd_err == -1)
		{
			perror("minishell: failed to open error log file");
			return (-1);
		}
		if (dup2(io_fds->fd_err, STDERR_FILENO) == -1)
		{
			perror("minishell: dup2 stderr redirection failed");
			close(io_fds->fd_err);
			return (-1);
		}
		close(io_fds->fd_err);
	}
	return (0);
}

int	setup_file_redirections(t_command *cmd)
{
	t_fds	*io_fds;

	io_fds = cmd->io_fds;
	if (setup_input_redirection(io_fds, cmd) == -1)
		return (-1);
	if (setup_output_redirection(io_fds) == -1)
		return (-1);
	if (setup_append_redirection(io_fds) == -1)
		return (-1);
	if (setup_error_redirection(io_fds) == -1)
		return (-1);
	return (SUCCESS);
}
