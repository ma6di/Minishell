//NORM OK
#include "../includes/minishell.h"

static int	error_code(int err_code, char *file)
{
	if (err_code == EACCES)
	{
		ft_fprintf("minishell: %s: Permission denied\n", file);
		return (-1);
	}
	else if (err_code == ENOENT)
	{
		ft_fprintf("minishell: %s: No such file or directory\n", file);
		return (-1);
	}
	else if (err_code == EISDIR)
	{
		ft_fprintf("minishell: %s: Is a directory\n", file);
		return (-1);
	}
	else
	{
		perror("Minishell");
		return (-1);
	}
}

static int	setup_input_redirection(char *filename, t_command *cmd)
{
	if (!is_special_builtin(cmd->command))
	{
		cmd->io_fds->fd_in = open(filename, O_RDONLY);
		if (cmd->io_fds->fd_in == -1)
		{
			return (error_code(errno, filename));
		}
		if (dup2(cmd->io_fds->fd_in, STDIN_FILENO) == -1)
		{
			ft_fprintf("minishell: dup2 input file redirection failed");
			close(cmd->io_fds->fd_in);
			return (-1);
		}
		close(cmd->io_fds->fd_in);
	}
	return (0);
}

static int	setup_output_redirection(char *filename, t_command *cmd)
{
	cmd->io_fds->fd_out = open(filename, \
		O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (cmd->io_fds->fd_out == -1)
	{
		return (error_code(errno, filename));
	}
	if (dup2(cmd->io_fds->fd_out, STDOUT_FILENO) == -1)
	{
		ft_fprintf("minishell: dup2 output file redirection failed");
		close(cmd->io_fds->fd_out);
		return (-1);
	}
	close(cmd->io_fds->fd_out);
	return (0);
}

static int	setup_append_redirection(char *filename, t_command *cmd)
{
	cmd->io_fds->fd_out = open(filename, \
		O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (cmd->io_fds->fd_out == -1)
	{
		return (error_code(errno, filename));
	}
	if (dup2(cmd->io_fds->fd_out, STDOUT_FILENO) == -1)
	{
		ft_fprintf("minishell: dup2 append output file redirection failed");
		close(cmd->io_fds->fd_out);
		return (-1);
	}
	close(cmd->io_fds->fd_out);
	return (0);
}

int	setup_file_redirections(t_command *cmd)
{
	t_token_type	type;
	char			*filename;
	int				i;

	i = 0;
	while (cmd->operators && cmd->operators[i])
	{
		type = cmd->operators[i]->type;
		filename = cmd->operators[i]->filename;
		if (type == INFILE)
			return (setup_input_redirection(filename, cmd));
		if (type == OUTFILE)
			return (setup_output_redirection(filename, cmd));
		if (type == APPENDFILE)
			return (setup_append_redirection(filename, cmd));
		i++;
	}
	return (SUCCESS);
}
