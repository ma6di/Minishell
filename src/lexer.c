/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrauh <nrauh@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 17:13:37 by nrauh             #+#    #+#             */
/*   Updated: 2024/10/25 12:12:45 by nrauh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// Where to do the error handling?
// Input can also be cat "hello">file.txt & cat "hello" > file.txt

// function to add token to the end of double linked list and connect it
void	add_token(t_token **head, t_token *new_token)
{
	t_token	*curr;

	if (!(*head))
	{
		*head = new_token;
		new_token->prev = NULL;
		printf("created token %s\n", (*head)->value);
	}
	else
	{
		curr = *head;
		while (curr->next)
			curr = curr->next;
		curr->next = new_token;
		new_token->prev = curr;
		printf("created token %s\n", curr->value);
	}
	new_token->next = NULL;
}

void	print_token_list(t_token **head)
{
	t_token	*curr;
	int		i;

	i = 0;
	curr = *head;
	while (curr)
	{
		i++;
		printf("----------\n");
		printf("#%d\n%s\n", i, curr->value);
		if (curr->type == STRING)
			printf("String\n");
		else if (curr->type == COMMAND)
			printf("Command\n");
		else if (curr->type == PIPE)
			printf("Pipe\n");
		else if (curr->type == REDIRECT)
			printf("Redirect\n");
		else if (curr->type == FILENAME)
			printf("Filename\n");
		printf("----------\n");
		printf(" <-    -> \n");
		curr = curr->next;
	}
}

// frees the list of tokens
void	free_tokens(t_token **head)
{
	t_token	*curr;
	t_token	*next;

	curr = *head;
	while (curr)
	{
		next = curr->next;
		free(curr->value);
		curr->value = NULL;
		free(curr);
		curr = next;
	}
	head = NULL;
}

// COMMAND could also not be the first option, 
// we need to include binary commands (found on the system)
// structure is always like COMMAND [args...] ...
// or REDIRECT FILE COMMAND [args...]
// now when we find a token, we check the prev 2 tokens
// if these are REDIRECT and FILE we know the next is a command
t_TokenType	get_token_type(char *value)
{
	if (ft_strncmp(value, "echo", 4) == 0
		|| ft_strncmp(value, "cd", 2) == 0
		|| ft_strncmp(value, "pwd", 3) == 0
		|| ft_strncmp(value, "export", 6) == 0
		|| ft_strncmp(value, "unset", 5) == 0
		|| ft_strncmp(value, "env", 3) == 0
		|| ft_strncmp(value, "exit", 4) == 0)
		return (COMMAND);
	else if (ft_strnstr(value, ".", ft_strlen(value)))
		return (FILENAME);
	else if (ft_strncmp(value, ">", 1) == 0)
		return (REDIRECT);
	else if (ft_strncmp(value, ">>", 2) == 0)
		return (APPEND);
	else if (ft_strncmp(value, "<", 1) == 0)
		return (INPUT_REDIRECT);
	else if (ft_strncmp(value, "<<", 2) == 0)
		return (HEREDOC);
	else if (ft_strncmp(value, "|", 1) == 0)
		return (PIPE);
	else
		return (ARGUMENT);
}

void	create_token(t_token **head, char *value)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	new_token->value = value;
	new_token->type = get_token_type(value);
	add_token(head, new_token);
}

int is_operator(char c)
{
	if (c == OP_PIPE[0] || c == OP_REDIRECT[0] || c == OP_INPUT_REDIRECT[0])
		return (0);
	return (-1);
}

// tokenize the received information
void	lexer(char *input)
{
	t_TokenState	curr_state;
	t_token			*head;
	// either with a fixed HUUUUGEEEE token_buffer or a dymanic one...
	// for now using token_buffer[] fixed
	char	token_buffer[100];
	int		pos;
	int		i;
	
	// loop through the input
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
		if (curr_state == STATE_GENERAL)
		{
			printf("IN_GENERAL\n");
			if (input[i] == '"')
			{
				curr_state = STATE_IN_DQUOTE;
				i++;
				continue ;
			}
			if (input[i] == '\'')
			{
				curr_state = STATE_IN_QUOTE;
				i++;
				continue ;
			}
			printf("char %c\n", input[i]);
			// adds chars to the token buffer until WHITESPACE or Command is encountered
			if (input[i] != WHITESPACE[0])
				token_buffer[pos++] = input[i];
			// if it encounteres WHITESPACE or a COMMAND > >> < << | the token is set
			if (pos != 0 && input[i] == WHITESPACE[0])
			{
				printf("in create token loop\n");
				// same code as below create fn to reset buffer and create token
				token_buffer[pos] = '\0';
				create_token(&head, ft_strdup(token_buffer));
				ft_bzero(token_buffer, pos);
				pos = 0;
			}
			if (pos == 0 && is_operator(input[i]) == 0)
			{
				printf("in operator loop\n");
				printf("%c\n", input[i]);
				token_buffer[pos++] = input[i];
				i++;
				if (is_operator(input[i]))
				{
					token_buffer[pos++] = input[i];
					i++;
				}
			}
		}
		if (curr_state == STATE_IN_DQUOTE)
		{
			printf("IN_D_QUOTES\n");
			if (input[i] != '"')
				token_buffer[pos++] = input[i];
			if (input[i] == '"')
				curr_state = STATE_GENERAL;
		}
		if (curr_state == STATE_IN_QUOTE)
		{
			printf("IN_QUOTES\n");
			if (input[i] != '\'')
				token_buffer[pos++] = input[i];
			if (input[i] == '\'')
				curr_state = STATE_GENERAL;
		}
		if (curr_state == STATE_OPERATOR)
		{
			printf("IN_OPERATOR\n");
			if (is_operator(input[i]) == -1)
				token_buffer[pos++] = input[i];
			if (is_operator(input[i]) == 0)
				curr_state = STATE_GENERAL;
		}
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
	{
		token_buffer[pos] = '\0';
		create_token(&head, ft_strdup(token_buffer));
		ft_bzero(token_buffer, pos);
		pos = 0;
	}
	print_token_list(&head);
	printf("----- FREEING TOKENS -----\n");
	free_tokens(&head);
}
