/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipe.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abquaoub <abquaoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 15:24:21 by abquaoub          #+#    #+#             */
/*   Updated: 2024/04/25 15:24:23 by abquaoub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

void	initialize(t_quotes *data)
{
	data->count_par = 0;
	data->count_qutes = 0;
	data->count_sgl = 0;
}

t_list	*ft_split_linked_pip(char *str, char c)
{
	int			i;
	int			count;
	t_list		*head;
	t_quotes	data;

	head = NULL;
	i = 0;
	count = 0;
	initialize(&data);
	while (str[i])
	{
		ft_check_quotes(str[i], &data);
		if (str[i] == c && !data.count_par && !data.count_sgl
			&& !data.count_qutes)
		{
			ft_lstadd_back(&head, ft_lstnew(ft_strtrim(ft_substr(str, i - count,
							count), "| ")));
			i++;
			count = 0;
		}
		count++;
		i++;
	}
	ft_lstadd_back(&head, ft_lstnew(ft_strtrim(ft_substr(str, i - count, count),
				"| ")));
	return (head);
}
