/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrauh <nrauh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 10:14:14 by nrauh             #+#    #+#             */
/*   Updated: 2024/11/08 03:22:43 by nrauh            ###   ########.fr       */
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
	printf("END TOKEN count %d\n", token_count);
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
	printf("BUFFER %s\n", new_buffer);
	return (new_buffer);
}

// we input + 1 is curr, we need to access the previous char
char	*change_state(t_token_state *state, char *str, char **buffer, t_token **head, int token_count)
{
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
		if (*buffer)
			end_token(buffer, head, *state, token_count);
		*state = GENERAL;
	}
	return (str + 1);
}

char	*handle_state_general(char **buffer, t_token **head, t_token_state state, char *input, int *token_count)
{
	if (!is_delimiter(*input))
		*buffer = add_to_buffer(buffer, *input);
	if (*buffer && is_delimiter(*input))
		end_token(buffer, head, state, *token_count);
	if (*input == ' ' && !is_operator(*(input + 1)) && *(input + 1) != ' ')
		(*token_count)++;
	if (is_operator(*input))
	{
		*buffer = add_to_buffer(buffer, *input);
		if (*(input + 1) == *input)
			*buffer = add_to_buffer(buffer, *input++);
		end_token(buffer, head, state, ++(*token_count));
		if (*(input + 1) != ' ')
			(*token_count)++;
	}
	return (input);
}

// parsing, seperates the args by spaces or operators
// reads everything inside quote including spaces and operators
t_token	**parse(t_token **head, char *str)
{
	t_token_state	state;
	char			*buffer;
	int	token_count;

	state = GENERAL;
	buffer = NULL;
	token_count = 1;
	while (*str)
	{
		str = change_state(&state, str - 1, &buffer, head, token_count);
		if (state == GENERAL && (*str != '\'' && *str != '"'))
			str = handle_state_general(&buffer, head, state, str, &token_count);
		else if ((state == DQUOTE && *str != '"') || (state == QUOTE && *str != '\''))
			buffer = add_to_buffer(&buffer, *str);
		str++;
	}
	if (buffer)
		end_token(&buffer, head, state, token_count);
	if (state != GENERAL)
		display_error("Unclosed quote", head);
	return (head);
}
