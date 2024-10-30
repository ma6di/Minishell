/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrauh <nrauh@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 17:13:37 by nrauh             #+#    #+#             */
/*   Updated: 2024/10/30 18:33:00 by nrauh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// Where to do the error handling?
// Input can also be cat "hello">file.txt & cat "hello" > file.txt

// COMMAND could also not be the first option, 
// we need to include binary commands (found on the system)
// structure is always like COMMAND [args...] ...
// or REDIRECT FILE COMMAND [args...]
// now when we find a token, we check the prev 2 tokens
// if these are REDIRECT and FILE we know the next is a command

// this doesnt matter right now, we do this later on

int	is_operator(char c)
{
	if (c == OP_PIPE[0] || c == OP_REDIRECT[0] || c == OP_INPUT_REDIRECT[0])
		return (0);
	return (-1);
}

int	end_of_token(char c)
{
	if (c == OP_PIPE[0] || c == OP_REDIRECT[0] 
		|| c == OP_INPUT_REDIRECT[0] || c == WHITESPACE[0])
		return (0);
	return (-1);
}

void	finish_reading_token(int *pos, char *token_buffer, 
								t_token **head, t_token_state *last_state)
{
	token_buffer[*pos] = '\0';
	printf("finish reading token %s\n", token_buffer);
	create_token(head, ft_strdup(token_buffer), *last_state);
	ft_bzero(token_buffer, *pos);
	*pos = 0;
	*last_state = STATE_GENERAL;
}

void	change_state(t_token_state *curr_state, 
						t_token_state *last_state, char c)
{
	if (*curr_state == STATE_GENERAL && c == '\'')
	{
		*curr_state = STATE_IN_QUOTE;
		*last_state = STATE_IN_QUOTE;
	}
	else if (*curr_state == STATE_GENERAL && c == '"')
	{
		*curr_state = STATE_IN_DQUOTE;
		*last_state = STATE_IN_DQUOTE;
	}
	else if ((*curr_state == STATE_IN_QUOTE && c == '\'')
		|| (*curr_state == STATE_IN_DQUOTE && c == '"'))
		*curr_state = STATE_GENERAL;
}

// tokenize the received information
void	lexer(char *input)
{
	t_token_state	curr_state;
	t_token_state	last_state;
	t_token			*head;
	char			token_buffer[100];
	int				pos;

	curr_state = STATE_GENERAL;
	last_state = STATE_GENERAL;
	pos = 0;
	head = NULL;
	while (*input)
	{
		// determine character type and change state in state machine
			// if STATE_GENERAL and not WHITESPACE[0] then add c to token_buffer
			// this creates the string that is later added to the struct
			// if STATE_GENERAL is WHITESPACE and token_buffer != 0 add_token()
		change_state(&curr_state, &last_state, *input);
		if (*(input - 1) != '\\' && (*input == '\'' || *input == '"'))
			input++;
		if (curr_state == STATE_GENERAL)
		{
			if (*input != WHITESPACE[0])
				token_buffer[pos++] = *input;
			if (pos != 0 && *input == WHITESPACE[0])
				finish_reading_token(&pos, token_buffer, &head, &last_state);
			if (pos == 0 && is_operator(*input) == 0)
			{
				token_buffer[pos++] = *input;
				input++;
				if (is_operator(*input))
				{
					token_buffer[pos++] = *input;
					input++;
				}
			}
		}
		if (curr_state == STATE_IN_DQUOTE || curr_state == STATE_IN_QUOTE)
			token_buffer[pos++] = *input;
			// if c == "'"  is encountered change state to STATE_IN_QUOTE
			// if STATE_IN_QUOTE add c to token_buffer
			// if STATE_IN_QUOTE && c == "'" change state to STATE_GENERAL 
			// and add_token()

			// if c == "'"  is encountered change state to STATE_IN_DQUOTE
			// if STATE_IN_DQUOTE add c to token_buffer
			// if STATE_IN_DQUOTE && c == "'" change state to STATE_GENERAL 
			// and add_token()
			
			// if c == "|" add_token() with type PIPE
			// if c == ">" || c == "<" add_token() with type REDIRECT
			// if c == ">>" add_token() with type APPEND
			// if c == "<<" add_token() with type HEREDOC
		// everytime after adding free token_buffer
		// DID NOT ADD YET TO CHECK FOR VARIABLES $XXX
		// DID NOT ADD YET TO CHECK FOR && or ||
		input++;
	}
	if (pos != 0)
		finish_reading_token(&pos, token_buffer, &head, &last_state);
	print_token_list(&head);
	printf("----- FREEING TOKENS -----\n");
	free_tokens(&head);
}
