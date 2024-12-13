/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrauh <nrauh@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 13:53:22 by nrauh             #+#    #+#             */
/*   Updated: 2024/12/13 13:55:47 by nrauh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*check_equal(t_token *curr, char *tmp)
{
	if (ft_strlen(tmp) == 0 && ft_strchr(curr->value, '='))
	{
		free(tmp);
		tmp = ft_strdup(ft_strnstr(curr->value, "=", ft_strlen(curr->value)));
	}
	return (tmp);
}
