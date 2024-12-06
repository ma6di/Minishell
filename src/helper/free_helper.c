/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrauh <nrauh@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 18:10:05 by nrauh             #+#    #+#             */
/*   Updated: 2024/12/05 14:22:42 by nrauh            ###   ########.fr       */
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

void free_commands(t_command **head) {
    t_command *curr;
    t_command *next;
    int i;

    if (!head || !*head)
        return;

    curr = *head;
    while (curr) {
        next = curr->next; // Store the next node

        // Free command string
        free(curr->command);
        curr->command = NULL;

        // Free arguments
        if (curr->args) {
            for (i = 0; curr->args[i]; i++) {
                free(curr->args[i]);
                curr->args[i] = NULL;
            }
            free(curr->args);
            curr->args = NULL;
        }

        // Free heredocs
        if (curr->heredocs) {
            for (i = 0; curr->heredocs[i]; i++) {
                free(curr->heredocs[i]->delimiter);
                curr->heredocs[i]->delimiter = NULL;
                free(curr->heredocs[i]);
                curr->heredocs[i] = NULL;
            }
            free(curr->heredocs);
            curr->heredocs = NULL;
        }

        // Free operators
        if (curr->operators) {
            for (i = 0; curr->operators[i]; i++) {
                free(curr->operators[i]->filename);
                curr->operators[i]->filename = NULL;
                free(curr->operators[i]);
                curr->operators[i] = NULL;
            }
            free(curr->operators);
            curr->operators = NULL;
        }

        // Free pipe file descriptors
        free(curr->pipe_fd);
        curr->pipe_fd = NULL;

        // Free IO file descriptors
        if (curr->io_fds) {
            free(curr->io_fds->infile);
            free(curr->io_fds->outfile);
            free(curr->io_fds->append_outfile);
            free(curr->io_fds);
            curr->io_fds = NULL;
        }

        // Free the current node and move to the next
        free(curr);
        curr = next;
    }

    // Set the head to NULL to indicate the list is empty
    *head = NULL;
}


void	free_command_child(t_command **cmd)
{
		int			i;
		int			j;

		i = 0;
		free((*cmd)->command);
		while ((*cmd)->args && (*cmd)->args[i])
		{
			//printf("freeing arg %p - %s\n", (*cmd)->args[i], (*cmd)->args[i]);
			free((*cmd)->args[i]);
			i++;
		}
		j = 0;
		while ((*cmd)->heredocs && (*cmd)->heredocs[j])
		{
			//printf("freeing heredoc %p - %s\n", (*cmd)->heredocs[j]->delimiter, (*cmd)->heredocs[j]->delimiter);
			free((*cmd)->heredocs[j]->delimiter);
			free((*cmd)->heredocs[j]);
			j++;
		}
		free((*cmd)->heredocs);
		j = 0;
		while ((*cmd)->operators && (*cmd)->operators[j])
		{
			//printf("freeing heredoc %p - %s\n", (*cmd)->operators[j]->delimiter, (*cmd)->operators[j]->delimiter);
			free((*cmd)->operators[j]->filename);
			free((*cmd)->operators[j]);
			j++;
		}
		free((*cmd)->operators);
		free((*cmd)->pipe_fd);
		(*cmd)->pipe_fd = NULL;
		//printf("freeing args %p\n", (*cmd)->args);
		free((*cmd)->args);
		free((*cmd)->heredocs);
		(*cmd)->heredocs = NULL;
		free((*cmd)->io_fds->infile);
		free((*cmd)->io_fds->outfile);
		free((*cmd)->io_fds->append_outfile);
		free((*cmd)->io_fds);
		(*cmd)->main = NULL;
		//printf("freeing command %p\n", (*cmd));
}

void free_main(t_main *main) {
    int i;

    if (!main)
        return; // Guard clause to avoid freeing a NULL pointer

    // Free environment variables
    if (main->env_vars) {
        for (i = 0; main->env_vars[i]; i++) {
            free(main->env_vars[i]);
            main->env_vars[i] = NULL;
        }
        free(main->env_vars);
        main->env_vars = NULL;
    }

    // Free the main structure
    free(main);
}


void	free_two_dim(char **env_keys)
{
	int	i;

	i = 0;
	while (env_keys && env_keys[i])
	{
		//printf("freeing array %s at %p\n", env_keys[i], env_keys[i]);
		free(env_keys[i]);
		env_keys[i] = NULL;
		i++;
	}
	free(env_keys);
	env_keys = NULL;
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

void	free_heredoc(t_command *cmd)
{
	int	j;

	j = 0;
	while (cmd->heredocs && cmd->heredocs[j])
		free(cmd->heredocs[j++]);
	free(cmd->heredocs);
	free_commands(&cmd);
}
