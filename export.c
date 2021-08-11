/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwoo <jwoo@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 12:17:25 by jwoo              #+#    #+#             */
/*   Updated: 2021/08/10 17:51:52 by jwoo             ###   ########.fr       */
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
