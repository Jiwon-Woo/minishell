#include "minishell.h"

void handle_pipe(t_cmd *cmd, t_fd *fd, t_envp *envp, int pipe_fds[][2]);

int	redir_err(char *error_file, t_cmd *cmd)
{
	if (is_redirection(cmd->command->next_flag) == 1 && cmd->command->next == 0)
	{
		write(2, "minish: syntax error near unexpected token `newline'\n", 53);
		g_status = 258;
		free_two_dimension(cmd->append_cmd);
		cmd->append_cmd = 0;
		cmd->command = cmd->first_cmd;
		// ft_lstclear(&cmd->argument, free);
		// ft_lstclear_two(&cmd->command, free_two_dimension);
		return (1);
	}
	else if (error_file != 0)
	{
		write(2, "minish: ", 8);
		write(2, error_file, ft_strlen(error_file));
		write(2, ": No such file or directory\n", 28);
		g_status = 1;
		free_two_dimension(cmd->append_cmd);
		cmd->append_cmd = 0;
		free(error_file);
		cmd->command = cmd->first_cmd;
		// ft_lstclear(&cmd->argument, free);
		// ft_lstclear_two(&cmd->command, free_two_dimension);
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

void	output_redirect(t_cmd *cmd, t_fd *fd, char **redirect_cmd)
{
	if (cmd->command->pre_flag == REDIRECT2)
		fd->fd[1] = open(redirect_cmd[0], O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (cmd->command->pre_flag == REDIRECT4)
		fd->fd[1] = open(redirect_cmd[0], O_WRONLY | O_APPEND | O_CREAT, 0644);
	append_cmd(cmd, redirect_cmd);
}

char	*input_redirect(t_cmd *cmd, t_fd *fd, char **redirect_cmd, char *error_file)
{
	fd->fd[0] = open(redirect_cmd[0], O_RDONLY);
	append_cmd(cmd, redirect_cmd);
	if (fd->fd[0] == -1)
	{
		if (error_file == 0)
			return (ft_strdup(redirect_cmd[0]));
	}
	return (error_file);
}

void	eof_redirect(t_cmd *cmd, t_fd *fd, char **redirect_cmd, char *error_file, int pipe_fds[][2])
{
	pid_t	pid;
	int		sig_get;
	char	*line;

	append_cmd(cmd, redirect_cmd);
	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		line = readline("> ");
		while (line != NULL && ft_strncmp(line, redirect_cmd[0], ft_strlen(redirect_cmd[0]) + 1) != 0)
		{
			write(pipe_fds[fd->idx - 1][1], line, ft_strlen(line));
			write(pipe_fds[fd->idx - 1][1], "\n", 1);
			free(line);
			line = readline("> ");
		}
		exit(0);
	}
	else if (pid == -1)
		exit(1);
	else
	{
		signal(SIGINT, redirect_handler);
		close(pipe_fds[fd->idx  - 1][1]);
		fd->fd[0] = pipe_fds[fd->idx - 1][0];
		wait(&sig_get);
		if (WIFSIGNALED(sig_get))
		{
			if (WTERMSIG(sig_get) == 2)
			{
				cmd->command = cmd->first_cmd;
				ft_lstclear(&cmd->argument, free);
				ft_lstclear_two(&cmd->command, free_two_dimension);
				if (cmd->append_cmd != 0)
					free_two_dimension(cmd->append_cmd);
				if (error_file != 0)
					free(error_file);
				return;
			}
		}
		g_status = WEXITSTATUS(sig_get);
		set_signal();
	}
}

void	redirection(t_cmd *cmd, t_fd *fd, int pipe_fds[][2])
{
	char	**redirect_cmd;
	char	*error_file;

	error_file = 0;
	while (is_redirection(cmd->command->next_flag) == 1 && cmd->command->next != 0)
	{
		cmd->command = cmd->command->next;
		fd->idx += 1;
		pipe(pipe_fds[fd->idx]);
		redirect_cmd = cmd->command->content;
		if (is_output_redirect(cmd->command->pre_flag) == 1)
		{
			output_redirect(cmd, fd, redirect_cmd);
		}
		if (cmd->command->pre_flag == REDIRECT1)
		{
			error_file = input_redirect(cmd, fd, redirect_cmd, error_file);
			if (fd->fd[0] == -1)
				continue ;
		}
		if (cmd->command->pre_flag == REDIRECT3)
		{
			eof_redirect(cmd, fd, redirect_cmd, error_file, pipe_fds);
		}
	}
	if (redir_err(error_file, cmd) > 0)
	{
		ft_lstclear(&cmd->argument, free);
		ft_lstclear_two(&cmd->command, free_two_dimension);
		return ;
	}
}

void	init_fd(t_fd *fd, t_list *cmd)
{
	// int	i;
	
	fd->size = ft_lstsize(cmd);
	fd->idx = -1;
	// pipe_fds = (int **)malloc(sizeof(int *) * (fd->size));
	// if (!(pipe_fds))
	// 	exit (1);
	// i = -1;
	// while (++i < fd->size)
	// {
	// 	(pipe_fds)[i] = (int *)malloc(sizeof(int) * 2);
	// 	if (!(pipe_fds[i]))
	// 		exit (1);
	// 	// pipe(pipe_fds[i]);
	// }
	// fd->fd = (int *)malloc(sizeof(int) * 2);
	// if (!(fd->fd))
	// 	exit (1);
	fd->fd[0] = -1;
	fd->fd[1] = -1;
}

void	handle_line(char **line_prompt, t_cmd *cmd, t_envp *envp)
{
	t_quote	quote;
	t_fd	*fd;

	set_signal();
	add_history(line_prompt[0]);
	check_quote(line_prompt[0], &quote);
	// cmd->argument = arg_to_list(line_prompt[0], &quote);
	// cmd->command = list_to_char_arr(cmd->argument, envp);
	// cmd->first_cmd = cmd->command;

	cmd->argument = arg_to_list(line_prompt[0], &quote);
	cmd->command = list_to_char_arr(cmd->argument, envp);
	cmd->first_cmd = cmd->command;
	cmd->current_cmd = 0;
	cmd->append_cmd = 0;

	fd = (t_fd *)malloc(sizeof(t_fd));
	init_fd(fd, cmd->command);
	int pipe_fds[fd->size][2];

	while (cmd->command)
	{
		fd->idx += 1;
		pipe(pipe_fds[fd->idx]);
		fd->fd[0] = -1;
		fd->fd[1] = -1;
		fd->current_idx = fd->idx;

		cmd->current_cmd = cmd->command;
		// char	*error_file = 0;

		if (cmd->append_cmd != 0)
			free_two_dimension(cmd->append_cmd);
		cmd->append_cmd = append_strarr((char **)(cmd->current_cmd->content), 0);
		fprintf(stderr, "before redirect\n");
		if (is_redirection(cmd->current_cmd->pre_flag) == 0 &&
			is_redirection(cmd->command->next_flag) == 1)
		{
			redirection(cmd, fd, pipe_fds);
			// while (is_redirection(cmd->command->next_flag) == 1 && cmd->command->next != 0)
			// {
			// 	cmd->command = cmd->command->next;
			// 	fd->idx += 1;
			// 	char **redirect_cmd = cmd->command->content;
			// 	if (is_output_redirect(cmd->command->pre_flag) == 1)
			// 	{
			// 		if (cmd->command->pre_flag == REDIRECT2)
			// 			fd->fd[1] = open(redirect_cmd[0], O_WRONLY | O_TRUNC | O_CREAT, 0644);
			// 		if (cmd->command->pre_flag == REDIRECT4)
			// 			fd->fd[1] = open(redirect_cmd[0], O_WRONLY | O_APPEND | O_CREAT, 0644);
			// 		if (get_arg_size(redirect_cmd) > 1)
			// 		{
			// 			char **temp = cmd->append_cmd;
			// 			cmd->append_cmd = append_strarr(cmd->append_cmd, redirect_cmd + 1);
			// 			free_two_dimension(temp);
			// 		}
			// 	}
			// 	if (cmd->command->pre_flag == REDIRECT1)
			// 	{
			// 		fd->fd[0] = open(redirect_cmd[0], O_RDONLY);
			// 		if (get_arg_size(redirect_cmd) > 1)
			// 		{
			// 			char **temp = cmd->append_cmd;
			// 			cmd->append_cmd = append_strarr(cmd->append_cmd, redirect_cmd + 1);
			// 			free_two_dimension(temp);
			// 		}
			// 		if (fd->fd[0] == -1)
			// 		{
			// 			if (error_file == 0)
			// 				error_file = ft_strdup(redirect_cmd[0]);
			// 			continue ;
			// 		}
			// 	}
			// 	if (cmd->command->pre_flag == REDIRECT3)
			// 	{
			// 		if (get_arg_size(redirect_cmd) > 1)
			// 		{
			// 			char **temp = cmd->append_cmd;
			// 			cmd->append_cmd = append_strarr(cmd->append_cmd, redirect_cmd + 1);
			// 			free_two_dimension(temp);
			// 			temp = 0;
			// 		}
			// 		pid = fork();
			// 		if (pid == 0)
			// 		{
			// 			signal(SIGINT, SIG_DFL);
			// 			char *line = readline("> ");
			// 			while (line != NULL && ft_strncmp(line, redirect_cmd[0], ft_strlen(redirect_cmd[0]) + 1) != 0)
			// 			{
			// 				write(pipe_fds[fd->idx - 1][1], line, ft_strlen(line));
			// 				write(pipe_fds[fd->idx - 1][1], "\n", 1);
			// 				free(line);
			// 				line = readline("> ");
			// 			}
			// 			exit(0);
			// 		}
			// 		else if (pid == -1)
			// 			exit(1);
			// 		else
			// 		{
			// 			signal(SIGINT, redirect_handler);
			// 			close(pipe_fds[fd->idx  - 1][1]);
			// 			fd->fd[0] = pipe_fds[fd->idx - 1][0];
			// 			int sig_get;
			// 			wait(&sig_get);
			// 			if (WIFSIGNALED(sig_get))
			// 			{
			// 				if (WTERMSIG(sig_get) == 2)
			// 				{
			// 					cmd->command = cmd->first_cmd;
			// 					ft_lstclear(&cmd->argument, free);
			// 					ft_lstclear_two(&cmd->command, free_two_dimension);
			// 					if (cmd->append_cmd != 0)
			// 						free_two_dimension(cmd->append_cmd);
			// 					if (error_file != 0)
			// 						free(error_file);
			// 					return;
			// 				}
			// 			}
			// 			g_status = WEXITSTATUS(sig_get);
			// 			set_signal();
			// 		}
			// 	}
			// }
			// if (redir_err(error_file, cmd->append_cmd, arg_cmd_tmp) > 0)
			// {
			// 	ft_lstclear(&cmd->argument, free);
			// 	ft_lstclear_two(&cmd->command, free_two_dimension);
			// 	return ;
			// }
		}
		fprintf(stderr, "before pipe\n");
		if (cmd->current_cmd->pre_flag == PIPE || (cmd->command->next_flag == PIPE))
		{
			handle_pipe(cmd, fd, envp, pipe_fds);
			// if (current_cmd->pre_flag == PIPE && fd->fd[0] == -1)
			// 	fd->fd[0] = pipe_fds[fd->current_idx - 1][0];
			// if (cmd->command->next_flag == PIPE && ((char **)(current_cmd->content))[0] == 0)
			// {
			// 	write(2, "minish: syntax error near unexpected token `|'\n", 47);
			// 	g_status = 258;
			// 	free_two_dimension(cmd->append_cmd);
			// 	cmd->command = cmd->first_cmd;
			// 	ft_lstclear(&cmd->argument, free);
			// 	ft_lstclear_two(&cmd->command, free_two_dimension);
			// 	return;
			// }
			// else if (cmd->command->next_flag == PIPE && fd->fd[1] == -1)
			// 	fd->fd[1] = pipe_fds[fd->idx][1];
			// pid = fork();
			// if (pid == 0)
			// {
			// 	if (fd->fd[0] != -1)
			// 		dup2(fd->fd[0], 0);
			// 	if (fd->fd[1] != -1)
			// 		dup2(fd->fd[1], 1);
			// 	signal(SIGQUIT, SIG_DFL);
			// 	exit (interpret(cmd->append_cmd, envp));
			// }
			// else if (pid == -1)
			// 	exit(1);
			// else
			// {
			// 	signal(SIGINT, child_handler);
			// 	signal(SIGQUIT, child_handler);
			// 	close(fd->fd[0]);
			// 	close(fd->fd[1]);
			// 	int sig_get;
			// 	wait(&sig_get);
			// 	if (!WIFSIGNALED(sig_get))
			// 		g_status = WEXITSTATUS(sig_get);
			// 	close(pipe_fds[fd->current_idx - 1][0]);
			// 	close(pipe_fds[fd->idx][1]);
			// 	set_signal();
			// }
		}
		else
		{
			fprintf(stderr, "nothing\n");
			int backup[2];
			pipe(backup);
			dup2(0, backup[0]);
			dup2(1, backup[1]);
			if (fd->fd[0] != -1)
				dup2(fd->fd[0], 0);
			if (fd->fd[1] != -1)
				dup2(fd->fd[1], 1);
			g_status = interpret2(cmd->append_cmd, envp);
			if (cmd->append_cmd != 0)
				free_two_dimension(cmd->append_cmd);
			cmd->append_cmd = 0;
			dup2(backup[0], 0);
			dup2(backup[1], 1);
			close(backup[0]);
			close(backup[1]);
			close(fd->fd[0]);
			close(fd->fd[1]);
			fprintf(stderr, "fds_idx: %d\n", pipe_fds[fd->idx][1]);
			fprintf(stderr, "current_fds: %d\n", pipe_fds[fd->current_idx - 1][0]);
			fprintf(stderr, "fd->size : %d\n", fd->size);
			if (fd->current_idx > 0)
				close(pipe_fds[fd->current_idx - 1][0]);
			close(pipe_fds[fd->idx][1]);
			fprintf(stderr, "nothing7\n");
		}
		cmd->command = cmd->command->next;
	}
	if (cmd->append_cmd != 0)
		free_two_dimension(cmd->append_cmd);
	cmd->command = cmd->first_cmd;
	cmd->first_cmd = 0;
	ft_lstclear(&cmd->argument, free);
	ft_lstclear_two(&cmd->command, free_two_dimension);
}

void handle_pipe(t_cmd *cmd, t_fd *fd, t_envp *envp, int pipe_fds[][2])
{
	int sig_get;
	int	pid;

	if (cmd->current_cmd->pre_flag == PIPE && fd->fd[0] == -1)
		fd->fd[0] = pipe_fds[fd->current_idx - 1][0];
	if (cmd->command->next_flag == PIPE && ((char **)(cmd->current_cmd->content))[0] == 0)
	{
		write(2, "minish: syntax error near unexpected token `|'\n", 47);
		g_status = 258;
		free_two_dimension(cmd->append_cmd);
		cmd->command = cmd->first_cmd;
		ft_lstclear(&cmd->argument, free);
		ft_lstclear_two(&cmd->command, free_two_dimension);
		return;
	}
	else if (cmd->command->next_flag == PIPE && fd->fd[1] == -1)
		fd->fd[1] = pipe_fds[fd->idx][1];
	pid = fork();
	if (pid == 0)
	{
		if (fd->fd[0] != -1)
			dup2(fd->fd[0], 0);
		if (fd->fd[1] != -1)
			dup2(fd->fd[1], 1);
		signal(SIGQUIT, SIG_DFL);
		exit (interpret(cmd->append_cmd, envp));
	}
	else if (pid == -1)
		exit(1);
	else
	{
		signal(SIGINT, child_handler);
		signal(SIGQUIT, child_handler);
		close(fd->fd[0]);
		close(fd->fd[1]);
		wait(&sig_get);
		if (!WIFSIGNALED(sig_get))
			g_status = WEXITSTATUS(sig_get);
		close(pipe_fds[fd->current_idx - 1][0]);
		close(pipe_fds[fd->idx][1]);
		set_signal();
	}
}