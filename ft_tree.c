/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tree.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abquaoub <abquaoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 16:27:06 by abquaoub          #+#    #+#             */
/*   Updated: 2024/05/08 15:18:56 by abquaoub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

int	ft_math(int i)
{
	int	number;

	number = 1;
	while (i)
	{
		number *= 2;
		i--;
	}
	return (number);
}

char	ft_reverse(char *str)
{
	int	number;
	int	i;

	number = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '1')
			number += ft_math(i);
		i++;
	}
	return (number + 65);
}

char	*ft_revers_to_base64(char *str)
{
	char	*join;
	int		i;

	join = NULL;
	i = 0;
	while (str[i])
	{
		join = ft_new_strjoin(join, ft_reverse(ft_substr(str, i, 6)));
		i += 6;
	}
	return (join);
}

char	*ft_convert(int c)
{
	int		number;
	char	*str;
	int		i;
	int		b;
	int		j;

	j = 0;
	str = malloc(9);
	i = 0;
	number = c;
	while (c)
	{
		c /= 2;
		i++;
	}
	while (i < 8)
	{
		str[j] = 48;
		j++;
		i++;
	}
	while (number)
	{
		b = number % 2;
		str[j] = b + 48;
		number /= 2;
		j++;
	}
	str[j] = 0;
	return (str);
}

char	*ft_base64(char *str)
{
	int		i;
	char	*join;

	join = NULL;
	i = 0;
	while (str[i])
	{
		join = ft_strjoin(join, ft_convert((int)str[i]));
		i++;
	}
	return (join);
}

char	*join_command(t_list *head)
{
	char	*join;

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

void	ft_nested(t_list *head, t_data *data)
{
	char		*cmd;
	char		*save;
	t_quotes	qutes;
	t_list		*list;

	save = NULL;
	list = head;
	ft_split_rediction(list->content, &(list));
	cmd = join_command(list->command);
	if (cmd)
	{
		if (ft_count_qutes(cmd, &qutes) == 1)
		{
			list->x = 1;
			save = ft_substr(cmd, 1, ft_strlen(cmd) - 2);
			list->new_list = ft_nested_pip(save, data);
		}
	}
	free(cmd);
	list->here_doc = ft_handel_qutes(list->here_doc);
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
					ft_nested(list, data);
				list = list->next;
			}
		}
		head = head->next;
	}
	return (new);
}
