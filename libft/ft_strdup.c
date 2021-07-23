#include "libft.h"

char	*ft_strdup(char *s1)
{
	char	*str;
	int	count;

	str = NULL;
	count = ft_strlen(s1);
	str = malloc(sizeof(char) * (count + 1));
	if (str == NULL)
		return (NULL);
	count = 0;
	while (s1[count])
	{
		str[count] = s1[count];
		count++;
	}
	str[count] = 0;
	return (str);
}
