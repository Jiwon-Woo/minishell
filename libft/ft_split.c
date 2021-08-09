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
		words[n] = 0;
	}
	free(words);
	words = 0;
	return (words);
}

static char		**word_len(char **words, char *s, char c)
{
	int	index;
	int	len;
	int	num;

	index = 0;
	num = 0;
	while (index < ft_strlen(s) && words != 0)
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
			if (words[num] == 0)
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
	words[num] = 0;
}

char			**ft_split(char *s, char c)
{
	char	**words;
	int	num;

	if (s == 0)
		return (0);
	num = word_num(s, c);
	words = (char **)malloc(sizeof(char *) * (num + 1));
	if (words == 0)
		return (0);
	if (word_len(words, s, c) == 0)
		exit (0);
	word_split(words, s, c);
	return (words);
}