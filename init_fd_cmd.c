/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_fd_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwoo <jwoo@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/12 16:59:13 by jwoo              #+#    #+#             */
/*   Updated: 2021/08/12 16:59:14 by jwoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_cmd(char *line, t_cmd *cmd, t_envp *envp, t_quote quote)
{
	cmd->argument = arg_to_list(line, &quote);
	cmd->command = list_to_char_arr(cmd->argument, envp);
	cmd->first_cmd = cmd->command;
	cmd->current_cmd = 0;
	cmd->append_cmd = 0;
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
