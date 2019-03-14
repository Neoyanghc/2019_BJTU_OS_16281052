#include <unistd.h>
#include <stdio.h>
int main(void)
{
    int i = 0;
    pid_t fpid,ppid;
    //pid指当前进程的pid,
    //fpid指fork返回给当前进程的值
    ppid = getppid();
    printf("the first node of tree‘s father %d\n",ppid);
    for (i = 0; i < 2; i++)
    {
        fpid = fork();
        if (fpid == 0)
        {
            // printf("%d child  %4d %4d %4d\n", i, getppid(), getpid(), fpid);
        }
        else{
            printf("%d parent %4d %4d\n", i, getpid(), fpid);
        }
        if (i == 1 && fpid != 0 && ppid != getppid())
        {
            fpid = fork();
            if (fpid == 0)
            {
                // printf("%d child  %4d %4d %4d\n", i, getppid(), getpid(), fpid);
            }
            else{
                 printf("%d parent %4d %4d\n", i,getpid(), fpid);
            }
        }
    }
    return 0;
}