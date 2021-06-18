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
# include "libft/libft.h"

int		factor_num(char *s);
char	**factor_len(char **factor, char *s, int factor_num);
void	factor_split(char **factor, char *s, int factor_num);
char	**free_two_dimension(char **word, int num);
char	**ft_split_space(char *s);
int 	get_arg_num(char *line);

int		atoi_sign(char *str);
int		ft_atoi(char *str, int *integer);


#endif