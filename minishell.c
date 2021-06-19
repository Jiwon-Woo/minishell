#include "minishell.h"

//$처리, 환경변수 관련 함수, " ' 제거, string not in pwd ?, 파이프 및 리다이렉션, $? 처리, 보너스??

int		get_arg_size(char **arg)
{
	int size;

	size = 0;
	while (arg[size])
		size++;
	return (size);
}

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

int mini_pwd(char **arg, char **envp)
{
	pid_t	pid;
	// char	*arg[] = {"pwd", (char *)0};
	int		status;

	if (get_arg_size(arg) > 1)
	{
		printf("%s : too many arguments\n", arg[0]);
		return (-1);
	}
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

int mini_env(char **arg, char **envp)
{
	pid_t	pid;
	// char	*arg[] = {"env", (char *)0};
	int		status;

	pid = fork();
	wait(&status);
	if (pid == 0)
	{
		execve("/usr/bin/env", arg, envp);
		return (-1);
	}
	else
	{
		return (status);
	}
	return (-1);
}

int mini_cd(char **arg, char **envp)
{
	pid_t	pid;
	// char	*arg[] = {"cd", "../", (char *)0};
	int		status;

	if (get_arg_size(arg) > 2)
	{
		printf("%s : too many arguments\n", arg[0]);
		return (-1);
	}
	// pid = fork();
	// wait(&status);
	// if (pid == 0)
	// {
	// execve("/usr/bin/cd", arg, envp);
	return (chdir(arg[1]));
	// }
	// else
	// {
	// 	return (status);
	// }
	// return (-1);
}

int mini_exit(char **arg, char **envp)
{
	printf("exit\n");
	exit (0);
}

int mini_echo(char **arg, char **envp)
{
	pid_t	pid;
	// char	*arg[] = {"echo", "hello", (char *)0};
	int		status;

	pid = fork();
	wait(&status);
	if (pid == 0)
	{
		execve("/bin/echo", arg, envp);
	}
	else
	{
		return (status);
	}
	return (-1);
}

int interpret(char **arg_arr, char **envp)
{
	int		i;
	int		ret_value;

	if (arg_arr[0] == 0)
		return (0);
	if (ft_strncmp(arg_arr[0], "echo", 5) == 0)
		return (mini_echo(arg_arr, envp));
	if (ft_strncmp(arg_arr[0], "cd", 3) == 0)
		return (mini_cd(arg_arr, envp));
	if (ft_strncmp(arg_arr[0], "pwd", 4) == 0)
		return (mini_pwd(arg_arr, envp));
	if (ft_strncmp(arg_arr[0], "env", 4) == 0)
		return (mini_env(arg_arr, envp));
	if (ft_strncmp(arg_arr[0], "export", 7) == 0)
		return (mini_export(arg_arr, envp));
	if (ft_strncmp(arg_arr[0], "unset", 6) == 0)
		return (mini_unset(arg_arr, envp));
	if (ft_strncmp(arg_arr[0], "exit", 5) == 0)
		return (mini_exit(arg_arr, envp));
	return (-1);
}

void	check_quote(char *line, t_quote *quote)
{
	int	idx;

	idx = -1;
	while (++idx < ft_strlen(line))
	{
		if (line[idx] == '\'' && quote->q_double == 1)
		{
			quote->q_single *= -1;
			quote->q_single_index = idx;
		}
		else if (line[idx] == '\"' && quote->q_single == 1)
		{
			quote->q_double *= -1;
			quote->q_double_index = idx;
		}
	}
	if (quote->q_single != -1)
		quote->q_single_index = -1;
	if (quote->q_double != -1)
		quote->q_double_index = -1;
}

t_list	* get_arg_list(char *line, t_quote quote)
{
	t_list 	*arg_list = 0;
	char	*arg;
	int	i = 0;

	while (line[i])
	{
		arg = 0;
		while (ft_isspace(line[i]) && line[i] != 0)
			i++;
		if ((line[i] == '\"' && i != quote.q_double_index) ||
			(line[i] == '\'' && i != quote.q_single_index))
		{	
			if (line[i] == '\'')
			{
				arg = str_append_char(arg, line[i++]);
				while (line[i] != '\'' && line[i] != 0)
					arg = str_append_char(arg, line[i++]);
			}
			else
			{
				arg = str_append_char(arg, line[i++]);
				while (line[i] != '\"' && line[i] != 0)
					arg = str_append_char(arg, line[i++]);
			}
			while (ft_isspace(line[i]) == 0 && line[i] != 0)
				arg = str_append_char(arg, line[i++]);
		}
		else
		{
			while (ft_isspace(line[i]) == 0 && line[i] != 0)
				arg = str_append_char(arg, line[i++]);
		}
		if (arg != NULL)
			ft_lstadd_back(&arg_list, ft_lstnew(arg));
	}
	return (arg_list);
}

void init_quote(t_quote *quote)
{
	quote->q_single = 1;
	quote->q_double = 1;
	quote->q_single_index = -1;
	quote->q_double_index = -1;
}

char *make_prompt()
{
	char		*prompt;
	char		buffer[1024];

	getcwd(buffer, 1024);
	prompt = ft_strjoin(buffer, "$ ");
	return (prompt);
}

int	main(int argc, char **argv, char **envp)
{
	char		buffer[1024];
	char		*prompt;
	t_list		*arg_list;
	t_list		*cmd_list;
	char		**arg_arr;
	char		*line;
	t_quote		quote;

	signal(SIGINT, (void *)sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	getcwd(buffer, 1024);
	prompt = ft_strjoin(buffer, "$ ");
	line = readline(prompt);
	init_quote(&quote);
	while (line != NULL)
	{
		add_history(line);
		check_quote(line, &quote);
		arg_list = get_arg_list(line, quote);
		arg_arr = list_to_char_arr(arg_list);
		interpret(arg_arr, envp);
		free(line);
		free(prompt);
		prompt = make_prompt();
		line = readline(prompt);
	}
	free(prompt);
	prompt = 0;
	return (0);
}
