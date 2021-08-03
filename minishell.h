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
# define PIPE 1
# define REDIRECT1 2 // <
# define REDIRECT2 3 // >
# define REDIRECT3 4 // <<
# define REDIRECT4 5 // >>
# define AND 6
# define OR 7

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
	int		last_status;
}	t_envp;

int		factor_num(char *s);
char	**factor_len(char **factor, char *s, int factor_num);
void	factor_split(char **factor, char *s, int factor_num);
void	free_two_dimension(char **word);
char	**ft_split_space(char *s);
void	ft_lstclear_two(t_list **lst, void (*del)(char **));

char	*str_append_char(char *str, char c);
t_list	*get_arg_list(char *line, t_quote quote);
t_list	*list_to_char_arr(t_list *arg_list, t_envp *envp);
void	check_quote(char *line, t_quote *quote);
void 	init_quote(t_quote *quote);

int		atoi_sign(char *str);
int		ft_atoi(char *str, int *integer);

int		get_equal_idx(char *env_line);
char	*get_value(char *key, char **envp);
char	*ft_strjoin_with_free(char *s1, char *s2);

int	is_separate(char *command);

#endif