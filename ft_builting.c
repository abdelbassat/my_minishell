/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builting.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abquaoub <abquaoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 19:25:03 by abquaoub          #+#    #+#             */
/*   Updated: 2024/05/10 11:12:36 by abquaoub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

void	ft_sort_name(t_list **head)
{
	int		swapped;
	t_list	*ptr1;
	t_list	*last_node;
	char	*temp;

	last_node = NULL;
	if (head == NULL)
		return ;
	swapped = 1;
	while (swapped)
	{
		swapped = 0;
		ptr1 = *head;
		while (ptr1->next != last_node)
		{
			if (strcmp((char *)ptr1->content, (char *)ptr1->next->content) > 0)
			{
				temp = ptr1->content;
				ptr1->content = ptr1->next->content;
				ptr1->next->content = temp;
				swapped = 1;
			}
			ptr1 = ptr1->next;
		}
		last_node = ptr1;
	}
}

void	ft_print_list_export(t_list *head)
{
	t_list	*tmp;
	int		c;

	tmp = head;
	ft_sort_name(&head);
	ft_link_node(tmp);
	c = 34;
	while (tmp)
	{
		if (ft_strchr_edit((char *)tmp->content, '=') == 0)
			printf("declare -x %s=%c%s%c\n", (char *)tmp->key, c,
				(char *)tmp->value, c);
		else
			printf("declare -x %s\n", (char *)tmp->content);
		tmp = tmp->next;
	}
}

void	ft_export(t_list *command_list, t_data *data)
{
	t_list	*test;

	ft_link_node(command_list->next);
	if (!command_list->next)
	{
		test = NULL;
		ft_buffer_to_list_v1(&test, data->env_list);
		ft_link_node(test);
		ft_print_list_export(test);
		ft_lstclear(&test, free);
		return ;
	}
	else
	{
		ft_link_node(data->env_list);
		ft_if_exist_var(data->env_list, command_list->next);
		if (command_list->next)
			data->env_list = ft_fill_out(&data->env_list, command_list->next);
	}
}

void	ft_unset(t_list *command_list, t_data *data)
{
	t_list	*temp;

	ft_link_node(data->env_list);
	ft_link_node(command_list->next);
	temp = command_list->next;
	while (temp)
	{
		ft_remove_if(&data->env_list, (char *)temp->content);
		temp = temp->next;
	}
}

void	ft_ECHO(t_list *command, t_data *data)
{
	ft_link_node(data->env_list);
	ft_echo(data->env_list, command);
}

void	ft_pwd_m(void)
{
	char	*path;

	path = (char *)malloc(sizeof(char) * 1024);
	if (!path)
		return ;
	path = getcwd(path, 1024);
	printf("%s\n", path);
	free(path);
}

int	ft_builting(t_data *data, t_list *command)
{
	char *str = ft_strtrim(command->content, " ");

	if (!strcmp(str, "env"))
		ft_print_list(data->env_list);
	else if (!strcmp(str, "export"))
		ft_export(command, data);
	else if (!strcmp(str, "unset"))
		ft_unset(command, data);
	else if (!strcmp(str, "echo"))
		ft_ECHO(command, data);
	else if (!strcmp(str, "cd"))
		ft_cd(command, data);
	else if (!strcmp(str, "pwd"))
		ft_pwd_m();
	else if (!strcmp(str, "exit"))
		ft_exit(command, data);
	else
		return (1);
	data->status = 0;
	return (0);
}