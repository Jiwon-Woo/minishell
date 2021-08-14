/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_free.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwoo <jwoo@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/12 16:41:28 by jwoo              #+#    #+#             */
/*   Updated: 2021/08/14 12:12:25 by jwoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_fd(int *fd)
{
	if (fd[0] > 0)
		close(fd[0]);
	if (fd[1] > 0)
		close(fd[1]);
}

void	free_fd(t_fd *fd)
{
	int	i;

	if (fd == 0)
		return ;
	if (fd->fd)
	{
		close_fd(fd->fd);
		free(fd->fd);
		fd->fd = 0;
	}
	if (fd->pipe_fds)
	{
		i = -1;
		while (++i < fd->size)
		{
			if (fd->pipe_fds[i] != 0)
			{
				close_fd(fd->pipe_fds[i]);
				free(fd->pipe_fds[i]);
			}
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

int	free_error_file_ret(char **error_file)
{
	if (*error_file)
		free(*error_file);
	*error_file = 0;
	return (1);
}

void	free_cmd_fd(t_cmd *cmd, t_fd *fd)
{
	free_cmd(cmd);
	free_fd(fd);
	return ;
}
