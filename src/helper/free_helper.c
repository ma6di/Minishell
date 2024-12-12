/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrauh <nrauh@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 18:10:05 by nrauh             #+#    #+#             */
/*   Updated: 2024/12/12 17:48:50 by nrauh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_two_dim(char **arr)
{
	int	i;

	i = 0;
	while (arr && arr[i])
	{
		free(arr[i]);
		arr[i] = NULL;
		i++;
	}
	free(arr);
	arr = NULL;
}

void	free_heredocs(t_heredoc **heredocs)
{
	int	i;

	i = 0;
	while (heredocs && heredocs[i])
	{
		free(heredocs[i]->delimiter);
		heredocs[i]->delimiter = NULL;
		free(heredocs[i]);
		heredocs[i] = NULL;
		i++;
	}
	free(heredocs);
	heredocs = NULL;
}

void	free_operators(t_operator **operators)
{
	int	i;

	i = 0;
	while (operators && operators[i])
	{
		free(operators[i]->filename);
		operators[i]->filename = NULL;
		free(operators[i]);
		operators[i] = NULL;
		i++;
	}
	free(operators);
	operators = NULL;
}

void	free_fds(t_fds *io_fds)
{
	if (io_fds)
	{
		free(io_fds->infile);
		free(io_fds->outfile);
		free(io_fds->append_outfile);
		free(io_fds);
		io_fds = NULL;
	}
}

void	free_commands(t_command **head)
{
	t_command	*curr;
	t_command	*next;

	if (!head || !*head)
		return ;
	curr = *head;
	while (curr)
	{
		next = curr->next;
		free(curr->command);
		curr->command = NULL;
		free_two_dim(curr->args);
		free_heredocs(curr->heredocs);
		free_operators(curr->operators);
		free(curr->pipe_fd);
		curr->pipe_fd = NULL;
		free_fds(curr->io_fds);
		free(curr);
		curr = next;
	}
	*head = NULL;
}
