#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <math.h>
# include <limits.h>
# include <fcntl.h>
# include <sys/stat.h>

int		ft_strlen(char *str);
char	*empty_line(void);
void	str_char_append(char **str, char c);
int		ret_gnl(char **line, char *gnl, int rs);
int		get_next_line(int fd, char **line);

int		is_space(char s);
int		factor_num(char *s);
char	**factor_len(char **factor, char *s, int factor_num);
void	factor_split(char **factor, char *s, int factor_num);
char	**free_two_dimension(char **word, int num);
char	**ft_split_space(char *s);
int 	get_arg_num(char *line);

int		atoi_sign(char *str);
int		ft_atoi(char *str, int *integer);

int 		get_arg_num(char *line);

#endif