/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_commands.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrauh <nrauh@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 14:45:20 by nrauh             #+#    #+#             */
/*   Updated: 2024/11/14 18:09:22 by nrauh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	**add_to_args(char **old_args, char *new_arg)
{
	char	**new_args;
	int		i;

	i = 0;
	if (old_args)
	{
		while (old_args)
			i++;
	}
	printf("i is %d\n", i);
	new_args = malloc((i + 2) * sizeof(char *));
	if (!new_args)
		return (NULL); // potentially do more here???
	while (old_args && old_args[i])
	{
		new_args[i] = old_args[i];
		i++;
	}
	new_args[i] = new_arg;
	i++;
	new_args[i] = NULL;
	printf("new arg %s\n", new_args[i - 1]);
	return (new_args);
}

t_command	*create_empty_cmd(void)
{
	t_command	*cmd;

	printf("creating empty command\n");
	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->io_fds = malloc(sizeof(t_fds));
	if (!cmd->io_fds)
		return (NULL);
	cmd->command = NULL;
	cmd->args = NULL;
	cmd->heredoc_delimiter = NULL;
	cmd->pipe_fd =NULL;
	cmd->has_pipe = 0;
	cmd->pipe_created = NULL;
	cmd->pid = getpid();
	return (cmd);
}

void	add_to_list(t_command **head, t_command *new_cmd)
{
	t_command	*curr;

	if (!head)
	{
		*head = new_cmd;
		new_cmd->prev = NULL;
	}
	else
	{
		curr = *head;
		while (curr->next)
			curr = curr->next;
		curr->next = new_cmd;
		new_cmd->prev = curr;
	}
	new_cmd->next = NULL;
}

t_command	**create_commands(t_token **head_t)
{
	t_command	**head_c;
	t_command	*cmd;
	t_token		*curr;
	char		**tmp;

	head_c = NULL;
	curr = *head_t;
	cmd = create_empty_cmd();
	while (curr)
	{
		printf("////////// NEW ROUND //////////\n");
		if (curr->type == COMMAND)
		{
			cmd->command = curr->value;
			printf("cmd %s\n", cmd->command);
		}
		else if (curr->type == ARGUMENT)
		{
			printf("arg will be added %s\n", curr->value);
			tmp = add_to_args(cmd->args, curr->value);
			if (cmd->args)
				free(cmd->args);
			printf("cmd args %s\n", cmd->args[0]);
			cmd->args = tmp;
		}
		else if (curr->type == PIPE)
		{
			cmd->has_pipe = 1;
			add_to_list(head_c, cmd);
			//cmd = create_empty_cmd();
		}
		// iterate through tokens till a pipe is found
		// check what type the token has
		// count args malloc args array
		// if filename check prev to add as in our outfile
		printf("NEXT ----->\n");
		curr = curr->next;
	}
	return (head_c);
}
