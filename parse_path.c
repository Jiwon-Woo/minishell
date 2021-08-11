/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_path.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwoo <jwoo@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 17:16:13 by jwoo              #+#    #+#             */
/*   Updated: 2021/08/10 17:17:28 by jwoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*parse_path(t_envp *envp)
{
	int	flag;
	int	idx;

	idx = -1;
	flag = -1;
	while (++idx < get_arg_size(envp->envp_list))
	{
		if ((get_equal_idx(envp->envp_list[idx]) == ft_strlen("PATH") \
			&& ft_strncmp(envp->envp_list[idx], "PATH", 4) == 0)
			|| ft_strncmp("PATH", envp->envp_list[idx], 5) == 0)
			flag = idx;
	}
	if (flag == -1)
		return (0);
	return (ft_strdup(envp->envp_list[flag] + 5));
}

int	get_last_slash_idx(char *arg)
{
	int	last;
	int	i;

	last = -1;
	i = -1;
	while (arg[++i])
	{
		if (arg[i] == '/')
			last = i;
	}
	return (last);
}
