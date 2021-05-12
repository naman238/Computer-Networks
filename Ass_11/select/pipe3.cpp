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
#include <sys/stat.h> 
#include<iostream>
#include <sys/select.h>
using namespace std;


int main(){
    const char *fifo2="xbz";
    mkfifo(fifo2,0666);
    int fd1=open(fifo2,O_WRONLY);
    
    int t=2;
    while(t--){
        cout<<"give input: ";
        char buff[1024];
        cin>>buff;
        write(fd1,buff,1024);
    }
}