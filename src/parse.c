/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrauh <nrauh@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 10:14:14 by nrauh             #+#    #+#             */
/*   Updated: 2024/11/21 14:37:04 by nrauh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// int	is_operator(char c)
// {
// 	if (c == OP_PIPE[0] || c == OP_REDIRECT[0] || c == OP_INPUT_REDIRECT[0])
// 		return (1);
// 	return (0);
// }

// // here all whitespace check should be added, including \t, \n etc.
// int	is_delimiter(char c)
// {
// 	if (c == OP_PIPE[0] || c == OP_REDIRECT[0]
// 		|| c == OP_INPUT_REDIRECT[0] || c == WHITESPACE[0])
// 		return (1);
// 	return (0);
// }

// void	end_token(char **buffer, t_token **head, t_state state)
// {
// 	create_token(head, ft_strdup(*buffer), state);
// 	free(*buffer);
// 	*buffer = NULL;
// }

// void	add_delimiter(t_token **head)
// {
// 	t_state	state;

// 	state = GENERAL;
// 	create_token(head, ft_strdup(" "), state);
// }

char	*handle_operator(char **buff, t_token **head, t_state state, char *str)
{
	if (is_operator_char(*str))
	{
		*buff = add_to_buffer(buff, *str);
		if (*(str + 1) == *str)
			*buff = add_to_buffer(buff, *str++);
		end_token(buff, head, state);
		while (*(str) && *(str + 1) == ' ')
			str++;
	}
	return (str);
}

// void	handle_quotes(t_token **head, char *str, t_state *state, char **buff)
// {
// 	if (*str == '\'')
// 	{
// 		if (*buff)
// 			end_token(buff, head, *state);
// 		*state = QUOTE;
// 	}
// 	else if (*str == '"')
// 	{
// 		if (*buff)
// 			end_token(buff, head, *state);
// 		*state = DQUOTE;
// 	}
// }

char	*handle_space(t_token **head, char *str)
{
	if (*str == ' ')
	{
		while (*(str) && *(str + 1) == ' ')
			str++;
		if (!is_operator_char(*(str + 1)))
			create_token(head, ft_strdup(" "), GENERAL);
	}
	return (str);
}

char	*handle_state_g(char **buff, t_token **head, t_state *state, char *str)
{
	if (*buff && (*str == '\'' || *str == '"'))
		end_token(buff, head, *state);
	if (*str == '\'')
		*state = QUOTE;
	else if (*str == '"')
		*state = DQUOTE;
	if (*str != '\'' && *str != '"')
	{
		if (!is_delimiter(*str))
			*buff = add_to_buffer(buff, *str);
		if (*buff && (is_delimiter(*str) || *(str + 1) == '$'))
			end_token(buff, head, *state);
		str = handle_space(head, str);
		str = handle_operator(buff, head, *state, str);
	}
	return (str);
}

char	*handle_state_dq(char **buff, t_token **head, t_state *state, char *str)
{
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

t_token	**parse(t_token **head, char *str)
{
	t_state	state;
	char	*buff;

	state = GENERAL;
	buff = NULL;
	while (*str)
	{
		if (state == GENERAL)
			str = handle_state_g(&buff, head, &state, str);
		else if (state == DQUOTE)
			str = handle_state_dq(&buff, head, &state, str);
		else if (state == QUOTE)
			str = handle_state_q(&buff, head, &state, str);
		str++;
	}
	if (buff)
		end_token(&buff, head, state);
	if (state != GENERAL)
		display_error("Unclosed quote", head);
	return (head);
}
