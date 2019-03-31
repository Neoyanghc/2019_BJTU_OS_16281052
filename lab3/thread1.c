#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>


volatile int counter = 0;
int loops;
sem_t *mySem=NULL;

volatile int buf[10];


void delay(){
 int i=10000;
 while(i--){
  int j=100000;
  while(j--);
 }
}

int i=0,j=0;
//read
void *worker1(void *arg) {

  sem_wait(mySem);
  printf("procees 1\n");
  sem_post(mySem);

//  for(;;i++){
//   if(i-j==10){
//    printf("buff full\n"); 
//    delay();
//        i--;}
//    else{scanf("%d",&buf[i%10]);}
//   //sem_post(mySem);
//  }

 return NULL;
}

//print
void *worker2(void *arg) {

  sem_wait(mySem);
  printf("procees 2\n");
  sem_post(mySem);

//  for(;;j++){
//   delay();
//   if (j-i==0){
//    printf("buff empty");
//    delay();
//    j--;
//   }
//   else
//    printf("print:%d\n",buf[j%10]);
//   //sem_post(mySem);
//  }

 return NULL;
}

int main(int argc, char *argv[])
{
  // loops = atoi(argv[1]);
  pthread_t p1, p2;
  //printf("Initial value : %d\n", counter);

  mySem = sem_open("myname",O_CREAT,0666,1);

  pthread_create(&p1, NULL, worker1, NULL);
  pthread_create(&p2, NULL, worker2, NULL);
  pthread_join(p1, NULL);
  pthread_join(p2, NULL);
  sem_close(mySem);

  printf("The end\n");

  sem_unlink("myname");
  return 0;
}
