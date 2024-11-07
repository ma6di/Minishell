/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrauh <nrauh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 10:14:14 by nrauh             #+#    #+#             */
/*   Updated: 2024/11/07 14:46:53 by nrauh            ###   ########.fr       */
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

void	end_token(char **buffer, t_token **head, t_token_state state)
{
	create_token(head, ft_strdup(*buffer), state);
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

void	change_state(t_token_state *state, char *input, char **buffer, t_token **head)
{
	if (*state == GENERAL && *input == '\'')
	{
		if (*buffer)
			end_token(buffer, head, *state);
		*state = QUOTE;
	}
	else if (*state == GENERAL && *input == '"')
	{
		if (*buffer)
			end_token(buffer, head, *state);
		*state = DQUOTE;
	}
	else if ((*state == QUOTE && *input == '\'')
		|| (*state == DQUOTE && *input == '"'))
	{
		if (*buffer)
			end_token(buffer, head, *state);
		*state = GENERAL;
	}
}

char	*handle_general(char **buffer, t_token **head, t_token_state state, char *input)
{
	if (!is_delimiter(*input))
		*buffer = add_to_buffer(buffer, *input);
	if (*buffer && is_delimiter(*input))
		end_token(buffer, head, state);
	if (!(*buffer) && *input == ' ')
	{
		*buffer = add_to_buffer(buffer, *input);
		end_token(buffer, head, state);
	}
	if (is_operator(*input))
	{
		*buffer = add_to_buffer(buffer, *input);
		if (*(input + 1) == *input)
			*buffer = add_to_buffer(buffer, *input++);
		end_token(buffer, head, state);
	}
	return (input);
}

// parsing, seperates the args by spaces or operators
// reads everything inside quote including spaces and operators
t_token	**parse(t_token **head, char *s)
{
	t_token_state	state;
	char			*buffer;

	state = GENERAL;
	buffer = NULL;
	while (*s)
	{
		change_state(&state, s, &buffer, head);
		if (state == GENERAL && (*s != '\'' && *s != '"'))
			s = handle_general(&buffer, head, state, s);
		else if ((state == DQUOTE && *s != '"') || (state == QUOTE && *s != '\''))
			buffer = add_to_buffer(&buffer, *s);
		s++;
	}
	if (buffer)
		end_token(&buffer, head, state);
	if (state != GENERAL)
		display_error("Unclosed quote", head);
	return (head);
}
