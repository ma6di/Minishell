//NORM OK
#include "../includes/minishell.h"

char	*get_env_name(const char *src)
{
	size_t	i;
	char	*dest;

	i = 0;
	dest = malloc(BUFF_SIZE);
	if (!dest)
		return (NULL);
	while (src[i] && src[i] != '=' && i < BUFF_SIZE - 1)
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

static int	is_valid_env(const char *arg)
{
	const char	*equal_pos;
	size_t		name_len;
	size_t		i;

	if (!arg)
		return (0);
	equal_pos = strchr(arg, '=');
	if (equal_pos)
		name_len = (size_t)(equal_pos - arg);
	else
		name_len = strlen(arg);
	if (!ft_isalpha(arg[0]) && arg[0] != '_')
		return (0);
	i = 1;
	while (i < name_len)
	{
		if (!(ft_isalnum(arg[i]) || arg[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

static int	print_error(int error, const char *arg)
{
	int	i;

	i = 0;
	if (error == -1)
		ft_putstr_fd("export: not valid in this context: ", STDERR_FILENO);
	else if (error == 0 || error == -3)
		ft_putstr_fd("export: not a valid identifier: ", STDERR_FILENO);
	while (arg[i] && (arg[i] != '=' || error == -3))
	{
		write(STDERR_FILENO, &arg[i], 1);
		i++;
	}
	write(STDERR_FILENO, "\n", 1);
	return (-1);
}

static void	ft_export_helper(char **args, int error_ret, t_main *main)
{
	int	i;

	i = 1;
	while (args[i])
	{
		if (!strchr(args[i], '='))
			break ;
		error_ret = is_valid_env(args[i]);
		if (args[i][0] == '=')
			error_ret = -3;
		if (error_ret <= 0)
		{
			print_error(error_ret, args[i]);
			continue ;
		}
		if (!is_in_env(main->env_vars, args[i]))
		{
			if (error_ret == 1)
				env_add(&main->env_vars, args[i]);
		}
		i++;
	}
}

int	ft_export(char **args, t_main *main)
{
	int	error_ret;
	int	i;

	i = 0;
	error_ret = 0;
	if (!args[1])
	{
		while (main->env_vars && main->env_vars[i])
		{
			if (main->env_vars[i] && main->env_vars[i][0] != '\0')
				ft_putendl_fd(main->env_vars[i], 1);
			i++;
		}
		return (SUCCESS);
	}
	ft_export_helper(args, error_ret, main);
	return (SUCCESS);
}
