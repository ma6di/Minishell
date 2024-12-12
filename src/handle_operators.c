/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_operators.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrauh <nrauh@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 12:45:19 by nrauh             #+#    #+#             */
/*   Updated: 2024/12/12 13:52:07 by nrauh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static t_operator	*init_operator(void)
{
	t_operator	*operator;

	operator = malloc(sizeof(t_operator));
	operator->type = UNINITIALIZED;
	operator->filename = NULL;
	return (operator);
}

static t_operator	**add_to_operators(t_operator **old_ops, t_operator *new_op)
{
	t_operator	**new_ops;
	int			i;

	i = 0;
	while (old_ops && old_ops[i])
		i++;
	new_ops = malloc((i + 2) * sizeof(char *));
	if (!new_ops)
		return (NULL);
	i = 0;
	while (old_ops && old_ops[i])
	{
		new_ops[i] = old_ops[i];
		i++;
	}
	new_ops[i++] = new_op;
	new_ops[i] = NULL;
	return (new_ops);
}

void	add_echo_n(t_command **cmd, t_token *curr)
{
	(*cmd)->command = ft_strdup("echo");
	handle_argument(cmd, "echo", curr->state);
	handle_argument(cmd, "-n", curr->state);
}

void	handle_operators(t_command **cmd, t_token *curr, t_token **head_t)
{
	t_operator		**tmp;
	t_operator		*operator;

	operator = init_operator();
	if (curr->type == HEREDOC)
	{
		handle_heredoc(cmd, curr);
		operator->filename = ft_strdup("heredoc.txt");
		operator->type = INFILE;
		if (((curr == *head_t || (curr->prev && curr->prev->type == PIPE))
				&& (!curr->next->next || curr->next->next->type != COMMAND)))
			add_echo_n(cmd, curr);
	}
	else
	{
		if (((curr == *head_t || (curr->prev && curr->prev->type == PIPE))
				&& (!curr->next->next || curr->next->next->type != COMMAND)))
			add_echo_n(cmd, curr);
		operator->filename = ft_strdup(curr->next->value);
		operator->type = curr->next->type;
	}
	tmp = add_to_operators((*cmd)->operators, operator);
	if ((*cmd)->operators)
		free((*cmd)->operators);
	(*cmd)->operators = tmp;
}
