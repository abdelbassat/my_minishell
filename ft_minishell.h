/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abquaoub <abquaoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 04:34:29 by abquaoub          #+#    #+#             */
/*   Updated: 2024/04/29 17:07:34 by abquaoub         ###   ########.fr       */
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
	int		ex;
	int		ex1;
	int		red;
	int		pid;
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
	char	*str1;
	char	*str2;
	char	c;
	int		i;
}			t_str;
void		ft_exec_redic(t_list *head, t_data *data);
void		ft_nested_pip_syntax(t_list *head, t_data *data);
t_list		*ft_split_rediction(t_list *head, t_list **new);
int			ft_check_syntax(t_list *head);
void		ft_nested_pip_ex(t_list *head, char **env, t_data *data, int fd1,
				int fd0);
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
void		ft_pwd(int flags, t_data *data);
void		ft_check_quotes(char c, t_quotes *data);
void		ft_command(char *line, char **env, t_data *data, int fd1, int fd0,
				int cls);
void		ft_print(char **arr);
t_list		*ft_new_split(char *str, t_quotes data);
void		ft_free(char **str);
char		*ft_check_command(char *cmd);
void		ft_print(char **arr);
char		**last_command(t_list *head);
int			access_outfile_herdoc(char *path);
void		initialize(t_quotes *data);
char		*ft_new_strjoin(char *str, char c);
#endif
