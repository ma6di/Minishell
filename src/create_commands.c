/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_commands.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrauh <nrauh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 14:45:20 by nrauh             #+#    #+#             */
/*   Updated: 2024/11/16 05:51:38 by nrauh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	**add_to_args(char **old_args, char *new_arg)
{
	char	**new_args;
	int		i;

	i = 0;
	while (old_args && old_args[i])
		i++;
	new_args = malloc((i + 2) * sizeof(char *));
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

void	handle_argument(t_command **cmd, t_token *curr)
{
	char		**tmp;

	tmp = add_to_args((*cmd)->args, ft_strdup(curr->value));
	if ((*cmd)->args)
		free((*cmd)->args);
	(*cmd)->args = tmp;
}

void	handle_heredoc(t_command **cmd, t_token *curr)
{
	(*cmd)->heredoc_delimiter = ft_strdup(curr->next->value);
	(*cmd)->expand_heredoc_content = curr->next->state == GENERAL;
	(*cmd)->io_fds->has_heredoc = 1;
	(*cmd)->io_fds->infile = ft_strdup("heredoc.txt");
}

void	handle_types(t_command **cmd, t_command **head_c, t_token *curr)
{
	if (curr->type == COMMAND)
		(*cmd)->command = ft_strdup(curr->value);
	else if (curr->type == ARGUMENT)
		handle_argument(cmd, curr);
	else if (curr->type == HEREDOC)
		handle_heredoc(cmd, curr);
	else if (curr->type == REDIRECT)
		(*cmd)->io_fds->outfile = ft_strdup(curr->next->value);
	else if (curr->type == APPEND)
		(*cmd)->io_fds->append_outfile = ft_strdup(curr->next->value);
	else if (curr->type == INPUT_REDIRECT)
		(*cmd)->io_fds->infile = ft_strdup(curr->next->value);
	else if (curr->type == PIPE)
	{
		(*cmd)->has_pipe = 1;
		add_command(head_c, (*cmd));
		(*cmd) = init_empty_cmd();
	}
}

t_command	**create_commands(t_command **head_c, t_token **head_t)
{
	t_token		*curr;
	t_command	*cmd;

	curr = *head_t;
	cmd = init_empty_cmd();
	while (curr)
	{
		handle_types(&cmd, head_c, curr);
		curr = curr->next;
	}
	if (cmd)
		add_command(head_c, cmd);
	return (head_c);
}
