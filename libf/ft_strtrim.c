/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abquaoub <abquaoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 18:01:28 by abquaoub          #+#    #+#             */
/*   Updated: 2024/05/04 16:56:02 by abquaoub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	check_chr(char const *str, char c)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		if (c == str[i])
			return (1);
		i++;
	}
	return (0);
}

static int	test(const char *s1, const char *set)
{
	int	i;
	int	c_e;

	c_e = 0;
	i = ft_strlen(s1) - 1;
	while (i > 0 && check_chr(set, s1[i]) == 1)
	{
		c_e++;
		i--;
	}
	return (c_e);
}

char	*ft_strtrim(char *s1, char const *set)
{
	char	*res;
	size_t	c_s;
	size_t	c_e;
	size_t	i;

	i = 0;
	c_s = 0;
	if (s1 == NULL || set == NULL)
		return (0);
	while (s1[i] != '\0' && check_chr(set, s1[i]) == 1)
	{
		c_s++;
		i++;
	}
	if (s1[i] == '\0')
	{
		res = (char *)malloc(sizeof(char) * 1);
		if (!res)
			return (0);
		res[0] = 0;
		return (res);
	}
	c_e = test(s1, set);
	res = ft_substr(s1, c_s, ft_strlen(s1) - c_e - c_s);
	return (res);
}
