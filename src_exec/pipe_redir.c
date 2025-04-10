/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcheragh <mcheragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 16:58:00 by mcheragh          #+#    #+#             */
/*   Updated: 2024/12/12 16:58:01 by mcheragh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	pipe_handler(t_command *cmd)
{
	if (cmd->next && cmd->has_pipe)
	{
		if (pipe(cmd->pipe_fd) == -1)
			ft_fprintf("Minishell: pipe creation failed\n");
		cmd->pipe_created = true;
	}
}

void	setup_pipe_redirections(t_command *cmd)
{
	if (cmd->prev)
	{
		if (cmd->prev->pipe_created && !(type_redir_exist(cmd, INFILE)))
			dup2_in(cmd->prev->pipe_fd);
	}
	if (cmd->pipe_created && cmd->next && !(type_redir_exist(cmd, OUTFILE)) && \
		!(type_redir_exist(cmd, APPENDFILE)))
		dup2_out(cmd->pipe_fd);
}

void	dup2_out(int *pipe_fd)
{
	if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
		ft_fprintf("Minishell: dup2 pipe_fd[1] failed\n");
	safe_close(&pipe_fd[1]);
}

void	dup2_in( int *pipe_fd)
{
	if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
		ft_fprintf("Minishell: dup2 pipe_fd[0] failed\n");
	safe_close(&pipe_fd[0]);
}
