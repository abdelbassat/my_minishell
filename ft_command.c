/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_command.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abquaoub <abquaoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 19:14:52 by abquaoub          #+#    #+#             */
/*   Updated: 2024/05/07 20:17:00 by abquaoub         ###   ########.fr       */
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

char	*ft_qutes(char *str, int *i, char c)
{
	char	*var;

	(*i)++;
	var = NULL;
	while (str[*i] && str[*i] != c)
	{
		var = ft_new_strjoin(var, str[*i]);
		(*i)++;
	}
	if (!var)
		var = ft_strdup("");
	return (var);
}

char	*ft_return_variable(char *str, int *i)
{
	char	*var;
	char	*join;
	int		flag;

	var = NULL;
	join = NULL;
	flag = 0;
	while (str[*i] && str[*i] != ' ' && str[*i] != 34 && str[*i] != 39)
	{
		if (flag == 1)
			var = ft_new_strjoin(var, str[*i]);
		else if (str[*i] == '$')
			flag = 1;
		(*i)++;
	}
	
	var = getenv(var);
	if (!var)
		var = ft_new_strjoin(NULL, 0);
	join = ft_strjoin(join, var);
	(*i)--;
	return (join);
}

char	*ft_remove(char *str)
{
	int		i;
	char	*join;
	char	c;
	char	*var;
	int		flag;

	flag = 1;
	var = NULL;
	i = 0;
	join = NULL;
	while (str[i])
	{
		if (str[i] == 34 || str[i] == 39)
		{
			c = str[i];
			if (c == 39)
				flag = 0;
		}
		if (str[i] == '$' && flag)
			join = ft_strjoin(join, ft_return_variable(str, &i));
		else if (str[i] == c)
			join = ft_strjoin(join, ft_qutes(str, &i, c));
		else
			join = ft_new_strjoin(join, str[i]);
		i++;
	}
	return (join);
}

t_list	*ft_handel_qutes(t_list *head)
{
	t_list	*command;
	char	*cmd;
	t_list	*tmp;

	tmp = head;
	command = NULL;
	while (head)
	{
		cmd = ft_remove(head->content);
		ft_lstadd_back(&command, ft_lstnew(cmd));
		head = head->next;
	}
	ft_lstclear(&tmp, free);
	return (command);
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
		arr[i] = ft_strtrim(head->content , " ");
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
	data->status = 127;
	perror(ft_strjoin("minishell :", cmd));
}

int	ft_handel_satatus_code(int n)
{
	while (n > 255)
		n -= 255;
	return (n);
}

void	ft_command(t_list *head, t_data *data, int cls)
{
	char	*cmd;
	char	**command;

	if (!strcmp(head->command->content, "echo") && head->command->next->content && !strcmp(head->command->next->content, "$?"))
	{
		printf("%d\n", data->status);
		return ;
	}
	head->command = ft_handel_qutes(head->command);
	head->here_doc = ft_handel_qutes(head->here_doc);
	head->redic = ft_handel_qutes(head->redic);
	ft_handel_redic(&head->redic, data, 1);
	if (head->int_file == 2)
		data->in = head->in;
	if (data->in == -1 || data->out == -1 || !head->command)
	{
		data->status = 1;
		return ;
	}
	command = last_command(head->command);
	cmd = ft_check_command(command[0], data);
	
	data->pid = fork();
	if (data->pid == 0)
		ft_exec_command(cmd, command, data, cls);
	free(cmd);
	ft_free(command);
}
