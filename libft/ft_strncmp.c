#include "libft.h"

int	ft_strncmp(char *s1, char *s2, int n)
{
	int	len;
	
	if (n == 0)
		return (0);
	if (s1 == 0 || s2 == 0)
		return (-42);
	len = 0;
	while (len + 1 < n && s1[len] && s2[len])
	{
		if (s1[len] == s2[len])
			len++;
		else
			break ;
	}
	return (s1[len] - s2[len]);
}