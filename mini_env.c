/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwoo <jwoo@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 16:32:05 by jwoo              #+#    #+#             */
/*   Updated: 2021/08/10 16:32:15 by jwoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	mini_env(char **arg, t_envp *envp)
{
	int		i;
	int		size;
	char	*error;
	int		equal_idx;

	error = "minish: env: too many arguments\n";
	if (arg[1] != 0)
	{
		write(2, error, ft_strlen(error));
		return (1);
	}
	i = -1;
	size = get_arg_size(envp->envp_list);
	while (++i < size)
	{
		equal_idx = get_equal_idx(envp->envp_list[i]);
		if (equal_idx != -1)
			printf("%s\n", envp->envp_list[i]);
	}
	return (0);
}
