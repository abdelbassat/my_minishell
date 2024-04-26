/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abquaoub <abquaoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 03:28:31 by abquaoub          #+#    #+#             */
/*   Updated: 2024/04/25 15:25:10 by abquaoub         ###   ########.fr       */
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

void	ft_pwd(int flag, t_data *data)
{
	char	*buff;
	char	*path;

	buff = malloc(100);
	getcwd(buff, 100);
	path = ft_strjoin(&ft_strrchr(buff, '/')[1], " ");
	if (flag == 0)
	{
		if (data->status == 0)
			green();
		else
			red();
		printf("\u2794");
		reset();
		yellow();
		printf("  %s", path);
		reset();
		cyan();
	}
	else
		printf("%s\n", buff);
	free(buff);
	free(path);
}
