#include "minishell.h"

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

void	free_fd(t_fd *fd)
{
	int	i;

	if (fd == 0)
		return ;
	if (fd->fd)
	{
		free(fd->fd);
		fd->fd = 0;
	}
	if (fd->pipe_fds)
	{
		i = -1;
		while (++i < fd->size)
		{
			if (fd->pipe_fds[i] != 0)
				free(fd->pipe_fds[i]);
			fd->pipe_fds[i] = 0;
		}
		free(fd->pipe_fds);
		fd->pipe_fds = 0;
	}
}

void	free_cmd(t_cmd *cmd)
{
	if (cmd->append_cmd)
		free_two_dimension(cmd->append_cmd);
	cmd->append_cmd = 0;
	cmd->command = cmd->first_cmd;
	if (cmd->argument)
		ft_lstclear(&cmd->argument, free);
	cmd->argument = 0;
	if (cmd->command)
		ft_lstclear_two(&cmd->command, free_two_dimension);
	cmd->command = 0;
}

int	redir_err(char *error_file, t_cmd *cmd)
{
	if (is_redirection(cmd->command->next_flag) == 1 && cmd->command->next == 0)
	{
		write(2, "minish: syntax error near unexpected token `newline'\n", 53);
		g_status = 258;
		return (1);
	}
	else if (error_file != 0)
	{
		write(2, "minish: ", 8);
		write(2, error_file, ft_strlen(error_file));
		write(2, ": No such file or directory\n", 28);
		g_status = 1;
		return (1);
	}
	return (0);
}

void	append_cmd(t_cmd *cmd, char **redirect_cmd)
{
	char	**temp;

	if (get_arg_size(redirect_cmd) > 1)
	{
		temp = cmd->append_cmd;
		cmd->append_cmd = append_strarr(cmd->append_cmd, redirect_cmd + 1);
		free_two_dimension(temp);
	}
}

int	output_redirect(t_cmd *cmd, t_fd *fd, char **redirect_cmd)
{
	if (cmd->command->pre_flag == REDIRECT2)
		fd->fd[1] = open(redirect_cmd[0], O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (cmd->command->pre_flag == REDIRECT4)
		fd->fd[1] = open(redirect_cmd[0], O_WRONLY | O_APPEND | O_CREAT, 0644);
	append_cmd(cmd, redirect_cmd);
	if (fd->fd[1] < 0)
		return (1);
	return (0);
}

int	input_redirect(t_cmd *cmd, t_fd *fd, char **redirect_cmd, char **error_file)
{
	if (redirect_cmd == 0 || redirect_cmd[0] == 0)
	{
		write(2, "minish: syntax error near unexpected token `newline'\n", 53);
		return (1);
	}
	fd->fd[0] = open(redirect_cmd[0], O_RDONLY);
	if (fd->fd[0] == -1)
	{
		if (*error_file == 0)
			*error_file = ft_strdup(redirect_cmd[0]);
	}
	append_cmd(cmd, redirect_cmd);
	return (0);
}

void	eof_child(t_fd *fd, char **redirect_cmd)
{
	char	*line;

	line = readline("> ");
	while (line && ft_strncmp(line, redirect_cmd[0], \
			ft_strlen(redirect_cmd[0]) + 1))
	{
		write(fd->pipe_fds[fd->idx - 1][1], line, ft_strlen(line));
		write(fd->pipe_fds[fd->idx - 1][1], "\n", 1);
		free(line);
		line = readline("> ");
	}
}

int	free_error_file_ret(char **error_file)
{
	if (*error_file)
		free(*error_file);
	*error_file = 0;
	return (1);
}

int	eof_redirect(t_cmd *cmd, t_fd *fd, char **redirect_cmd, char **error_file)
{
	pid_t	pid;
	int		sig_get;

	append_cmd(cmd, redirect_cmd);
	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		eof_child(fd, redirect_cmd);
		exit(0);
	}
	else if (pid == -1)
		exit(1);
	else
	{
		signal(SIGINT, redirect_handler);
		close(fd->pipe_fds[fd->idx - 1][1]);
		fd->fd[0] = fd->pipe_fds[fd->idx - 1][0];
		wait(&sig_get);
		if (WIFSIGNALED(sig_get) && WTERMSIG(sig_get) == 2)
			return (free_error_file_ret(error_file));
		g_status = WEXITSTATUS(sig_get);
		set_signal();
		return (0);
	}
}

int	redirect_case(t_cmd *cmd, t_fd *fd, char **redirect_cmd, char **error_file)
{
	if (is_output_redirect(cmd->command->pre_flag) == 1)
	{
		if (output_redirect(cmd, fd, redirect_cmd) > 0)
			return (1);
	}
	if (cmd->command->pre_flag == REDIRECT1)
	{
		if (input_redirect(cmd, fd, redirect_cmd, error_file) > 0)
			return (1);
		if (fd->fd[0] == -1)
			return (-42);
	}
	if (cmd->command->pre_flag == REDIRECT3)
	{
		if (eof_redirect(cmd, fd, redirect_cmd, error_file) > 0)
			return (1);
	}
	return (0);
}

