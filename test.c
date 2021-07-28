// #include <unistd.h>
// #include <sys/wait.h>
// #include <stdio.h>
// #include <stdlib.h>

// int main(int argc, char *argv[]) {//  a | b | c | d
//   int fds[2];                      // 2개의 fd를 담을 배열을 정의한다.
//   pipe(fds);                       // pipe를 호출해 두 개의 fd로 배열을 채워준다.
//   pid_t pid = fork();              // 부모 프로세스의 fd와 메모리를 복제한 자식 프로세스 생성한다.

//   if (pid == 0) {                  // if pid == 0, 자식 프로세스
//     dup2(fds[0], STDIN_FILENO);    // fds[0]으로 표준입력을 넘겨준다.
//     close(fds[0]);                 // fds[0]은 자식 프로세스에서 더이상 필요하지 않기 떄문에 닫아준다. 복사본이기 때문에(?)
//     close(fds[1]);                 // 원래부터 필요없었던 fd라 닫아준다.
//     char *cmd[] = {(char *)"cat", "-e", NULL};   // sort 명령어 인자를 만들어준다.
//     if (execvp(cmd[0], cmd) < 0) 
//       exit(0);  // sort 명령어 실행하고 문제있으면 exit
//   } 
  
//   /* 현재 : mini_env 안에서 -> fork 
//   fork 먼저 -> 
//     <명령어 함수는 fork 안하고 그냥 그 기능만 수행하도록, 그 다음 파이프가 있으면 그거만 포크>

//   ex. x | y
//   fork ()
//    -> x를 실행{
//      다음에 파이프로 연결되면 fork ()실행
//       자식 프로세스에서 y 실행
//    }

	
// 	while (cmd list가 널일때까지 (끝까지))
// 	{
// 		if (유효한 명령어면)
// 		{
// 			fork();
// 			if (pipe 플래그면)
// 			{
// 				dup2();
// 			}
// 		}
// 	}


//   -> fork
//   {if 자식 => x 라는 명령어 함수(파이프 정보(앞,뒤)){ x | y
    
//     전에 파이프 있으면 -> 표준 입력을 파이프로 교체
    
//     후에 파이프 있으면 -> 표준 출력을 파이프로 교체 (x 함수 내에서 자식을 하나 더 만들어야 함)
//     -> fork()
//     if(자식의 자식)
//     {
//       y라는 명령어 함수(파이프 정보도 함께 인자로);
//     }
//     x 명령어 실행 후 표준 출력 파이프로 보내주기
//    }}
//    return (status)
//   */

//   // 부모 프로세스 코드 시작
//   close(fds[0]);                 // 쓰기만 하면되는 부모 프로세스에서는 필요 없는 fd라 닫아준다. 
//   // const char *words[] = {"pear", "peach", "apple"}; // 자식 프로세스에서 읽을 write input 

//   for (int i = 1; i < argc; i++) {
//     dprintf(fds[1], "%s\n", argv[i]); // fds[1]에 출력을 쓴다.
//   }
// //   dprintf(fds[1], 0);
//   close(fds[1]);

//   int status;
//   pid_t wpid = waitpid(pid, &status, 0); // 자식 프로세스가 종료될때까지 기다린다.
//   return (wpid == pid && WIFEXITED(status) ? WEXITSTATUS(status) : -1);
// }

/*
** pipex.c - multipipes support
*/

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

/*
 * loop over commands by sharing
 * pipes.
 */
static void pipeline(char ***cmd)
{
	  int fd[2];
	  pid_t pid;
	  int fdd = 0;				/* Backup */

	  while (*cmd != NULL) 
    {
	      pipe(fd);				/* Sharing bidiflow */
	      if ((pid = fork()) == -1) 
        {
	      	  perror("fork");
	      	  exit(1);
	      }
	      else if (pid == 0) 
        {
	      	  dup2(fdd, 0);
	      	  if (*(cmd + 1) != NULL) 
            {
	      	  	  dup2(fd[1], 1);
	      	  }
	      	  close(fd[0]);
	      	  execvp((*cmd)[0], *cmd);
	      	  exit(1);
	      }
	      else 
        {
	      	  wait(NULL); 		/* Collect childs */
	      	  close(fd[1]);
	      	  fdd = fd[0];
	      	  cmd++;
	      }
	  }
}

/*
 * Compute multi-pipeline based
 * on a command list.
 */
int main(int argc, char *argv[])
{
	char *ls[] = {"ls", "-al", NULL};
	char *rev[] = {"rev", NULL};
	char *nl[] = {"nl", NULL};
	char *cat[] = {"cat", "-e", NULL};
	char **cmd[] = {ls, rev, nl, cat, NULL};

	pipeline(cmd);
	return (0);
}