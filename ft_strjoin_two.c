#include "minishell.h"

char **append_strarr(char **str1, char **str2)
{
	char 	**ret_strarr;
	int 	idx;
	int		i;

	ret_strarr = (char **)malloc(sizeof(char *) * (get_arg_size(str1) + get_arg_size(str2) + 1));
	if (!ret_strarr)
		exit(1);
	i = 0;
	idx = 0;
	while(str1 && str1[i])
		ret_strarr[idx++] = ft_strdup(str1[i++]);
	i = 0;
	while(str2 && str2[i])
		ret_strarr[idx++] = ft_strdup(str2[i++]);
	ret_strarr[idx] = 0;
	return(ret_strarr);
}