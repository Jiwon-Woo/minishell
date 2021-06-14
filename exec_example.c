#include "minishell.h"
//사용가능 함수, escape \ 방식, "" '' 
int main()
{
	pid_t	pid;
	char	*arg[] = {"ls", (char *)0};
	char	*arg1[] = {"echo", "hello world", (char *)0};
	char	*envp[] = {"USER=user", (char *) 0};
	int 	status;
	
	pid = fork();
	wait(&status);
	if (pid == 0)
	{
		execve("/bin/echo", arg1, envp);
		execve("/bin/ls", arg, envp);
	}
	else
	{
		wait(&status);
		printf("pid = %d\n", pid);
		printf("child ps exit status -> %d\n", status);
		return(0);
	}
}