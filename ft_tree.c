/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tree.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abquaoub <abquaoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 16:27:06 by abquaoub          #+#    #+#             */
/*   Updated: 2024/04/30 21:28:38 by abquaoub         ###   ########.fr       */
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
	data->str = NULL;
	data->join = NULL;
	data->flag = 0;
	data->c = 0;
}

void	ft_lstnew_back(t_list **head, char *cont, int flag)
{
	t_list	*node;

	node = ft_lstnew(cont);
	if (((char *)node->content)[0])
	{
		node->x = flag;
		ft_lstadd_back(head, node);
	}
	else
		ft_lstdelone(node, free);
}

void	utils_split(t_str *strr, t_list **head, int *i)
{
	if (strr->join)
	{
		ft_lstnew_back(head, strr->join, 0);
		free(strr->join);
	}
	if (strr->str[*i + 1])
	{
		if (strr->str[*i] != ' ')
			strr->join = ft_new_strjoin(NULL, strr->str[*i]);
		if (strr->str[*i + 1] == strr->c && strr->c != ' ')
		{
			*i = *i + 1;
			strr->join = ft_strjoin(strr->join, strr->join);
		}
		if (strr->c != ' ')
		{
			ft_lstnew_back(head, strr->join, 4);
			free(strr->join);
		}
	}
	strr->join = NULL;
	strr->flag = 0;
}

void	charset(char c, char *set, t_str *strr)
{
	int	i;

	i = 0;
	while (set[i])
	{
		if (c == set[i])
		{
			strr->c = set[i];
			break ;
		}
		i++;
	}
}

t_list	*split_end_or(char *line, char *set, int check)
{
	int			i;
	t_list		*head;
	t_quotes	data;
	t_str		strr;

	head = NULL;
	i = 0;
	ini_str(&strr);
	initialize(&data);
	strr.str = line;
	while (line[i])
	{
		ft_check_quotes(line[i], &data);
		charset(line[i], set, &strr);
		if (line[i] == strr.c && (check == 0 || line[i + 1] == strr.c)
			&& !data.cp && !data.cq && !data.cs)
			strr.flag = 1;
		else
			strr.join = ft_new_strjoin(strr.join, line[i]);
		if (strr.flag == 1 || !line[i + 1])
			utils_split(&strr, &head, &i);
		i++;
	}
	return (head);
}

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
	t_quotes	qutes;
	t_list		*command_list;
	t_list		*redic;

	command_list = NULL;
	redic = NULL;
	head = split_end_or(line, "|&", 1);
	new = head;
	while (head)
	{
		if (!head->x)
		{
			list = split_end_or((char *)head->content, "|", 0);
			head->new_list = list;
			while (list)
			{
				if (ft_count_qutes((char *)list->content, &qutes) == 1)
				{
					ft_handel_redic((char *)list->content, &command_list,
						&redic, data);
					list->x = 1;
					list->in = data->in;
					list->content = ft_substr((char *)command_list->content, 1,
							ft_strlen(command_list->content) - 2);
					list->new_list = ft_nested_pip((char *)list->content, data);
				}
				list = list->next;
			}
		}
		head = head->next;
	}
	return (new);
}

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

void	wait_proccess(t_data *data)
{
	int	status;
	int	tmp;

	while (1)
	{
		status = wait(&tmp);
		if (status == data->pid)
		{
			if (data->status != 127)
				data->status = tmp;
		}
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

void	ft_exec_pip(t_list *head, t_data *data, int fd1, int fd0)
{
	int	p;

	if (head->new_list->next != NULL)
	{
		pipe(data->fd);
		data->out = data->fd[1];
	}
	else
		data->out = fd1;
	if (head->new_list->x == 1)
	{
		p = fork();
		if (p == 0)
		{
			data->in = head->new_list->in;
			data->out = head->new_list->out;
			ft_nested_pip_ex(head->new_list->new_list, data, data->out,
				data->in);
			exit(1);
		}
	}
	else
		ft_command((char *)head->new_list->content, data);
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

void	ft_nested_pip_ex(t_list *head, t_data *data, int fd1, int fd0)
{
	data->exec = 0;
	// data->out = fd1;
	// data->in = fd0;
	while (head)
	{
		check_eo(head, data, fd1, fd0);
		if (data->exec == 0)
		{
			while (head->new_list)
			{
				if (head->new_list->x != 4)
					ft_exec_pip(head, data, fd1, fd0);
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
