/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tree.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abquaoub <abquaoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 16:27:06 by abquaoub          #+#    #+#             */
/*   Updated: 2024/04/29 18:10:58 by abquaoub         ###   ########.fr       */
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
	if (c == '&' && !data->cq && !data->cs && !data->cp)
		data->en++;
	if (c == '`' && !data->cq && !data->cs && !data->cp)
		data->en++;
}

void	ini_str(t_str *data)
{
	data->str1 = NULL;
	data->str2 = NULL;
}

t_list	*split_end_or(char *str, char *set, int check)
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
		if (str[i] == set[0] || str[i] == set[1] || str[i] == ' ')
			c = str[i];
		if (str[i] == c && (check == 0 || str[i + 1] == c) && !data.cp
			&& !data.cq && !data.cs)
			flag = 1;
		else
			join = ft_new_strjoin(join, str[i]);
		if (flag == 1 || !str[i + 1])
		{
			if (join && ft_strtrim(join, " ")[0])
				ft_lstadd_back(&head, ft_lstnew(join));
			if (str[i + 1])
			{
				if (str[i] != ' ')
					join = ft_new_strjoin(NULL, str[i]);
				if (str[i + 1] == c && c != ' ')
				{
					i++;
					join = ft_strjoin(join, join);
				}
				node = ft_lstnew(join);
				node->x = 4;
				if (str[i] != ' ')
					ft_lstadd_back(&head, node);
			}
			join = NULL;
			flag = 0;
		}
		i++;
	}
	return (head);
}

int	ft_count_qutes(char *str, t_quotes *qutes)
{
	int	i;

	i = 0;
	initialize(qutes);
	while (str[i])
	{
		ft_check_quotes(str[i], qutes);
		if (qutes->cp == 1)
			return (1);
		i++;
	}
	return (0);
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
	t_list		*head;
	t_list		*new;
	t_list		*list;
	t_list		*par;
	t_quotes	qutes;
	char		*cmd;
	char		*command;

	new = NULL;
	head = split_end_or(line, "|&", 1);
	if (data->red == 0)
	{
		new = head;
		list = NULL;
		while (head)
		{
			cmd = (char *)head->content;
			if (!head->x)
			{
				list = ft_split_linked_pip(cmd, '|', 0);
				if (data->red == 0)
				{
					head->new_list = list;
					while (list)
					{
						command = (char *)list->content;
						if (ft_count_qutes(command, &qutes) == 1)
						{
							par = ft_split_linked_pip(command, ' ', 1);
							par->content = ft_strtrim((char *)par->content, " ");
							list->x = 1;
							list->content = ft_substr(par->content, 1,
									ft_strlen(par->content) - 2);
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
	char	*op;

	op = ft_strtrim((char *)head->content, " ");
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

void	wait_proccess(t_data *data)
{
	int	status;
	int	tmp;

	// problem in command not found
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
				if (head->new_list->x != 4)
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

void	ft_nested_pip_syntax(t_list *head, t_data *data)
{
	if (ft_check_syntax(head) == 1)
		data->red = 1;
	while (head && !data->red)
	{
		if (ft_check_syntax(head->new_list) == 1)
			data->red = 1;
		while (head->new_list && !data->red)
		{
			if (head->new_list->x != 4)
			{
				if (head->new_list->x == 1)
					ft_nested_pip_syntax(head->new_list->new_list, data);
			}
			head->new_list = head->new_list->next;
		}
		head = head->next;
	}
}

//////////////////check syntax ////////////////////

void	ft_check_string(char *str, t_data *data)
{
	int			i;
	t_quotes	qutes;

	i = 0;
	initialize(&qutes);
	while (str[i])
	{
		ft_check_quotes(str[i], &qutes);
		i++;
	}
	if (qutes.cp || qutes.cq || qutes.cs || (qutes.en % 2) || (qutes.bk % 2))
		data->red = 1;
}

void	ft_check_syntax_command(t_list *head, t_data *data)
{
	while (head)
	{
		ft_check_string((char *)head->content, data);
		if (data->red == 1)
			break ;
		head = head->next;
	}
}
