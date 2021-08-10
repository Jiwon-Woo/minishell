/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi_ret_error.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwoo <jwoo@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 12:58:30 by jwoo              #+#    #+#             */
/*   Updated: 2021/08/10 12:58:32 by jwoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	atoi_sign(char *str)
{
	int	sign;
	int	idx;

	idx = 0;
	sign = 1;
	if (str == 0)
		return (0);
	if (str[idx] == '+' || str[idx] == '-')
	{
		if (str[idx] == '-')
			sign = -1;
		idx++;
	}
	if (!('0' <= str[idx] && str[idx] <= '9'))
		return (0);
	return (sign);
}

int	ft_atoi(char *str, int *integer)
{
	int	idx;
	int	num;
	int	sign;

	sign = atoi_sign(str);
	if (sign == 0 || ft_strlen(str) > 11)
		return (-1);
	idx = 0;
	if (str[idx] == '+' || str[idx] == '-')
		idx++;
	num = 0;
	while (str[idx])
	{
		if (num < 214748364 && '0' <= str[idx] && str[idx] <= '9')
			num = num * 10 + (str[idx] - '0');
		else if (num == 214748364 && '0' <= str[idx] && str[idx] <= '7')
			num = num * 10 + (str[idx] - '0');
		else if (num == 214748364 && str[idx] == '8' && sign == -1)
			num = num * (-10) - (str[idx] - '0');
		else
			return (-1);
		idx++;
	}
	*integer = sign * num;
	return (1);
}
