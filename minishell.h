#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdlib.h>
# include <string.h>

typedef struct	s_history
{
	char		*line;	
	struct s_history	*next;
}				t_history;

typedef struct	s_env
{
	char		*name;
	char		*value;
	struct s_env		*next;
}				t_env;

typedef struct	s_echo
{
	char		*str;
}				t_echo;

typedef struct	s_pwd
{
	char		*cur_dir;
}				t_pwd;

#endif

