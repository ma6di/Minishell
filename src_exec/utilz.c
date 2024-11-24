//NORM OK
#include "../includes/minishell.h"

void	cd_print_error(const char *arg)
{
	ft_putstr_fd("cd: ", 2);
	ft_putstr_fd((char *)arg, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(strerror(errno), 2);
}

void	remove_heredoc_file(t_main *main)
{
	t_command	*cmd;

	cmd = main->command_list;
	while (cmd)
	{
		if (cmd->io_fds->has_heredoc)
			unlink("heredoc.txt");
		cmd = cmd->next;
	}
}

int	is_builtin(char *command)
{
	size_t	cmd_len;

	cmd_len = ft_strlen(command);
	if (!command)
		return (0);
	if (ft_strncmp(command, "echo", ft_strlen("echo") + cmd_len) == 0 || \
		ft_strncmp(command, "pwd", ft_strlen("pwd") + cmd_len) == 0 || \
		ft_strncmp(command, "env", ft_strlen("env") + cmd_len) == 0)
	{
		return (1);
	}
	return (0);
}

int	is_special_builtin(char *command)
{
	size_t	cmd_len;

	cmd_len = ft_strlen(command);
	if (!command)
		return (0);
	if (ft_strncmp(command, "cd", ft_strlen("cd") + cmd_len) == 0 || \
		ft_strncmp(command, "export", ft_strlen("export") + cmd_len) == 0 || \
		ft_strncmp(command, "unset", ft_strlen("unset") + cmd_len) == 0 || \
		ft_strncmp(command, "exit", ft_strlen("exit") + cmd_len) == 0)
	{
		return (1);
	}
	return (0);
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>

#define FD_PATH_MAX 256

void list_open_fds() {
    const char *fd_dir_path = "/proc/self/fd"; // Path to file descriptors in /proc
    char fd_target_path[FD_PATH_MAX];
    DIR *fd_dir;
    struct dirent *entry;

    fd_dir = opendir(fd_dir_path);
    if (!fd_dir) {
        perror("Failed to open /proc/self/fd");
        return;
    }

    printf("Open file descriptors:\n");

    while ((entry = readdir(fd_dir)) != NULL) {
        // Skip special entries "." and ".."
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        // Construct the full path of the file descriptor
        if ((size_t)snprintf(fd_target_path, sizeof(fd_target_path), "%s/%s", fd_dir_path, entry->d_name) >= sizeof(fd_target_path)) {
            fprintf(stderr, "Path truncated for FD: %s\n", entry->d_name);
            continue;
        }

        // Resolve the symbolic link to the file descriptor
        char resolved_path[FD_PATH_MAX];
        ssize_t len = readlink(fd_target_path, resolved_path, sizeof(resolved_path) - 1);
        if (len == -1) {
            perror("Failed to read FD link");
            continue;
        }

        resolved_path[len] = '\0'; // Null-terminate the resolved path
        printf("FD %s -> %s\n", entry->d_name, resolved_path);
    }

    if (closedir(fd_dir) == -1) {
        perror("Failed to close /proc/self/fd");
    }
}
