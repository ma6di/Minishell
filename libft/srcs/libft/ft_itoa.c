/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrauh <nrauh@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 10:36:58 by nrauh             #+#    #+#             */
/*   Updated: 2024/06/27 13:53:47 by nrauh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"
#include <limits.h>

static int	ft_countdigits(int n)
{
	int		count;

	if (n == INT_MIN)
		return (11);
	count = 1;
	if (n < 0)
	{
		n = -n;
		count++;
	}
	while (n > 9)
	{
		n /= 10;
		count++;
	}
	return (count);
}

static char	*ft_write_negative(char *str, int *count, int *n)
{
	if (*n == INT_MIN)
	{
		str[10] = '8';
		*n /= 10;
		(*count)--;
	}
	str[0] = '-';
	*n = -(*n);
	(*count)--;
	return (str + 1);
}

char	*ft_itoa(int n)
{
	char	*a;
	char	*str;
	int		count;

	count = ft_countdigits(n);
	a = malloc((count + 1) * sizeof(char));
	if (!a)
		return (NULL);
	a[count] = '\0';
	str = a;
	if (n < 0)
		str = ft_write_negative(str, &count, &n);
	while (count--)
	{
		str[count] = (n % 10) + '0';
		n /= 10;
	}
	return (a);
}
