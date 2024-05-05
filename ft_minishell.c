/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abquaoub <abquaoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 04:33:37 by abquaoub          #+#    #+#             */
/*   Updated: 2024/05/05 01:03:49 by abquaoub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

int	main(int ac, char **av, char **env)
{
	char	*line;
	t_data	data;
	t_list	*head;
	char	*save;

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
	data.check_Cmd = 0;
	data.env = env;
	(void)av;
	(void)env;
	if (ac != 1)
		return (1);
	while (1)
	{
		line = ft_pwd(0);
		save = line;
		line = readline(line);
		free(save);
		add_history(line);
		if (!line)
			exit(1);
		head = NULL;
		ft_syntax(line, &data);
		if (data.red == 1)
		{
			printf("minishell: syntax error near unexpected token\n");
			data.red = 0;
		}
		else
		{
			head = ft_nested_pip(line, &data);
			// ft_free_tree(head);
			ft_nested_pip_ex(head, &data, STDOUT_FILENO, STDIN_FILENO);
			data.in = 0;
			data.out = 1;
		}
		data.status = 0;
	}
	return (0);
}
