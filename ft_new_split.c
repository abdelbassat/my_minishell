/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_new_split.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abquaoub <abquaoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 16:59:46 by abquaoub          #+#    #+#             */
/*   Updated: 2024/05/09 10:28:43 by abquaoub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

void	ini_str(t_str *data)
{
	data->str = NULL;
	data->join = NULL;
	data->flag = 0;
	data->c = 0;
}

void	ft_lstnew_back(t_list **head, char *cont, int flag)
{
	t_list	*node;

	node = ft_lstnew(cont);
	if (node->content[0])
	{
		node->x = flag;
		ft_lstadd_back(head, node);
	}
	else
		ft_lstdelone(node, free);
}

void	charset(char c, char *set, t_str *strr)
{
	int	i;

	i = 0;
	while (set[i])
	{
		if (c == set[i])
		{
			strr->c = set[i];
			break ;
		}
		i++;
	}
}

void	ft_check_quotes(char c, t_quotes *data)
{
	if (c == '(' && !data->cq && !data->cs)
		data->cp++;
	else if (c == ')' && !data->cq && !data->cs)
		data->cp--;
	if (c == 34 && !data->cq && !data->cs)
		data->cq++;
	else if (c == 34 && !data->cs)
		data->cq--;
	if (c == 39 && !data->cq && !data->cs)
		data->cs++;
	else if (c == 39 && !data->cq)
		data->cs--;
	if (c == '&' && !data->cq && !data->cs && !data->cp)
		data->en++;
	if (c == '`' && !data->cq && !data->cs && !data->cp)
		data->en++;
}

void	utils_split(t_str *strr, t_list **head, int *i, int flag)
{
	char	*save;

	save = NULL;
	if (strr->join)
	{
		ft_lstnew_back(head, strr->join, 0);
		free(strr->join);
	}
	if (strr->str[*i + 1] || strr->str[*i] == strr->c)
	{
		if (strr->str[*i] != ' ')
			strr->join = ft_new_strjoin(NULL, strr->str[*i]);
		if (strr->str[*i + 1] == strr->c && strr->c != ' ')
		{
			*i = *i + 1;
			save = strr->join;
			strr->join = ft_strjoin(strr->join, strr->join);
			free(save);
		}
		if (strr->str[*i] != ' ')
		{
			ft_lstnew_back(head, strr->join, flag);
			free(strr->join);
		}
	}
	strr->join = NULL;
	strr->flag = 0;
}

t_list	*split_end_or(char *line, char *set, int check)
{
	int			i;
	t_list		*head;
	t_quotes	data;
	t_str		strr;
	int			ff;

	head = NULL;
	i = 0;
	ini_str(&strr);
	initialize(&data);
	strr.str = line;
	ff = 4;
	if (!strcmp(set, "<> "))
		ff = 2;
	while (line[i])
	{
		ft_check_quotes(line[i], &data);
		charset(line[i], set, &strr);
		if (line[i] == strr.c && (line[i + 1] == strr.c || check == 0)
			&& !data.cp && !data.cq && !data.cs)
			strr.flag = 1;
		else
			strr.join = ft_new_strjoin(strr.join, line[i]);
		if (strr.flag == 1 || !line[i + 1])
			utils_split(&strr, &head, &i, ff);
		i++;
	}
	return (head);
}
