/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_list.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrauh <nrauh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 13:07:56 by nrauh             #+#    #+#             */
/*   Updated: 2024/11/16 05:16:27 by nrauh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	init_empty_fds(t_command **new_cmd)
{
	(*new_cmd)->io_fds->infile = NULL;
	(*new_cmd)->io_fds->outfile = NULL;
	(*new_cmd)->io_fds->append_outfile = NULL;
	(*new_cmd)->io_fds->fd_in = 0;
	(*new_cmd)->io_fds->fd_out = 0;
	(*new_cmd)->io_fds->has_heredoc = 0;
	(*new_cmd)->io_fds->fd_err = 0; //not using at the moment
	(*new_cmd)->io_fds->is_stderr_redirected = 0;
	(*new_cmd)->io_fds->in_duped = 0; //not using at the moment
	(*new_cmd)->io_fds->out_duped = 0;
}

t_command	*init_empty_cmd(void)
{
	t_command	*new_cmd;

	new_cmd = malloc(sizeof(t_command));
	if (!new_cmd)
		return (NULL);
	new_cmd->io_fds = malloc(sizeof(t_fds));
	if (!new_cmd->io_fds)
		return (NULL);
	new_cmd->command = NULL;
	new_cmd->args = NULL;
	new_cmd->heredoc_delimiter = NULL;
	new_cmd->expand_heredoc_content = 0;
	new_cmd->nr_of_pipes = 0;
	// free later on 
	new_cmd->pipe_fd = malloc(2 * sizeof(int));
	new_cmd->has_pipe = 0;
	new_cmd->error_code = 0;
	new_cmd->error_message = NULL;
	new_cmd->result_file = NULL;
	new_cmd->pipe_created = NULL;
	new_cmd->pid = getpid();
	init_empty_fds(&new_cmd);
	new_cmd->next = NULL;
	new_cmd->prev = NULL;
	new_cmd->main = NULL;
	return (new_cmd);
}

void	add_command(t_command **head, t_command *new_cmd)
{
	t_command	*curr;

	if (!head || !(*head))
	{
		*head = new_cmd;
		new_cmd->prev = NULL;
	}
	else
	{
		curr = *head;
		while (curr->next)
			curr = curr->next;
		curr->next = new_cmd;
		new_cmd->prev = curr;
	}
	new_cmd->next = NULL;
}
