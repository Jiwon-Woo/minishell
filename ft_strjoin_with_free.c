#include "minishell.h"

char	*strjoin_exception_case(char *s1, char *s2)
{
	if (s1 == 0 && s2 == 0)
		return (0);
	else if (s1 == 0)
		return (s2);
	else if (s2 == 0)
		return (s1);
	return (0);
}

void	free_strjoin_arg(char *s1, char *s2)
{
	free(s1);
	s1 = 0;
	free(s2);
	s2 = 0;
}

char	*ft_strjoin_with_free(char *s1, char *s2)
{
	char	*str;
	int		len1;
	int		len2;
	int		index;
	int		i;

	if (s1 == 0 || s2 == 0)
		return (strjoin_exception_case(s1, s2));
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	str = (char *)malloc(sizeof(char) * (len1 + len2 + 1));
	if (str == 0)
		return (0);
	index = 0;
	i = 0;
	while (i < len1)
		str[index++] = s1[i++];
	i = 0;
	while (i < len2)
		str[index++] = s2[i++];
	str[index] = '\0';
	free_strjoin_arg(s1, s2);
	return (str);
}