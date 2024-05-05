/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abquaoub <abquaoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 18:02:26 by abquaoub          #+#    #+#             */
/*   Updated: 2023/11/15 04:40:33 by abquaoub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *theString)
{
	int	s;
	int	res;
	int	i;

	s = 1;
	res = 0;
	i = 0;
	while (theString[i] == ' ' || (theString[i] >= 9 && theString[i] <= 13))
		i++;
	if (theString[i] == '-' || theString[i] == '+')
	{
		if (theString[i] == '-')
			s = -1;
		i++;
	}
	while ((theString[i] >= '0' && theString[i] <= '9') && theString[i] != '\0')
	{
		res = res * 10 + theString[i] - 48;
		i++;
	}
	return (res * s);
}
