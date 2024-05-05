/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abquaoub <abquaoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 18:03:24 by abquaoub          #+#    #+#             */
/*   Updated: 2023/11/18 11:59:22 by abquaoub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *pointer, int value, size_t count)
{
	unsigned char	*str;
	size_t			i;

	str = (unsigned char *)pointer;
	i = 0;
	while (i < count)
	{
		str[i] = (unsigned char)value;
		i++;
	}
	return (str);
}
