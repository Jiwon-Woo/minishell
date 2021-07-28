int		ft_isspace(char s)
{
	if (s == 32 || (9 <= s && s <= 13))
		return (1);
	return (0);
}
