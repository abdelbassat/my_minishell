/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abquaoub <abquaoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 18:01:44 by abquaoub          #+#    #+#             */
/*   Updated: 2023/11/18 12:05:54 by abquaoub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	**ft_free(char **ptr, int i)
{
	i--;
	while (i >= 0)
	{
		free(ptr[i]);
		i--;
	}
	free(ptr);
	return (0);
}

static int	count_word(char *s, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	if (s[i] != c && s[i] != '\0')
		count++;
	while (s[i] != '\0')
	{
		if (s[i] == c && s[i + 1] != c && s[i + 1] != '\0')
			count++;
		i++;
	}
	return (count);
}

static char	**mini_split(char const *s, char c, char **arr, int len_words)
{
	int	i;
	int	word;
	int	j;

	i = 0;
	word = 0;
	while (word < len_words)
	{
		while (s[i] && s[i] == c)
			i++;
		j = 0;
		while (s[i + j] && s[i + j] != c)
			j++;
		arr[word] = ft_substr(s, i, j);
		if (arr[word] == NULL)
			return (ft_free(arr, word));
		i = i + j;
		word++;
	}
	arr[word] = 0;
	return (arr);
}

char	**ft_split(char *s, char c)
{
	char			**arr;
	unsigned int	len_words;

	if (!s)
		return (0);
	len_words = count_word(s, c);
	arr = (char **)malloc(sizeof(char *) * (len_words + 1));
	if (!arr)
		return (0);
	arr = mini_split(s, c, arr, len_words);
	return (arr);
}
