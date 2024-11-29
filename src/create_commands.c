/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_commands.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrauh <nrauh@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 14:45:20 by nrauh             #+#    #+#             */
/*   Updated: 2024/11/29 11:59:01 by nrauh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char    **add_to_args(char **old_args, char *new_arg)
{
    char    **new_args;
    int     i;

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

static void	handle_argument(t_command **cmd, char *value)
{
	char		**tmp;

	if (ft_strlen(value) != 0)
	{
		tmp = add_to_args((*cmd)->args, ft_strdup(value));
		if ((*cmd)->args)
			free((*cmd)->args);
		(*cmd)->args = tmp;
	}
}

static t_heredoc    *init_heredoc(void)
{
    t_heredoc   *heredoc;

    heredoc = malloc(sizeof(t_heredoc));
    heredoc->delimiter = NULL;
    heredoc->should_expand = 0;
    heredoc->line = NULL;
    heredoc->filename = NULL;
    heredoc->expanded_line = NULL;
    heredoc->heredoc_fd = 0;
    return (heredoc);
}

t_heredoc   **add_to_heredocs(t_heredoc **old_heredocs, t_heredoc *new_heredoc)
{
    t_heredoc   **new_heredocs;
    int         i;

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

t_operator  **add_to_operators(t_operator **old_operators, t_operator *new_operator)
{
    t_operator  **new_operators;
    int         i;

    i = 0;
    while (old_operators && old_operators[i])
        i++;
    new_operators = malloc((i + 2) * sizeof(char *));
    if (!new_operators)
        return (NULL);
    i = 0;
    while (old_operators && old_operators[i])
    {
        new_operators[i] = old_operators[i];
        i++;
    }
    new_operators[i++] = new_operator;
    new_operators[i] = NULL;
    return (new_operators);
}

void    handle_heredoc(t_command **cmd, t_token *curr)
{
    t_heredoc   *heredoc;
    t_heredoc   **tmp;

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

static t_operator   *init_operator(void)
{
    t_operator  *operator;

    operator = malloc(sizeof(t_operator));
    //operator->type = NULL;
    operator->filename = NULL;
    return (operator);
}

static void	handle_operators(t_command **cmd, t_token *curr, t_token **head_t)
{
	t_operator		**tmp;
	t_operator		*operator;

	operator = init_operator();
	if (curr->type == HEREDOC)
	{
		handle_heredoc(cmd, curr);
		operator->filename = ft_strdup("heredoc.txt");
	}
	else if ((curr == *head_t || (curr->prev && curr->prev->type == PIPE))
				&& (!curr->next->next || curr->next->next->type != COMMAND))
	{
		(*cmd)->command = ft_strdup("echo");
		handle_argument(cmd, ft_strdup("echo"));
		handle_argument(cmd, ft_strdup("-n"));
		operator->filename = ft_strdup(curr->next->value);
	}
	else
		operator->filename = ft_strdup(curr->next->value);
	operator->type = curr->next->type;
	tmp = add_to_operators((*cmd)->operators, operator);
	if ((*cmd)->operators)
		free((*cmd)->operators);
	(*cmd)->operators = tmp;
}

static t_command	*handle_types(t_command **cmd, t_token **head_t, 
						t_token *curr, t_main **main)
{
	if (curr->type == COMMAND && !(*cmd)->command)
	{
		(*cmd)->command = ft_strdup(curr->value);
		handle_argument(cmd, curr->value);
		if (!(*cmd)->args)
			return (NULL);
	}
	else if (curr->type == COMMAND)
		handle_argument(cmd, curr->value);
	else if (curr->type == ARGUMENT)
	{
		handle_argument(cmd, curr->value);
		if (!(*cmd)->args)
			return (NULL);
	}
	else if (curr->type == REDIRECT || curr->type == APPEND 
		|| curr->type == INPUT_REDIRECT || curr->type == HEREDOC)
		handle_operators(cmd, curr, head_t);
	else if (curr->type == PIPE)
	{
		(*cmd)->has_pipe = 1;
		(*main)->command_list = add_command(&(*main)->command_list, (*cmd));
		(*cmd) = init_empty_cmd(main);
	}
	return ((*main)->command_list);
}

t_command   **create_commands(t_command **head_c, t_token **head_t, t_main **main)
{
    t_token     *curr;
    t_command   *cmd;

	if (!head_t || !(*head_t))
		return (NULL);
	curr = *head_t;
	cmd = init_empty_cmd(main);
	if (!cmd)
		return (free_commands(&cmd), NULL);
	while (curr)
	{
		*head_c = handle_types(&cmd, head_t, curr, main);
		curr = curr->next;
	}
	if (cmd)
		add_command(head_c, cmd);
	return (head_c);
}
