/*
 *File: fifo_rcv.c
 */
 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <fcntl.h>


#define FIFO "/tmp/my_fifo"

int main()
{
    char buf[20] ;
    memset(buf, '\0', sizeof(buf));

    //`. check the fifo file existed or not
    int ret;
    ret = access(FIFO, F_OK);
    if(ret != 0)    //file /tmp/my_fifo existed
    {
        fprintf(stderr, "FIFO %s does not existed", FIFO);
        exit(EXIT_FAILURE);
    }

    //2.Open the fifo file
    int fifo_fd;
    fifo_fd = open(FIFO, O_RDONLY);
    if(-1 == fifo_fd)
    {
        perror("open");
        exit(EXIT_FAILURE);

    }

    //4. read the fifo file
    int num = 0;
    num = read(fifo_fd, buf, sizeof(buf));

    printf("Read %d words: %s\n", num, buf);

    close(fifo_fd);

    return 0;
}