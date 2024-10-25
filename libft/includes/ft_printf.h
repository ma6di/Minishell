/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrauh <nrauh@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 14:53:58 by nrauh             #+#    #+#             */
/*   Updated: 2024/07/26 15:12:47 by nrauh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <limits.h>
# include <unistd.h>
# include <errno.h>
# include "./libft.h"

int	ft_print_number(int n, char type);
int	ft_print_string(char *s);
int	ft_print_hex(unsigned long n, int len, char type);
int	ft_print_pointer(void *ptr);
int	ft_printf(const char *format, ...);
int	ft_putchar(char c);
int	ft_putstr(char *s);
int	ft_putnbr(int n);
int	ft_putnbr_ui(unsigned int n);
int	ft_count_digits(long n);

#endif
