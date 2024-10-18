/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_pointer.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrauh <nrauh@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 12:01:20 by nrauh             #+#    #+#             */
/*   Updated: 2024/06/27 13:55:30 by nrauh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_printf.h"

int	ft_print_pointer(void *ptr)
{
	unsigned long	n;
	int				len;

	n = (unsigned long) ptr;
	if (ptr == 0)
		return (ft_putstr("(nil)"));
	if (write(1, "0x", 2) == -1)
		return (-1);
	len = ft_print_hex(n, 1, 'x');
	if (len == -1)
		return (-1);
	return (len + 2);
}
