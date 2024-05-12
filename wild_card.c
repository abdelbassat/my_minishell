/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wild_card.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abquaoub <abquaoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 10:21:59 by mmad              #+#    #+#             */
/*   Updated: 2024/05/12 14:15:10 by abquaoub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

//---------------------------------
/*
echo *.md
echo built*
echo oi*
echo *.supp
echo 'bye *' bo*s bye
echo 'bye *' mini* bye
echo *ME*
echo "*"
"*"
manual*
echo * | awk -v RS=" " '{print}' | sort
<*.txt
echo 'bye *' t*.c bye
echo *EMPTY hello READ* world
cat <README*
echo "pip*"
echo *bonus *.supp bonjour
 */
//--------------------
int	ft_wild_card_string_matching(char *s, char *p)
{
	int	s_index;
	int	p_index;
	int	last_wildcard_idx;
	int	s_backtrack_idx;
	int	next_to_wildcard_idx;

	s_index = 0;
	p_index = 0;
	last_wildcard_idx = -1;
	s_backtrack_idx = -1;
	next_to_wildcard_idx = -1;
	while (s[s_index] != '\0')
	{
		if (p[p_index] != '\0' && (p[p_index] == s[s_index]))
		{
			s_index++;
			p_index++;
		}
		else if (p[p_index] != '\0' && p[p_index] == '*')
		{
			last_wildcard_idx = p_index;
			next_to_wildcard_idx = ++p_index;
			s_backtrack_idx = s_index;
		}
		else if (last_wildcard_idx == -1)
			return (0);
		else
		{
			p_index = next_to_wildcard_idx;
			s_index = ++s_backtrack_idx;
		}
	}
	while (p[p_index] != '\0')
	{
		if (p[p_index] != '*')
			return (0);
		p_index++;
	}
	return (1);
}

t_list	*ft_wild_card(char *p)
{
	t_list			*buff;
	DIR				*dir;
	struct dirent	*dirptr;

	buff = NULL;
	if (!p)
		return (NULL);
	dir = opendir(".");
	if (!dir)
	{
		perror("");
		exit(EXIT_FAILURE);
	}
	else
	{
		while ((dirptr = readdir(dir)) && dirptr->d_name)
		{
			if (dirptr->d_name[0] != '.')
			{
				if (ft_wild_card_string_matching((char *)dirptr->d_name, p))
				{
					ft_lstadd_back(&buff, ft_lstnew((char *)dirptr->d_name));
				}
			}
		}
	}
	closedir(dir);
	if (!buff)
		buff = ft_lstnew(p);
	return (buff);
}

t_list	*ft_join(t_list *head)
{
	t_list	*new;
	t_list	*node;

	node = NULL;
	new = NULL;
	while (head)
	{
		if (ft_strchr(head->content, '*'))
		{
			node = ft_wild_card(head->content);
			if (!node || ft_check_wildcard(head->content) == 1)
				ft_lstadd_back(&new, ft_lstnew(head->content));
			else
				ft_lstadd_back(&new, node);
		}
		else
			ft_lstadd_back(&new, ft_lstnew(head->content));
		head = head->next;
	}
	return (new);
}
