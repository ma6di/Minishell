#include <string.h>
#include <stdlib.h>
#include <stdio.h>

char	*add_to_buffer(char **buffer, char c)
{
	size_t	len;
	int		i;
	char	*new_buffer;
	char	*tmp;

	if (!(*buffer))
		*buffer = strdup("");
	i = 0;
	len = strlen(*buffer);
	new_buffer = malloc((len + 2) * sizeof(char));
	if (!new_buffer)
		return (free(*buffer), NULL);
	tmp = *buffer;
	while (*tmp)
		*new_buffer++ = *tmp++;
	new_buffer = new_buffer - len;
	new_buffer[len] = c;
	new_buffer[len + 1] = '\0';
	free(*buffer);
	return (new_buffer);
}

int	main(void)
{
	char	*buffer = NULL;

	buffer = add_to_buffer(&buffer, 'h');
	buffer = add_to_buffer(&buffer, 'a');
	buffer = add_to_buffer(&buffer, 'l');
	printf("buffer: %s\n", buffer);
	free(buffer);
	return (0);
}

// dquote
// hello"$USER"hello -> valid END is ", ' ', $
// echo "$USERlala"hello -> hello
// echo "$USER$HOME" -> natalierauh/Users/natalierauh
// echo "$USER lala"hello -> natalierauh lalahello

// no dquote
// hello$USER -> valid END is ", ' ', $, '
// hello$USERhello -> hello
// hello$USER"hallo" -> hellonatalierauhhallo

// count when ANY char is read
// count till $ is reached
// read till end ' ' $ " ' is reached
