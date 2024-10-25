/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_number.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrauh <nrauh@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 10:45:27 by nrauh             #+#    #+#             */
/*   Updated: 2024/06/27 13:55:30 by nrauh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_printf.h"

int	ft_print_number(int n, char type)
{
	unsigned int	un;

	if (type == 'u')
	{
		un = n;
		if (ft_putnbr_ui(un) == -1)
			return (-1);
		return (ft_count_digits(un));
	}
	if (ft_putnbr(n) == -1)
		return (-1);
	return (ft_count_digits(n));
}
