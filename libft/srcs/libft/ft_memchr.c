/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrauh <nrauh@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 13:42:15 by nrauh             #+#    #+#             */
/*   Updated: 2024/06/27 13:53:47 by nrauh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*p1;
	unsigned char	ch;

	p1 = (unsigned char *) s;
	ch = (unsigned char) c;
	if (n == 0)
		return ((void *) NULL);
	while (*p1 != ch && n - 1 > 0)
	{
		p1++;
		n--;
	}
	if (*p1 == ch)
		return ((unsigned char *) p1);
	return ((void *) NULL);
}
