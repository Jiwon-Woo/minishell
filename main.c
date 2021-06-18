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

	getcwd(buffer, 1024);
	prompt = ft_strjoin(buffer, "$ ");
	line = readline(prompt);
	while (line != NULL)
	{
		// printf("arg_num = %d\n", get_arg_num(line + 7));
		// split = ft_split_space(line);
		// arg_num = 0;
		// while (split[arg_num])
		// 	arg_num++;
		// if (ft_strncmp(split[0], "echo", 5) == 0)
		// {
		// 	int	i = 0;
		// 	while (++i < arg_num - 1)
		// 	{
		// 		if (i == 1 && ft_strncmp(split[i], "-n", 3) == 0)
		// 			continue;
		// 		else
		// 		{
		// 			write(1, split[i], ft_strlen(split[i]));
		// 			write(1, " ", 1);
		// 		}
		// 	}
		// 	if (arg_num > 1 && ft_strncmp(split[1], "-n", 3) == 0)
		// 		write(1, split[i], ft_strlen(split[i]));
		// 	else
		// 		printf("%s\n", split[i]);
		// }
		// else if (ft_strncmp(split[0], "pwd", 4) == 0)
		// {
		// 	if (arg_num == 1)
		// 		printf("%s\n", buffer);
		// 	else
		// 		printf("pwd: too many arguments\n");
		// }
		// else if (ft_strncmp(split[0], "exit", 4) == 0)
		// {
		// 	printf("exit\n");
		// 	is_num = is_numeric(split, arg_num);
		// 	if (is_num == 0)
		// 		exit(0);
		// 	else if (is_num == 1)
		// 	{
		// 		ft_atoi(split[1], &exit_num);
		// 		exit(exit_num % 256);
		// 	}
		// 	else if (is_num == -1)
		// 	{
		// 		printf("exit: %s numeric argument required\n", split[1]);
		// 		exit(255);
		// 	}
		// 	else
		// 		printf("exit: too many arguments\n");
		// }
		// else
		// 	printf("zsh: command not found: %s\n", line);
		free(line);
		line = readline(prompt);
	}
	printf("%s\n", line);
	free(line);
	line = 0;
	return (0);
}

