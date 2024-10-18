/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrauh <nrauh@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 17:13:37 by nrauh             #+#    #+#             */
/*   Updated: 2024/10/18 17:36:56 by nrauh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

// function to add token to the list
void	add_token(t_token value, t_token head, t_TokenType type)
{
	// create the token
	// determine the token type
	// add token to list
}

// tokenize the received information
void	create_tokens(char *input)
{
	// definition of state enum
	// enum TokenState { STATE_GENERAL, STATE_IN_QUOTE, STATE_IN_DQUOTE }
	// either with a fixed HUUUUGEEEE token_buffer or a dymanic one...
	// char	*token_buffer;
	
	// loop through the input
	// determine character type and change state in state machine
		// if STATE_GENERAL and not WHITESPACE then add c to token_buffer
		// if STATE_GENERAL is WHITESPACE and token_buffer != 0 add_token()
		
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
}
