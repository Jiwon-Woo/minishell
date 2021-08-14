/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirect.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwoo <jwoo@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/12 17:07:13 by jwoo              #+#    #+#             */
/*   Updated: 2021/08/14 11:27:41 by jwoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_redirection(int flag)
{
	if (flag <= REDIRECT4 && flag >= REDIRECT1)
		return (1);
	return (0);
}

char	*to_sperate(int num)
{
	if (num == REDIRECT1)
		return (ft_strdup("<"));
	else if (num == REDIRECT2)
		return (ft_strdup(">"));
	else if (num == REDIRECT3)
		return (ft_strdup("<<"));
	else if (num == REDIRECT4)
		return (ft_strdup(">>"));
	return (0);
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
		temp = 0;
	}
}

int	handle_redirect(t_cmd *cmd, t_fd *fd)
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
