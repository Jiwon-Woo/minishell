/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_exit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwoo <jwoo@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/11 09:42:44 by jwoo              #+#    #+#             */
/*   Updated: 2021/08/11 10:02:28 by jwoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	print_exit_err(char *arg, int err_type)
{
	if (err_type == 0)
	{
		write(2, "minish: exit: ", ft_strlen("minish: exit: "));
		write(2, arg, ft_strlen(arg));
		write(2, ": numeric argument required\n", \
			ft_strlen(": numeric argument required\n"));
		return (255);
	}
	else
	{
		write(2, "minish: exit: too many arguments\n", \
			ft_strlen("minish: exit: too many arguments\n"));
		return (1);
	}
}

int	mini_exit(char **arg_arr, bool is_parent)
{
	int		ret;
	int		error;

	error = 0;
	if (arg_arr == 0 || arg_arr[1] == 0)
	{
		if (is_parent == true)
			ret = g_status;
		else
			ret = 0;
	}
	else
		error = ft_atoi(arg_arr[1], &ret);
	if (is_parent == true)
		write(2, "exit\n", 5);
	if (error == -1)
		ret = print_exit_err(arg_arr[1], 0);
	else if (get_arg_size(arg_arr) > 2)
		ret = print_exit_err(0, 1);
	exit (((int)(char)(ret)));
}
