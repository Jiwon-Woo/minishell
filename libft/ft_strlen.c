#include "libft.h"

int	ft_strlen(char *s)
{
	int	len;

	len = 0;
	if (s == 0)
		return (-1);
	while (s[len])
		len++;
	return (len);
}
