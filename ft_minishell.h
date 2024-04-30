/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abquaoub <abquaoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 04:34:29 by abquaoub          #+#    #+#             */
/*   Updated: 2024/04/30 21:10:12 by abquaoub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_MINISHELL_H
# define FT_MINISHELL_H

# include "libft/libft.h"
# include <dirent.h>
# include <fcntl.h>
# include <libgen.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

typedef struct Data
{
	int		intfile;
	int		outfile;
	int		status;
	int		fd[2];
	int		in;
	int		out;
	int		fd1;
	int		fd0;
	int		red;
	int		pid;
	char	**env;
	int		exec;
}			t_data;

typedef struct quotes
{
	int		cp;
	int		cq;
	int		cs;
	int		en;
	int		bk;
}			t_quotes;

typedef struct STR
{
	char	*str;
	char	*join;
	char	c;
	int		i;
	int		flag;
}			t_str;
void		ft_handel_redic(char *line, t_list **command_list, t_list **redic,
				t_data *data);
void		ft_exec_utils(t_list *head, t_data *data);
void		ft_exec_redic(t_list *head, t_data *data);
void		ft_lstnew_back(t_list **head, char *cont, int flag);
void		ft_exec_redic(t_list *head, t_data *data);
void		ft_nested_pip_syntax(t_list *head, t_data *data);
t_list		*ft_split_rediction(t_list *head, t_list **new);
int			ft_check_syntax(t_list *head);
void		ft_nested_pip_ex(t_list *head, t_data *data, int fd1, int fd0);
void		ft_print_tree(t_list *head);
t_list		*ft_split_linked_pip(char *str, char c, int flag);
t_list		*ft_nested_pip(char *line, t_data *data);
t_list		*split_end_or(char *str, char *set, int flag);
void		ft_check_syntax_command(t_list *head, t_data *data);
void		ft_check_string(char *str, t_data *data);
void		ft_display(t_list *ptr);
int			access_intfile(char *path);
int			access_outfile(char *path);
void		red(void);
void		cyan(void);
void		yellow(void);
void		reset(void);
char		*ft_pwd(int flags);
void		ft_check_quotes(char c, t_quotes *data);
void		ft_command(char *line, t_data *data);
int			ft_read_stdin(char *end);
void		ft_free(char **str);
char		*ft_check_command(char *cmd);
char		**last_command(t_list *head);
int			access_outfile_herdoc(char *path);
void		initialize(t_quotes *data);
char		*ft_new_strjoin(char *str, char c);
#endif
