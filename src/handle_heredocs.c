/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_heredocs.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcheragh <mcheragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 12:46:34 by nrauh             #+#    #+#             */
/*   Updated: 2024/12/12 19:06:14 by mcheragh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

static t_heredoc	**add_to_heredocs(t_heredoc **old_heredocs, 
										t_heredoc *new_heredoc)
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
	if (curr->next->state == EMPTY)
		heredoc->delimiter = ft_strdup("");
	else
		heredoc->delimiter = ft_strdup(curr->next->value);
	heredoc->should_expand = curr->next->state == GENERAL;
	tmp = add_to_heredocs((*cmd)->heredocs, heredoc);
	if ((*cmd)->heredocs)
		free((*cmd)->heredocs);
	(*cmd)->heredocs = tmp;
	(*cmd)->io_fds->has_heredoc++;
	(*cmd)->main->heredoc_fork_permit++;
}
