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

void handler(int sig){
    //printf("This is second child handler\n");

    write(1,"This is second child handler\n",sizeof "This is second child handler\n" -1);
}

int main(){

    int x=getpgid(getpid());
    printf("%d\n",x);
    //setpgid(getpid(),getppid());
    // printf("This is second child\n");
    signal(SIGUSR1,handler);
    sleep(5);
}