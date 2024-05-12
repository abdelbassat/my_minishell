/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abquaoub <abquaoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 04:33:37 by abquaoub          #+#    #+#             */
/*   Updated: 2024/05/12 16:41:06 by abquaoub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

void	handle_signal(int sig)
{
	if (sig == SIGINT)
	{
		printf("\n");
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}
void	handle_signal_cat(int sig)
{
	if (sig == SIGINT)
	{
		printf("\n");
		rl_replace_line("", 0);
		rl_on_new_line();
	}
}
void	ft_handle_signals(void)
{
	if (signal(SIGINT, handle_signal) == SIG_ERR)
		printf("Error catch signal\n");
	signal(SIGINT, handle_signal);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTERM, SIG_IGN);
}

int	main(int ac, char **av, char **env)
{
	char	*line;
	t_data	data;
	t_list	*head;
	char	*save;
	int		i;

	line = NULL;
	ft_handle_signals();
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
	data.env_list = NULL;
	ft_buffer_to_list(&data.env_list, env);
	(void)av;
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
