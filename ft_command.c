/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_command.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abquaoub <abquaoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 19:14:52 by abquaoub          #+#    #+#             */
/*   Updated: 2024/05/13 12:29:22 by abquaoub         ###   ########.fr       */
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

char	*ft_qutes(t_str *strr, t_data *data, int flag)
{
	char	*var;
	char	*join;

	(strr->i)++;
	join = NULL;
	while (strr->str[strr->i] && strr->str[strr->i] != strr->c)
	{
		if (strr->str[strr->i] == '$' && strr->c == 34 && !flag)
		{
			var = ft_return_variable(strr->str, &(strr->i), data);
			join = ft_strjoin(join, var);
		}
		else
			join = ft_new_strjoin(join, strr->str[(strr->i)++]);
	}
	(strr->i)++;
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
	return (var);
}

void	ft_for_wild(char *str, t_list **head)
{
	t_list	*node;

	node = NULL;
	node = ft_wild_card(str);
	if (!node)
		node = ft_lstnew(str);
	ft_lstadd_back(head, node);
}
void	ft_handel_expend(t_list **head, t_str *strr, t_data *data)
{
	t_list	*expend;
	char	*var;

	expend = NULL;
	var = ft_return_variable(strr->str, &(strr->i), data);
	if (var && var[0])
	{
		expend = split_end_or(var, " ", 0);
		expend->content = ft_strjoin(strr->join, expend->content);
		while (expend->next)
		{
			ft_for_wild(expend->content, head);
			expend = expend->next;
			strr->join = NULL;
		}
		var = ft_strdup(expend->content);
	}
	strr->join = ft_strjoin(strr->join, var);
}

char	*ft_remove(char *str, t_data *data, int flag, t_list **head)
{
	t_str	strr;

	// problem  in expend     ex: var="ls          *"
	ini_str(&strr);
	strr.str = str;
	while (str[strr.i])
	{
		if (str[strr.i] == 34 || str[strr.i] == 39)
			strr.c = str[strr.i];
		if (str[strr.i] == '$' && !flag)
			ft_handel_expend(head, &strr, data);
		else if (str[strr.i] == strr.c && strr.c)
			strr.join = ft_strjoin(strr.join, ft_qutes(&strr, data, flag));
		else
		{
			strr.join = ft_new_strjoin(strr.join, str[strr.i]);
			strr.i++;
		}
	}
	return (strr.join);
}

t_list	*ft_handel_qutes(t_list *head, t_data *data, int flag)
{
	t_list	*command;
	char	*cmd;
	t_list	*tmp;
	t_list	*node;

	tmp = head;
	command = NULL;
	while (head)
	{
		if (ft_strchr(head->content, '*') && !ft_check_wildcard(head->content)
			&& !flag)
			node = ft_wild_card(head->content);
		else
		{
			cmd = ft_remove(head->content, data, flag, &command);
			node = ft_lstnew(cmd);
		}
		ft_lstadd_back(&command, node);
		node = NULL;
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
