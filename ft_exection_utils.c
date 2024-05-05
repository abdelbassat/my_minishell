/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exection_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abquaoub <abquaoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/05 01:35:45 by abquaoub          #+#    #+#             */
/*   Updated: 2024/05/05 20:26:07 by abquaoub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

void	check_eo(t_list *head, t_data *data, int fd1, int fd0)
{
	char	*op;

	op = (char *)head->content;
	if (strcmp(op, "&&") == 0)
	{
		if (data->status == 0)
			data->exec = 0;
		else
			data->exec = 1;
	}
	else if (strcmp(op, "||") == 0)
	{
		if (data->status != 0)
			data->exec = 0;
		else
			data->exec = 1;
	}
	if (data->exec == 0)
	{
		data->out = fd1;
		data->in = fd0;
	}
}

void	wait_proccess(t_data *data, int proc)
{
	int	status;
	int	tmp;

	while (1)
	{
		status = wait(&tmp);
		if (status == proc)
		{
			if (data->status != 127)
				data->status = tmp;
		}
		else if (status == -1)
			break ;
	}
}

void	close_fd(t_list **tmp, t_data *data, int fd0)
{
	t_list *head = *tmp;
	if (head->new_list->next != NULL)
	{
		close(data->fd[1]);
		if (data->in != fd0)
			close(data->in);
		data->in = data->fd[0];
	}
	else if (head->next == NULL)
	{
		if (data->in != fd0)
			close(data->in);
	}
}