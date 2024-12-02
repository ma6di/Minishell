//NORM OK
#include "../includes/minishell.h"

char	*heredoc_get_value(const char *var_name, char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], var_name, ft_strlen(var_name)) == 0 && \
			envp[i][ft_strlen(var_name)] == '=')
		{
			return (ft_strdup(envp[i] + ft_strlen(var_name) + 1));
		}
		i++;
	}
	return (ft_strdup(""));
}

static char	*init_result(char *line)
{
	char	*res;

	res = malloc(strlen(line) + 1);
	if (!res)
		ft_fprintf("malloc failed");
	else
		res[0] = '\0';
	return (res);
}

void	append_text(char **res, char *text, size_t len)
{
	size_t	cur_len;
	char	*new_res;

	if (!res || !*res || !text)
		return ;
	cur_len = ft_strlen(*res);
	new_res = malloc(cur_len + len + 1);
	if (!new_res)
	{
		ft_fprintf("malloc failed");
		return ;
	}
	ft_strlcpy(new_res, *res, cur_len + 1);
	ft_strlcat(new_res, text, cur_len + len + 1);
	free(*res);
	*res = new_res;
}

char	*expand_variables_in_line(char *line, char **envp, t_main *main)
{
	char	*res;
	char	*start;

	res = init_result(line);
	if (!res)
		return (NULL);
	while (*line)
	{
		start = ft_strchr(line, '$');
		if (!start)
			break ;
		append_text(&res, line, start - line);
		line = process_variable(&res, start + 1, envp, main);
		if (!line)
			return (free(res), NULL);
	}
	append_text(&res, line, ft_strlen(line));
	return (res);
}
