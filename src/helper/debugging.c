/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debugging.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrauh <nrauh@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 18:09:29 by nrauh             #+#    #+#             */
/*   Updated: 2024/11/27 18:24:09 by nrauh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
		printf("state %d\n", curr->state);
		if (curr->type == APPEND)
			printf("APPEND\n");
		else if (curr->type == COMMAND)
			printf("COMMAND\n");
		else if (curr->type == INPUT_REDIRECT)
			printf("INPUT_REDIRECT\n");
		else if (curr->type == ENV_VAR)
			printf("ENV_VAR\n");
		else if (curr->type == PIPE)
			printf("PIPE\n");
		else if (curr->type == LOGICAL_OR)
			printf("LOGICAL_OR\n");
		else if (curr->type == REDIRECT)
			printf("REDIRECT\n");
		else if (curr->type == APPENDFILE)
			printf("APPENDFILE\n");
		else if (curr->type == INFILE)
			printf("INFILE\n");
		else if (curr->type == OUTFILE)
			printf("OUTFILE\n");
		else if (curr->type == ARGUMENT)
			printf("ARGUMENT\n");
		else if (curr->type == HEREDOC)
			printf("HEREDOC\n");
		else if (curr->type == HEREDOC_DELIMITER)
			printf("HEREDOC_DELIMITER\n");
		printf("value length: %zu\n", ft_strlen(curr->value));
		printf("----------\n");
		printf(" <-    -> \n");
		curr = curr->next;
	}
}

void	print_cmd_list(t_command **head)
{
	t_command	*curr;
	int			i;
	int			j;
	int			k;

	i = 0;
	curr = *head;
	while (curr)
	{
		i++;
		printf("------------------------------\n");
		printf("#%d\nCommand: %s\n", i, curr->command);
		if (curr->args)
		{
			j = 0;
			printf("Arguments: [");
			while (curr->args[j])
			{
				if (j != 0)
					printf(", ");
				printf("%s", curr->args[j++]);
			}
			printf("]\n");
		}
		printf("HEREDOC -----\n");
		if (curr->heredocs)
		{
			k = 0;
			printf("Heredocs:\n");
			while (curr->heredocs[k])
			{
				if (k != 0)
					printf(",\n");
				printf("\t{ Delimiter: %s, ", curr->heredocs[k]->delimiter);
				printf("Should expand: %d }", curr->heredocs[k]->should_expand);
				k++;
			}
			printf("\n");
		}
		printf("END HEREDOC\n");
		printf("OPERATORS -----\n");
		if (curr->operators)
		{
			k = 0;
			printf("Operators:\n");
			while (curr->operators[k])
			{
				if (k != 0)
					printf(",\n");
				printf("\t{ Filename: %s, ", curr->operators[k]->filename);
				printf("Type: %d }", curr->operators[k]->type);
				k++;
			}
			printf("\n");
		}
		printf("END OPERATORS\n");
		printf("Has Pipe: %d\n", curr->has_pipe);
		printf("PID: %d\n", curr->pid);
		printf("FDS -----\n");
		printf("Infile: %s\n", curr->io_fds->infile);
		printf("Outfile: %s\n", curr->io_fds->outfile);
		printf("Append outfile: %s\n", curr->io_fds->append_outfile);
		printf("Has Heredoc: %d\n", curr->io_fds->has_heredoc);
		printf("Heredoc fork permit: %d\n", curr->main->heredoc_fork_permit);
		printf("Main %p\n", curr->main);
		printf("------------------------------\n");
		printf(" <-    -> \n");
		curr = curr->next;
	}
}

void	print_keys(char **env_keys)
{
	int	i;

	i = 0;
	while (env_keys[i])
	{
		printf("i: %i - %s\n", i, env_keys[i]);
		i++;
	}
	printf("---------\n");
}

void	print_key_val(char ***envp_key_val)
{
	int	i;

	i = 0;
	while (envp_key_val[i])
	{
		printf("pointer %p i %d \n", envp_key_val[i], i);
		printf("key: %s | %p\nvalue: %s | %p\n",
			envp_key_val[i][0], envp_key_val[i][0],
			envp_key_val[i][1], envp_key_val[i][1]);
		printf("----------\n");
		i++;
	}
}
