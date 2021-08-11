/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_export.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwoo <jwoo@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/11 09:34:08 by jwoo              #+#    #+#             */
/*   Updated: 2021/08/11 09:34:09 by jwoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	print_export_err(char *arg)
{
	write(2, "minish: export: `", ft_strlen("minish: export: `"));
	write(2, arg, ft_strlen(arg));
	write(2, "\': not a valid identifier\n", \
		ft_strlen("\': not a valid identifier\n"));
	return (1);
}

int	mini_export(char **arg_arr, t_envp *envp)
{
	int	i;
	int	ret;

	if (export_without_arg(arg_arr))
	{
		print_export(envp);
		return (0);
	}
	ret = 0;
	i = 1;
	while (arg_arr[i])
	{
		if (env_validation(arg_arr[i]) == -1)
		{
			if (ft_strlen(arg_arr[i]) > 0)
				ret = print_export_err(arg_arr[i]);
		}
		else if (get_equal_idx(arg_arr[i]) > 0)
			with_eq(arg_arr, envp, i);
		else
			without_eq(arg_arr, envp, i);
		i++;
	}
	sort_envp_idx(envp);
	return (ret);
}