int	redirection(t_cmd *cmd, t_fd *fd)
{
	char	**redirect_cmd;
	char	*error_file;
	int		redirect_ret;

	error_file = 0;
	while (is_redirection(cmd->command->next_flag) == 1 \
			&& cmd->command->next != 0)
	{
		cmd->command = cmd->command->next;
		pipe(fd->pipe_fds[++(fd->idx)]);
		redirect_cmd = cmd->command->content;
		redirect_ret = redirect_case(cmd, fd, redirect_cmd, &error_file);
		if (redirect_ret > 0)
			return (1);
		else if (redirect_ret < 0)
			continue ;
	}
	if (redir_err(error_file, cmd) > 0)
		return (free_error_file_ret(&error_file));
	return (0);
}

void	init_fd(t_fd *fd, t_list *cmd)
{
	int	i;

	fd->size = ft_lstsize(cmd);
	fd->idx = -1;
	fd->pipe_fds = (int **)malloc(sizeof(int *) * (fd->size));
	if (!(fd->pipe_fds))
		exit (1);
	i = -1;
	while (++i < fd->size)
	{
		(fd->pipe_fds)[i] = (int *)malloc(sizeof(int) * 2);
		if (!(fd->pipe_fds[i]))
			exit (1);
	}
	fd->fd = (int *)malloc(sizeof(int) * 2);
	if (!(fd->fd))
		exit (1);
	fd->fd[0] = -1;
	fd->fd[1] = -1;
}

void	handle_simple(t_fd *fd, t_cmd *cmd, t_envp *envp)
{
	int	backup[2];

	pipe(backup);
	dup2(0, backup[0]);
	dup2(1, backup[1]);
	if (fd->fd[0] != -1)
		dup2(fd->fd[0], 0);
	if (fd->fd[1] != -1)
		dup2(fd->fd[1], 1);
	g_status = interpret_without_pipe(cmd->append_cmd, envp);
	if (cmd->append_cmd != 0)
		free_two_dimension(cmd->append_cmd);
	cmd->append_cmd = 0;
	dup2(backup[0], 0);
	dup2(backup[1], 1);
	close(backup[0]);
	close(backup[1]);
	close(fd->fd[0]);
	close(fd->fd[1]);
	if (fd->current_idx > 0)
		close(fd->pipe_fds[fd->current_idx - 1][0]);
	close(fd->pipe_fds[fd->idx][1]);
}

void	init_cmd(char *line, t_cmd *cmd, t_envp *envp, t_quote quote)
{
	cmd->argument = arg_to_list(line, &quote);
	cmd->command = list_to_char_arr(cmd->argument, envp);
	cmd->first_cmd = cmd->command;
	cmd->current_cmd = 0;
	cmd->append_cmd = 0;
}

void	set_fd_cmd(t_fd *fd, t_cmd *cmd)
{
	fd->idx += 1;
	pipe(fd->pipe_fds[fd->idx]);
	fd->fd[0] = -1;
	fd->fd[1] = -1;
	fd->current_idx = fd->idx;
	cmd->current_cmd = cmd->command;
	if (cmd->append_cmd != 0)
		free_two_dimension(cmd->append_cmd);
	cmd->append_cmd = append_strarr((char **)(cmd->current_cmd->content), 0);
}

void	free_cmd_fd(t_cmd *cmd, t_fd *fd)
{
	free_cmd(cmd);
	free_fd(fd);
	return ;
}

int	print_pipe_err(void)
{
	write(2, "minish: syntax error near unexpected token `|'\n", 47);
	g_status = 258;
	return (1);
}

int	pipe_parent(t_fd *fd)
{
	int	sig_get;

	signal(SIGINT, child_handler);
	signal(SIGQUIT, child_handler);
	close(fd->fd[0]);
	close(fd->fd[1]);
	wait(&sig_get);
	if (!WIFSIGNALED(sig_get))
		g_status = WEXITSTATUS(sig_get);
	if (fd->current_idx > 0)
		close(fd->pipe_fds[fd->current_idx - 1][0]);
	close(fd->pipe_fds[fd->idx][1]);
	set_signal();
	return (0);
}

int	handle_pipe(t_cmd *cmd, t_fd *fd, t_envp *envp)
{
	int	pid;

	if (cmd->current_cmd->pre_flag == PIPE && fd->fd[0] == -1)
		fd->fd[0] = fd->pipe_fds[fd->current_idx - 1][0];
	if (cmd->command->next_flag == PIPE && \
		((char **)(cmd->current_cmd->content))[0] == 0)
		return (print_pipe_err());
	else if (cmd->command->next_flag == PIPE && fd->fd[1] == -1)
		fd->fd[1] = fd->pipe_fds[fd->idx][1];
	pid = fork();
	if (pid == 0)
	{
		if (fd->fd[0] != -1)
			dup2(fd->fd[0], 0);
		if (fd->fd[1] != -1)
			dup2(fd->fd[1], 1);
		signal(SIGQUIT, SIG_DFL);
		exit (interpret_with_pipe(cmd->append_cmd, envp));
	}
	else if (pid == -1)
		exit(1);
	else
		return (pipe_parent(fd));
}
