/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrauh <nrauh@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 16:02:32 by nrauh             #+#    #+#             */
/*   Updated: 2024/06/27 13:53:47 by nrauh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

char	*ft_strrchr(const char *s, int c)
{
	size_t		len;
	char		*p;

	len = ft_strlen(s);
	p = (char *) s + len;
	while (*p != (char)c && len > 0)
	{
		p--;
		len--;
	}
	if (*p == (char)c)
		return (p);
	return (NULL);
}
