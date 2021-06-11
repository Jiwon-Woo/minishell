#include "minishell.h"

// 그냥 해본 것
int main()
{
	char		cwd[100];
	int 		fd;
	char		*line;

	fd = 1;
	getcwd((char *)cwd, 100);
	cwd[strlen((const char *)cwd)] = '$';
	cwd[strlen((const char *)cwd)] = 0;
	write(1, cwd, strlen((const char *)cwd));
	printf("\nchdir(\"../../\") function return %d\n", chdir("../../"));
	getcwd((char *)cwd, 100);
	cwd[strlen((const char *)cwd)] = '$';
	cwd[strlen((const char *)cwd)] = 0;
	printf("\nafter chdir ->");
	write(1, cwd, strlen((const char *)cwd));	
}
