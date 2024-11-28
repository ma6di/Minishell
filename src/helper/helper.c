/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrauh <nrauh@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 10:34:32 by nrauh             #+#    #+#             */
/*   Updated: 2024/11/28 16:38:48 by nrauh            ###   ########.fr       */
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

void	display_error(char *message, char *value, t_token **head)
{
	printf("minishell: %s", message);
	if (ft_strlen(value) > 0)
		printf(" `%s'", value);
	printf("\n");
	free_tokens(head);
}
