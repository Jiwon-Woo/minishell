#include "minishell.h"

// fprintf -> write 변경 / bash -> minish 변경 / fork 및 malloc 가드
// 메모리 누수 / norm3

void	sigint_handler(int signo)
{
	if (signo == SIGINT)
	{
		rl_on_new_line();
		rl_redisplay();
		printf("%c[K\n", 27);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		g_status = 1;
	}
	signal(SIGINT, (void *)sigint_handler);
}

char	*make_prompt(void)
{
	char	*prompt;
	char	buffer[1024];

	getcwd(buffer, 1024);
	prompt = ft_strjoin(buffer, "$ ");
	return (prompt);
}

int	is_redirection(int flag)
{
	if (flag <= REDIRECT4 && flag >= REDIRECT1)
		return (1);
	return (0);
}

int	is_output_redirect(int flag)
{
	if (flag == REDIRECT2 || flag == REDIRECT4)
		return (1);
	return (0);
}

int	is_input_redirect(int flag)
{
	if (flag == REDIRECT1 || flag == REDIRECT3)
		return (1);
	return (0);
}

int redir_err(char *error_file, char **copy_cmd, t_list **arg_cmd_tmp)
{
	if (is_redirection(arg_cmd_tmp[1]->next_flag) == 1 && arg_cmd_tmp[1]->next == 0)
	{
		write(2, "minish: syntax error near unexpected token `newline'\n", 53);
		g_status = 258;
		free_two_dimension(copy_cmd);
		arg_cmd_tmp[1] = arg_cmd_tmp[2];
		ft_lstclear(&arg_cmd_tmp[0], free);
		ft_lstclear_two(&arg_cmd_tmp[1], free_two_dimension);
		return (1);
	}
	else if (error_file != 0)
	{
		write(2, "minish: ", 8); 
		write(2, error_file, ft_strlen(error_file));
		write(2, ": No such file or directory\n", 28);
		g_status = 1;
		free_two_dimension(copy_cmd);
		free(error_file);
		arg_cmd_tmp[1] = arg_cmd_tmp[2];
		ft_lstclear(&arg_cmd_tmp[0], free);
		ft_lstclear_two(&arg_cmd_tmp[1], free_two_dimension);
		return (1);
	}
	return (0);
}

