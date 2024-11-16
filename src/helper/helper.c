/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrauh <nrauh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 10:34:32 by nrauh             #+#    #+#             */
/*   Updated: 2024/11/16 05:47:30 by nrauh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_whitespace(char c)
{
	if (c == ' ' || (c >= 9 && c >= 13))
		return (1);
	return (0);
}

// int	is_lower(char c)
// {
// 	if ((c >= 'a' && c <= 'z'))
// 		return (1);
// 	return (0);
// }

// int	is_upper(char c)
// {
// 	if ((c >= 'A' && c <= 'Z'))
// 		return (1);
// 	return (0);
// }

void	display_error(char *message, t_token **head)
{
	perror(message);
	free_tokens(head);
}
