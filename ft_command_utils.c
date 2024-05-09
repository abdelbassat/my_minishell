/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_command_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abquaoub <abquaoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/05 01:37:39 by abquaoub          #+#    #+#             */
/*   Updated: 2024/05/09 10:12:35 by abquaoub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

char	*ft_check_command(t_data *data, char *command)
{
	char	*path;
	t_list	*head;
	char	*cmd;
	t_list	*tmp;

	path = ft_getenv(data, "PATH");
	if (path && !ft_strrchr(command, '/') && ft_strtrim(command, " ")[0]
		&& !ft_strrchr(command, '.'))
	{
		head = split_end_or(path, ":", 0);
		tmp = head;
		cmd = ft_strjoin("/", command);
		while (head)
		{
			if (head->x == 0)
			{
				path = ft_strjoin(head->content, cmd);
				if (access(path, F_OK) == 0)
					return (free(cmd), ft_lstclear(&tmp, free), path);
			}
			head = head->next;
		}
		free(cmd);
		ft_lstclear(&tmp, free);
	}
	return (ft_strdup(command));
}

void	ft_display(t_list *ptr)
{
	printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
	while (ptr)
	{
		printf("--%s--\n", (char *)ptr->content);
		ptr = ptr->next;
	}
	printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
}

char	**create_command(t_list *head)
{
	int i;
	int size;
	char **command;

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