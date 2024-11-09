/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrauh <nrauh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 10:14:14 by nrauh             #+#    #+#             */
/*   Updated: 2024/11/09 09:48:22 by nrauh            ###   ########.fr       */
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
	create_token(head, ft_strdup(" "), state);
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

char	*handle_operator(char **buffer, t_token **head, t_token_state state, char *str)
{
	if (is_operator(*str))
	{
		*buffer = add_to_buffer(buffer, *str);
		if (*(str + 1) == *str)
			*buffer = add_to_buffer(buffer, *str++);
		end_token(buffer, head, state);
		while(*(str) && *(str + 1) == ' ')
			str++;
	}
	return (str);
}

void	handle_quotes(t_token **head, char *str, t_token_state *state, char **buffer)
{
	if (*str == '\'')
	{
		if (*buffer)
			end_token(buffer, head, *state);
		*state = QUOTE;
	}
	else if (*str == '"')
	{
		if (*buffer)
			end_token(buffer, head, *state);
		*state = DQUOTE;
	}
}

char	*handle_space(t_token **head, char *str)
{
	if (*str == ' ')
	{
		while(*(str) && *(str + 1) == ' ')
			str++;
		if (!is_operator(*(str + 1)))
			add_delimiter(head);
	}
	return (str);
}

char	*handle_state_general(char **buffer, t_token **head, t_token_state *state, char *str)
{
	handle_quotes(head, str, state, buffer);
	if (*str != '\'' && *str != '"')
	{
		if (!is_delimiter(*str))
			*buffer = add_to_buffer(buffer, *str);
		if (*buffer && (is_delimiter(*str) || *(str + 1) == '$'))
			end_token(buffer, head, *state);
		str = handle_space(head, str);
		str = handle_operator(buffer, head, *state, str);
	}
	return (str);
}

char	*handle_state_dquote(char **buffer, t_token **head, t_token_state *state, char *str)
{
	if (*str == '"' && *(str + 1) == '"')
		str++;
	else if (*str == '"')
	{
		if (*buffer)
			end_token(buffer, head, *state);
		*state = GENERAL;
	}
	if (*str != '"')
	{
		*buffer = add_to_buffer(buffer, *str);
		if (*(str + 1) == '$' || *(str + 1) == ' ' || *(str + 1) == '\'')
			end_token(buffer, head, *state);
	}
	return (str);
}

char	*handle_state_quote(char **buffer, t_token **head, t_token_state *state, char *str)
{
	if (*str == '\'' && *(str + 1) == '\'')
		str++;
	else if (*str == '\'')
	{
		if (*(str + 1) == '\'')
			str++;
		if (*buffer)
			end_token(buffer, head, *state);
		*state = GENERAL;
	}
	if (*str != '\'')
		*buffer = add_to_buffer(buffer, *str);
	return (str);
}

t_token	**parse(t_token **head, char *str)
{
	t_token_state	state;
	char			*buffer;

	state = GENERAL;
	buffer = NULL;
	while (*str)
	{
		if (state == GENERAL)
			str = handle_state_general(&buffer, head, &state, str);
		else if (state == DQUOTE)
			str = handle_state_dquote(&buffer, head, &state, str);
		else if (state == QUOTE)
			str = handle_state_quote(&buffer, head, &state, str);
		str++;
	}
	if (buffer)
		end_token(&buffer, head, state);
	if (state != GENERAL)
		display_error("Unclosed quote", head);
	return (head);
}
