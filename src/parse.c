/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrauh <nrauh@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 10:14:14 by nrauh             #+#    #+#             */
/*   Updated: 2024/12/12 17:05:04 by nrauh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*handle_op(char **buff, t_token **head, t_state state, char *str)
{
	if (is_operator_char(*str))
	{
		*buff = add_to_buffer(buff, *str);
		if (*(str + 1) == *str)
			*buff = add_to_buffer(buff, *(++str));
		if (ft_strncmp(*buff, "<<", ft_strlen(*buff) + 2) == 0
			&& *(str + 1) == OP_INPUT_REDIRECT[0])
			*buff = add_to_buffer(buff, *(++str));
		else if (ft_strncmp(*buff, "<", ft_strlen(*buff) + 1) == 0
			&& *(str + 1) == OP_REDIRECT[0])
			*buff = add_to_buffer(buff, *(++str));
		end_token(buff, head, state);
		while (*(str) && *(str + 1) == ' ')
			str++;
	}
	return (str);
}

static char	*handle_space(t_token **head, char *str)
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
	if (*str == '#')
		return (NULL);
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
		str = handle_op(buff, head, *state, str);
	}
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
		if (*str == '$' && state != QUOTE)
			str = handle_variable(&buff, head, &state, str);
		if (*str && state == GENERAL)
			str = handle_state_g(&buff, head, &state, str);
		else if (*str && state == DQUOTE)
			str = handle_state_dq(&buff, head, &state, str);
		else if (*str && state == QUOTE)
			str = handle_state_q(&buff, head, &state, str);
		if (*str != '\0')
			str++;
	}
	end_token(&buff, head, state);
	if (state != GENERAL)
	{
		ft_fprintf("minishell: Unclosed quote.\n");
		return (free_tokens(head), NULL);
	}
	return (head);
}
