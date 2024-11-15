/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrauh <nrauh@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 18:10:05 by nrauh             #+#    #+#             */
/*   Updated: 2024/11/15 17:35:40 by nrauh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
	*head = NULL;
}

void	free_commands(t_command **head)
{
	t_command	*curr;
	t_command	*next;
	int			i;

	curr = *head;
	while (curr)
	{
		i = 0;
		next = curr->next;
		free(curr->command);
		while (curr->args && curr->args[i])
			free(curr->args[i++]);
		free(curr->args);
		free(curr->heredoc_delimiter);
		free(curr->io_fds->infile);
		free(curr->io_fds->outfile);
		free(curr->io_fds->append_outfile);
		free(curr->io_fds);
		free(curr);
		curr = next;
	}
	*head = NULL;
}

void	free_two_dim(char **env_keys)
{
	int	i;

	i = 0;
	while (env_keys[i])
	{
		//printf("freeing array %s at %p\n", env_keys[i], env_keys[i]);
		free(env_keys[i]);
		i++;
	}
	free(env_keys);
}

void	free_three_dim(char ***envp_key_val) // [["HOME", "path"], ["USER", "username"]]
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (envp_key_val[i])
		i++;
	//printf("there ARE i %d elements\n", i);
	i = 0;
	while (envp_key_val[i]) // 0 ["HOME", "path"] AND 1 ["USER", "username"]
	{
		while (envp_key_val[i][j]) // 0 "HOME" 1 "path"
		{
			//printf("freeing %s | %p\n", envp_key_val[i][j], envp_key_val[i][j]);
			free(envp_key_val[i][j]);
			j++;
		}
		j = 0;
		//printf("freeing pointer i %d %p\n", i, envp_key_val[i]);
		free(envp_key_val[i]);
		i++;
	}
	//printf("freeing pointer %p\n", envp_key_val);
	free(envp_key_val);
}
