/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrauh <nrauh@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 14:38:46 by nrauh             #+#    #+#             */
/*   Updated: 2024/06/27 13:55:30 by nrauh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_printf.h"

int	ft_putnbr(int n)
{
	char	c;

	if (n == INT_MIN)
		return (ft_putstr("-2147483648"));
	if (n < 0)
	{
		if (ft_putchar('-') == -1)
			return (-1);
		n = -n;
	}
	if (n > 9)
	{
		if (ft_putnbr(n / 10) == -1)
			return (-1);
	}
	c = (n % 10) + '0';
	return (ft_putchar(c));
}
