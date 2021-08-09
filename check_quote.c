#include "minishell.h"

void init_quote(t_quote *quote)
{
	quote->q_single = 1;
	quote->q_double = 1;
	quote->q_single_index = -1;
	quote->q_double_index = -1;
}

int		is_remain_quote(char *line, int idx, char quote)
{
	while (line[++idx])
	{
		if (line[idx] == quote)
			return (idx);
	}
	return (-1);
}

void	check_quote(char *line, t_quote *quote)
{
	int	idx;
	int remain_single_quote;
	int remain_double_quote;

	init_quote(quote);
	idx = -1;
	remain_single_quote = -42;
	remain_double_quote = -42;
	while (++idx < ft_strlen(line))
	{
		if (line[idx] == '\'' && (quote->q_double == 1
			|| (quote->q_double != 1 && remain_double_quote == -1)))
		{
			quote->q_single *= -1;
			quote->q_single_index = idx;
			if (quote->q_single == -1)
				remain_single_quote = is_remain_quote(line, idx, '\'');
		}
		else if (line[idx] == '\"' && (quote->q_single == 1
			|| (quote->q_single != 1 && remain_single_quote == -1)))
		{
			quote->q_double *= -1;
			quote->q_double_index = idx;
			if (quote->q_double == -1)
				remain_double_quote = is_remain_quote(line, idx, '\"');
		}
	}
	if (quote->q_single == 1)
		quote->q_single_index = -1;
	if (quote->q_double == 1)
		quote->q_double_index = -1;
}
