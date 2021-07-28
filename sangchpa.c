#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
// #include "ft_split_space.c"

/////////
#include <stdio.h>
#include <stdlib.h>

#define childnum 5

int main(void)
{
	pid_t pids[childnum];
	int processnum = 0;

	//main -> 부모
	//pids[0] -> 1자식
	//pipe[1]에 쓰고 
	//exit(pids[0])
	//pids[1] -> 2자식
	//pipe[0]에 읽어서 처리하고
	//pipe2[1]에 써놓는거죠
	//exit(pids[1])
	//pids[2] -> 3자식
	//pipe2[0]에 내용 읽고 처리

	int status;

	while(processnum < childnum)
	{
		pids[processnum] = fork();
		if(pids[processnum] < 0)
		{
    	    printf("error");
        	return -1;
		}
		else if(pids[processnum]==0)
		{
        	printf("I am a child : %d, processnum : %d\n", (int)getpid(), processnum);
        	sleep(1);
        	exit(0);
		}
		// else
		// {
		// 	wait(&status);
        // 	printf("I am a parent %d, My child is %d\n", (int)getpid() , (int)pids[processnum]);
        // 	sleep(1);
		// }
		processnum++;
	}
	wait(&status);
	printf("I am a parent %d, the end\n", (int)getpid());
	sleep(1);
	return 0;
} 
