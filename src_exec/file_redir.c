//NORM OK
#include "../includes/minishell.h"

static void	no_such_dir(char *file)
{
	write(2, "minishell: ", 11);
	write(2, file, ft_strlen(file));
	write(2, ": No such file or directory\n", 28);
}

static void	is_a_dir(char *file)
{
	write(2, "minishell: ", 11);
	write(2, file, ft_strlen(file));
	write(2, ": Is a directory\n", 17);
}

static int error_code(int err_code, char *file)
{
    if (err_code == EACCES)
    {
        fprintf(stderr,"minishell: %s: Permission denied\n", file);
        return (-1);
    }
    else if (err_code == ENOENT)
    {
        no_such_dir(file);
        return (-1);  // Command not found error code
    }
    else if (err_code == EISDIR)
    {
       	is_a_dir(file);
        return (-1);  // Directory execution error code
    }
    else
    {
        perror("minishell");
        return (-1);  // General execution error code
    }
}

static int	setup_input_redirection(t_fds *io_fds, t_command *cmd)
{
	if (io_fds->infile && !is_special_builtin(cmd->command))
	{
		io_fds->fd_in = open(io_fds->infile, O_RDONLY);
		if (io_fds->fd_in == -1)
		{
			return (error_code(errno, io_fds->infile));
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
			return (error_code(errno, io_fds->outfile));
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
			return (error_code(errno, io_fds->append_outfile));
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
	return (SUCCESS);
}
