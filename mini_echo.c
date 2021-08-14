/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_echo.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwoo <jwoo@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/14 11:08:28 by jwoo              #+#    #+#             */
/*   Updated: 2021/08/14 11:08:29 by jwoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	mini_echo(char **arg_arr)
{
	int		idx;
	bool	n_option;

	idx = 0;
	n_option = false;
	if (ft_strncmp(arg_arr[1], "-n", 3) == 0)
	{
		n_option = true;
		idx++;
	}
	while (arg_arr[++idx])
	{
		write(1, arg_arr[idx], ft_strlen(arg_arr[idx]));
		if (arg_arr[idx + 1])
			write(1, " ", 1);
	}
	if (!n_option)
		write(1, "\n", 1);
	return (0);
}
