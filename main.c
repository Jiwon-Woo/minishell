#include "minishell.h"

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

