//NORM OK
#include "../includes/minishell.h"

static void	no_such_dir(char *io_fds)
{
	write(2, "minishell: ", 11);
	write(2, io_fds, ft_strlen(io_fds));
	write(2, ": No such file or directory\n", 28);
}

static int	setup_input_redirection(t_fds *io_fds, t_command *cmd)
{
	if (io_fds->infile && !is_special_builtin(cmd->command))
	{
		io_fds->fd_in = open(io_fds->infile, O_RDONLY);
		if (io_fds->fd_in == -1)
		{
			no_such_dir(io_fds->infile);
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
			no_such_dir(io_fds->outfile);
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
			no_such_dir(io_fds->append_outfile);
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
			perror("minishell");
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
