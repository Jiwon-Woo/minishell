/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwoo <jwoo@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 10:00:18 by jwoo              #+#    #+#             */
/*   Updated: 2021/08/10 10:05:15 by jwoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*strjoin_exception(char *s1, char *s2)
{
	if (s1 == 0 && s2 == 0)
		return (0);
	else if (s1 == 0)
		return (s2);
	else
		return (s1);
}

char	*ft_strjoin(char *s1, char *s2)
{
	char	*str;
	int		len1;
	int		len2;
	int		index;
	int		i;

	if (s1 == 0 || s2 == 0)
		return (strjoin_exception(s1, s2));
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	str = (char *)malloc(sizeof(char) * (len1 + len2 + 1));
	if (str == 0)
		exit (1);
	index = 0;
	i = 0;
	while (i < len1)
		str[index++] = s1[i++];
	i = 0;
	while (i < len2)
		str[index++] = s2[i++];
	str[index] = '\0';
	return (str);
}
