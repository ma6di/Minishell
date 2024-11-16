/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buffer.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrauh <nrauh@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 17:09:06 by nrauh             #+#    #+#             */
/*   Updated: 2024/11/13 17:09:39 by nrauh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*add_to_buffer(char **buffer, char c)
{
	size_t	len;
	char	*new_buffer;
	char	*tmp;

	if (!(*buffer))
		*buffer = strdup("");
	len = strlen(*buffer);
	new_buffer = malloc((len + 2) * sizeof(char));
	if (!new_buffer)
		return (free(*buffer), NULL);
	tmp = *buffer;
	while (*tmp)
		*new_buffer++ = *tmp++;
	new_buffer = new_buffer - len;
	new_buffer[len] = c;
	new_buffer[len + 1] = '\0';
	free(*buffer);
	return (new_buffer);
}
