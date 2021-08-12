/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pipe.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwoo <jwoo@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/12 17:04:44 by jwoo              #+#    #+#             */
/*   Updated: 2021/08/12 17:04:45 by jwoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
