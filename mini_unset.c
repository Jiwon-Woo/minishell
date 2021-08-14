/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_unset.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwoo <jwoo@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 16:21:14 by jwoo              #+#    #+#             */
/*   Updated: 2021/08/14 11:59:23 by jwoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_flag(t_envp *envp, char **arg, int i)
{
	int	flag;
	int	j;

	j = -1;
	flag = -1;
	while (++j < get_arg_size(envp->envp_list))
		if ((get_equal_idx(envp->envp_list[j]) == ft_strlen(arg[i]) \
		&& ft_strncmp(envp->envp_list[j], arg[i], ft_strlen(arg[i])) == 0) \
		|| ft_strncmp(arg[i], envp->envp_list[j], ft_strlen(arg[i]) + 1) == 0)
			flag = j;
	return (flag);
}

void	unset_var(t_envp *envp, int flag)
{
	char	**temp;
	int		i;
	int		idx;
	int		size;

	i = -1;
	idx = 0;
	size = get_arg_size(envp->envp_list);
	temp = (char **)malloc(sizeof(char *) * size);
	if (temp == 0)
		exit(1);
	while (++i < size)
	{
		if (i != flag)
			temp[idx++] = envp->envp_list[i];
		else
		{
			free(envp->envp_list[i]);
			envp->envp_list[i] = 0;
		}
	}
	temp[idx] = 0;
	if (envp->envp_list != 0)
		free(envp->envp_list);
	envp->envp_list = temp;
}

int	print_unset_err(char *arg)
{
	write(2, "minish: unset: `", ft_strlen("minish: unset: `"));
	write(2, arg, ft_strlen(arg));
	write(2, "\': not a valid identifier\n", \
		ft_strlen("\': not a valid identifier\n"));
	return (1);
}

int	mini_unset(char **arg, t_envp *envp)
{
	int	size;
	int	i;
	int	flag;
	int	ret;

	ret = 0;
	size = get_arg_size(arg);
	if (size == 1)
		return (0);
	i = 0;
	while (++i < size)
	{
		if (get_equal_idx(arg[i]) != -1 || env_validation(arg[i]) == -1)
		{
			if (ft_strlen(arg[i]) > 0)
				ret = print_unset_err(arg[i]);
			continue ;
		}
		flag = get_flag(envp, arg, i);
		if (flag != -1)
			unset_var(envp, flag);
	}
	sort_envp_idx(envp);
	return (ret);
}
