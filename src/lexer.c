/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrauh <nrauh@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 17:13:37 by nrauh             #+#    #+#             */
/*   Updated: 2024/11/14 15:29:58 by nrauh            ###   ########.fr       */
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

void	lexer(char *input, char **envp)
{
	t_token			*first;
	t_token			**head_t;
	t_command		**head_c;

	// why do i need *first???
	(void ) envp;
	first = NULL;
	head_t = &first;
	head_t = parse(head_t, input);
	head_t = expand(head_t, envp);
	head_t = join_token(head_t);
	head_t = assign_types(head_t);
	//head = check_validity(head);
	head_c = create_commands(head_t);
	if (*head_c)
	{
		print_cmd_list(head_c);
		printf("----- FREEING COMMANDS -----\n");
		free_commands(head_c);
	}
	/*if (*head_t)
	{
		print_token_list(head_t);
		printf("----- FREEING TOKENS -----\n");
		free_tokens(head_t);
	}*/
}
