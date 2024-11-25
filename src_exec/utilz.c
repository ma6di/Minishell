//NORM OK
#include "../includes/minishell.h"

void	cd_print_error(const char *arg)
{
	ft_putstr_fd("cd: ", 2);
	ft_putstr_fd((char *)arg, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(strerror(errno), 2);
}

void	remove_heredoc_file(t_main *main)
{
	t_command	*cmd;

	cmd = main->command_list;
	while (cmd)
	{
		if (cmd->io_fds->has_heredoc)
			unlink("heredoc.txt");
		cmd = cmd->next;
	}
}

int	is_builtin(char *command)
{
	size_t	cmd_len;

	cmd_len = ft_strlen(command);
	if (!command)
		return (0);
	if (ft_strncmp(command, "echo", ft_strlen("echo") + cmd_len) == 0 || \
		ft_strncmp(command, "pwd", ft_strlen("pwd") + cmd_len) == 0 || \
		ft_strncmp(command, "env", ft_strlen("env") + cmd_len) == 0)
	{
		return (1);
	}
	return (0);
}

int	is_special_builtin(char *command)
{
	size_t	cmd_len;

	cmd_len = ft_strlen(command);
	if (!command)
		return (0);
	if (ft_strncmp(command, "cd", ft_strlen("cd") + cmd_len) == 0 || \
		ft_strncmp(command, "export", ft_strlen("export") + cmd_len) == 0 || \
		ft_strncmp(command, "unset", ft_strlen("unset") + cmd_len) == 0 || \
		ft_strncmp(command, "exit", ft_strlen("exit") + cmd_len) == 0)
	{
		return (1);
	}
	return (0);
}

static int file_size(const char *file)
{
    struct stat file_stat;
    if (stat(file, &file_stat) == -1)
	{
        perror("stat");
        return -1;
    }
    return (int)file_stat.st_size;
}

void is_it_cat(t_command *cmd)
{
    if (cmd->args && cmd->args[0] && strncmp(cmd->args[0], "cat", 3) == 0)
	{
		if(cmd->next && cmd->has_pipe)
		{
			int i = 1;
			while (cmd->args[i])
			{
				int size = file_size(cmd->args[i]);
				if (size == -1)
					return;
				if (size > 64)
				{
					cmd->io_fds->outfile = strdup("cat.txt");
					if (cmd->next && !cmd->next->io_fds->infile)
						cmd->next->io_fds->infile = strdup("cat.txt");
					break;
				}
				i++;
			}
        }
    }
}
