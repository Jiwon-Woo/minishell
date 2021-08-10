#include "minishell.h"

int	get_arg_size(char **arg)
{
	int	size;

	size = 0;
	if (arg == 0)
		return (size);
	while (arg[size])
		size++;
	return (size);
}

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

char	*str_append_char(char *str, char c)
{
	char	*ret;
	int		i;

	if (str == 0)
		str = ft_strdup("");
	ret = (char *)malloc(ft_strlen(str) + 2);
	if (!ret)
		exit(1);
	i = -1;
	while (++i < ft_strlen(str))
		ret[i] = str[i];
	ret[i] = c;
	ret[i + 1] = 0;
	free(str);
	str = 0;
	return (ret);
}