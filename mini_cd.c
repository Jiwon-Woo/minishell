/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_cd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwoo <jwoo@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/11 09:36:59 by jwoo              #+#    #+#             */
/*   Updated: 2021/08/14 11:23:41 by jwoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	free_ret(char **str, int ret)
{
	if (*str != 0)
		free(*str);
	*str = 0;
	return (ret);
}

void	print_cd_err(char *arg)
{
	write(2, "minish: ", ft_strlen("minish: "));
	write(2, arg, ft_strlen(arg));
	write(2, ": Not a directory\n", ft_strlen(": Not a directory\n"));
}

int	mini_cd(char **arg, t_envp *envp)
{
	int		ret;
	char	**pwd;
	char	*home;

	ret = 0;
	home = get_value("HOME", envp->envp_list);
	if (arg[1] == 0)
		ret = chdir(home);
	else
		ret = chdir(arg[1]);
	if (ret != -1)
	{
		pwd = get_env_ptr("PWD", envp->envp_list);
		free(*pwd);
		*pwd = ft_strjoin_with_free(ft_strdup("PWD="), getcwd(NULL, 0));
		return (free_ret(&home, 0));
	}
	else if (arg[1] == 0)
		file_or_directory(home);
	else if (!file_or_directory(arg[1]))
		print_cd_err(arg[1]);
	return (free_ret(&home, 1));
}
