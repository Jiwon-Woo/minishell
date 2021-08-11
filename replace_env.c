/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwoo <jwoo@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 11:23:56 by jwoo              #+#    #+#             */
/*   Updated: 2021/08/10 17:48:57 by jwoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_env(char *cmd_line, int i)
{
	if (cmd_line[i] != 0 && ft_isspace(cmd_line[i]) == 0
		&& cmd_line[i] != '$' && cmd_line[i] != '?' && cmd_line[i] != '='
		&& cmd_line[i] != '\"' && cmd_line[i] != '\'')
	{
		return (1);
	}
	return (0);
}

int	get_equal_idx(char *env_line)
{
	int	i;

	i = -1;
	while (env_line[++i])
	{
		if (env_line[i] == '=')
			return (i);
	}
	return (-1);
}

char	*get_value(char *key, char **envp)
{
	int	i;
	int	equal_idx;

	i = -1;
	while (envp[++i])
	{
		equal_idx = get_equal_idx(envp[i]);
		if (ft_strlen(key) == equal_idx && \
			ft_strncmp(envp[i], key, equal_idx) == 0)
			return (ft_strdup(envp[i] + (ft_strlen(key) + 1)));
	}
	return (ft_strdup(""));
}

void	replace_env_value(char *key, char **ret, t_envp *envp)
{
	char	*value;

	if (key == 0)
		*ret = str_append_char(*ret, '$');
	else
	{
		value = get_value(key, envp->envp_list);
		*ret = ft_strjoin_with_free(*ret, value);
	}
}
