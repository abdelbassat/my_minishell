/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rediction_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abquaoub <abquaoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/05 01:40:54 by abquaoub          #+#    #+#             */
/*   Updated: 2024/05/10 20:32:21 by abquaoub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

void	ft_Error(char *path)
{
	char	*join;

	join = ft_strjoin("minishell: ", path);
	perror(join);
	free(join);
}

int	access_outfile_herdoc(char *path)
{
	int	fd;

	fd = open(path, O_RDWR | O_APPEND | O_CREAT, 0644);
	if (fd == -1)
		ft_Error(path);
	return (fd);
}

int	access_intfile(char *path)
{
	int	fd;

	fd = open(path, O_RDONLY);
	if (fd == -1)
		ft_Error(path);
	return (fd);
}

int	access_outfile(char *path)
{
	int fd;

	fd = open(path, O_TRUNC | O_RDWR | O_CREAT, 0644);
	if (fd == -1)
		ft_Error(path);
	return (fd);
}