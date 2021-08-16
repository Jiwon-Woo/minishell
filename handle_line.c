/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_line.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwoo <jwoo@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/12 17:12:50 by jwoo              #+#    #+#             */
/*   Updated: 2021/08/16 10:57:04 by jwoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	handle_line(char **line_prompt, t_cmd *cmd, t_envp *envp, t_fd *fd)
{
	t_quote	quote;

	set_signal();
	add_history(line_prompt[0]);
	check_quote(line_prompt[0], &quote);
	init_cmd(line_prompt[0], cmd, envp, quote);
	init_fd(fd, cmd->command);
	while (cmd->command)
	{
		set_fd_cmd(fd, cmd);
		if (is_redirection(cmd->current_cmd->pre_flag) == 0 \
			&& is_redirection(cmd->command->next_flag) == 1)
			if (handle_redirect(cmd, fd) > 0)
				return (free_cmd_fd(cmd, fd));
		if (cmd->current_cmd->pre_flag == PIPE \
			|| (cmd->command->next_flag == PIPE))
		{
			if (handle_pipe(cmd, fd, envp) > 0)
				return (free_cmd_fd(cmd, fd));
		}
		else
			handle_simple(fd, cmd, envp);
		cmd->command = cmd->command->next;
	}
	return (free_cmd_fd(cmd, fd));
}
