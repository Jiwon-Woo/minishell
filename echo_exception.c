
#include "minishell.h"

// int		is_space(char s)
// {
// 	if (s == 32 || (9 <= s && s <= 13))
// 		return (1);
// 	return (0);
// }

int 		get_arg_num(char *line)
{
	int 	arg_num;

	while (is_space(*line))
		line++;
	arg_num = 1;
	while(*line)
	{
		if (*line == '\"' &&|| *line == '\'')
		{
			line++;
			if (*line == '\"')
				while (*line != '\"' && *line)
					line++;
			else if (*line == '\'')
				while (*line != '\'' && *line)
					line++;
			if (*line++ == 0)
				return (-1);
			else if (is_space(*line))
			{
				while (is_space(*line))
					line++;
				arg_num++;
			}
		}
		else if (is_space(*line))
		{
			while (is_space(*line))
				line++;
			if (*line)
				arg_num++;
		}
		else
			line++;
	}
	return (arg_num);
}
// int main()
// {
// 	printf("ARG NUM = %d\n", get_arg_num("        \"d \"f \"ssd\" d 0         "));
// }