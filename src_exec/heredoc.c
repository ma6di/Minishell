#include "../includes/minishell.h"


static int	ft_check_delimiter(char *line, char *delimiter)
{
	if (!line)
	{
		ft_fprintf("Minishell: warning: here-document delimited");
		ft_fprintf("by end-of-file (wanted `%s')\n", delimiter);
		return (1);
	}
	if (ft_strncmp(line, delimiter, ft_strlen(delimiter) + \
		ft_strlen(line)) == 0)
	{
		free(line);
		return (1);
	}
	return (0);
}

static void	ft_heredoc_wait(t_command *cmd, pid_t pid)
{
	int		status;
	int		signal_number;

	if (waitpid(pid, &status, 0) == -1)
	{
		ft_fprintf("waitpid failed");
		cmd->main->exit_code = 1;
		return ;
	}
	if (pid > 0)
	{
		if (WIFEXITED(status))
		{
			int exit_status = WEXITSTATUS(status);
			if(exit_status == 1)
			{
				cmd->main->heredoc_fork_permit = -1;
				write(1, "\n", 1);
				cmd->main->exit_code = 130;
			}
		}
	}
	if (WIFSIGNALED(status))
	{
		signal_number = WTERMSIG(status);
		if (signal_number == SIGINT)
		{
			write(1, "\n", 1);
			cmd->main->exit_code = 130;
			cmd->main->heredoc_fork_permit = -1;
		}
	}
}

static void	ft_heredoc_write_to_file(t_command *cmd, t_heredoc *heredoc)
{
	if (heredoc->should_expand)
	{
		heredoc->expanded_line = expand_variables_in_line(heredoc->line, \
								cmd->main->env_vars, cmd->main);
		write(heredoc->heredoc_fd, heredoc->expanded_line, \
			ft_strlen(heredoc->expanded_line));
		free(heredoc->expanded_line);
	}
	else
		write(heredoc->heredoc_fd, heredoc->line, ft_strlen(heredoc->line));
	write(heredoc->heredoc_fd, "\n", 1);
	free(heredoc->line);
}

static void	ft_heredoc_readline(t_command *cmd, t_heredoc **heredoc)
{
	int	i;

	i = 0;
	while (cmd->io_fds->has_heredoc > i)
	{
		heredoc[i]->filename = "heredoc.txt";
		heredoc[i]->heredoc_fd = open(heredoc[i]->filename, \
			O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (heredoc[i]->heredoc_fd == -1)
		{
			ft_fprintf("Failed to open heredoc file");
			exit(1);
		}
		while (1)
		{
			rl_clear_history();
			heredoc[i]->line = readline("> ");
			if (ft_check_delimiter(heredoc[i]->line, heredoc[i]->delimiter))
				break ;
			ft_heredoc_write_to_file(cmd, heredoc[i]);
		}
		close(heredoc[i]->heredoc_fd);
		free(heredoc[i]->delimiter);
		i++;
	}
}

void	exec_heredoc(t_command *cmds)
{
	t_command	*cmd;
	t_command	*begin;

	begin = cmds;
	if (cmds->main->heredoc_fork_permit)
	{
		cmd = cmds;
		cmd->heredoc_pid = fork();
		if (cmd->heredoc_pid == 0)
		{
			set_signals_heredoc();
			while (cmd)
			{
				if (cmd->io_fds->has_heredoc)
					ft_heredoc_readline(cmd, cmd->heredocs);
				cmd = cmd->next;
			}
			char *new_prog = "/bin/true";
			char *args[] = {"true", NULL};
			char *env[] = {NULL};
			if(execve(new_prog, args, env) == -1)
				perror("true failed");
			free_heredoc(begin);
			free_main(begin->main);
			if (begin)
				free_command_child(&begin);
			exit(0);
		}
		else
			ft_heredoc_wait(cmds, cmd->heredoc_pid);
	}
}
