/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_quote.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwoo <jwoo@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 10:41:03 by jwoo              #+#    #+#             */
/*   Updated: 2021/08/10 10:41:31 by jwoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_quote(t_quote *quote)
{
	quote->q_single = 1;
	quote->q_double = 1;
	quote->q_single_index = -1;
	quote->q_double_index = -1;
	quote->q_remain_single = -42;
	quote->q_remain_double = -42;
}

int	is_remain_quote(char *line, int idx, char quote)
{
	while (line[++idx])
	{
		if (line[idx] == quote)
			return (idx);
	}
	return (-1);
}

void	valid_or_invalid_quote(char *line, int idx, t_quote *quote)
{
	if (line[idx] == '\'' && (quote->q_double == 1 \
		|| (quote->q_double != 1 && quote->q_remain_double == -1)))
	{
		quote->q_single *= -1;
		quote->q_single_index = idx;
		if (quote->q_single == -1)
			quote->q_remain_single = is_remain_quote(line, idx, '\'');
	}
	else if (line[idx] == '\"' && (quote->q_single == 1 \
		|| (quote->q_single != 1 && quote->q_remain_single == -1)))
	{
		quote->q_double *= -1;
		quote->q_double_index = idx;
		if (quote->q_double == -1)
			quote->q_remain_double = is_remain_quote(line, idx, '\"');
	}
}

void	check_quote(char *line, t_quote *quote)
{
	int	idx;

	init_quote(quote);
	idx = -1;
	while (++idx < ft_strlen(line))
	{
		valid_or_invalid_quote(line, idx, quote);
	}
	if (quote->q_single == 1)
		quote->q_single_index = -1;
	if (quote->q_double == 1)
		quote->q_double_index = -1;
}
