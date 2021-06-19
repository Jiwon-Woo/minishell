#include "minishell.h"

// static char	*line;


void	sigint_handler(int signo)
{
	printf("\n");
    rl_on_new_line();
    // rl_replace_line("", 0);
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
	{
		char **dir;
		ret_value = mini_echo(arg_arr, envp);
		return (ret_value);
	}
	if (ft_strncmp(arg_arr[0], "cd", 3) == 0)
	{
		char **dir;
		ret_value = mini_cd(arg_arr, envp);
		return (ret_value);
	}
	if (ft_strncmp(arg_arr[0], "pwd", 4) == 0)
	{
		ret_value = mini_pwd(arg_arr, envp);
		return (ret_value);
	}
	if (ft_strncmp(arg_arr[0], "env", 4) == 0)
	{
		ret_value = mini_env(arg_arr, envp);
		return (ret_value);
	}
	return (-1);
}

void	check_qnote(char *line, t_qnote *qnote)
{
	int	idx;

	idx = -1;
	while (++idx < ft_strlen(line))
	{
		if (line[idx] == '\'' && qnote->q_double == 1)
		{
			qnote->q_single *= -1;
			qnote->q_single_index = idx;
		}
		else if (line[idx] == '\"' && qnote->q_single == 1)
		{
			qnote->q_double *= -1;
			qnote->q_double_index = idx;
		}
	}
	if (qnote->q_single != -1)
		qnote->q_single_index = -1;
	if (qnote->q_double != -1)
		qnote->q_double_index = -1;
}

t_list	*get_arg_list(char *line, t_qnote qnote)
{
	t_list 	*arg_list = 0;
	char	*arg;
	int	q = 0;
	int	i = 0;

	while (line[i])
	{
		arg = ft_strdup("");
		while (ft_isspace(line[i]) && line[i] != 0)
			i++;
		if ((line[i] == '\"' && i != qnote.q_double_index) ||
			(line[i] == '\'' && i != qnote.q_single_index))
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

int	main(int argc, char **argv, char **envp)
{
	char	buffer[1024];
	char	*prompt;
	t_list	*arg_list;
	char	**arg_arr;
	char	*line;
	t_qnote	qnote;

	signal(SIGINT, (void *)sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	getcwd(buffer, 1024);
	prompt = ft_strjoin(buffer, "$ ");
	line = readline(prompt);
	qnote.q_single = 1;
	qnote.q_double = 1;
	qnote.q_single_index = -1;
	qnote.q_double_index = -1;
	while (line != NULL)
	{
		add_history(line);
		check_qnote(line, &qnote);
		arg_list = get_arg_list(line, qnote);
		arg_arr = list_to_char_arr(arg_list);
		interpret(arg_arr, envp);
		free(line);
		line = readline(prompt);
	}
	free(prompt);
	prompt = 0;
	return (0);
}
