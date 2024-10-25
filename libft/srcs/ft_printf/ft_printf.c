/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   ft_printf.c										:+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: nrauh <nrauh@student.42berlin.de>		  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2024/05/09 14:56:51 by nrauh			 #+#	#+#			 */
/*   Updated: 2024/05/10 21:11:43 by nrauh			###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "../../includes/ft_printf.h"

int	ft_format(char type, va_list args)
{
	if (type == 'c')
		return (ft_putchar((va_arg(args, int))));
	else if (type == '%')
		return (ft_putchar(type));
	else if (type == 's')
		return (ft_print_string(va_arg(args, char *)));
	else if (type == 'p')
		return (ft_print_pointer(va_arg(args, void *)));
	else if (type == 'd' || type == 'i' || type == 'u')
		return (ft_print_number(va_arg(args, int), type));
	else if (type == 'x' || type == 'X')
		return (ft_print_hex(va_arg(args, unsigned int), 1, type));
	else
		return (-1);
}

int	ft_printf(const char *format, ...)
{
	va_list	args;
	int		len;
	int		res;

	va_start(args, format);
	len = 0;
	res = 0;
	while (*format)
	{
		if (*format == '%' && ft_strchr("cspdiuxX%", *(format + 1)))
		{
			res = ft_format(*(format + 1), args);
			format += 2;
		}
		else
			res = ft_putchar(*format++);
		len += res;
		if (res == -1)
			return (-1);
	}
	va_end(args);
	return (len);
}
