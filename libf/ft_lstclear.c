/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abquaoub <abquaoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 10:53:23 by abquaoub          #+#    #+#             */
/*   Updated: 2024/05/05 01:36:48 by abquaoub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*tmp;

	if (*lst != NULL)
	{
		while (*lst)
		{
			tmp = (*lst)->next;
			free((*lst)->content);
			del(*lst);
			*lst = tmp;
		}
	}
	return ;
}
