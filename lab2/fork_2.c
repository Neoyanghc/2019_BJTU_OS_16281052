#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int execl(const char* path,const char* arg,...); 

int main(int argc, char *argv[])
{
	pid_t pid;
	pid = fork();
	
	if( pid < 0 ){	// 没有创建成功
		perror("fork create error");
	}
	
	if(0 == pid){ // 子进程
		printf("I am son\n");
        int ret;
        ret = execl("/usr/bin/vi","vi","text.txt",(char*)0);
        if (ret == -1) 
        {
            perror ("execl");
        }
	}else if(pid > 0){ // 父进程
        printf("I am father\n");
		while(1){
			sleep(1);
		}
	}
	return 0;
}
