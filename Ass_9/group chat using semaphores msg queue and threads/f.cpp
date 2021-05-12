#include <iostream>
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
#include <sys/poll.h>
#include <sys/time.h>
#include <assert.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/msg.h> // for msg queue
#include <string.h>
using namespace std;
const char* sem1="i1";
const char* sem2="i";
const char* sem3="i2";
const char* sem4="i3";
int main()
{
   
    
    sem_t* e=sem_open(sem2,O_CREAT,0666,0);
    
    if(e==SEM_FAILED)perror("error");
    sem_wait(e);
    sem_wait(e);
    sem_wait(e);
    sem_t* e1=sem_open(sem1,0);
    sem_t* e2=sem_open(sem3,0);
    sem_t* e3=sem_open(sem4,0);
    sem_post(e1);
    sem_post(e2);
    sem_post(e3);
    sem_unlink(sem1);
    sem_unlink(sem2);
    sem_unlink(sem3);
    sem_unlink(sem4);

     
    
return 0;
}