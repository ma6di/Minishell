/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_commands.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrauh <nrauh@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 14:45:20 by nrauh             #+#    #+#             */
/*   Updated: 2024/11/22 17:18:32 by nrauh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	**add_to_args(char **old_args, char *new_arg)
{
	char	**new_args;
	int		i;

	i = 0;
	//printf("duped arg %p - %s\n", new_arg, new_arg);
	while (old_args && old_args[i])
		i++;
	new_args = malloc((i + 2) * sizeof(char *));
	//printf("created new args array %p\n", new_args);
	if (!new_args)
		return (NULL);
	i = 0;
	while (old_args && old_args[i])
	{
		new_args[i] = old_args[i];
		i++;
	}
	new_args[i++] = new_arg;
	new_args[i] = NULL;
	return (new_args);
}

static void	handle_argument(t_command **cmd, t_token *curr)
{
	char		**tmp;

	if (ft_strlen(curr->value) != 0)
	{
		tmp = add_to_args((*cmd)->args, ft_strdup(curr->value));
		if ((*cmd)->args)
		{
			//printf("freed old args array %p\n", (*cmd)->args);
			free((*cmd)->args);
			
		}
		(*cmd)->args = tmp;
	}
}

static t_heredoc	*init_heredoc(void)
{
	t_heredoc	*heredoc;

	heredoc = malloc(sizeof(t_heredoc));
	heredoc->delimiter = NULL;
	heredoc->should_expand = 0;
	heredoc->line = NULL;
	heredoc->filename = NULL;
	heredoc->expanded_line = NULL;
	heredoc->heredoc_fd = 0;
	return (heredoc);
}

t_heredoc	**add_to_heredocs(t_heredoc **old_heredocs, t_heredoc *new_heredoc)
{
	t_heredoc	**new_heredocs;
	int			i;

	i = 0;
	while (old_heredocs && old_heredocs[i])
		i++;
	new_heredocs = malloc((i + 2) * sizeof(char *));
	if (!new_heredocs)
		return (NULL);
	i = 0;
	while (old_heredocs && old_heredocs[i])
	{
		new_heredocs[i] = old_heredocs[i];
		i++;
	}
	new_heredocs[i++] = new_heredoc;
	new_heredocs[i] = NULL;
	return (new_heredocs);
}

void	handle_heredoc(t_command **cmd, t_token *curr)
{
	t_heredoc	*heredoc;
	t_heredoc	**tmp;

	heredoc = init_heredoc();
	heredoc->delimiter = ft_strdup(curr->next->value);
	heredoc->should_expand = curr->next->state == GENERAL;
	tmp = add_to_heredocs((*cmd)->heredocs, heredoc);
	if ((*cmd)->heredocs)
		free((*cmd)->heredocs);
	(*cmd)->heredocs = tmp;
	(*cmd)->io_fds->has_heredoc++;
	if ((*cmd)->io_fds->infile)
		free((*cmd)->io_fds->infile);
	(*cmd)->io_fds->infile = ft_strdup("heredoc.txt");
	(*cmd)->main->heredoc_fork_permit++;
}

static void	*handle_types(t_command **cmd, t_command **head_c, t_token *curr, t_main **main)
{
	if (curr->type == COMMAND)
	{
		(*cmd)->command = ft_strdup(curr->value);
		handle_argument(cmd, curr);
		if (!(*cmd)->args)
			return (NULL);
	}
	else if (curr->type == ARGUMENT)
	{
		handle_argument(cmd, curr);
		if (!(*cmd)->args)
			return (NULL);
	}
	else if (curr->type == HEREDOC)
		handle_heredoc(cmd, curr);
	else if (curr->type == REDIRECT)
		(*cmd)->io_fds->outfile = ft_strdup(curr->next->value);
	else if (curr->type == APPEND)
		(*cmd)->io_fds->append_outfile = ft_strdup(curr->next->value);
	else if (curr->type == INPUT_REDIRECT)
	{
		if ((*cmd)->io_fds->infile)
			free((*cmd)->io_fds->infile);
		(*cmd)->io_fds->infile = ft_strdup(curr->next->value);
	}
	else if (curr->type == PIPE)
	{
		(*cmd)->has_pipe = 1;
		add_command(head_c, (*cmd));
		(*cmd) = init_empty_cmd(main);
	}
	return (cmd);
}

t_command	**create_commands(t_command **head_c, t_token **head_t, t_main **main)
{
	t_token		*curr;
	t_command	*cmd;

	if (!head_t || !(*head_t))
		return (NULL);
	curr = *head_t;
	cmd = init_empty_cmd(main);
	if (!cmd)
	{
		free_commands(&cmd);
		return (NULL);
	}
	while (curr)
	{
		handle_types(&cmd, head_c, curr, main);
		curr = curr->next;
	}
	if (cmd)
		add_command(head_c, cmd);
	return (head_c);
}
