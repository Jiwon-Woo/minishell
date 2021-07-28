#include "minishell.h"

typedef	struct s_quote
{
	int	q_single;
	int	q_double;
	int	q_single_index;
	int	q_double_index;
}	t_quote;


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
	str = 0;
	return (ret);
}

t_list	*get_arg_list(char *line)
{
	t_list 	*arg_list = 0;
	char	*arg;
	int	quote = 0;

	while (ft_isspace(*line))
		line++;
	while (ft_isspace(*line) == 0 && *line != 0)
		arg = str_append_char(arg, *(line++));
	ft_lstadd_back(&arg_list, ft_lstnew(arg));
	while (*line)
	{
		arg = ft_strdup("");
		while (ft_isspace(*line))
			line++;
		if (*line == '\"' || *line == '\'')
		{
			if (*line == '\"')
				quote = 2;
			else
				quote = 1;
			line++;
		}
		if (quote == 1)
		{
			while (*line != '\'' && *line != 0)
				arg = str_append_char(arg, *(line++));
			if (*line == '\'')
			{
				quote = 0;
				line++;
			}
		}
		else if (quote == 2)
		{
			while (*line != '\"' && *line != 0)
				arg = str_append_char(arg, *(line++));
			if (*line == '\"')
			{
				quote = 0;
				line++;
			}
		}
		else 
		{
			while (ft_isspace(*line) == 0 && *line != 0)
				arg = str_append_char(arg, *(line++));
		}
		if (quote != 0)
			exit(1);
		if (arg != NULL)
			ft_lstadd_back(&arg_list, ft_lstnew(arg));
	}
	return (arg_list);
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

int	main(int argc, char **argv, char **envp)
{
	char	buffer[1024];
	char	*prompt;
	t_list 	*arg_list;
	char	*line;
	char	**arg_arr;
	t_quote	quote;
	int	i;
	getcwd(buffer, 1024);
	prompt = ft_strjoin(buffer, "$ ");
	line = readline(prompt);
	init_quote(&quote);
	while (line != NULL)
	{
		add_history(line);
		check_quote(line, &quote);
		arg_list = get_arg_list(line);
		arg_arr = list_to_char_arr(arg_list);
		i = 0;
		while (arg_arr[i])
			printf("%s\n", arg_arr[i++]);
		line = readline(prompt);
	}
	free(prompt);
	prompt = 0;
	return (0);
}

