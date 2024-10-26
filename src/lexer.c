/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrauh <nrauh@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 17:13:37 by nrauh             #+#    #+#             */
/*   Updated: 2024/10/26 12:34:04 by nrauh            ###   ########.fr       */
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

void	finish_reading_token(int *pos, char *token_buffer, t_token **head)
{
	printf("finish reading token");
	token_buffer[*pos] = '\0';
	create_token(head, ft_strdup(token_buffer));
	ft_bzero(token_buffer, *pos);
	*pos = 0;
}

void	change_state(t_TokenState *curr_state, char c)
{
	if (*curr_state == STATE_GENERAL && c == '\'')
		*curr_state = STATE_IN_QUOTE;
	else if (*curr_state == STATE_GENERAL && c == '"')
		*curr_state = STATE_IN_DQUOTE;
	else if ((*curr_state == STATE_IN_QUOTE && c == '\'')
		|| (*curr_state == STATE_IN_DQUOTE && c == '"'))
		*curr_state = STATE_GENERAL;
}

// tokenize the received information
void	lexer(char *input)
{
	t_TokenState	curr_state;
	t_token			*head;
	char			token_buffer[100];
	int				pos;
	int				i;

	curr_state = STATE_GENERAL;
	i = 0;
	pos = 0;
	head = NULL;
	while (input[i])
	{
		// determine character type and change state in state machine
			// if STATE_GENERAL and not WHITESPACE[0] then add c to token_buffer
			// this creates the string that is later added to the struct
			// if STATE_GENERAL is WHITESPACE and token_buffer != 0 add_token()
		change_state(&curr_state, input[i]);
		if (input[i] == '\'' || input[i] == '"')
			i++;
		if (curr_state == STATE_GENERAL)
		{
			if (input[i] != WHITESPACE[0])
				token_buffer[pos++] = input[i];
			if (pos != 0 && input[i] == WHITESPACE[0])
				finish_reading_token(&pos, token_buffer, &head);
			if (pos == 0 && is_operator(input[i]) == 0)
			{
				token_buffer[pos++] = input[i];
				i++;
				if (is_operator(input[i]))
				{
					token_buffer[pos++] = input[i];
					i++;
				}
			}
		}
		if (curr_state == STATE_IN_DQUOTE || curr_state == STATE_IN_QUOTE)
			token_buffer[pos++] = input[i];
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
		i++;
	}
	if (pos != 0)
		finish_reading_token(&pos, token_buffer, &head);
	print_token_list(&head);
	printf("----- FREEING TOKENS -----\n");
	free_tokens(&head);
}
