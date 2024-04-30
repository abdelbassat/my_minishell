/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_command.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abquaoub <abquaoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 19:14:52 by abquaoub          #+#    #+#             */
/*   Updated: 2024/04/30 21:36:29 by abquaoub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

char	*ft_check_command(char *command)
{
	char	*env_path;
	char	*path;
	char	*cmd;
	char	**bins;
	int		i;

	if (command[0] == 0)
		return (NULL);
	env_path = getenv("PATH");
	bins = ft_split(env_path, ':');
	i = 0;
	while (bins[i])
	{
		path = ft_strjoin(bins[i], "/");
		cmd = ft_strjoin(path, command);
		if (access(cmd, F_OK) == 0)
			return (ft_free(bins), free(path), cmd);
		free(path);
		free(cmd);
		i++;
	}
	ft_free(bins);
	return (NULL);
}

void	ft_display(t_list *ptr)
{
	printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
	while (ptr)
	{
		printf("%s\n", (char *)ptr->content);
		ptr = ptr->next;
	}
	printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
}

char	**create_command(t_list *head)
{
	int		i;
	int		size;
	char	**command;

	i = 0;
	size = ft_lstsize(head) + 1;
	command = (char **)malloc(sizeof(char *) * size);
	while (head)
	{
		command[i] = ft_strdup((char *)head->content);
		i++;
		head = head->next;
	}
	command[i] = NULL;
	return (command);
}

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
				join = ft_new_strjoin(join, 0);
			else
			{
				while (str[++i] != c)
					join = ft_new_strjoin(join, str[i]);
			}
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
		if (arr[i] == NULL)
			return (NULL);
		i++;
		head = head->next;
	}
	arr[i] = NULL;
	return (arr);
}

void	ft_handel_redic(char *line, t_list **command_list, t_list **redic,
		t_data *data)
{
	t_list	*head;

	head = split_end_or(line, ">< ", 0);
	*command_list = ft_split_rediction(head, redic);
	ft_exec_redic(*redic, data);
	if (data->outfile != 1)
		data->out = data->outfile;
	if (data->intfile)
		data->in = data->intfile;
}

void	ft_exec_command(char *cmd, char **command, t_data *data)
{
	if (data->in != STDIN_FILENO)
	{
		dup2(data->in, STDIN_FILENO);
		close(data->in);
	}
	if (data->out != STDOUT_FILENO)
	{
		dup2(data->out, STDOUT_FILENO);
		close(data->fd[0]);
		close(data->out);
	}
	execve(cmd, command, data->env);
	perror("execve failing");
}

void	ft_command(char *line, t_data *data)
{
	char	*cmd;
	char	**command;
	t_list	*command_list;
	t_list	*redic;

	command_list = NULL;
	redic = NULL;
	ft_handel_redic(line, &command_list, &redic, data);
	if (!command_list)
		return ;
	command = last_command(command_list);
	cmd = ft_check_command(command[0]);
	if (cmd)
	{
		data->pid = fork();
		if (data->pid == 0)
			ft_exec_command(cmd, command, data);
	}
	else
	{
		data->status = 127;
		printf("Command '%s' not found.\n", command[0]);
	}
	if (data->intfile != 0)
		close(data->intfile);
	ft_free(command);
	ft_lstclear(&command_list, free);
	ft_lstclear(&redic, free);
}
