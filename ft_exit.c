/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abquaoub <abquaoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 11:03:56 by abquaoub          #+#    #+#             */
/*   Updated: 2024/05/10 17:26:54 by abquaoub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

void	ft_exit(t_list *head, t_data *data)
{
	int at;
	int size = ft_lstsize(head);
	if (size > 2)
		data->status = 1;
	else
	{
		head = head->next;
		if (!head)
			exit(0);
		at = ft_atoi(head->content);
		exit(at);
	}
}