/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tree.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abquaoub <abquaoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 16:27:06 by abquaoub          #+#    #+#             */
/*   Updated: 2024/04/28 16:53:02 by abquaoub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

void	ft_check_quotes(char c, t_quotes *data)
{
	if (c == '(' && !data->cq && !data->cs)
		data->cp++;
	else if (c == ')' && !data->cq && !data->cs)
		data->cp--;
	if (c == 34 && !data->cq)
		data->cq++;
	else if (c == 34)
		data->cq--;
	if (c == 39 && !data->cq && !data->cs)
		data->cs++;
	else if (c == 39 && !data->cq)
		data->cs--;
}

void	ini_str(t_str *data)
{
	data->str1 = NULL;
	data->str2 = NULL;
}

t_list	*split_end_or(char *str)
{
	int			i;
	t_list		*head;
	t_quotes	data;
	char		c;
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
		if (str[i] == '|' || str[i] == '&')
			c = str[i];
		if (str[i] == c && str[i + 1] == c && !data.cp && !data.cq && !data.cs)
			flag = 1;
		else
			join = ft_new_strjoin(join, str[i]);
		if (flag == 1 || !str[i + 1])
		{
			if (join && ft_strtrim(join, " ")[0])
				ft_lstadd_back(&head, ft_lstnew(join));
			if (str[i + 1])
			{
				join = ft_new_strjoin(NULL, str[i]);
				node = ft_lstnew(ft_strjoin(join, join));
				node->x = 4;
				ft_lstadd_back(&head, node);
				i++;
			}
			join = NULL;
			flag = 0;
		}
		i++;
	}
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

t_list	*ft_nested_pip(char *line, t_data *data)
{
	t_list	*head;
	t_list	*new;
	t_list	*list;
	char	*cmd;

	new = NULL;
	head = split_end_or(line);
	if (ft_check_syntax(head) == 1)
		data->red = 1;
	if (data->red == 0)
	{
		new = head;
		list = NULL;
		while (head)
		{
			cmd = (char *)head->content;
			if (!head->x)
			{
				list = ft_split_linked_pip(cmd, '|');
				if (ft_check_syntax(list) == 1)
					data->red = 1;
				if (data->red == 0)
				{
					head->new_list = list;
					while (list)
					{
						if (ft_strchr((char *)list->content, '(') != NULL)
						{
							list->x = 1;
							list->content = ft_strtrim((char *)list->content,
									"() ");
							list->new_list = ft_nested_pip((char *)list->content,
									data);
						}
						list = list->next;
					}
				}
			}
			else
				head->new_list = NULL;
			head = head->next;
		}
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

int	ft_check_syntax(t_list *head)
{
	int	i;

	i = 0;
	while (head)
	{
		if (head->x == 4 && (!head->next || head->next->x == 4 || i == 0))
			return (1);
		head = head->next;
		i++;
	}
	return (0);
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
				if (head->new_list->x == 0)
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
							ft_nested_pip_ex(head->new_list->new_list, env,
								data, data->out, data->in);
							exit(1);
						}
					}
					else
						ft_command((char *)head->new_list->content, env, data,
							data->out, data->in, fd[0]);
					if (head->new_list->next != NULL)
					{
						close(fd[1]);
						if (data->in != fd0)
							close(data->in);
						data->in = fd[0];
					}
					else if (head->next == NULL)
					{
						if (data->in != fd0)
							close(data->in);
					}
				}
				head->new_list = head->new_list->next;
			}
			wait_proccess(data);
		}
		head = head->next;
	}
}
