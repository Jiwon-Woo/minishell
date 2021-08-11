/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_envp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwoo <jwoo@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 12:07:49 by jwoo              #+#    #+#             */
/*   Updated: 2021/08/10 17:44:20 by jwoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	arr_swap(int *arr, int i, int j)
{
	int	temp;

	temp = arr[i];
	arr[i] = arr[j];
	arr[j] = temp;
}

void	sort_envp_idx(t_envp *envp)
{
	int	i;
	int	j;
	int	size;
	int	*sort;

	size = get_arg_size(envp->envp_list);
	sort = (int *)malloc(sizeof(int) * size);
	if (!sort)
		exit(1);
	i = -1;
	while (++i < size)
		sort[i] = i;
	i = -1;
	while (++i < size)
	{
		j = i;
		while (++j < size)
			if (ft_strncmp(envp->envp_list[sort[i]], envp->envp_list[sort[j]],
					ft_strlen(envp->envp_list[sort[i]])) > 0)
				arr_swap(sort, i, j);
	}
	if (envp->sort_idx != 0)
		free(envp->sort_idx);
	envp->sort_idx = sort;
}

void	init_envp_status(t_envp *envp, char **first_envp)
{
	int	i;
	int	size;

	g_status = 0;
	size = get_arg_size(first_envp);
	envp->envp_list = (char **)malloc(sizeof(char *) * (size + 1));
	if (!envp->envp_list)
		exit(1);
	envp->sort_idx = 0;
	i = -1;
	while (++i < size)
		envp->envp_list[i] = ft_strdup(first_envp[i]);
	envp->envp_list[size] = 0;
	sort_envp_idx(envp);
}
