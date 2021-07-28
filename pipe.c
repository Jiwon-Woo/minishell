#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "ft_split_space.c"

int main(int argc, char *argv[], char *envp[])
{
	int fds[2];			// 2개의 fd를 담을 배열을 정의한다.
	int status;

	pipe(fds);			// pipe를 호출해 두 개의 fd로 배열을 채워준다.
	fprintf(stderr, "\nfds[0] (read) : %d, fds[1] (write): %d\n", fds[0], fds[1]);

	pid_t pid = fork(); // 부모 프로세스의 fd와 메모리를 복제한 자식 프로세스 생성한다.

	
	if (pid == 0)
	{
		fprintf(stderr, "this is child process\n");	// if pid == 0, 자식 프로세스
		close(fds[0]);								// 자식 프로세스는 파이프로부터 읽지 않기 때문에 파이프의 읽는 쪽을 닫아준다. 표준 입력으로부터 읽어옴(?)
		dup2(fds[1], STDOUT_FILENO);				// 자식 프로세스의 표준 출력은 fds[1]이 됨, 자식 프로세스가 표준 출력으로 내보내는 모든 데이터가 파이프에 쓰여짐
		// close(fds[1]);							// 원래부터 필요없었던 fd라 닫아준다.
		execve("/bin/echo", argv, envp);	
	}
	// 부모 프로세스 코드 시작, 부모 프로세스만 여기 도달함
	fprintf(stderr, "this is parent process\n");
	close(fds[1]);									// 부모 프로세스는 파이프에 쓰지 않으므로 닫는다. 자식 프로세스가 파이프에 쓴 출력값을 입력값으로 받아 표준 출력에 출력할 것이다.
	
	char buf[1024];
	int n = read(fds[0], buf, 1024);	// 자식 프로세스가 파이프 입구에 쓴 것을 read해옴
	dprintf(1, "dprintf to stdout\n");
	write(1, buf, n);

	// char *cmd[] = {(char *)"grep", "h", NULL};

	// execve("/usr/bin/grep", cmd, envp);

	close(fds[1]);
	wait(&status);
	return (status);
}