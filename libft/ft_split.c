/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwoo <jwoo@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/02 22:41:08 by jwoo              #+#    #+#             */
/*   Updated: 2021/06/28 19:43:39 by jwoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	word_num(char *s, char c)
{
	int	num;
	int	index;

	num = 0;
	if (s[0] != c)
		num++;
	index = 1;
	while (index < ft_strlen(s))
	{
		if (s[index - 1] == c && s[index] != c)
			num++;
		index++;
	}
	return (num);
}

static char		**free_split(char **words, int num)
{
	int	n;

	n = 0;
	while (n <= num)
	{
		free(words[n]);
		words[n] = NULL;
	}
	free(words);
	words = NULL;
	return (words);
}

static char		**word_len(char **words, char *s, char c)
{
	int	index;
	int	len;
	int	num;

	index = 0;
	num = 0;
	while (index < ft_strlen(s) && words != NULL)
	{
		while (s[index] == c && index < ft_strlen(s))
			index++;
		len = 0;
		while (s[index] != c && index < ft_strlen(s))
		{
			len++;
			index++;
		}
		if (len != 0)
		{
			words[num] = (char *)malloc(sizeof(char) * (len + 1));
			if (words[num] == NULL)
				free_split(words, num);
			num++;
		}
	}
	return (words);
}

static void		word_split(char **words, char *s, char c)
{
	int	index;
	int	len;
	int	num;

	index = 0;
	num = 0;
	len = 0;
	while (index < ft_strlen(s))
	{
		if (s[index] == c)
		{
			index++;
			continue ;
		}
		len = 0;
		while (s[index] != c && index < ft_strlen(s))
		{
			words[num][len] = s[index];
			len++;
			index++;
		}
		words[num][len] = '\0';
		num++;
	}
	words[num] = NULL;
}

char			**ft_split(char *s, char c)
{
	char	**words;
	int	num;

	if (s == NULL)
		return (NULL);
	num = word_num(s, c);
	words = (char **)malloc(sizeof(char *) * (num + 1));
	if (words == NULL)
		return (NULL);
	if (word_len(words, s, c) == NULL)
		return (NULL);
	word_split(words, s, c);
	return (words);
}