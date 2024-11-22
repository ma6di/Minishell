//NORM OK
#include "../includes/minishell.h"
int env_add(char ***env_vars, const char *value)
{
    int     count;
    char    **old_env;
    char    **new_env_vars;
    int     i;
    count = 0;
    old_env = *env_vars;
    while (old_env && old_env[count])
        count++;
    new_env_vars = malloc(sizeof(char *) * (count + 2));
    if (!new_env_vars)
        return (-1);
    i = 0;
    while (i < count)
    {
        new_env_vars[i] = old_env[i];
        i++;
    }
    new_env_vars[count] = ft_strdup(value);
    new_env_vars[count + 1] = NULL;
    *env_vars = new_env_vars;
    return (SUCCESS);
}
int is_in_env(char **env_vars, const char *args)
{
    char    *var_name;
    char    *env_name;
    int     i;
    i = 0;
    var_name = get_env_name(args); // Extract variable name from input
    if (!var_name)
        return (-1);
    while (env_vars && env_vars[i])
    {
        env_name = get_env_name(env_vars[i]); // Extract name from env list
        if (!env_name)
        {
            free(var_name);
            return (-1);
        }
        if (ft_strncmp(var_name, env_name, ft_strlen(env_name) + ft_strlen(var_name)) == 0) // Match found
        {
            free(var_name);
            free(env_name);
            return (i); // Return index of existing variable
        }
        free(env_name);
        i++;
    }
    free(var_name);
    return (-1); // Not found
}
int exp_env_update(char **env_vars, int index, const char *value)
{
    char *new_value;
    new_value = ft_strdup(value);
    if (!new_value)
        return (-1);
    free(env_vars[index]); // Free the old variable's value
    env_vars[index] = new_value; // Replace with the new value
    return (0);
}
