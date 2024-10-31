/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrauh <nrauh@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 10:14:14 by nrauh             #+#    #+#             */
/*   Updated: 2024/10/31 16:58:02 by nrauh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_operator(char c)
{
	if (c == OP_PIPE[0] || c == OP_REDIRECT[0] || c == OP_INPUT_REDIRECT[0])
		return (0);
	return (-1);
}

int	is_delimiter(char c)
{
	if (c == OP_PIPE[0] || c == OP_REDIRECT[0] 
		|| c == OP_INPUT_REDIRECT[0] || c == WHITESPACE[0])
		return (0);
	return (-1);
}

void	end_token(char **buffer, 
					t_token **head, t_token_state *last_state)
{
	create_token(head, ft_strdup(*buffer), *last_state);
	free(*buffer);
	*buffer = NULL;
	*last_state = STATE_GENERAL;
}

int	is_escaped(char *str)
{
	if (*str == '\\')
		return (0);
	return (-1);
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
	else if ((*curr_state == STATE_QUOTE && *str == '\'' && !(*(str - 1) == '\\'))
		|| (*curr_state == STATE_DQUOTE && *str == '"' && !(*(str - 1) == '\\')))
		*curr_state = STATE_GENERAL;
}

char	*add_to_buffer(char **buffer, char c)
{
	size_t	len;
	int		i;
	char	*new_buffer;
	char	*tmp;

	if (!(*buffer))
		*buffer = strdup("");
	i = 0;
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
		//if (*(input - 1) != '\\' && (*input == '\'' || *input == '"'))
		//	input++;
		// " or ' is never read when in STATE_GENERAL
		if (curr_state == STATE_GENERAL && *input != '\'' && *input != '"')
		{
			if (is_delimiter(*input) == -1)
			{
				printf("add %c in GENERAL\n", *input);
				buffer = add_to_buffer(&buffer, *input);
			}
			if (buffer && is_delimiter(*input) == 0)
				end_token(&buffer, head, &last_state);
			if (is_operator(*input) == 0)
			{
				buffer = add_to_buffer(&buffer, *input);
				if (*(input + 1) == *input)
					buffer = add_to_buffer(&buffer, *input++);
				end_token(&buffer, head, &last_state);
			}
		}
		else if (curr_state == STATE_DQUOTE)
			//&& (*input == '"' && is_escaped(input - 1) == 0))
			//	|| (*input == '\'')))
			{
				if (*input != '"' || (*input == '"' && *(input - 1) == '\\'))
				{
					printf("add %c in DQUOTE\n", *input);
					buffer = add_to_buffer(&buffer, *input);
				}
			}
		else if (curr_state == STATE_QUOTE && *input != '\'')
			//&& (*input == '"' && is_escaped(input - 1) == 0))
			{
				if (*input != '\'')
				{
					printf("add %c in QUOTE\n", *input);
					buffer = add_to_buffer(&buffer, *input);
				}
			}
		input++;
	}
	if (buffer)
		end_token(&buffer, head, &last_state);
	return (head);
}
