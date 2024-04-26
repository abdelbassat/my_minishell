/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abquaoub <abquaoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 04:33:37 by abquaoub          #+#    #+#             */
/*   Updated: 2024/04/26 09:41:40 by abquaoub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

int	main(int ac, char **av, char **env)
{
	char	*line;
	t_data	data;
	t_list	*head;

	printf("PID %d\n", getpid());
	(signal(SIGTERM, SIG_IGN));
	printf("%d\n", getpid());
	head = NULL;
	data.status = 0;
	data.in = 0;
	data.out = 1;
	data.exec = 0;
	(void)av;
	(void)env;
	if (ac != 1)
		return (1);
	while (1)
	{
		ft_pwd(0, &data);
		line = readline(" ");
		add_history(line);
		head = ft_nested_pip(line);
		ft_nested_pip_ex(head, env, &data, STDOUT_FILENO, STDIN_FILENO);
	}
	return (0);
}
