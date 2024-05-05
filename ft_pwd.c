/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abquaoub <abquaoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 03:28:31 by abquaoub          #+#    #+#             */
/*   Updated: 2024/05/04 18:01:34 by abquaoub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

void	green(void)
{
	printf("\e[1;32m");
}

void	red(void)
{
	printf("\e[1;31m");
}

void	cyan(void)
{
	printf("\033[0;37m");
}

void	yellow(void)
{
	printf("\e[1;33m");
}

void	reset(void)
{
	printf("\033[0m");
}

char	*ft_pwd(int flag)
{
	char	*buff;
	char	*path;
	char	*save;
	char	*arrow;

	buff = malloc(4000);
	getcwd(buff, 4000);
	path = ft_strjoin(&ft_strrchr(buff, '/')[1], " ");
	if (flag == 0)
	{
		arrow = ft_strjoin("\e[1;32m", "\u2794");
		save = arrow;
		arrow = ft_strjoin(arrow, " ");
		free(save);
		save = arrow;
		arrow = ft_strjoin(arrow, "\e[1;33m");
		free(save);
		save = path;
		path = ft_strjoin(arrow, path);
		free(arrow);
		free(save);
		save = path;
		path = ft_strjoin(path, "\033[0m");
		free(save);
		save = path;
		path = ft_strjoin("\033[0;37m", path);
		free(save);
	}
	else
		printf("%s\n", buff);
	free(buff);
	return (path);
}
