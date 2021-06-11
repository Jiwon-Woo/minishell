#ifndef MINISHELL_H
# define MINI_SHELL_H

# include <stdio.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdlib.h>

typedef struct	s_history
{
	char		*line;	
	t_history	*next;
}				t_history;

typedef struct	s_env
{
	char		*name;
	char		*value;
	t_env		*next;
}				t_env;

typedef struct	s_echo
{
	char		*str;
}

typedef struct	s_pwd
{
	char		*cur_dir;
}

#endif

