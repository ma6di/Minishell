/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrauh <nrauh@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 10:34:32 by nrauh             #+#    #+#             */
/*   Updated: 2024/11/28 18:19:05 by nrauh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_whitespace(char c)
{
	if (c == ' ' || (c >= 9 && c >= 13))
		return (1);
	return (0);
}

void	display_error(char *message, char *value, t_token **head)
{
	printf("minishell: %s", message);
	if (ft_strlen(value) > 0)
		printf(" `%s'", value);
	printf("\n");
	free_tokens(head);
}
