/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrauh <nrauh@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 17:13:37 by nrauh             #+#    #+#             */
/*   Updated: 2024/10/31 00:59:17 by nrauh            ###   ########.fr       */
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

int	is_delimiter(char c)
{
	if (c == OP_PIPE[0] || c == OP_REDIRECT[0] 
		|| c == OP_INPUT_REDIRECT[0] || c == WHITESPACE[0])
		return (0);
	return (-1);
}

/*void	finish_reading_token(int *pos, char *token_buffer, 
								t_token **head, t_token_state *last_state)
{
	//token_buffer[*pos] = '\0';
	printf("finish reading token %s\n", token_buffer);
	create_token(head, ft_strdup(token_buffer), *last_state);
	ft_bzero(token_buffer, *pos);
	*pos = 0;
	*last_state = STATE_GENERAL;
	printf("RESET pos %d, RESET state %d\n", *pos, *last_state);
}*/

void	end_token(char **token_buffer, 
								t_token **head, t_token_state *last_state)
{
	create_token(head, ft_strdup(*token_buffer), *last_state);
	free(*token_buffer);
	*token_buffer = NULL;
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

t_token	**parse(t_token **head, char *input)
{
	t_token_state	curr_state;
	t_token_state	last_state;
	char			*token_buffer;

	curr_state = STATE_GENERAL;
	last_state = STATE_GENERAL;
	token_buffer = NULL;
	while (*input)
	{
		change_state(&curr_state, &last_state, *input);
		if (*(input - 1) != '\\' && (*input == '\'' || *input == '"'))
			input++;
		if (curr_state == STATE_GENERAL)
		{
			if (*input != WHITESPACE[0] && is_operator(*input) == -1)
				token_buffer = add_to_buffer(&token_buffer, *input);
			if (token_buffer && is_delimiter(*input) == 0)
				end_token(&token_buffer, head, &last_state);
			/*while (is_operator(*input) == 0)
			{
				token_buffer = add_to_buffer(&token_buffer, *input);
				input++;
			}*/
			if (is_operator(*input) == 0)
			{
				token_buffer = add_to_buffer(&token_buffer, *input);
				if (is_operator(*(input + 1)) == 0)
				{
					input++;
					token_buffer = add_to_buffer(&token_buffer, *input);
				}
				end_token(&token_buffer, head, &last_state);
			}
		}
		else if (curr_state == STATE_IN_DQUOTE || curr_state == STATE_IN_QUOTE)
			token_buffer = add_to_buffer(&token_buffer, *input);
		input++;
	}
	if (token_buffer)
		end_token(&token_buffer, head, &last_state);
	return (head);
}


void	lexer(char *input)
{
	t_token			*first;
	t_token			**head;

	// why do i need first???
	first = NULL;
	head = &first;
	head = parse(head, input);
	print_token_list(head);
	printf("----- FREEING TOKENS -----\n");
	free_tokens(head);
}

// tokenize the received information
/*void	lexer(char *input)
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
		change_state(&curr_state, &last_state, *input);
		if (*(input - 1) != '\\' && (*input == '\'' || *input == '"'))
			input++;
		if (curr_state == STATE_GENERAL)
		{
			if (*input != WHITESPACE[0] && is_operator(*input) == -1)
				// token_buffer = add_to_buffer(token_buffer, *input);
				token_buffer[pos++] = *input;
			if (pos != 0 && end_of_token(*input) == 0)
				finish_reading_token(&pos, token_buffer, &head, &last_state);
			if (pos == 0 && is_operator(*input) == 0)
			{
				token_buffer[pos++] = *input;
				if (is_operator(*(input + 1)) == 0)
				{
					input++;
					token_buffer[pos++] = *input;
				}
				finish_reading_token(&pos, token_buffer, &head, &last_state);
			}
		}
		if (curr_state == STATE_IN_DQUOTE || curr_state == STATE_IN_QUOTE)
			token_buffer[pos++] = *input;
		input++;
	}
	if (pos != 0)
		finish_reading_token(&pos, token_buffer, &head, &last_state);
	print_token_list(&head);
	printf("----- FREEING TOKENS -----\n");
	free_tokens(&head);
}*/
