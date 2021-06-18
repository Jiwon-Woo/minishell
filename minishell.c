#include "minishell.h"

static char	*line;

void	sigint_handler(int signo)
{
	printf("\n");
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}

void	sigquit_handler(int signo)
{
	exit(0);
}

int			ft_isspace(char c)
{
	return ((c >= 9 && c <= 13) || (c == 32));
}

int mini_pwd(char **envp)
{
	pid_t	pid;
	char	*arg[] = {"pwd", (char *)0};
	int		status;

	pid = fork();
	wait(&status);
	if (pid == 0)
	{
		execve("/bin/pwd", arg, envp);
	}
	else
	{
		return (status);
	}
	return (-1);
}

int mini_env(char **envp)
{
	pid_t	pid;
	char	*arg[] = {"env", (char *)0};
	int		status;

	pid = fork();
	wait(&status);
	if (pid == 0)
	{
		execve("/usr/bin/env", arg, envp);
	}
	else
	{
		return (status);
	}
	return (-1);
}

int mini_cd(char *dir, char **envp)
{
	pid_t	pid;
	char	*arg[] = {"cd", "../", (char *)0};
	int		status;

	pid = fork();
	wait(&status);
	if (pid == 0)
	{
		execve("/usr/bin/cd", arg, envp);
	}
	else
	{
		return (status);
	}
	return (-1);
}

int interpret(char *line, char **envp)
{
	int		i;
	int		ret_value;

	if (ft_strncmp(line, "pwd", 3) == 0)
	{
		i = 3;
		while (line[i])
		{
			if (!ft_isspace(line[i++]))
				return (-1);
		}
		ret_value = mini_pwd(envp);
		return (ret_value);
	}
	if (ft_strncmp(line, "env", 3) == 0)
	{
		i = 3;
		while (line[i])
		{
			if (!ft_isspace(line[i++]))
				return (-1);
		}
		ret_value = mini_env(envp);
		return (ret_value);
	}
	if (ft_strncmp(line, "cd", 2) == 0)
	{
		char **dir;
		dir = ft_split(line, ' ');
		ret_value = mini_cd(dir[1], envp);
		return (ret_value);
	}
	return (-1);
}

int	main(int argc, char **argv, char **envp)
{
	char	buffer[1024];
	char	*prompt;

	signal(SIGINT, (void *)sigint_handler);
	// signal(SIGINT, SIG_IGN);
	getcwd(buffer, 1024);
	prompt = ft_strjoin(buffer, "$ ");
	line = readline(prompt);
	add_history(line);
	interpret(line, envp);
	while (line != NULL)
	{
		free(line);
		line = readline(prompt);
		add_history(line);
		interpret(line, envp);
	}
	printf("line -> %s\n", line);
	//free(line);
	// line = 0;
	return (0);
}
