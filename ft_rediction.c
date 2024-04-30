/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rediction.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abquaoub <abquaoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 23:19:20 by abquaoub          #+#    #+#             */
/*   Updated: 2024/04/30 21:34:51 by abquaoub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

int	access_outfile_herdoc(char *path)
{
	if (access(path, F_OK) == -1)
		return (open(path, O_CREAT | O_RDWR, 0644));
	else if (access(path, W_OK) == -1)
		return (0);
	return (open(path, O_RDWR | O_APPEND));
}

int	access_intfile(char *path)
{
	int	f;
	int	r;
	int	fd;

	f = access(path, F_OK);
	r = access(path, R_OK);
	if (f == -1)
		return (0);
	if (r == -1)
		return (0);
	fd = open(path, O_RDONLY);
	return (fd);
}

int	access_outfile(char *path)
{
	int	fd;

	if (access(path, F_OK) == -1)
	{
		fd = open(path, O_CREAT | O_RDWR, 0644);
		return (fd);
	}
	else if (access(path, W_OK) == -1)
		return (0);
	fd = open(path, O_TRUNC | O_RDWR);
	return (fd);
}

void	ft_exec_utils(t_list *head, t_data *data)
{
	char	*red;

	red = (char *)head->content;
	if (strcmp(red, ">>") == 0)
		data->outfile = access_outfile_herdoc((char *)head->next->content);
	else if (strcmp(red, ">") == 0)
		data->outfile = access_outfile((char *)head->next->content);
	else if (strcmp(red, "<") == 0)
		data->intfile = access_intfile((char *)head->next->content);
	else if (strcmp(red, "<<") == 0)
		data->intfile = ft_read_stdin((char *)head->next->content);
}

void	ft_exec_redic(t_list *head, t_data *data)
{
	data->outfile = 1;
	data->intfile = 0;
	while (head)
	{
		ft_exec_utils(head, data);
		head = head->next;
		if (head)
			head = head->next;
	}
}

int	ft_read_stdin(char *end)
{
	char	*buff;
	int		fd[2];

	buff = NULL;
	pipe(fd);
	while (1)
	{
		buff = readline("> ");
		if (!buff || !strcmp(buff, end))
			break ;
		write(fd[1], buff, ft_strlen(buff));
	}
	if (!buff)
		printf("bash: warning: here-document at line 1 delimited by end-of-file (wanted `%s')\n",
			end);
	return (close(fd[1]), fd[0]);
}

t_list	*ft_split_rediction(t_list *head, t_list **new)
{
	t_list	*tmp;

	tmp = NULL;
	while (head)
	{
		if (head->x == 4 && head->next->x != 4)
		{
			ft_lstadd_back(new, ft_lstnew(ft_strdup((char *)head->content)));
			head = head->next;
			ft_lstadd_back(new, ft_lstnew(ft_strdup((char *)head->content)));
		}
		else
			ft_lstadd_back(&tmp, ft_lstnew(ft_strdup((char *)head->content)));
		head = head->next;
	}
	return (tmp);
}
