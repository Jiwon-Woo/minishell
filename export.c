/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwoo <jwoo@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 12:17:25 by jwoo              #+#    #+#             */
/*   Updated: 2021/08/10 12:47:46 by jwoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	envp_add(t_envp *envp, char *content)
{
	int		size;
	char	**old_envp;
	int		i;

	size = get_arg_size(envp->envp_list);
	old_envp = envp->envp_list;
	envp->envp_list = (char **)malloc(sizeof(char *) * (size + 2));
	if (!envp->envp_list)
		exit(1);
	i = -1;
	while (old_envp[++i])
		envp->envp_list[i] = old_envp[i];
	free(old_envp);
	envp->envp_list[i++] = ft_strdup(content);
	envp->envp_list[i] = 0;
}

int	export_without_arg(char **arg_arr)
{
	int	size;
	int	i;

	size = get_arg_size(arg_arr);
	i = 0;
	while (++i < size)
	{
		if (ft_strlen(arg_arr[i]) > 0)
			return (0);
	}
	return (1);
}

void	print_export(t_envp *envp)
{
	int	i;
	int	size;
	int	equal_idx;

	i = -1;
	size = get_arg_size(envp->envp_list);
	while (++i < size)
	{
		equal_idx = get_equal_idx(envp->envp_list[envp->sort_idx[i]]);
		if (equal_idx == -1)
			equal_idx = ft_strlen(envp->envp_list[envp->sort_idx[i]]);
		write(1, "declare -x ", ft_strlen("declare -x "));
		write(1, envp->envp_list[envp->sort_idx[i]], equal_idx + 1);
		if (equal_idx != ft_strlen(envp->envp_list[envp->sort_idx[i]]))
			write(1, "\"", 1);
		write(1, envp->envp_list[envp->sort_idx[i]] + equal_idx + 1, \
			ft_strlen(envp->envp_list[envp->sort_idx[i]] + equal_idx + 1));
		if (equal_idx != ft_strlen(envp->envp_list[envp->sort_idx[i]]))
			write(1, "\"", 1);
		write(1, "\n", 1);
	}
}

void	with_eq(char **arg_arr, t_envp *envp, int i)
{
	int	j;
	int	key_last;
	int	input_key_last;

	input_key_last = get_equal_idx(arg_arr[i]);
	j = -1;
	while (envp->envp_list[++j])
	{
		key_last = get_equal_idx(envp->envp_list[j]);
		if (key_last < 0)
			key_last = ft_strlen(envp->envp_list[j]);
		if (key_last == input_key_last && ft_strncmp(envp->envp_list[j], \
			arg_arr[i], key_last) == 0)
		{
			free(envp->envp_list[j]);
			envp->envp_list[j] = ft_strdup(arg_arr[i]);
			break ;
		}
	}
	if (envp->envp_list[j] == 0)
		envp_add(envp, arg_arr[i]);
}

void	without_eq(char **arg_arr, t_envp *envp, int i)
{
	int	j;
	int	key_last;
	int	input_key_last;

	input_key_last = ft_strlen(arg_arr[i]);
	j = -1;
	while (envp->envp_list[++j])
	{
		key_last = get_equal_idx(envp->envp_list[j]);
		if (key_last < 0)
			key_last = ft_strlen(envp->envp_list[j]);
		if (key_last == input_key_last && \
			ft_strncmp(envp->envp_list[j], arg_arr[i], key_last) == 0)
			break ;
	}
	if (envp->envp_list[j] == 0)
		envp_add(envp, arg_arr[i]);
}
