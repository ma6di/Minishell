/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_hex.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrauh <nrauh@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 11:01:05 by nrauh             #+#    #+#             */
/*   Updated: 2024/06/27 13:55:30 by nrauh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_printf.h"

static char	ft_calc_hex(int n)
{
	if (n < 10)
		return (n + 48);
	return (n + 87);
}

int	ft_print_hex(unsigned long n, int len, char type)
{
	char	hex;

	if (n > 15)
		len = ft_print_hex(n / 16, len + 1, type);
	if (len == -1)
		return (len);
	hex = ft_calc_hex(n % 16);
	if (type == 'X')
		hex = ft_toupper(hex);
	if (ft_putchar(hex) == -1)
		return (-1);
	return (len);
}
