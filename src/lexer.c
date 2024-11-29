/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrauh <nrauh@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 17:13:37 by nrauh             #+#    #+#             */
/*   Updated: 2024/11/29 11:46:34 by nrauh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_command	*lexer(char *input, char **envp, t_main **main)
{
	t_token			*first_token;
	t_token			**head_t;
	t_command		*first_cmd;
	t_command		**head_c;

	first_token = NULL;
	head_t = &first_token;
	first_cmd = NULL;
	head_c = &first_cmd;
	head_t = parse(head_t, input);
	head_t = expand(head_t, envp, (*main));
	head_t = join_token(head_t);
	head_t = assign_types(head_t);
	if (head_t && *head_t)
	{
		printf("assigning types ---------- \n");
		print_token_list(head_t);
	}
	head_t = check_validity(head_t);
	head_c = create_commands(head_c, head_t, main);
	if (head_t && *head_t)
	{
		print_token_list(head_t);
		free_tokens(head_t);
	}
	if (head_c && *head_c)
	 	print_cmd_list(head_c);
	if (!head_c || !(*head_c))
		return (NULL);
	return (*head_c);
}
