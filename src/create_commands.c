/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_commands.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrauh <nrauh@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 14:45:20 by nrauh             #+#    #+#             */
/*   Updated: 2024/12/12 13:53:24 by nrauh            ###   ########.fr       */
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

void	handle_argument(t_command **cmd, char *value, t_state state)
{
	char		**tmp;

	if (ft_strlen(value) != 0)
	{
		if (state == EMPTY)
			tmp = add_to_args((*cmd)->args, ft_strdup(""));
		else
			tmp = add_to_args((*cmd)->args, ft_strdup(value));
		if ((*cmd)->args)
			free((*cmd)->args);
		(*cmd)->args = tmp;
	}
}

static t_command	*handle_types(t_command **cmd, t_token **head_t,
						t_token *curr, t_main **main)
{
	if (curr->type == COMMAND && !(*cmd)->command)
	{
		(*cmd)->command = ft_strdup(curr->value);
		handle_argument(cmd, curr->value, curr->state);
		if (!(*cmd)->args)
			return (NULL);
	}
	else if (curr->type == COMMAND)
		handle_argument(cmd, curr->value, curr->state);
	else if (curr->type == ARGUMENT)
	{
		handle_argument(cmd, curr->value, curr->state);
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

t_command	**create_commands(t_command **head_c, t_token **head_t, 
								t_main **main)
{
	t_token		*curr;
	t_command	*cmd;

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
