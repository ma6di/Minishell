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
			perror("minishell: dup2 input redirection failed");
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
		perror("minishell: dup2 output redirection failed");
		close(cmd->io_fds->fd_out);
		return (-1);
	}
	close(cmd->io_fds->fd_out);
	return (0);
}

static int	setup_append_redirection(char *filename, t_command *cmd)
{
	//printf("doing append\n");
	cmd->io_fds->fd_out = open(filename, \
		O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (cmd->io_fds->fd_out == -1)
	{
		//printf("append error 1\n");
		return (error_code(errno, filename));
	}
	if (dup2(cmd->io_fds->fd_out, STDOUT_FILENO) == -1)
	{
		//printf("append error 2\n");
		perror("minishell: dup2 append output redirection failed");
		close(cmd->io_fds->fd_out);
		return (-1);
	}
	close(cmd->io_fds->fd_out);
	return (0);
}

int	setup_file_redirections(t_command *cmd)
{
	int	i;

	i = 0;
	while(cmd->operators && cmd->operators[i])
	{
		t_token_type	type;
		char			*filename;

		type = cmd->operators[i]->type;
		filename = cmd->operators[i]->filename;
		if(type == INFILE)
		{
			if (setup_input_redirection(filename, cmd) == -1)
				return (-1);
		}
		if(type == OUTFILE)
		{
			if (setup_output_redirection(filename, cmd) == -1)
				return (-1);
		}
		if(type == APPENDFILE)
		{
			if (setup_append_redirection(filename, cmd) == -1)
				return (-1);
		}
		i++;
	}
	// printf("here77\n");
	return (SUCCESS);
}
