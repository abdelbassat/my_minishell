/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abquaoub <abquaoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 18:03:02 by abquaoub          #+#    #+#             */
/*   Updated: 2023/11/15 21:09:48 by abquaoub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *str, int c, size_t size)
{
	unsigned char	*s;

	s = (unsigned char *)str;
	while (size)
	{
		if (*s == (unsigned char)c)
			return (s);
		s++;
		size--;
	}
	return (NULL);
}
