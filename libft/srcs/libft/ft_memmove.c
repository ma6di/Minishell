/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrauh <nrauh@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 16:49:27 by nrauh             #+#    #+#             */
/*   Updated: 2024/06/27 13:53:47 by nrauh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char		*d;
	const unsigned char	*s;
	size_t				i;

	if ((!dest && !src) || dest == src)
		return (dest);
	d = dest;
	s = src;
	i = 0;
	if (d > s)
	{
		while (n--)
			((char *) d)[n] = ((char *) s)[n];
	}
	else
	{
		while (i < n)
		{
			((char *) d)[i] = ((char *) s)[i];
			i++;
		}
	}
	return (dest);
}
