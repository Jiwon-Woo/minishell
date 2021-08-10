/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_arg_to_lst.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwoo <jwoo@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 11:50:42 by jwoo              #+#    #+#             */
/*   Updated: 2021/08/10 12:02:38 by jwoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	valid_quote(char **arg, char *line, t_quote *quote, int *i)
{
	if ((line[*i] == '\"' && *i != quote->q_double_index)
		|| (line[*i] == '\'' && *i != quote->q_single_index))
	{
		if (line[*i] == '\'')
		{
			*arg = str_append_char(*arg, line[(*i)++]);
			while (line[*i] != '\'' && line[*i] != 0)
				*arg = str_append_char(*arg, line[(*i)++]);
			if (line[*i] != 0)
				*arg = str_append_char(*arg, line[(*i)++]);
		}
		else
		{
			*arg = str_append_char(*arg, line[(*i)++]);
			while (line[*i] != '\"' && line[*i] != 0)
				*arg = str_append_char(*arg, line[(*i)++]);
			if (line[*i] != 0)
				*arg = str_append_char(*arg, line[(*i)++]);
		}
		return (1);
	}
	return (0);
}

void	invalid_quote(char **arg, char *line, int *i)
{
	if (line[*i] == '\'' || line[*i] == '\"')
		*arg = str_append_char(*arg, line[(*i)++]);
	while (ft_isspace(line[*i]) == 0 && line[*i] != 0 \
		&& line[*i] != '\'' && line[*i] != '\"')
		*arg = str_append_char(*arg, line[(*i)++]);
}

t_list	*arg_to_list(char *line, t_quote *quote)
{
	t_list	*arg_list;
	char	*arg;
	int		i;

	i = 0;
	arg_list = 0;
	while (line[i])
	{
		arg = 0;
		while (ft_isspace(line[i]) == 1 && line[i] != 0)
			i++;
		while (line[i])
		{
			if (valid_quote(&arg, line, quote, &i) == 0)
				invalid_quote(&arg, line, &i);
			if (ft_isspace(line[i]) == 1 || line[i] == 0)
				break ;
		}
		if (arg != NULL)
			ft_lstadd_back(&arg_list, ft_lstnew(arg));
	}
	return (arg_list);
}