void	handle_line(char **line_prompt, t_list **arg_cmd_tmp, t_envp *envp)
{
	char	**arg_arr;
	t_quote	quote;
	pid_t	pid;

	set_signal();
	add_history(line_prompt[0]);
	check_quote(line_prompt[0], &quote);
	arg_cmd_tmp[0] = arg_to_list(line_prompt[0], &quote);
	arg_cmd_tmp[1] = list_to_char_arr(arg_cmd_tmp[0], envp);
	arg_cmd_tmp[2] = arg_cmd_tmp[1];

	int	size = ft_lstsize(arg_cmd_tmp[1]);
	int fds[size][2];
	int idx = -1;
	char	**copy_cmd = 0;

	while (arg_cmd_tmp[1])
	{
		arg_arr = (char **)arg_cmd_tmp[1]->content;
		pipe(fds[++idx]);
		int fd[2];

		fd[0] = -1;
		fd[1] = -1;

		t_list	*current_cmd = arg_cmd_tmp[1];
		int		current_fds = idx;
		char	*error_file = 0;

		if (copy_cmd != 0)
			free_two_dimension(copy_cmd);
		copy_cmd = append_strarr((char **)(current_cmd->content), 0);

		if (is_redirection(current_cmd->pre_flag) == 0 &&
			is_redirection(arg_cmd_tmp[1]->next_flag) == 1)
		{
			while (is_redirection(arg_cmd_tmp[1]->next_flag) == 1 && arg_cmd_tmp[1]->next != 0)
			{
				arg_cmd_tmp[1] = arg_cmd_tmp[1]->next;
				pipe(fds[++idx]);
				char **redirect_cmd = arg_cmd_tmp[1]->content;
				if (is_output_redirect(arg_cmd_tmp[1]->pre_flag) == 1)
				{
					if (arg_cmd_tmp[1]->pre_flag == REDIRECT2)
						fd[1] = open(redirect_cmd[0], O_WRONLY | O_TRUNC | O_CREAT, 0644);
					if (arg_cmd_tmp[1]->pre_flag == REDIRECT4)
						fd[1] = open(redirect_cmd[0], O_WRONLY | O_APPEND | O_CREAT, 0644);
					if (get_arg_size(redirect_cmd) > 1)
					{
						char **temp = copy_cmd;
						copy_cmd = append_strarr(copy_cmd, redirect_cmd + 1);
						free_two_dimension(temp);
						temp = 0;
					}
				}
				if (arg_cmd_tmp[1]->pre_flag == REDIRECT1)
				{
					fd[0] = open(redirect_cmd[0], O_RDONLY);
					if (get_arg_size(redirect_cmd) > 1)
					{
						char **temp = copy_cmd;
						copy_cmd = append_strarr(copy_cmd, redirect_cmd + 1);
						free_two_dimension(temp);
						temp = 0;
					}
					if (fd[0] == -1)
					{
						if (error_file == 0)
							error_file = ft_strdup(redirect_cmd[0]);
						continue ;
					}
				}
				if (arg_cmd_tmp[1]->pre_flag == REDIRECT3)
				{
					if (get_arg_size(redirect_cmd) > 1)
					{
						char **temp = copy_cmd;
						copy_cmd = append_strarr(copy_cmd, redirect_cmd + 1);
						free_two_dimension(temp);
						temp = 0;
					}
					pid = fork();
					if (pid == 0)
					{
						signal(SIGINT, SIG_DFL);
						char *line = readline("> ");
						while (line != NULL && ft_strncmp(line, redirect_cmd[0], ft_strlen(redirect_cmd[0]) + 1) != 0)
						{
							write(fds[idx - 1][1], line, ft_strlen(line));
							write(fds[idx - 1][1], "\n", 1);
							free(line);
							line = readline("> ");
						}
						exit(0);
					}
					else if (pid == -1)
						exit(1);
					else
					{
						signal(SIGINT, redirect_sigint_handler);
						close(fds[idx - 1][1]);
						fd[0] = fds[idx - 1][0];
						int sig_get;
						wait(&sig_get);
						if (WIFSIGNALED(sig_get))
						{
							if (WTERMSIG(sig_get) == 2)
							{
								arg_cmd_tmp[1] = arg_cmd_tmp[2];
								ft_lstclear(&arg_cmd_tmp[0], free);
								ft_lstclear_two(&arg_cmd_tmp[1], free_two_dimension);
								if (copy_cmd != 0)
									free_two_dimension(copy_cmd);
								if (error_file != 0)
									free(error_file);
								copy_cmd  = 0;
								error_file = 0;
								return;
							}
						}
						g_status = WEXITSTATUS(sig_get);
						set_signal();
					}
				}
			}
			if (redir_err(error_file, copy_cmd, arg_cmd_tmp) > 0)
				return ;
		}
		if (current_cmd->pre_flag == PIPE || (arg_cmd_tmp[1]->next_flag == PIPE))
		{
			if (current_cmd->pre_flag == PIPE && fd[0] == -1)
				fd[0] = fds[current_fds - 1][0];
			if (arg_cmd_tmp[1]->next_flag == PIPE && ((char **)(current_cmd->content))[0] == 0)
			{
				write(2, "minish: syntax error near unexpected token `|'\n", 47);
				g_status = 258;
				free_two_dimension(copy_cmd);
				arg_cmd_tmp[1] = arg_cmd_tmp[2];
				ft_lstclear(&arg_cmd_tmp[0], free);
				ft_lstclear_two(&arg_cmd_tmp[1], free_two_dimension);
				return;
			}
			else if (arg_cmd_tmp[1]->next_flag == PIPE && fd[1] == -1)
				fd[1] = fds[idx][1];
			pid = fork();
			if (pid == 0)
			{
				if (fd[0] != -1)
					dup2(fd[0], 0);
				if (fd[1] != -1)
					dup2(fd[1], 1);
				signal(SIGQUIT, SIG_DFL);
				exit (interpret(copy_cmd, envp));
			}
			else if (pid == -1)
				exit(1);
			else
			{
				signal(SIGINT, child_sigquit_handler);
				signal(SIGQUIT, child_sigquit_handler);
				close(fd[0]);
				close(fd[1]);
				int sig_get;
				wait(&sig_get);
				if (!WIFSIGNALED(sig_get))
					g_status = WEXITSTATUS(sig_get);
				close(fds[current_fds - 1][0]);
				close(fds[idx][1]);
				set_signal();
			}
		}
		else
		{
			int backup[2];
			pipe(backup);
			dup2(0, backup[0]);
			dup2(1, backup[1]);
			if (fd[0] != -1)
				dup2(fd[0], 0);
			if (fd[1] != -1)
				dup2(fd[1], 1);
			g_status = interpret2(copy_cmd, envp);
			free_two_dimension(copy_cmd);
			dup2(backup[0], 0);
			dup2(backup[1], 1);
			close(backup[0]);
			close(backup[1]);
			close(fd[0]);
			close(fd[1]);
			close(fds[current_fds - 1][0]);
			close(fds[idx][1]);
		}
		arg_cmd_tmp[1] = arg_cmd_tmp[1]->next;
	}
	arg_cmd_tmp[1] = arg_cmd_tmp[2];
	ft_lstclear(&arg_cmd_tmp[0], free);
	ft_lstclear_two(&arg_cmd_tmp[1], free_two_dimension);
}

int	main(int argc, char **argv, char **first_envp)
{
	t_list		**arg_cmd_tmp;
	char		**line_prompt;
	t_envp		envp;

	if (!argc || !(*argv[0]))
		return (-1);
	init_envp_status(&envp, first_envp);
	set_signal();
	line_prompt = (char **)malloc(sizeof(char *) * 2);
	arg_cmd_tmp = (t_list **)malloc(sizeof(t_list *) * 3);
	if (!line_prompt || !arg_cmd_tmp)
		exit(1);
	line_prompt[1] = make_prompt();
	line_prompt[0] = readline(line_prompt[1]);
	while (line_prompt[0] != NULL)
	{
		handle_line(line_prompt, arg_cmd_tmp, &envp);
		free(line_prompt[0]);
		free(line_prompt[1]);
		line_prompt[1] = make_prompt();
		line_prompt[0] = readline(line_prompt[1]);
	}
	free(line_prompt[1]);
	mini_exit(0, true);
	return (0);
}
