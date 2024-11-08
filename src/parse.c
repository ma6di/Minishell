/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrauh <nrauh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 10:14:14 by nrauh             #+#    #+#             */
/*   Updated: 2024/11/08 06:34:30 by nrauh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// for 'he\'llo' it is counted as unclosed quote.
// show the quote> prompt to close the quote and then add it to the string....

int	is_operator(char c)
{
	if (c == OP_PIPE[0] || c == OP_REDIRECT[0] || c == OP_INPUT_REDIRECT[0])
		return (1);
	return (0);
}

int	is_delimiter(char c)
{
	if (c == OP_PIPE[0] || c == OP_REDIRECT[0]
		|| c == OP_INPUT_REDIRECT[0] || c == WHITESPACE[0])
		return (1);
	return (0);
}

void	end_token(char **buffer, t_token **head, t_token_state state, int token_count)
{
	//printf("END TOKEN %s TOKEN COUNT %d\n", *buffer, token_count);
	create_token(head, ft_strdup(*buffer), state, token_count);
	free(*buffer);
	*buffer = NULL;
}

char	*add_to_buffer(char **buffer, char c)
{
	size_t	len;
	char	*new_buffer;
	char	*tmp;

	if (!(*buffer))
		*buffer = strdup("");
	len = strlen(*buffer);
	new_buffer = malloc((len + 2) * sizeof(char));
	if (!new_buffer)
		return (free(*buffer), NULL);
	tmp = *buffer;
	while (*tmp)
		*new_buffer++ = *tmp++;
	new_buffer = new_buffer - len;
	new_buffer[len] = c;
	new_buffer[len + 1] = '\0';
	free(*buffer);
	return (new_buffer);
}

int	last_token_count(t_token **head)
{
	t_token	*curr;

	curr = *head;
	if (!curr)
		return (0);
	while (curr->next)
		curr = curr->next;
	return (curr->token_count);
}

// we str + 1 is current, we need to access the previous (*str) char
char	*change_state(t_token_state *state, char *str, char **buffer, t_token **head)
{
	int	token_count;

	token_count = last_token_count(head);
	if (*state == GENERAL && *(str + 1) == '\'')
	{
		if (*buffer)
			end_token(buffer, head, *state, token_count);
		*state = QUOTE;
	}
	else if (*state == GENERAL && *(str + 1) == '"')
	{
		if (*buffer)
			end_token(buffer, head, *state, token_count);
		*state = DQUOTE;
	}
	else if ((*state == QUOTE && *(str + 1) == '\'' && *str != '\'' && *(str + 2) != '\'')
			|| (*state == DQUOTE && *(str + 1) == '"' && *str != '"' && *(str + 2) != '"'))
	{
		if (is_delimiter(*(str + 2)))
			token_count++;
		printf("curr char %c next char %c\n", *(str + 1), *(str + 2));
		printf("read buffer %s, now token increased to %d\n", *buffer, token_count);
		if (*buffer)
			end_token(buffer, head, *state, token_count);
		*state = GENERAL;
	}
	return (str + 1);
}

char	*handle_state_general(char **buffer, t_token **head, t_token_state state, char *input)
{
	int	token_count;

	token_count = last_token_count(head);
	if (is_delimiter(*input) && !is_delimiter(*(input + 1)))
		token_count++;
	if (!is_delimiter(*input))
		*buffer = add_to_buffer(buffer, *input);
	if (*buffer && is_delimiter(*input))
		end_token(buffer, head, state, token_count);
	if (is_operator(*input))
	{
		*buffer = add_to_buffer(buffer, *input);
		if (*(input + 1) == *input)
			*buffer = add_to_buffer(buffer, *input++);
		token_count++;
		printf("curr char %c next char %c\n", *input, *(input + 1));
		printf("read buffer %s, now token increased to %d\n", *buffer, token_count);
		end_token(buffer, head, state, token_count);
	}

	return (input);
}

// parsing, seperates the args by spaces or operators
// reads everything inside quote including spaces and operators
t_token	**parse(t_token **head, char *str)
{
	t_token_state	state;
	char			*buffer;

	state = GENERAL;
	buffer = NULL;
	while (*str)
	{
		str = change_state(&state, str - 1, &buffer, head);
		if (state == GENERAL && (*str != '\'' && *str != '"'))
			str = handle_state_general(&buffer, head, state, str);
		else if ((state == DQUOTE && *str != '"') || (state == QUOTE && *str != '\''))
			buffer = add_to_buffer(&buffer, *str);
		str++;
	}
	if (buffer)
		end_token(&buffer, head, state, last_token_count(head) + 1);
	if (state != GENERAL)
		display_error("Unclosed quote", head);
	return (head);
}

// 1
// 'echo'
// ADD
// ' ' oder || -> STATE CHANGE works for operators but not spaces cause spaces are not added!!!!
// INCREMENT
// 2
// ADD
// "hello" -> STATE CHANGE
// ADD

// need to check BEFORE STATE change if prev was a SPACE, if yes, increase the count!
