#include "minishell.h"

char	*str_append_char(char *str, char c)
{
	char	*ret;
	int	len;
	int	i;
	
	if (str == 0)
		str = ft_strdup("");
	ret = (char *)malloc(ft_strlen(str) + 2);
	i = -1;
	while (++i < ft_strlen(str))
		ret[i] = str[i];
	ret[i] = c;
	ret[i + 1] = 0;
	free(str);
	str = 0;
	return (ret);
}



char	**list_to_char_arr(t_list *arg_list)
{
	t_list	*first;
	int	lst_size;
	int	idx;
	char	**arg_arr;

	lst_size = ft_lstsize(arg_list);
	arg_arr = (char **)malloc(sizeof(char *) * (lst_size + 1));
	idx = -1;
	while (++idx < lst_size)
	{
		arg_arr[idx] = (char *)arg_list->content;
		arg_list = arg_list->next;
	}
	arg_arr[lst_size] = (char *)0;
	arg_list = first;
	return (arg_arr);
}