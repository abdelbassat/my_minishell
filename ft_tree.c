/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tree.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abquaoub <abquaoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 16:27:06 by abquaoub          #+#    #+#             */
/*   Updated: 2024/05/05 01:49:14 by abquaoub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

char	*join_command(t_list *head)
{
	char	*join;
	t_list	*tmp;

	tmp = head;
	join = NULL;
	while (head)
	{
		join = ft_strjoin(join, head->content);
		if (head->next)
			join = ft_strjoin(join, " ");
		head = head->next;
	}
	return (join);
}

void	ft_free_node(t_list *head)
{
	if (head->here_doc)
		ft_lstclear(&(head->here_doc), free);
	if (head->redic)
		ft_lstclear(&(head->redic), free);
	if (head->command)
		ft_lstclear(&(head->command), free);
	if (head->new_list)
		ft_lstclear(&(head->new_list), free);
}

void	ft_free_tree(t_list *head)
{
	t_list	*tmp;

	tmp = head;
	while (head)
	{
		ft_free_node(head);
		head = head->next;
	}
	ft_lstclear(&tmp, free);
}

void	ft_nested(t_list **head, t_data *data)
{
	char		*cmd;
	char		*save;
	t_quotes	qutes;
	t_list		*list;

	save = NULL;
	list = *head;
	ft_split_rediction(list->content, &(list));
	cmd = join_command(list->command);
	if (ft_count_qutes(cmd, &qutes) == 1)
	{
		list->x = 1;
		save = cmd;
		cmd = ft_substr(cmd, 1, ft_strlen(cmd) - 2);
		free(save);
		save = cmd;
		list->new_list = ft_nested_pip(cmd, data);
		free(save);
	}
	ft_handel_redic(&(list->here_doc), data, 0);
	list->in = data->in;
}

t_list	*ft_nested_pip(char *line, t_data *data)
{
	t_list	*head;
	t_list	*new;
	t_list	*list;

	head = split_end_or(line, "|&", 1);
	new = head;
	while (head)
	{
		if (!head->x)
		{
			list = split_end_or(head->content, "|", 0);
			head->new_list = list;
			while (list)
			{
				if (list->x != 4)
					ft_nested(&list, data);
				list = list->next;
			}
		}
		head = head->next;
	}
	return (new);
}
