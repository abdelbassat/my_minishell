/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abquaoub <abquaoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 08:25:27 by abquaoub          #+#    #+#             */
/*   Updated: 2024/05/04 22:05:14 by abquaoub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_initialize(t_list *new_node)
{
	new_node->next = NULL;
	new_node->command = NULL;
	new_node->here_doc = NULL;
	new_node->new_list = NULL;
	new_node->redic = NULL;
	new_node->x = 0;
	new_node->int_file = 0;
	new_node->in = 0;
	new_node->out = 1;
}

t_list	*ft_lstnew(void *content)
{
	t_list	*new_node;

	new_node = (t_list *)malloc(sizeof(t_list));
	if (!new_node)
		return (0);
	new_node->content = ft_strtrim((char *)content, " ");
	ft_initialize(new_node);
	return (new_node);
}
