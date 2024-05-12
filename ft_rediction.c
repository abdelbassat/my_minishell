/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rediction.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abquaoub <abquaoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 23:19:20 by abquaoub          #+#    #+#             */
/*   Updated: 2024/05/11 12:21:16 by abquaoub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

void	ft_exec_utils(t_list *head, t_data *data, int flag)
{
	char	*red;

	red = (char *)head->content;
	if (flag == 1)
	{
		if (strcmp(red, ">>") == 0)
			data->outfile = access_outfile_herdoc((char *)head->next->content);
		else if (strcmp(red, ">") == 0)
			data->outfile = access_outfile((char *)head->next->content);
		else if (strcmp(red, "<") == 0)
			data->intfile = access_intfile((char *)head->next->content);
		if (data->intfile == -1 || data->outfile == -1)
			return ;
	}
	else
	{
		if (strcmp(red, "<<") == 0)
			data->intfile = ft_read_stdin((char *)head->next->content);
	}
}

void	ft_exec_redic(t_list *head, t_data *data, int flag)
{
	data->outfile = 1;
	data->intfile = 0;
	while (head)
	{
		ft_exec_utils(head, data, flag);
		if (data->intfile == -1 || data->outfile == -1)
		{
			data->status = 1;
			break ;
		}
		head = head->next;
		if (head)
			head = head->next;
	}
}
void	ft_handle_red(int i)
{
	(void)i;
	printf("\n");
	close(0);
}
int	ft_read_stdin(char *end)
{
	char	*buff;
	int		fd;
	int		input;

	fd = open("a", O_CREAT | O_RDWR, 0600);
	input = dup(0);
	signal(SIGINT, ft_handle_red);
	while (1)
	{
		buff = readline("> ");
		if (!buff || !strcmp(buff, end))
			break ;
		write(fd, buff, ft_strlen(buff));
		write(fd, "\n", 1);
	}
	dup2(input, 0);
	close(input);
	signal(SIGINT, handle_signal);
	close(fd);
	fd = open("a", O_CREAT | O_RDWR, 0400);
	return (fd);
}

void	ft_split_rediction(char *line, t_list **command)
{
	t_list	*head;
	t_list	*tmp;

	head = split_end_or(line, ">< ", 0);
	tmp = head;
	while (head)
	{
		if (head->x == 4 && head->next->x != 4)
		{
			if (strcmp(head->content, "<<") == 0)
			{
				(*command)->int_file = 2;
				ft_lstnew_back(&((*command)->here_doc), head->content, 0);
				head = head->next;
				ft_lstnew_back(&((*command)->here_doc), head->content, 4);
			}
			else
			{
				if (!strcmp(head->content, "<"))
					(*command)->int_file = 1;
				ft_lstnew_back(&((*command)->redic), head->content, 0);
				head = head->next;
				ft_lstnew_back(&((*command)->redic), head->content, 4);
			}
		}
		else
			ft_lstnew_back(&(*command)->command, head->content, 0);
		head = head->next;
	}
	ft_lstclear(&tmp, free);
}
