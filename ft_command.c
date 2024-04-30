/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_command.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abquaoub <abquaoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 19:14:52 by abquaoub          #+#    #+#             */
/*   Updated: 2024/04/29 17:20:46 by abquaoub         ###   ########.fr       */
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
		printf("%d\n", ptr->x);
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
	ft_lstclear(&head, free);
	return (command);
}

t_list	*ft_new_split(char *str, t_quotes data)
{
	int		i;
	int		count;
	char	*cmd;
	t_list	*head;
	char	*s;

	i = 0;
	count = 0;
	cmd = NULL;
	head = NULL;
	str = ft_strtrim(str, " ");
	while (str[i])
	{
		ft_check_quotes(str[i], &data);
		if ((str[i] == ' ' || !str[i + 1]) && data.cq == 0 && !data.cs
			&& !data.cp)
		{
			if (!str[i + 1])
				s = ft_strtrim(ft_substr(str, i - count, count + 1), " ");
			else
				s = ft_strtrim(ft_substr(str, i - count, count), " ");
			if (s[0])
				cmd = s;
			else
				cmd = NULL;
		}
		if (cmd)
		{
			ft_lstadd_back(&head, ft_lstnew(cmd));
			count = 0;
			cmd = NULL;
		}
		count++;
		i++;
	}
	if (data.cq || data.cs || data.cp)
		return (NULL);
	return (head);
}

char	*ft_new_strjoin(char *str, char c)
{
	int		i;
	char	*join;

	if (!str)
		i = 0;
	else
		i = ft_strlen(str);
	join = malloc(i + 2);
	i = 0;
	if (str != NULL)
	{
		while (str[i])
		{
			join[i] = str[i];
			i++;
		}
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
				i--;
			}
			i += 2;
		}
		else
		{
			join = ft_new_strjoin(join, str[i]);
			i++;
		}
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

void	ft_print(char **arr)
{
	int	i;

	if (!arr)
	{
		printf("Syntax Error\n");
		return ;
	}
	i = 0;
	while (arr[i])
	{
		printf("%s\n", arr[i]);
		i++;
	}
}

void	ft_command(char *line, char **env, t_data *data, int fd1, int fd0,
		int cls)
{
	char		*cmd;
	char		**command;
	t_quotes	dataa;
	t_list		*command_list;
	t_list		*head;
	t_list		*redic;

	command_list = NULL;
	redic = NULL;
	head = split_end_or(line, "><", 0);
	command_list = ft_split_rediction(head, &redic);
	ft_exec_redic(redic, data);
	if (data->outfile != 1)
		fd1 = data->outfile;
	if (data->intfile)
		fd0 = data->intfile;
	if (!ft_strtrim(line, " \n")[0])
		return ;
	command = NULL;
	initialize(&dataa);
	command = last_command(command_list);
	if (strcmp(command[0], "cd") == 0)
		chdir(command[1]);
	else
	{
		cmd = ft_check_command(command[0]);
		if (cmd)
		{
			data->pid = fork();
			if (data->pid == 0)
			{
				if (fd0 != STDIN_FILENO)
				{
					dup2(fd0, STDIN_FILENO);
					close(fd0);
				}
				if (fd1 != STDOUT_FILENO)
				{
					dup2(fd1, STDOUT_FILENO);
					close(cls);
					close(fd1);
				}
				execve(cmd, command, env);
				perror("execve failing");
			}
		}
		else
		{
			data->status = 127;
			printf("Command '%s' not found.\n", command[0]);
		}
	}
	ft_free(command);
}
