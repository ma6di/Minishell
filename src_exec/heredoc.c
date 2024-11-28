#include "../includes/minishell.h"
static int	ft_check_delimiter(char *line, char *delimiter)
{
	if (!line)
	{
		write(2, "bash: warning: here-document", 28);
		write(2, " delimited by end-of-file (wanted `", 35);
		write(2, delimiter, ft_strlen(delimiter));
		write(2, "')\n", 3);
		return (1);
	}
	if (ft_strncmp(line, delimiter, ft_strlen(delimiter) + ft_strlen(line)) == 0)
	{
		free(line);
		return (1);
	}
	return (0);
}

static void	ft_heredoc_wait(t_command *cmd, pid_t pid)
{
	int		status;

	if (waitpid(pid, &status, 0) == -1)
	{
		perror("waitpid failed");
		cmd->main->exit_code = 1;
		return ;
	}
	if (WIFSIGNALED(status))
	{
		int signal_number = WTERMSIG(status);
		if (signal_number == SIGKILL)
		{
			write(1, "\n", 1);
			cmd->main->exit_code = 130;
			g_pid = 1;
		}
		cmd->main->heredoc_fork_permit = 0;
	}
}
static void ft_heredoc_write_to_file(t_command *cmd, t_heredoc *heredoc)
{
	if (heredoc->should_expand)
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
static void ft_heredoc_readline(t_command *cmd, t_heredoc *heredoc)
{
	heredoc->filename = "heredoc.txt";
	heredoc->heredoc_fd = open(heredoc->filename, \
		O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (heredoc->heredoc_fd == -1)
	{
		perror("Failed to open heredoc file");
		exit(1);
	}
	while (1)
	{
		rl_clear_history();
		heredoc->line = readline("> ");
		if (ft_check_delimiter(heredoc->line, heredoc->delimiter))
			break ;
		ft_heredoc_write_to_file(cmd, heredoc);
	}
	close(heredoc->heredoc_fd);
	free(heredoc->delimiter);
}
void	exec_heredoc(t_command *cmds)
{
	t_command	*cmd;
	int			i;
	int			j;

	if (cmds->main->heredoc_fork_permit)
	{
		cmd = cmds;
		i = 0;
		cmd->heredoc_pid = fork();
		if (cmd->heredoc_pid == 0)
		{
			set_signals_heredoc();
			g_pid = getpid();
			while (cmd)
			{
				if (cmd->io_fds->has_heredoc)
					while (cmd->io_fds->has_heredoc > i)
					{
						ft_heredoc_readline(cmd, cmd->heredocs[i]);
						i++;
					}
				cmd = cmd->next;
			}
			j = 0;
			while (cmd->heredocs[j])
				free(cmd->heredocs[j++]);
			free(cmd->heredocs);
			free_commands(&cmds);
			exit(0);
		}
		else
			ft_heredoc_wait(cmds, cmd->heredoc_pid);
	}
}
