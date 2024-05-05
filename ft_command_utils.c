/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_command_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abquaoub <abquaoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/05 01:37:39 by abquaoub          #+#    #+#             */
/*   Updated: 2024/05/05 01:37:53 by abquaoub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

char	*ft_check_command(char *command, t_data *data)
{
	char	*env_path;
	char	*path;
	char	*cmd;
	char	**bins;
	int		i;

	env_path = getenv("PATH");
	bins = ft_split(env_path, ':');
	i = 0;
	while (bins[i])
	{
		path = ft_strjoin(bins[i], "/");
		cmd = ft_strjoin(path, command);
		data->check_Cmd = access(cmd, F_OK & X_OK);
		if (data->check_Cmd == 0)
			return (ft_free(bins), free(path), cmd);
		free(path);
		free(cmd);
		i++;
	}
	ft_free(bins);
	return (ft_strdup(command));
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