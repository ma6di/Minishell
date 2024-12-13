/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrauh <nrauh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 10:14:14 by nrauh             #+#    #+#             */
/*   Updated: 2024/12/11 14:13:55 by nrauh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handle_empty_str(char **buff, t_token **head, char *str, char c)
{
	if (*str == c && *(str - 1) == c)
	{
		if ((is_delimiter(*(str + 1))
				&& *(str - 2) && is_delimiter(*(str - 2)))
			|| *(str + 1) == '\0')
		{
			*buff = add_to_buffer(buff, ' ');
			end_token(buff, head, EMPTY);
		}
	}
}

char	*handle_state_dq(char **buff, t_token **head, t_state *state, char *str)
{
	handle_empty_str(buff, head, str, '"');
	if (*str == '"' && *(str + 1) == '"')
		str++;
	else if (*str == '"')
	{
		if (*buff)
			end_token(buff, head, *state);
		*state = GENERAL;
	}
	if (*str != '"')
	{
		*buff = add_to_buffer(buff, *str);
		if (*(str + 1) == '$' || *(str + 1) == ' ' || *(str + 1) == '\'')
			end_token(buff, head, *state);
	}
	return (str);
}

char	*handle_state_q(char **buff, t_token **head, t_state *state, char *str)
{
	handle_empty_str(buff, head, str, '\'');
	if (*str == '\'' && *(str + 1) == '\'')
		str++;
	else if (*str == '\'')
	{
		if (*(str + 1) == '\'')
			str++;
		if (*buff)
			end_token(buff, head, *state);
		*state = GENERAL;
	}
	if (*str != '\'')
		*buff = add_to_buffer(buff, *str);
	return (str);
}

char	*handle_variable(char **buff, t_token **head, t_state *state, char *str)
{
	while (*str && *str != '/' && *str != '\'' && *str != '"'
		&& *str != ' ' && !is_delimiter(*str))
	{
		*buff = add_to_buffer(buff, *str);
		str++;
		if (*str == '$')
			break ;
	}
	if (*buff)
		end_token(buff, head, *state);
	return (str);
}
