#include "minishell.h"

char	**free_two_dimension(char **word, int num)
{
	int	n;

	n = -1;
	while (++n <= num)
	{
		free(word[n]);
		word[n] = 0;
	}
	free(word);
	word = 0;
	return (NULL);
}

int		factor_num(char *s)
{
	int	num;
	int	idx;

	if (*s == 0)
		return (0);
	num = 0;
	if (ft_isspace(s[0]) == 0)
		num++;
	idx = 0;
	while (++idx < ft_strlen(s))
	{
		if (ft_isspace(s[idx - 1]) == 1 && ft_isspace(s[idx]) == 0)
			num++;
	}
	return (num);
}

char	**factor_len(char **factor, char *s, int factor_num)
{
	int	len;
	int	idx;
	int	num;

	idx = 0;
	num = 0;
	while (num < factor_num && idx < ft_strlen(s))
	{
		while (ft_isspace(s[idx]) == 1 && idx < ft_strlen(s))
			idx++;
		len = 0;
		while (ft_isspace(s[idx]) == 0 && idx < ft_strlen(s))
		{
			len++;
			idx++;
		}
		if (len != 0)
		{
			factor[num] = (char *)malloc(sizeof(char) * (len + 1));
			if (factor[num] == 0)
				return (free_two_dimension(factor, num));
			num++;
		}
	}
	return (factor);
}

void	factor_split(char **factor, char *s, int factor_num)
{
	int	len;
	int	idx;
	int	num;

	idx = 0;
	num = 0;
	len = 0;
	while (num < factor_num && idx < ft_strlen(s))
	{
		if (ft_isspace(s[idx]) == 1)
		{
			idx++;
			continue ;
		}
		len = 0;
		while (ft_isspace(s[idx]) == 0 && idx < ft_strlen(s))
		{
			factor[num][len] = s[idx];
			len++;
			idx++;
		}
		factor[num][len] = '\0';
		num++;
	}
	factor[num] = 0;
}

char	**ft_split_space(char *s)
{
	char	**words;
	int		num;

	if (s == 0)
		return (0);
	num = factor_num(s);
	words = (char **)malloc(sizeof(char *) * (num + 1));
	if (words == 0)
		return (0);
	if (num == 0)
	{
		words[0] = NULL;
		return (words);
	}
	if (factor_len(words, s, num) == 0)
		return (0);
	factor_split(words, s, num);
	return (words);
}