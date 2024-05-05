/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rediction.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abquaoub <abquaoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 23:19:20 by abquaoub          #+#    #+#             */
/*   Updated: 2024/05/05 20:09:32 by abquaoub         ###   ########.fr       */
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
		if (data->intfile == -1)
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
			break ;
		head = head->next;
		if (head)
			head = head->next;
	}
}

int	ft_read_stdin(char *end)
{
	char	*buff;
	int		fd;
	char	*file;

	buff = malloc(6);
	read(open("/dev/random", O_RDONLY), buff, 5);
	buff[5] = 0;
	file = ft_strjoin("/tmp/.", ft_revers_to_base64(ft_base64(buff)));
	fd = open(file, O_CREAT | O_RDWR, 0644);
	while (1)
	{
		buff = readline("> ");
		if (!buff || !strcmp(buff, end))
			break ;
		write(fd, buff, ft_strlen(buff));
		write(fd, "\n", 1);
	}
	if (!buff)
		printf("bash: warning: here-document at line 1 delimited by end-of-file (wanted `%s')\n",
			end);
	close(fd);
	fd = open(file, O_CREAT | O_RDWR, 0644);
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
				(*command)->int_file = 0;
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
