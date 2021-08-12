/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirect_case.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwoo <jwoo@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/12 17:07:18 by jwoo              #+#    #+#             */
/*   Updated: 2021/08/12 17:07:19 by jwoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
