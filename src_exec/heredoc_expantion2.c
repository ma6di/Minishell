#include "../includes/minishell.h"

static char	*process_exit_code(char **res, char *start, t_main *main)
{
	char	*val;

	val = ft_itoa(main->exit_code);
	if (!val)
		return (NULL);
	append_text(res, val, ft_strlen(val));
	free(val);
	return (start + 1);
}

static char	*process_regular_variable(char **res, char *start, char **envp)
{
	char	var[256];
	int		len;
	char	*val;

	ft_memset(var, 0, sizeof(var));
	len = 0;
	while (ft_isalnum(start[len]) || start[len] == '_')
	{
		var[len] = start[len];
		len++;
	}
	val = heredoc_get_value(var, envp);
	if (!val)
		val = ft_strdup("");
	append_text(res, val, ft_strlen(val));
	free(val);
	return (start + len);
}

char	*process_variable(char **res, char *start, char **envp, t_main *main)
{
	if (start[0] == '?' && !ft_isalnum(start[1]) && start[1] != '_')
		return (process_exit_code(res, start, main));
	return (process_regular_variable(res, start, envp));
}
