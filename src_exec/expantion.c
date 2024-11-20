//NORM OK
#include "../includes/minishell.h"

static char	*get_value(const char *var_name, char **envp)
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
		perror("malloc");
	else
		res[0] = '\0';
	return (res);
}

static void	append_text(char **res, char *text, size_t len)
{
	size_t	cur_len;

	cur_len = ft_strlen(*res);
	*res = realloc(*res, cur_len + len + 1);
	if (!*res)
	{
		perror("realloc");
		return ;
	}
	ft_strlcat(*res, text, cur_len + len + 1);
}

static char	*process_variable(char **res, char *start, char **envp)
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
	val = get_value(var, envp);
	if (!val)
		val = ft_strdup("");
	append_text(res, val, ft_strlen(val));
	free(val);
	return (start + len);
}

char	*expand_variables_in_line(char *line, char **envp)
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
		line = process_variable(&res, start + 1, envp);
		if (!line)
			return (free(res), NULL);
	}
	append_text(&res, line, ft_strlen(line));
	return (res);
}
