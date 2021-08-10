/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwoo <jwoo@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 12:07:49 by jwoo              #+#    #+#             */
/*   Updated: 2021/08/10 12:34:30 by jwoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	env_validation(char *name)
{
	int	i;

	i = 0;
	if (!name)
		return (-1);
	if (!((name[i] == '_') || ft_isalpha(name[i])))
		return (-1);
	i++;
	while (name[i] && name[i] != '=')
	{
		if (!(ft_isdigit(name[i]) || ft_isalpha(name[i]) || name[i] == '_'))
			return (-1);
		i++;
	}
	return (0);
}

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

char	**get_env_ptr(char *key, char **envp)
{
	int	i;
	int	equal_idx;

	i = -1;
	while (envp[++i])
	{
		equal_idx = get_equal_idx(envp[i]);
		if (ft_strlen(key) == equal_idx && !ft_strncmp(envp[i], key, equal_idx))
			return (&envp[i]);
	}
	return (0);
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
