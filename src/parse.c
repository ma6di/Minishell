/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrauh <nrauh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 10:14:14 by nrauh             #+#    #+#             */
/*   Updated: 2024/11/08 09:39:21 by nrauh            ###   ########.fr       */
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

void	add_delimiter(t_token **head)
{
	t_token_state	state;

	state = GENERAL;
	create_token(head, ft_strdup("SPACE"), state);
}

char	*skip_whitespace(char *str)
{
	while(*(str) && *(str + 1) == ' ')
		str++;
	return (str);
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

char	*change_state(t_token_state *state, char *str, char **buffer, t_token **head)
{
	if (*state == GENERAL && *str == '\'')
	{
		if (*buffer)
			end_token(buffer, head, *state);
		*state = QUOTE;
	}
	else if (*state == GENERAL && *str == '"')
	{
		if (*buffer)
			end_token(buffer, head, *state);
		*state = DQUOTE;
	}
	else if ((*state == QUOTE && *str == '\'' && *(str + 1) == '\'')
			|| (*state == DQUOTE && *str == '"' && *(str + 1) == '"'))
		str++;
	else if ((*state == QUOTE && *str == '\'') || (*state == DQUOTE && *str == '"'))
	{
		if (*buffer)
			end_token(buffer, head, *state);
		*state = GENERAL;
	}
	return (str);
}

/*char	*handle_state_general(char **buffer, t_token **head, t_token_state state, char *input)
{
	if (!is_delimiter(*(input + 1)))
		*buffer = add_to_buffer(buffer, *(input + 1));
	if (*buffer && is_delimiter(*(input + 1)))
		end_token(buffer, head, state);
	if (*(input + 1) == ' ')
	{
		input = skip_whitespace(input);
		if (!is_operator(*(input + 2)))
			add_delimiter(head);
	}
	if (is_operator(*(input + 1)))
	{
		*buffer = add_to_buffer(buffer, *(input + 1));
		if (*(input + 2) == *(input + 1))
		{
			*buffer = add_to_buffer(buffer, *(input + 1));
			input++;
		}
		end_token(buffer, head, state);
		input = skip_whitespace(input);
	}
	return (input + 1);
}*/

char	*handle_general(char **buffer, t_token **head, t_token_state state, char *str)
{
	if (!is_delimiter(*str))
		*buffer = add_to_buffer(buffer, *str);
	if (*buffer && is_delimiter(*str))
		end_token(buffer, head, state);
	if (*str == ' ')
	{
		str = skip_whitespace(str);
		if (!is_operator(*(str + 1)))
			add_delimiter(head);
	}
	if (is_operator(*str))
	{
		*buffer = add_to_buffer(buffer, *str);
		if (*(str + 1) == *str)
			*buffer = add_to_buffer(buffer, *str++);
		end_token(buffer, head, state);
		str = skip_whitespace(str);
	}
	return (str);
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
		str = change_state(&state, str, &buffer, head);
		if (state == GENERAL && (*str != '\'' && *str != '"'))
			str = handle_general(&buffer, head, state, str);
		else if ((state == DQUOTE && *str != '"')
			|| (state == QUOTE && *str != '\''))
			buffer = add_to_buffer(&buffer, *str);
		str++;
	}
	if (buffer)
		end_token(&buffer, head, state);
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
