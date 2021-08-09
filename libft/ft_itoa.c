#include "libft.h"

int		itoa_sign(int n)
{
	if (n < 0)
		return (-1);
	else if (n == 0)
		return (0);
	else
		return (1);
}

int	itoa_len(int n)
{
	int		len;

	len = 0;
	if (itoa_sign(n) <= 0)
		len++;
	while (n != 0)
	{
		n /= 10;
		len++;
	}
	return (len);
}

char			*ft_itoa(int n)
{
	char		*integer;
	int			len;
	int			index;
	int				sign;
	unsigned int	num;

	sign = itoa_sign(n);
	len = itoa_len(n);
	num = sign * n;
	integer = (char *)malloc(sizeof(char) * (len + 1));
	if (integer == 0)
		exit (1);
	index = len;
	integer[index] = 0;
	while (--index > 0)
	{
		integer[index] = (num % 10) + '0';
		num /= 10;
	}
	if (sign == -1)
		integer[index] = '-';
	else
		integer[index] = num + '0';
	return (integer);
}
