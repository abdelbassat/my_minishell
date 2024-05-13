/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exection_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abquaoub <abquaoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/05 01:35:45 by abquaoub          #+#    #+#             */
/*   Updated: 2024/05/13 12:33:43 by abquaoub         ###   ########.fr       */
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

unsigned char	ft_test(unsigned char nn)
{
	return (nn);
}

void	wait_proccess(t_data *data, int proc)
{
	int	status;
	int	tmp;

	tmp = 0;
	while (1)
	{
		status = wait(&tmp);
		if (status == proc)
			data->status = tmp;
		if (status == -1)
			break ;
	}
	if (data->status >= 256)
		data->status = ft_reverse(ft_convert(data->status));
}

void	close_fd(t_list **tmp, t_data *data, int fd0)
{
	t_list	*head;

	head = *tmp;
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
