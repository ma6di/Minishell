/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assign_types.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrauh <nrauh@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 10:34:32 by nrauh             #+#    #+#             */
/*   Updated: 2024/11/27 18:11:39 by nrauh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// this should not work like that, echo pwd -> pwd ...
/*int	is_inbuilt_cmd(char *value)
{
	if (ft_strncmp(value, "echo", ft_strlen(value)) == 0
		|| ft_strncmp(value, "cd", ft_strlen(value)) == 0
		|| ft_strncmp(value, "pwd", ft_strlen(value)) == 0
		|| ft_strncmp(value, "export", ft_strlen(value)) == 0
		|| ft_strncmp(value, "unset", ft_strlen(value)) == 0
		|| ft_strncmp(value, "env", ft_strlen(value)) == 0
		|| ft_strncmp(value, "exit", ft_strlen(value)) == 0)
		return (1);
	return (0);
}*/

int	assign_redirect(t_token *token)
{
	if (ft_strncmp(token->value, ">", ft_strlen(token->value)) == 0)
		return (token->type = REDIRECT, 0);
	else if (ft_strncmp(token->value, ">>", ft_strlen(token->value)) == 0)
		return (token->type = APPEND, 0);
	else if (ft_strncmp(token->value, "<", ft_strlen(token->value)) == 0)
		return (token->type = INPUT_REDIRECT, 0);
	else if (ft_strncmp(token->value, "<<", ft_strlen(token->value)) == 0)
		return (token->type = HEREDOC, 0);
	else
		return (-1);
}

int	assign_other_operator(t_token *token)
{
	if (ft_strncmp(token->value, "|", ft_strlen(token->value)) == 0)
		return (token->type = PIPE, 0);
	else if (ft_strncmp(token->value, "||", ft_strlen(token->value)) == 0)
		return (token->type = LOGICAL_OR, 0);
	return (-1);
}

int	assign_by_prev(t_token *token)
{
	if ((token->prev->type == HEREDOC_DELIMITER || token->prev->type == FILENAME)
		&& (!token->prev->prev->prev || token->prev->prev->prev->type != COMMAND))
		return (token->type = COMMAND, 0);
	else if (token->prev->type == PIPE
		|| token->prev->type == LOGICAL_OR)
		return (token->type = COMMAND, 0);
	else if (token->prev->type == HEREDOC)
		return (token->type = HEREDOC_DELIMITER, 0);
	else if (token->prev->type == REDIRECT)
		return (token->type = OUTFILE, 0);
	else if (token->prev->type == APPEND)
		return (token->type = APPENDFILE, 0);
	else if (token->prev->type == INPUT_REDIRECT)
		return (token->type = INFILE, 0);
	return (-1);
}

t_token	**assign_types(t_token **head)
{
	t_token	*curr;

	curr = *head;
	if (!head || !(*head))
		return (NULL);
	while (curr)
	{
		if (ft_strlen(curr->value) == 0)
			curr->type = ARGUMENT;
		else if (curr == *head && assign_redirect(curr) == -1)
			curr->type = COMMAND;
		else if (assign_redirect(curr) == -1
			&& assign_other_operator(curr) == -1
			&& assign_by_prev(curr) == -1)
			curr->type = ARGUMENT;
		curr = curr->next;
	}
	return (head);
}
