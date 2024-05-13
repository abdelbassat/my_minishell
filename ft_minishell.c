/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abquaoub <abquaoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 04:33:37 by abquaoub          #+#    #+#             */
/*   Updated: 2024/05/13 13:43:05 by abquaoub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

int	main(int ac, char **av, char **env)
{
	char	*line;
	t_data	data;
	t_list	*head;
	char	*save;

	(void)av;
	ft_handle_signals();
	head = NULL;
	data.env = env;
	initialize(NULL, &data);
	ft_buffer_to_list(&data.env_list, env);
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
		ft_syntax(line, &data);
		// exit(0);
		if (data.red == 1)
		{
			ft_putstr_fd("minishell: syntax error near unexpected token\n", 2);
			data.red = 0;
		}
		else
		{
			head = ft_nested_pip(line, &data);
			ft_nested_pip_ex(head, &data, STDOUT_FILENO, STDIN_FILENO);
			ft_lstclear(&head, free);
			data.in = 0;
			data.out = 1;
		}
	}
	return (0);
}
