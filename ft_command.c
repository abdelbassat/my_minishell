/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_command.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abquaoub <abquaoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 19:14:52 by abquaoub          #+#    #+#             */
/*   Updated: 2024/05/12 20:42:17 by abquaoub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

void	ft_print_error(char *cmd, char *str)
{
	ft_putstr_fd("minishell : ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(" : ", 2);
	if (!ft_strrchr(cmd, '/'))
		str = "command not found";
	ft_putstr_fd(str, 2);
	ft_putstr_fd("\n", 2);
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

char	*ft_qutes(char *str, int *i, char c, t_data *data, int flag)
{
	char	*var;
	char	*join;

	(*i)++;
	join = NULL;
	while (str[*i] && str[*i] != c)
	{
		if (str[*i] == '$' && c == 34 && !flag)
		{
			var = ft_return_variable(str, i, data);
			if (!join)
				join = ft_strdup("");
			join = ft_strjoin(join, var);
			free(var);
		}
		else
		{
			join = ft_new_strjoin(join, str[*i]);
			(*i)++;
		}
	}
	if (!join)
		join = ft_strdup("");
	(*i)++;
	return (join);
}

char	*ft_return_variable(char *str, int *i, t_data *data)
{
	char	*var;

	var = NULL;
	(*i)++;
	if (str[*i] == '?')
	{
		var = ft_getenv(data, "?");
		(*i)++;
		return (var);
	}
	while (str[*i] && ft_isalnum(str[*i]))
	{
		var = ft_new_strjoin(var, str[*i]);
		(*i)++;
	}
	var = ft_getenv(data, var);
	if (!var)
		var = ft_strdup("");
	return (var);
}

char	*ft_handel_expend(t_list **head, char *var, char *join)
{
	t_list	*expend;
	char	*value;
	t_list	*node;

	expend = NULL;
	value = ft_strdup("");
	if (var[0])
	{
		expend = split_end_or(var, " ", 0);
		expend->content = ft_strjoin(join, expend->content);
		while (expend->next)
		{
			node = ft_wild_card(expend->content);
			ft_lstadd_back(head, node);
			expend = expend->next;
		}
		value = ft_strdup(expend->content);
	}
	return (value);
}

char	*ft_remove(char *str, t_data *data, int flag, t_list **head)
{
	int		i;
	char	*join;
	char	c;
	char	*var;
	int		ff;
	char	*res;

	ff = 0;
	i = 0;
	join = NULL;
	while (str[i])
	{
		if (str[i] == 34 || str[i] == 39)
			c = str[i];
		if (str[i] == '$' && !flag)
		{
			ff = 1;
			var = ft_return_variable(str, &i, data);
			var = ft_handel_expend(head, var, join);
			join = NULL;
			join = ft_strjoin(join, var);
			free(var);
		}
		else if (str[i] == c)
		{
			res = ft_qutes(str, &i, c, data, flag);
			join = ft_strjoin(join, res);
			free(res);
		}
		else
		{
			join = ft_new_strjoin(join, str[i]);
			i++;
		}
	}
	if (ff == 1)
	{
		var = ft_strdup("");
		var = ft_handel_expend(head, join, var);
		join = NULL;
		join = ft_strjoin(join, var);
	}
	return (join);
}

t_list	*ft_handel_qutes(t_list *head, t_data *data, int flag)
{
	t_list	*command;
	char	*cmd;
	t_list	*tmp;

	tmp = head;
	command = NULL;
	while (head)
	{
		cmd = ft_remove(head->content, data, flag, &command);
		if (cmd)
			ft_lstadd_back(&command, ft_lstnew(cmd));
		head = head->next;
	}
	ft_lstclear(&tmp, free);
	return (command);
}

int	ft_lstsize_v1(t_list *head)
{
	int	i;
	int	flag;

	i = 0;
	flag = 0;
	while (head)
	{
		if ((head->content)[0] || flag == 1)
		{
			i++;
			flag = 1;
		}
		head = head->next;
	}
	if (i == 0)
		i++;
	return (i);
}

char	**last_command(t_list *head)
{
	int		i;
	int		size;
	char	**arr;
	int		flag;

	i = 0;
	flag = 0;
	size = ft_lstsize_v1(head);
	arr = malloc(sizeof(char *) * (size + 1));
	while (head)
	{
		if ((head->content)[0] || flag == 1)
		{
			arr[i] = ft_strdup(head->content);
			flag = 1;
			i++;
		}
		head = head->next;
	}
	if (i == 0)
	{
		arr[i] = ft_strdup("");
		i++;
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

void	ft_exec_command(t_data *data, int cls, t_list *head)
{
	char	**command;
	char	*cmd;
	int		flag;

	flag = 0;
	command = last_command(head);
	cmd = ft_check_command(data, command[0]);
	if (cmd[0])
	{
		if (ft_strrchr(cmd, '/') && opendir(cmd))
		{
			ft_print_error(cmd, "Is a directory");
			data->status = 126;
		}
		else
		{
			if (data->check_Cmd == 1)
				if (!ft_builting(data, head))
					return ;
			data->pid = fork();
			if (!data->pid)
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
				if ((flag = ft_builting(data, head) == 1))
					execve(cmd, command, data->env);
				if (flag == 1)
				{
					ft_print_error(cmd, strerror(errno));
					exit(127);
				}
				exit(0);
			}
		}
	}
}

void	ft_command(t_list *head, t_data *data, int cls)
{
	ft_handel_redic(&(head->redic), data, 1);
	if (head->int_file == 2)
		data->in = head->in;
	if (data->intfile == -1 || data->outfile == -1 || !head->command)
	{
		data->status = 1;
		return ;
	}
	ft_exec_command(data, cls, head->command);
}
