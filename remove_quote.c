/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_quote.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwoo <jwoo@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 11:02:06 by jwoo              #+#    #+#             */
/*   Updated: 2021/08/10 11:33:52 by jwoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	in_single_quote(char **ret, char *cmd_line, int i)
{
	while (cmd_line[i] != '\'' && cmd_line[i] != 0)
		*ret = str_append_char(*ret, cmd_line[i++]);
	if (cmd_line[i] == '\'')
		i++;
	return (i);
}

int	in_double_quote(char **ret, char *cmd_line, int i, t_envp *envp)
{
	while (cmd_line[i] != '\"' && cmd_line[i] != 0)
	{
		if (cmd_line[i] == '$')
			i = find_env(ret, cmd_line, ++i, envp);
		else
			*ret = str_append_char(*ret, cmd_line[i++]);
	}
	if (cmd_line[i] == '\"')
		i++;
	return (i);
}

int	without_quote_env(char **ret, char *cmd_line, int i)
{
	if (cmd_line[i] == '\'' || cmd_line[i] == '\"')
		*ret = str_append_char(*ret, cmd_line[i++]);
	while (cmd_line[i] != '\'' && cmd_line[i] != '\"'
		&& cmd_line[i] != '$' && cmd_line[i] != 0)
		*ret = str_append_char(*ret, cmd_line[i++]);
	return (i);
}

char	*remove_quote(char *cmd_line, t_envp *envp)
{
	int		i;
	t_quote	quote;
	char	*ret;

	init_quote(&quote);
	check_quote(cmd_line, &quote);
	ret = ft_strdup("");
	i = 0;
	while (cmd_line[i])
	{
		if (cmd_line[i] == '\'' && i != quote.q_single_index)
			i = in_single_quote(&ret, cmd_line, ++i);
		else if (cmd_line[i] == '\"' && i != quote.q_double_index)
			i = in_double_quote(&ret, cmd_line, ++i, envp);
		else if (cmd_line[i] == '$')
			i = find_env(&ret, cmd_line, ++i, envp);
		else
			i = without_quote_env(&ret, cmd_line, i);
	}
	return (ret);
}
