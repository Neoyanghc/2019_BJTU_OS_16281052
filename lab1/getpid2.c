#include <stdio.h>  
#include <unistd.h>  
int main()  
{  
        pid_t tt;  
        asm volatile (  
             "movl $0x14,%%eax\n\t"  
             "int $0x80\n\t"  
             "movl %%eax,%0\n\t"  
             :"=m"(tt)  
        );  
        printf(" current PID is : %u\n",tt); 
        return 0; 
}
