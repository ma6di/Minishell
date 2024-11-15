/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_commands.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrauh <nrauh@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 14:45:20 by nrauh             #+#    #+#             */
/*   Updated: 2024/11/15 14:55:17 by nrauh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	**add_to_args(char **old_args, char *new_arg)
{
	char	**new_args;
	int		i;

	i = 0;
	if (old_args)
	{
		while (old_args)
			i++;
	}
	new_args = malloc((i + 2) * sizeof(char *));
	if (!new_args)
		return (NULL); // potentially do more here freeing or so ???
	while (old_args && old_args[i])
	{
		new_args[i] = old_args[i];
		i++;
	}
	new_args[i] = new_arg;
	i++;
	new_args[i] = NULL;
	return (new_args);
}

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
	new_cmd->heredoc_content = NULL;
	new_cmd->nr_of_pipes = 0;
	new_cmd->pipe_fd = NULL;
	new_cmd->has_pipe = 0;
	init_empty_fds(&new_cmd);
	new_cmd->error_code = 0;
	new_cmd->error_message = NULL;
	new_cmd->result_file = NULL;
	new_cmd->pipe_created = NULL;
	new_cmd->pid = getpid();
	new_cmd->next = NULL;
	new_cmd->prev = NULL;
	new_cmd->main = NULL;
	return (new_cmd);
}

void	add_to_list(t_command **head, t_command *new_cmd)
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

t_command	**create_commands(t_command **head_c, t_token **head_t)
{
	t_token		*curr;
	t_command	*cmd;
	char		**tmp;

	curr = *head_t;
	cmd = init_empty_cmd();
	while (curr)
	{
		if (curr->type == COMMAND)
			cmd->command = ft_strdup(curr->value);
		else if (curr->type == ARGUMENT)
		{
			tmp = add_to_args(cmd->args, ft_strdup(curr->value));
			if (cmd->args)
				free(cmd->args);
			cmd->args = tmp;
		}
		else if (curr->type == HEREDOC)
		{
			cmd->heredoc_delimiter = ft_strdup(curr->next->value);
			cmd->expand_heredoc_content = curr->next->state == GENERAL;
			cmd->io_fds->has_heredoc = 1;
			cmd->io_fds->infile = ft_strdup("heredoc_tmp.txt");
		}
		else if (curr->type == REDIRECT)
			cmd->io_fds->outfile = ft_strdup(curr->next->value);
		else if (curr->type == APPEND)
			cmd->io_fds->append_outfile = ft_strdup(curr->next->value);
		else if (curr->type == PIPE)
		{
			cmd->has_pipe = 1;
			add_to_list(head_c, cmd);
			cmd = init_empty_cmd();
		}
		curr = curr->next;
	}
	if (cmd)
		add_to_list(head_c, cmd);
	return (head_c);
}
