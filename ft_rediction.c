/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rediction.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abquaoub <abquaoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 23:19:20 by abquaoub          #+#    #+#             */
/*   Updated: 2024/05/12 20:08:58 by abquaoub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

void	ft_exec_utils(t_list *head, t_data *data, int flag)
{
	char	*red;

	red = head->content;
	if (flag == 1)
	{
		if (strcmp(red, ">>") == 0)
			data->outfile = access_outfile_herdoc(head->next->content);
		else if (strcmp(red, ">") == 0)
			data->outfile = access_outfile(head->next->content);
		else if (strcmp(red, "<") == 0)
			data->intfile = access_intfile(head->next->content);
		if (data->intfile == -1 || data->outfile == -1)
			return ;
	}
	else
	{
		if (strcmp(red, "<<") == 0)
		{
			data->intfile = ft_read_stdin(head->next->content, data);
			unlink(data->save);
		}
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

char	*ft_remove_v1(char *str, t_data *data)
{
	int		i;
	char	*join;
	char	c;
	char	*var;

	var = NULL;
	i = 0;
	join = NULL;
	while (str[i])
	{
		if (str[i] == 34 || str[i] == 39)
			c = str[i];
		if (str[i] == '$')
		{
			var = ft_return_variable(str, &i, data);
			join = ft_strjoin(join, var);
		}
		else if (str[i] == c)
			join = ft_strjoin(join, ft_qutes(str, &i, c, data, 0));
		else
		{
			join = ft_new_strjoin(join, str[i]);
			i++;
		}
	}
	return (join);
}
void	ft_handle_red(int i)
{
	(void)i;
	printf("\n");
	close(0);
}

char	*ft_generate(void)
{
	int		fd;
	char	*buff;
	int		i;

	fd = open("/dev/random", O_RDWR);
	buff = malloc(11);
	read(fd, buff, 10);
	buff[10] = 0;
	i = 0;
	while (buff[i])
	{
		if (ft_isalnum(buff[i]) == 0)
			buff[i] = 'a';
		i++;
	}
	return (ft_strjoin("/tmp/.", buff));
}

int	ft_read_stdin(char *end, t_data *data)
{
	char	*buff;
	int		fd;
	int		input;

	data->save = ft_generate();
	printf("%s\n", data->save);
	fd = open(data->save, O_CREAT | O_RDWR, 0600);
	input = dup(0);
	signal(SIGINT, ft_handle_red);
	while (1)
	{
		buff = readline("> ");
		if (!buff || !strcmp(buff, end))
			break ;
		if (data)
			buff = ft_remove_v1(buff, data);
		write(fd, buff, ft_strlen(buff));
		write(fd, "\n", 1);
	}
	dup2(input, 0);
	close(input);
	signal(SIGINT, handle_signal);
	close(fd);
	fd = open(data->save, O_RDWR, 0400);
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
