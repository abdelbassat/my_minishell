/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tree.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abquaoub <abquaoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 16:27:06 by abquaoub          #+#    #+#             */
/*   Updated: 2024/04/25 22:33:29 by abquaoub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

void	ft_check_quotes(char c, t_quotes *data)
{
	if (c == '(' && !data->count_qutes && !data->count_sgl)
		data->count_par++;
	else if (c == ')' && !data->count_qutes && !data->count_sgl)
		data->count_par--;
	if (c == 34 && !data->count_qutes)
		data->count_qutes++;
	else if (c == 34)
		data->count_qutes--;
	if (c == 39 && !data->count_qutes && !data->count_sgl)
		data->count_sgl++;
	else if (c == 39 && !data->count_qutes)
		data->count_sgl--;
}

t_list	*split_end_or(char *str)
{
	int			i;
	int			count;
	char		*cmd;
	t_list		*head;
	t_quotes	data;

	cmd = NULL;
	head = NULL;
	i = 0;
	initialize(&data);
	count = 0;
	while (str[i])
	{
		ft_check_quotes(str[i], &data);
		if (str[i] == '|' && str[i + 1] == '|' && !data.count_par
			&& !data.count_qutes && !data.count_sgl)
			cmd = ft_strtrim(ft_substr(str, i - count, count), " ");
		else if (str[i] == '&' && str[i + 1] == '&' && !data.count_par
			&& !data.count_qutes && !data.count_sgl)
			cmd = ft_strtrim(ft_substr(str, i - count, count), " ");
		if (cmd)
		{
			ft_lstadd_back(&head, ft_lstnew(cmd));
			cmd = ft_substr(str, i, 2);
			ft_lstadd_back(&head, ft_lstnew(cmd));
			i += 2;
			count = 0;
			cmd = NULL;
		}
		count++;
		i++;
	}
	cmd = ft_strtrim(ft_substr(str, i - count, count), " ");
	ft_lstadd_back(&head, ft_lstnew(cmd));
	return (head);
}

void	ft_print_tree(t_list *head)
{
	if (!head)
		return ;
	while (head)
	{
		while (head->new_list)
		{
			if (head->new_list->x == 1)
				ft_print_tree(head->new_list->new_list);
			else
				ft_display(head->new_list);
			head->new_list = head->new_list->next;
		}
		head = head->next;
	}
	printf("--------------------------------------------\n");
}

t_list	*ft_nested_pip(char *line)
{
	t_list	*head;
	t_list	*new;
	t_list	*list;
	char	*cmd;

	head = split_end_or(line);
	new = head;
	list = NULL;
	while (head)
	{
		cmd = (char *)head->content;
		if (strcmp(cmd, "||") != 0 && strcmp(cmd, "&&") != 0)
		{
			list = ft_split_linked_pip(cmd, '|');
			head->new_list = list;
			while (list)
			{
				if (ft_strchr((char *)list->content, '(') != NULL)
				{
					list->x = 1;
					list->content = ft_strtrim((char *)list->content, "() |");
					list->new_list = ft_nested_pip((char *)list->content);
				}
				list = list->next;
			}
		}
		else
			head->new_list = NULL;
		head = head->next;
	}
	return (new);
}

void	check_eo(t_list *head, t_data *data, int fd1, int fd0)
{
	if (strcmp((char *)head->content, "&&") == 0)
	{
		if (data->status == 0)
			data->exec = 0;
		else
			data->exec = 1;
	}
	else if (strcmp((char *)head->content, "||") == 0)
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

void	wait_proccess(t_data *data)
{
	int	status;
	int	tmp;

	while (1)
	{
		status = wait(&tmp);
		if (status == data->pid)
			data->status = tmp;
		else if (status == -1)
			break ;
	}
}

void	ft_nested_pip_ex(t_list *head, char **env, t_data *data, int fd1,
		int fd0)
{
	int	fd[2];
	int	p;

	data->exec = 0;
	data->out = fd1;
	data->in = fd0;
	while (head)
	{
		check_eo(head, data, fd1, fd0);
		if (data->exec == 0)
		{
			while (head->new_list)
			{
				if (head->new_list->next != NULL)
				{
					pipe(fd);
					data->out = fd[1];
				}
				else
					data->out = fd1;
				if (head->new_list->x == 1)
				{
					p = fork();
					if (p == 0)
					{
						ft_nested_pip_ex(head->new_list->new_list, env, data,
							data->out, data->in);
						data->status = 1;
						exit(1);
					}
				}
				else
					ft_command((char *)head->new_list->content, env, data,
						data->out, data->in, fd[0]);
				if (head->new_list->next != NULL)
				{
					close(fd[1]);
					if (data->in != 0)
						close(data->in);
					data->in = fd[0];
				}
				else if (head->next == NULL)
				{
					if (data->in != 0)
						close(data->in);
				}
				head->new_list = head->new_list->next;
			}
			wait_proccess(data);
		}
		head = head->next;
	}
}
