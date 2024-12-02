//NROM OK
#include "../includes/minishell.h"

int	ft_pwd(void)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, PATH_MAX))
	{
		ft_putendl_fd(cwd, 1);
		return (SUCCESS);
	}
	else
	{
		ft_fprintf("minishell: pwd: %s\n", strerror(errno));
		return (CD_ERROR);
	}
}
