//NORM OK
#include "../includes/minishell.h"

static int	ft_check_delimiter(char *line, char *delimiter)
{
	if (!line)
	{
		write(2, "bash: warning: here-document \
		delimited by end-of-file (wanted `", 63);
		write(2, delimiter, ft_strlen(delimiter));
		write(2, "')\n", 3);
		return (1);
	}
	if (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0)
	{
		free(line);
		return (1);
	}
	return (0);
}

static void	ft_heredoc_wait(t_command *cmd, pid_t pid)
{
	int			status;
	t_command	*cmd_tmp;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
	{
		cmd->main->exit_code = WEXITSTATUS(status);
		if (cmd->main->exit_code == 130)
		{
			cmd_tmp = cmd;
			while (cmd_tmp)
			{
				if (cmd_tmp->io_fds->infile)
				{
					unlink(cmd_tmp->io_fds->infile);
					free(cmd_tmp->io_fds->infile);
					cmd_tmp->io_fds->infile = NULL;
				}
				cmd_tmp = cmd_tmp->next;
			}
			cmd->main->heredoc_fork_permit = 0;
		}
	}
}

static void	ft_heredoc_write_to_file(t_command *cmd, t_heredoc *heredoc)
{
	if (cmd->expand_heredoc_content)
	{
		heredoc->expanded_line = \
			expand_variables_in_line(heredoc->line, cmd->main->env_vars);
		write(heredoc->heredoc_fd, heredoc->expanded_line, \
			ft_strlen(heredoc->expanded_line));
		free(heredoc->expanded_line);
	}
	else
		write(heredoc->heredoc_fd, heredoc->line, ft_strlen(heredoc->line));
	write(heredoc->heredoc_fd, "\n", 1);
	free(heredoc->line);
}

static void	ft_heredoc_readline(t_command *cmd, t_heredoc *heredoc)
{
	heredoc->filename = "heredoc.txt";
	heredoc->heredoc_fd = open(heredoc->filename, \
		O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (heredoc->heredoc_fd == -1)
	{
		perror("Failed to open heredoc file");
		exit(1);
	}
	while (g_sigint_received)
	{
		rl_clear_history();
		heredoc->line = readline("> ");
		if (ft_check_delimiter(heredoc->line, cmd->heredoc_delimiter))
			break ;
		ft_heredoc_write_to_file(cmd, heredoc);
	}
	close(heredoc->heredoc_fd);
	if (!g_sigint_received)
		exit(130);
}

void	exec_heredoc(t_command *cmds)
{
	t_command	*cmd;
	t_heredoc	*heredoc;

	if (cmds->main->heredoc_fork_permit)
	{
		cmd = cmds;
		heredoc = malloc(sizeof(t_heredoc));
		heredoc->pid = fork();
		if (heredoc->pid == 0)
		{
			while (cmd && g_sigint_received)
			{
				if (cmd->io_fds->has_heredoc)
					ft_heredoc_readline(cmd, heredoc);
				cmd = cmd->next;
			}
			free(heredoc);
			exit(0);
		}
		else
			ft_heredoc_wait(cmds, heredoc->pid);
	}
}
