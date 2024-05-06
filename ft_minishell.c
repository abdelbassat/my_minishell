/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abquaoub <abquaoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 04:33:37 by abquaoub          #+#    #+#             */
/*   Updated: 2024/05/06 20:28:15 by abquaoub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

int	main(int ac, char **av, char **env)
{
	char	*line;
	t_data	data;
	t_list	*head;
	char	*save;
	int		i;

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
	i = 0;
	while (1)
	{
		i++;
		line = ft_pwd(0);
		save = line;
		line = readline(line);
		free(save);
		add_history(line);
		if (!line)
			exit(1);
		ft_syntax(line, &data);
		if (data.red == 1)
		{
			printf("minishell: syntax error near unexpected token\n");
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
		data.status = 0;
	}
	return (0);
}

void	ft_free_trees(t_list **head)
{
	t_list	*tmp_list;

	while ((*head))
	{
		tmp_list = (*head)->new_list;
		while ((*head)->new_list)
		{
			if ((*head)->new_list->x == 1)
				ft_free_trees((&(*head)->new_list->new_list));
			else
				ft_free_list_node(&((*head)->new_list));
			(*head)->new_list = (*head)->new_list->next;
		}
		ft_lstclear(&tmp_list, free);
		(*head) = (*head)->next;
	}
}
