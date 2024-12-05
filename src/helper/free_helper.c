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

void	free_commands(t_command **head)
{
	t_command	*curr;
	t_command	*next;
	int			i;
	int			j;

	curr = *head;
	while (curr)
	{
		i = 0;
		next = curr->next;
		free(curr->command);
		while (curr->args && curr->args[i])
		{
			//printf("freeing arg %p - %s\n", curr->args[i], curr->args[i]);
			free(curr->args[i]);
			i++;
		}
		free(curr->args);
		j = 0;
		while (curr->heredocs && curr->heredocs[j])
		{
			//printf("freeing heredoc %p - %s\n", curr->heredocs[j]->delimiter, curr->heredocs[j]->delimiter);
			free(curr->heredocs[j]->delimiter);
			free(curr->heredocs[j]);
			curr->heredocs[j] = NULL;
			j++;
		}
		free(curr->heredocs);
		curr->heredocs = NULL;
		j = 0;
		while (curr->operators && curr->operators[j])
		{
			//printf("freeing heredoc %p - %s\n", curr->operators[j]->delimiter, curr->operators[j]->delimiter);
			free(curr->operators[j]->filename);
			free(curr->operators[j]);
			curr->operators[j] = NULL;
			j++;
		}
		free(curr->operators);
		curr->operators = NULL;
		free(curr->pipe_fd);
		curr->pipe_fd = NULL;
		//printf("freeing args %p\n", curr->args);
		//free(curr->heredocs);
		free(curr->io_fds->infile);
		free(curr->io_fds->outfile);
		free(curr->io_fds->append_outfile);
		free(curr->io_fds);
		//curr->main = NULL;
		//printf("freeing command %p\n", curr);
		free(curr);
		curr = next;
	}
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

void	free_main(t_main *main)
{
	int	i;

	i = 0;
	while (main->env_vars && main->env_vars[i])
	{
		free(main->env_vars[i]);
		main->env_vars[i] = NULL;
		i++;
	}
	free(main->env_vars);
	main->env_vars = NULL;
	free(main);
	main = NULL;
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
