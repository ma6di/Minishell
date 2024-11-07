/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrauh <nrauh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 10:14:14 by nrauh             #+#    #+#             */
/*   Updated: 2024/11/07 02:51:14 by nrauh            ###   ########.fr       */
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

void	end_token(char **buffer,
					t_token **head, t_token_state *last_state)
{
	create_token(head, ft_strdup(*buffer), *last_state);
	free(*buffer);
	*buffer = NULL;
	*last_state = STATE_GENERAL;
}

void	change_state(t_token_state *curr_state,
						t_token_state *last_state, char *str)
{
	if (*curr_state == STATE_GENERAL && *str == '\'')
	{
		*curr_state = STATE_QUOTE;
		*last_state = STATE_QUOTE;
	}
	else if (*curr_state == STATE_GENERAL && *str == '"')
	{
		*curr_state = STATE_DQUOTE;
		*last_state = STATE_DQUOTE;
	}
	else if ((*curr_state == STATE_QUOTE && *str == '\'')
		|| (*curr_state == STATE_DQUOTE && *str == '"'))
		*curr_state = STATE_GENERAL;
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

// try with operator as a state.....
t_token	**parse(t_token **head, char *input)
{
	t_token_state	curr_state;
	t_token_state	last_state;
	char			*buffer;

	curr_state = STATE_GENERAL;
	last_state = STATE_GENERAL;
	buffer = NULL;
	while (*input)
	{
		printf("1: state %d, char %c\n", curr_state, *input);
		change_state(&curr_state, &last_state, input);
		printf("2: state %d, char %c\n", curr_state, *input);
		if (curr_state == STATE_GENERAL && *input != '\'' && *input != '"')
		{
			if (!is_delimiter(*input))
			{
				printf("add %c in GENERAL\n", *input);
				buffer = add_to_buffer(&buffer, *input);
			}
			if ((buffer && is_delimiter(*input)))
				end_token(&buffer, head, &last_state);
			if (is_operator(*input))
			{
				buffer = add_to_buffer(&buffer, *input);
				if (*(input + 1) == *input)
					buffer = add_to_buffer(&buffer, *input++);
				end_token(&buffer, head, &last_state);
			}
		}
		// took out (curr_state == STATE_DQUOTE && *input != '"')
		// took out (curr_state == STATE_QUOTE && *input != '\'')
		// cause i need the dquotes later on when expanding to separate $ENV'string'
		else if (curr_state == STATE_DQUOTE || (curr_state == STATE_QUOTE && *input != '\''))
			buffer = add_to_buffer(&buffer, *input);
		input++;
	}
	if (buffer)
		end_token(&buffer, head, &last_state);
	if (curr_state != STATE_GENERAL)
	{
		perror("Unclosed quote");
		free_tokens(head);
	}
	return (head);
}
