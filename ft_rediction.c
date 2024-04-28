/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rediction.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abquaoub <abquaoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 23:19:20 by abquaoub          #+#    #+#             */
/*   Updated: 2024/04/26 16:03:22 by abquaoub         ###   ########.fr       */
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

t_list	*ft_redic(char *line)
{
	int		i;
	t_list	*head;
	char	*join;
	int		flag;
	char	c;

	i = 0;
	head = NULL;
	join = NULL;
	flag = 0;
	while (line[i])
	{
		if (line[i] == '>' || line[i] == '<')
			c = line[i];
		if (line[i] == c)
		{
			if (line[i + 1] == c)
			{
				ft_lstadd_back(&head, ft_lstnew(ft_substr(line, i, 2)));
				i++;
			}
			else
				ft_lstadd_back(&head, ft_lstnew(ft_substr(line, i, 1)));
			i++;
			while (line[i] != '<' && line[i] != '>' && line[i])
			{
				flag = 1;
				join = ft_new_strjoin(join, line[i]);
				i++;
			}
			if (flag == 1)
			{
				flag = 0;
				ft_lstadd_back(&head, ft_lstnew(join));
			}
		}
		else
			return (NULL);
	}
	return (head);
}
