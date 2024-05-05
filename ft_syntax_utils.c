/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_syntax_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abquaoub <abquaoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/05 01:43:33 by abquaoub          #+#    #+#             */
/*   Updated: 2024/05/05 01:48:08 by abquaoub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

int	ft_count_qutes(char *line, t_quotes *qutes)
{
	int	i;

	i = 0;
	initialize(qutes);
	while (line[i])
	{
		ft_check_quotes(line[i], qutes);
		if (qutes->cp == 1)
			return (1);
		i++;
	}
	return (0);
}

void	free_list(t_free *head, int flag)
{
	if (flag != 0)
		ft_lstclear(&(head->head), free);
	else
		ft_lstclear(&(head->new_list), free);
}
