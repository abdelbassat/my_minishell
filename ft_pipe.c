/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipe.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abquaoub <abquaoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 15:24:21 by abquaoub          #+#    #+#             */
/*   Updated: 2024/04/28 23:13:55 by abquaoub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

void	initialize(t_quotes *data)
{
	data->cp = 0;
	data->cq = 0;
	data->cs = 0;
	data->en = 0;
	data->bk = 0;
}

t_list	*ft_split_linked_pip(char *str, char c, int f)
{
	int			i;
	t_list		*head;
	t_quotes	data;
	char		*join;
	int			flag;
	t_list		*node;

	join = NULL;
	head = NULL;
	i = 0;
	flag = 0;
	initialize(&data);
	while (str[i])
	{
		ft_check_quotes(str[i], &data);
		if (str[i] == c && !data.cp && !data.cq && !data.cs)
			flag = 1;
		else
			join = ft_new_strjoin(join, str[i]);
		if (flag == 1 || !str[i + 1])
		{
			if (join && ft_strtrim(join, " ")[0])
				ft_lstadd_back(&head, ft_lstnew(join));
			if (str[i] == c && !f)
			{
				join = ft_new_strjoin(NULL, str[i]);
				node = ft_lstnew(join);
				node->x = 4;
				ft_lstadd_back(&head, node);
			}
			join = NULL;
			flag = 0;
		}
		i++;
	}
	return (head);
}
