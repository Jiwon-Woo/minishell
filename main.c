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
	// free(str);
	str = 0;
	return (ret);
}

t_list	*get_arg_list(char *line)
{
	t_list 	*arg_list = 0;
	char	*arg;
	int	qnote = 0;

	while (ft_isspace(*line))
		line++;
	// arg_num = 1;
	while (ft_isspace(*line) == 0 && *line != 0)
		arg = str_append_char(arg, *(line++));
	ft_lstadd_back(&arg_list, ft_lstnew(arg));
	// free(arg);
	arg = 0;
	while (*line)
	{
		while (ft_isspace(*line))
			line++;
		if (*line == '\"' || *line == '\'')
		{
			if (*line == '\"')
				qnote = 2;
			else
				qnote = 1;
			line++;
		}
		if (qnote == 1)
		{
			while (*line != '\'' && *line != 0)
				arg = str_append_char(arg, *(line++));
			if (*line == '\'')
			{
				qnote = 0;
				line++;
			}
		}
		else if (qnote == 2)
		{
			while (*line != '\"' && *line != 0)
				arg = str_append_char(arg, *(line++));
			if (*line == '\"')
			{
				qnote = 0;
				line++;
			}
		}
		else 
		{
			while (ft_isspace(*line) == 0 && *line != 0)
				arg = str_append_char(arg, *(line++));
		}
		if (qnote != 0)
			exit(1);
		if (arg != NULL)
		{
			ft_lstadd_back(&arg_list, ft_lstnew(arg));
			free(arg);
			arg = 0;
		}
	}
	return (arg_list);
}

int	main(int argc, char **argv, char **envp)
{
	char	buffer[1024];
	char	*prompt;
	t_list 	*arg_list;
	char	*line;
	t_list	*first;

	// signal(SIGINT, (void *)sigint_handler);
	// signal(SIGINT, SIG_IGN);
	getcwd(buffer, 1024);
	prompt = ft_strjoin(buffer, "$ ");
	line = readline(prompt);
	while (line != NULL)
	{
		add_history(line);
		arg_list = get_arg_list(line);
		printf("lstsize : %d\n", ft_lstsize(arg_list));
		first = arg_list;
		while (arg_list)
		{
			// printf("%s\n", (char *)arg_list->content);
			arg_list = arg_list->next;
		}
		arg_list = first;
		line = readline(prompt);
	}
	// free(prompt);
	prompt = 0;
	return (0);
}

