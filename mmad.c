/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mmad.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abquaoub <abquaoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 14:37:46 by abquaoub          #+#    #+#             */
/*   Updated: 2024/05/07 14:37:52 by abquaoub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

void	ft_cd(t_list *head)
{
	char	*path;
	t_list	*temp;
	char	*new_dir;

	path = (char *)malloc(sizeof(char) * 100);
	if (!path)
		return ;
	temp = head;
	path = getcwd(path, 1024);
	if (ft_lstsize(temp) == 2)
	{
		new_dir = ft_strdup((char *)temp->next->content);
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
		chdir("/nfs/homes/mmad");
	free(path);
}

void	ft_buffer_to_list(t_list **head, char **command)
{
	int	i;

	i = 0;
	while (command[i])
	{
		ft_lstadd_back(head, ft_lstnew((char *)command[i]));
		i++;
	}
}

///////////////////////////////////
