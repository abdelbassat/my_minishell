/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abquaoub <abquaoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 09:42:36 by abquaoub          #+#    #+#             */
/*   Updated: 2024/05/13 12:34:12 by abquaoub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

void	ft_free_trees(t_list **head)
{
	t_list	*tmp_list;

	while ((*head))
	{
		tmp_list = (*head)->new_list;
		while ((*head)->new_list)
		{
			if ((*head)->new_list->x == 1)
				ft_free_trees((&(*head)->new_list->new_list));
			else
				ft_free_list_node(&((*head)->new_list));
			(*head)->new_list = (*head)->new_list->next;
		}
		ft_lstclear(&tmp_list, free);
		(*head) = (*head)->next;
	}
}
void	ft_free(char **str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
}
void	ft_free_list_node(t_list **head)
{
	ft_lstclear(&((*head)->command), free);
	ft_lstclear(&((*head)->redic), free);
	ft_lstclear(&((*head)->here_doc), free);
}

void	free_list(t_free *head, int flag)
{
	if (flag != 0)
		ft_lstclear(&(head->head), free);
	else
		ft_lstclear(&(head->new_list), free);
}
