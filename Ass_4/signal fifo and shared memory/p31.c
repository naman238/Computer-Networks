#include <stdio.h> 
#include <unistd.h> //for fork system call
#include <sys/types.h>
#include <stdlib.h> //for exit system call
#include <sys/wait.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <fcntl.h>
#include <pthread.h> 
#include <semaphore.h> 
#include <string.h>
#include<signal.h>
#include <sys/poll.h>
#include <sys/time.h>
#include <assert.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>
#include <unistd.h>
#include<stdio.h>



char* str2,*str1;
char *a="a";
 int pp;
int c=0;

void func(int kk){
    
    write(1, "P1 writing to shared memory X: \n", sizeof "P1 writing to shared memory X:  \n" - 1);

    strcpy(str1,strcat(str2,a));
   
    c++;
    printf("%s\n",str1);
   if(c==10) exit(1);
    kill(pp,SIGUSR1);
    wait(NULL);
    sleep(1);
} 

int main(){
   
   
    mkfifo("1to2",0666);
    mkfifo("2to1",0666);
    int fd1=open("1to2",O_WRONLY);
    int fd2=open("2to1",O_RDONLY);

    char buf1[1024],buf2[1024];
    sprintf(buf1,"%d",getpid());
    write(fd1,buf1,1024);

    read(fd2,buf2,1024);//wait until something is available

   
    sscanf(buf2,"%d",&pp);

    key_t key1 = ftok("x",65); 
  
    int shmid1 = shmget(key1,1024,0666|IPC_CREAT); 
  
    str1 = (char*) shmat(shmid1,(void*)0,0); 
    
    
    key_t key2 = ftok("y",66); 
  
    
    int shmid2 = shmget(key2,1024,0666|IPC_CREAT); 
  
    
    str2 = (char*) shmat(shmid2,(void*)0,0); 
    
    strcpy(str1,"a");
 

    signal(SIGUSR2,func);
    while(1)
    kill(pp,SIGUSR1);
    sleep(4);
   
   close(fd1);
   close(fd2);

    shmdt(str1); 
    shmdt(str2); 
 
    shmctl(shmid1,IPC_RMID,NULL);
   shmctl(shmid2,IPC_RMID,NULL);  

}