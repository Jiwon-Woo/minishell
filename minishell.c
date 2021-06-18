#include "minishell.h"

int		ft_strlen(char *str)
{
	int	len;

	len = 0;
	if (str == 0)
		return (-1);
	while (str[len])
		len++;
	return (len);
}

char	*ft_strjoin(char *s1, char *s2)
{
	char	*str;
	int		len1;
	int		len2;
	int		index;
	int		i;

	if (s1 == 0 || s2 == 0)
		return (0);
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	str = (char *)malloc(sizeof(char) * (len1 + len2 + 1));
	if (str == 0)
		return (0);
	index = 0;
	i = 0;
	while (i < len1)
		str[index++] = s1[i++];
	i = 0;
	while (i < len2)
		str[index++] = s2[i++];
	str[index] = '\0';
	return (str);
}

void	sigint_handler(int signo)
{
	printf("\n");
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}

int	main()
{
	static char	*line;
	char	buffer[1024];
	char	*prompt;

	signal(SIGINT, (void *)sigint_handler);
	getcwd(buffer, 1024);
	prompt = ft_strjoin(buffer, "$ ");
	line = readline(prompt);
	while (line != NULL)
	{
		free(line);
		line = readline(prompt);
		add_history(line);
		rl_redisplay();
	}
	printf("%s\n", line);
	free(line);
	line = 0;
	return (0);
}