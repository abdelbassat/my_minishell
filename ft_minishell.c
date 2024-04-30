/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abquaoub <abquaoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 04:33:37 by abquaoub          #+#    #+#             */
/*   Updated: 2024/04/30 19:02:45 by abquaoub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

int	aa = 1;

int	main(int ac, char **av, char **env)
{
	char	*line;
	t_data	data;
	t_list	*head;

	line = NULL;
	printf("PID %d\n", getpid());
	(signal(SIGTERM, SIG_IGN));
	head = NULL;
	data.status = 0;
	data.in = 0;
	data.intfile = 0;
	data.outfile = 1;
	data.out = 1;
	data.exec = 0;
	data.red = 0;
	data.env = env;
	(void)av;
	(void)env;
	if (ac != 1)
		return (1);
	while (1)
	{
		line = ft_pwd(0);
		line = readline(line);
		add_history(line);
		if (!line)
			return (1);
		// printf("%s\n", line);
		// head = split_end_or(line, "|", 0);
		// ft_display(head);
		head = ft_nested_pip(line, &data);
		if (data.red == 1)
		{
			printf("minishell: syntax error near unexpected token\n");
			data.red = 0;
		}
		else
			ft_nested_pip_ex(head, &data, STDOUT_FILENO, STDIN_FILENO);
	}
	return (0);
}
