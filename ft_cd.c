/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abquaoub <abquaoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/05 12:20:44 by mmad              #+#    #+#             */
/*   Updated: 2024/05/09 11:20:36 by abquaoub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

char	*ft_search_if_key_exist_env_home(t_list **env, char *head, t_data *data)
{
	t_list	*temp_env;

	if (!head || !env || !(*env))
		return (NULL);
	else if (!strcmp(head, "?"))
		return (ft_itoa(data->status));
	temp_env = *env;
	if (temp_env->key && temp_env->value)
	{
		while (temp_env)
		{
			if (head)
			{
				if (strcmp((char *)temp_env->key, head) == 0)
					return ((char *)temp_env->value);
			}
			temp_env = temp_env->next;
		}
	}
	temp_env = *env;
	return (NULL);
}

char	*ft_getenv(t_data *data, char *search)
{
	if (search[0] == 0)
		return (ft_strdup("$"));
	ft_link_node(data->env_list);
	return (ft_search_if_key_exist_env_home(&data->env_list, search, data));
}

void	ft_cd(t_list *head, t_data *data)
{
	char *path = (char *)malloc(sizeof(char) * 100);
	if (!path)
		return ;
	t_list *temp = head;
	path = getcwd(path, 1024);
	if (ft_lstsize(temp) == 2)
	{
		char *new_dir = ft_strdup((char *)temp->next->content);
		if (strcmp((char *)temp->next->content, "..") != 0
			&& chdir(new_dir) != 0)
		{
			perror(ft_strjoin("minishell: cd: ", (char *)temp->next->content));
			return ;
		}
		if (strcmp((char *)temp->next->content, "..") == 0)
		{
			path = getcwd(path, 1024);
			chdir("..");
			path = getcwd(path, 1024);
			chdir(path);
		}
		free(new_dir);
	}
	else if (ft_lstsize(temp) > 2)
		printf("minishell: cd: too many arguments\n");
	else
		chdir(ft_getenv(data, "HOME"));
	// error
	free(path);
}