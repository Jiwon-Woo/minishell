#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <math.h>
# include <limits.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <unistd.h>
# include <errno.h>
# include <string.h>
# include <stdbool.h>
# include <signal.h>
# include "libft/libft.h"

# define NONE 0
# define REDIRECT1 1
# define REDIRECT2 2
# define REDIRECT3 3
# define REDIRECT4 4
# define PIPE 5

int g_status;

typedef	struct s_quote
{
	int	q_single;
	int	q_double;
	int	q_single_index;
	int	q_double_index;
}	t_quote;

typedef	struct s_envp
{
	char	**envp_list;
	int		*sort_idx;
}	t_envp;

int		factor_num(char *s);
char	**factor_len(char **factor, char *s, int factor_num);
void	factor_split(char **factor, char *s, int factor_num);
void	free_two_dimension(char **word);
char	**ft_split_space(char *s);
void	ft_lstclear_two(t_list **lst, void (*del)(char **));

int		get_arg_size(char **arg);
char	*str_append_char(char *str, char c);
t_list	*get_arg_list(char *line, t_quote *quote);
t_list	*list_to_char_arr(t_list *arg_list, t_envp *envp);
int	is_separate(char *command);
int	get_list_size(t_list *arg_list);
int valid_quote(char **arg, char *line, t_quote *quote, int *i);
void invalid_quote(char **arg, char *line, int *i);

void	init_quote(t_quote *quote);
int	is_remain_quote(char *line, int idx, char quote);
void	check_quote(char *line, t_quote *quote);

int		atoi_sign(char *str);
int		ft_atoi(char *str, int *integer);

int		get_equal_idx(char *env_line);
char	*get_value(char *key, char **envp);
char	*ft_strjoin_with_free(char *s1, char *s2);

void	sigint_handler(int signo);
void	child_sigquit_handler(int signo);
void	child_sigint_handler(int signo);
void	redirect_sigint_handler(int signo);
void	set_signal();
void	sigquit_handler(int signo);

void	envp_add(t_envp *envp_, char *content);
void	envp_substitute(t_envp *envp_, char *content, int idx);
void	print_export(t_envp *envp);
void with_eq(char **arg_arr, t_envp *envp, int i);
void	without_eq(char **arg_arr, t_envp *envp, int i);
int	env_validation(char *name);
int	export_without_arg(char **arg_arr);
void	arr_swap(int *arr, int i, int j);
void	sort_envp_idx(t_envp *envp);
char	**get_env_ptr(char *key, char **envp);
void	init_envp(t_envp *envp, char **first_envp);

int		mini_export(char **arg_arr, t_envp *envp);
int mini_pwd();
int mini_env(char **arg, t_envp *envp);
int mini_cd(char **arg, t_envp *envp);
int mini_exit(char **arg_arr, bool is_parent);
int mini_unset(char **arg, t_envp *envp);

char *parse_path(t_envp *envp);
int	get_last_slash_idx(char *arg);
int get_file_type(char *path);
int	file_or_directory(char *arg);

int with_path(char **arg_arr, t_envp *envp);
int exec_file(char *path, char **arg_arr, t_envp *envp);
int without_path(char **arg_arr, t_envp *envp);
int	interpret(char **arg_arr, t_envp *envp);
int	interpret2(char **arg_arr, t_envp *envp);

char **append_strarr(char **str1, char **str2);

#endif