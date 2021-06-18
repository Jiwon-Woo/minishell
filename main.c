#include "minishell.h"

int	is_numeric(char **split, int arg_num)
{
	int	i = 0;
	int	j;

	if (split[1] == 0)
		return (0);
	j = -1;
	while (++j < ft_strlen(split[1]))
	{
		if (j == 0 && (split[1][j] == '-' || split[1][j] == '+'))
			continue;
		if (!(split[i][j] >= '0' && split[i][j] <= '9'))
			return (-1);
	}
	if (arg_num == 1)
		return (1);
	return (-42);
}

int	main()
{
	// int		ret;
	char	*line;
	char	buffer[1024];
	char	*prompt;
	t_list	*command = 0;
	t_list	*first = 0;

	getcwd(buffer, 1024);
	prompt = ft_strjoin(buffer, "$ ");
	line = readline(prompt);
	while (line != NULL)
	{
		ft_lstadd_front(&command, ft_lstnew(line));
		printf("%d\n", ft_lstsize(command));
		first = command;
		while (command)
		{
			printf("%s\n", (char *)command->content);
			command = command->next;
		}
		command = first;
		line = readline(prompt);
	}
	return (0);
}

