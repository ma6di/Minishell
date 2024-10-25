/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrauh <nrauh@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 18:52:49 by nrauh             #+#    #+#             */
/*   Updated: 2024/08/23 11:21:00 by nrauh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

static int	ft_countwords(char const *s, char c)
{
	size_t	words;

	words = 0;
	while (*s)
	{
		if (*s != c && (*(s + 1) == c || !(*(s + 1))))
			words++;
		s++;
	}
	return (words);
}

static void	ft_free(char **split)
{
	size_t	i;

	i = 0;
	while (split[i])
		free(split[i++]);
	free(split);
}

static char	**ft_create_words(char **split, char const *s, char c)
{
	size_t	w;
	size_t	i;
	size_t	len;

	w = 0;
	i = 0;
	len = 0;
	while (s[i])
	{
		if (s[i] != c)
			len++;
		if ((s[i] != c && (s[i + 1] == c || !s[i + 1])))
		{
			split[w] = ft_substr(s, i - len + 1, len);
			if (!split[w])
				return (ft_free(split), NULL);
			len = 0;
			w++;
		}
		i++;
	}
	return (split);
}

char	**ft_split(char const *s, char c)
{
	size_t	words;
	char	**split;

	words = ft_countwords(s, c);
	if (words == 0)
		return (NULL);
	split = ft_calloc(words + 1, sizeof(char *));
	if (!split)
		return (NULL);
	split = ft_create_words(split, s, c);
	if (!split)
		return (NULL);
	return (split);
}
