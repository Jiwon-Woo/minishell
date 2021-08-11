/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpret.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwoo <jwoo@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/11 09:33:57 by jwoo              #+#    #+#             */
/*   Updated: 2021/08/11 09:33:58 by jwoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_not_bulitin(char **arg_arr, t_envp *envp)
{
	if (ft_strncmp(arg_arr[0], "cd", 3) == 0)
		return (mini_cd(arg_arr, envp));
	if (ft_strncmp(arg_arr[0], "export", 7) == 0)
		return (mini_export(arg_arr, envp));
	if (ft_strncmp(arg_arr[0], "unset", 6) == 0)
		return (mini_unset(arg_arr, envp));
	if (ft_strncmp(arg_arr[0], "exit", 5) == 0)
		return (mini_exit(arg_arr, false));
	if (ft_strncmp(arg_arr[0], "pwd", 4) == 0)
		return (mini_pwd());
	if (ft_strncmp(arg_arr[0], "env", 4) == 0)
		return (mini_env(arg_arr, envp));
	return (-1);
}

int	is_bulitin(char **arg_arr, t_envp *envp, int last_slash)
{
	pid_t	pid;
	int		sig_get;

	pid = fork();
	if (pid < 0)
		exit(1);
	else if (pid == 0)
	{
		signal(SIGQUIT, SIG_DFL);
		if (last_slash != -1)
			exit (with_path(arg_arr, envp));
		else
			exit (without_path(arg_arr, envp));
	}
	else
	{
		set_child_signal();
		wait(&sig_get);
		if (!WIFSIGNALED(sig_get))
			g_status = WEXITSTATUS(sig_get);
		set_signal();
		return (g_status);
	}
}

int	interpret(char **arg_arr, t_envp *envp)
{
	int	last_slash;
	int	bulit;

	if (arg_arr[0] == 0)
		return (0);
	last_slash = get_last_slash_idx(arg_arr[0]);
	bulit = is_not_bulitin(arg_arr, envp);
	if (bulit != -1)
		return (bulit);
	if (last_slash != -1)
		return (with_path(arg_arr, envp));
	else
		return (without_path(arg_arr, envp));
}

int	interpret2(char **arg_arr, t_envp *envp)
{
	int		last_slash;
	int		bulit;

	if (arg_arr[0] == 0)
		return (0);
	last_slash = get_last_slash_idx(arg_arr[0]);
	bulit = is_not_bulitin(arg_arr, envp);
	if (bulit != -1)
		return (bulit);
	else
		return (is_bulitin(arg_arr, envp, last_slash));
}
