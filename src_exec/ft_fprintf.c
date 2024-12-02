#include "../includes/minishell.h"

void	ft_fputchar(int fd, char c)
{
	write(fd, &c, 1);
}

void	ft_fputstr(int fd, char *s)
{
	if (!s)
		s = "(null)";
	while (*s)
		write(fd, s++, 1);
}

void	ft_fputnbr(int fd, int n)
{
	if (n == -2147483648)
	{
		ft_fputstr(fd, "-2147483648");
		return ;
	}
	if (n < 0)
	{
		ft_fputchar(fd, '-');
		n = -n;
	}
	if (n > 9)
		ft_fputnbr(fd, n / 10);
	ft_fputchar(fd, (n % 10) + '0');
}

void	ft_fprintf(const char *format, ...)
{
	va_list	args;
	int		fd;

	fd = STDERR_FILENO;
	va_start(args, format);
	while (format && *format)
	{
		if (*format == '%' && (*(format + 1) == 'd' || *(format + 1) == 's' || \
			*(format + 1) == 'c'))
		{
			if (*(format + 1) == 'd')
				ft_fputnbr(fd, va_arg(args, int));
			else if (*(format + 1) == 's')
				ft_fputstr(fd, va_arg(args, char *));
			else if (*(format + 1) == 'c')
				ft_fputchar(fd, (char)va_arg(args, int));
			format += 2;
		}
		else
		{
			ft_fputchar(fd, *format);
			format++;
		}
	}
	va_end(args);
}
