
#include "minishell.h"

// int		ft_isspace(char s)
// {
// 	if (s == 32 || (9 <= s && s <= 13))
// 		return (1);
// 	return (0);
// }
// 음    음,, fork 해서 exec 까지 해볼게요 그럼
// 일단 미니쉘이 바쉬기준이니까 ./minishell 하자마자 bash를 켜야할까요
// 좋아요 어떻게 하는 건진 모르겠지만...
// 아님 할줄 아시면 한번 보여주세요~! // 오 감사합니다 포커싱해놓을게요

int 		get_arg_num(char *line)
{
	int 	arg_num;

	while (ft_isspace(*line))
		line++;
	arg_num = 1;
	while(*line)
	{
		if (*line == '\"' && *line == '\'')
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
			else if (ft_isspace(*line))
			{
				while (ft_isspace(*line))
					line++;
				arg_num++;
			}
		}
		else if (ft_isspace(*line))
		{
			while (ft_isspace(*line))
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