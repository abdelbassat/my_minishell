/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abquaoub <abquaoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 04:34:29 by abquaoub          #+#    #+#             */
/*   Updated: 2024/05/12 18:42:36 by abquaoub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_MINISHELL_H
# define FT_MINISHELL_H

# include "libft/libft.h"
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <libgen.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

extern int	errno;

typedef struct freee
{
	t_list	*head;
	t_list	*new_list;
}			t_free;

typedef struct Data
{
	char	*save;
	int		check_Cmd;
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
	t_list	*env_list;
	int		t;
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

void		ft_buffer_to_list(t_list **head, char **command);
char		*ft_revers_to_base64(char *str);
int			ft_reverse(char *str);
char		*ft_base64(char *str);
char		*ft_convert(int c);
void		free_list(t_free *head, int flag);
int			ft_count_qutes(char *line, t_quotes *qutes);
void		ft_handel_redic(t_list **redic, t_data *data, int flag);
void		ft_exec_utils(t_list *head, t_data *data, int flag);
void		ft_exec_redic(t_list *head, t_data *data, int flag);
void		ft_lstnew_back(t_list **head, char *cont, int flag);
void		ft_exec_redic(t_list *head, t_data *data, int flag);
int			ft_check_syntax(t_list *head, int flag);
void		ft_nested_pip_ex(t_list *head, t_data *data, int fd1, int fd0);
t_list		*ft_nested_pip(char *line, t_data *data);
t_list		*split_end_or(char *str, char *set, int flag);
void		ft_check_string(char *str, t_data *data);
void		ft_display(t_list *ptr);
int			access_intfile(char *path);
int			access_outfile(char *path);
void		ft_free_list_node(t_list **head);
char		*ft_qutes(char *str, int *i, char c, t_data *data, int flag);
void		ft_free_trees(t_list **head);
void		red(void);
void		cyan(void);
void		yellow(void);
void		reset(void);
t_list		*ft_handel_qutes(t_list *head, t_data *data, int flag);
void		ft_Error(char *path);
char		*ft_pwd(int flags);
void		ft_check_quotes(char c, t_quotes *data);
void		ft_command(t_list *head, t_data *data, int cls);
void		ft_split_rediction(char *line, t_list **new);
void		ft_free_tree(t_list *head);
void		ft_syntax(char *line, t_data *data);
char		*join_command(t_list *head);
int			ft_read_stdin(char *end, t_data *data);
void		ft_free(char **str);
char		*ft_check_command(t_data *data, char *command);
char		**last_command(t_list *head);
int			access_outfile_herdoc(char *path);
int			ft_wild_card_string_matching(char *s, char *p);
t_list		*ft_wild_card(char *p);
int			ft_check_wildcard(char *line);
t_list		*ft_join(t_list *head);
void		initialize(t_quotes *data);
char		*ft_return_variable(char *str, int *i, t_data *data);
char		*ft_new_strjoin(char *str, char c);
char		*ft_getenv(t_data *data, char *search);
void		close_fd(t_list **tmp, t_data *data, int fd0);
void		check_eo(t_list *head, t_data *data, int fd1, int fd0);
void		wait_proccess(t_data *data, int proc);
char		*ft_handel_expend(t_list **head, char *var, char *join);
char		*ft_search_if_key_exist_env_home(t_list **env, char *head,
				t_data *data);
t_list		*ft_create_var(char *command, char c);
void		ft_print_list(t_list *head);
void		ft_buffer_to_list(t_list **head, char **command);
void		ft_if_exist_var(t_list *env, t_list *head);
void		ft_link_node(t_list *head);
void		ft_remove_if(t_list **head, char *target);
t_list		*ft_fill_out(t_list **env_list, t_list *head);
void		ft_echo(t_list *env, t_list *command);
int			ft_strchr_edit(const char *s, int c);
void		ft_cd(t_list *head, t_data *data);
char		*ft_strdup_if(char *str, char c);
void		ft_buffer_to_list_v1(t_list **head, t_list *command);
int			ft_builting(t_data *data, t_list *command);
void		ft_handle_signals(void);
void		ft_exit(t_list *head, t_data *data);
void		handle_signal(int sig);
void		handle_signal_cat(int sig);
char		*ft_remove(char *str, t_data *data, int flag, t_list **head);
#endif
