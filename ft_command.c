/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_command.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abquaoub <abquaoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 19:14:52 by abquaoub          #+#    #+#             */
/*   Updated: 2024/05/05 01:47:26 by abquaoub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

char	*ft_new_strjoin(char *str, char c)
{
	int		i;
	char	*join;

	join = malloc(ft_strlen(str) + 2);
	i = 0;
	if (str != NULL)
	{
		while (str[i])
		{
			join[i] = str[i];
			i++;
		}
		free(str);
	}
	if (c == 0)
		join[i] = c;
	else
	{
		join[i] = c;
		i++;
		join[i] = 0;
	}
	return (join);
}

char	*ft_remove(char *str)
{
	int		i;
	char	*join;
	char	c;

	i = 0;
	join = NULL;
	while (str[i])
	{
		if (str[i] == 34 || str[i] == 39)
			c = str[i];
		if (str[i] == c)
		{
			if (str[i + 1] == c)
			{
				join = ft_new_strjoin(join, 0);
				i++;
			}
			else
				while (str[++i] != c)
					join = ft_new_strjoin(join, str[i]);
		}
		else
			join = ft_new_strjoin(join, str[i]);
		i++;
	}
	return (join);
}

char	**last_command(t_list *head)
{
	int		i;
	int		size;
	char	**arr;

	if (!head)
		return (NULL);
	i = 0;
	size = ft_lstsize(head);
	arr = malloc(sizeof(char *) * (size + 1));
	while (head)
	{
		arr[i] = ft_remove((char *)head->content);
		i++;
		head = head->next;
	}
	arr[i] = NULL;
	return (arr);
}

void	ft_handel_redic(t_list **redic, t_data *data, int flag)
{
	ft_exec_redic(*redic, data, flag);
	if (data->intfile != 0)
		data->in = data->intfile;
	if (data->outfile != 1)
		data->out = data->outfile;
}

void	ft_exec_command(char *cmd, char **command, t_data *data, int cls)
{
	if (data->in != STDIN_FILENO)
	{
		dup2(data->in, STDIN_FILENO);
		close(data->in);
	}
	if (data->out != STDOUT_FILENO)
	{
		dup2(data->out, STDOUT_FILENO);
		close(cls);
		close(data->out);
	}
	execve(cmd, command, data->env);
	printf("minishell: command not found: %s\n", cmd);
	data->status = 127;
}

void	ft_command(t_list *head, t_data *data, int cls)
{
	char	*cmd;
	char	**command;

	ft_handel_redic(&head->redic, data, 1);
	if (data->in == -1 || data->out == -1)
		return ;
	if (head->int_file == 0)
	{
		if (head->in)
		{
			data->in = head->in;
			head->in = 0;
		}
	}
	command = last_command(head->command);
	cmd = ft_check_command(command[0], data);
	data->pid = fork();
	if (data->pid == 0)
		ft_exec_command(cmd, command, data, cls);
	free(cmd);
	ft_free(command);
}
