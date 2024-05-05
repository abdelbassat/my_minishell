/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_syntax.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abquaoub <abquaoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 16:49:51 by abquaoub          #+#    #+#             */
/*   Updated: 2024/05/05 01:50:56 by abquaoub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

void	ft_nested_syntax(t_list **head, t_data *data)
{
	char		*cmd;
	char		*save;
	t_quotes	qutes;
	t_list		*list;

	save = NULL;
	list = *head;
	ft_split_rediction(list->content, &(list));
	data->red = ft_check_syntax(list->here_doc, 0);
	data->red = ft_check_syntax(list->redic, 0);
	cmd = join_command(list->command);
	ft_check_string(cmd, data);
	if (ft_count_qutes(cmd, &qutes) == 1 && !data->red)
	{
		list->x = 1;
		save = cmd;
		cmd = ft_substr(cmd, 1, ft_strlen(cmd) - 2);
		free(save);
		save = cmd;
		list->new_list = ft_nested_pip(cmd, data);
		free(save);
	}
}

void	ft_syntax(char *line, t_data *data)
{
	t_free	free_data;
	t_list	*head;
	t_list	*list;

	head = split_end_or(line, "|&", 1);
	free_data.head = head;
	data->red = ft_check_syntax(head, 0);
	while (head && !data->red)
	{
		if (head->x != 4)
		{
			list = split_end_or(head->content, "|", 0);
			free_data.new_list = list;
			data->red = ft_check_syntax(list, 0);
			head->new_list = list;
			while (list && !data->red)
			{
				if (list->x != 4)
					ft_nested_syntax(&list, data);
				list = list->next;
			}
			free_list(&free_data, 0);
		}
		head = head->next;
	}
	free_list(&free_data, 3);
}

int	ft_check_syntax(t_list *head, int flag)
{
	int	i;

	i = 0;
	while (head)
	{
		if (head->x == 4 && (!head->next || head->next->x == 4 || i == flag))
			return (1);
		head = head->next;
		i++;
	}
	return (0);
}

void	ft_check_string(char *line, t_data *data)
{
	int			i;
	t_quotes	qutes;

	i = 0;
	initialize(&qutes);
	while (line[i])
	{
		ft_check_quotes(line[i], &qutes);
		i++;
	}
	if (qutes.cp || qutes.cq || qutes.cs || (qutes.en % 2) || (qutes.bk % 2))
		data->red = 1;
}
