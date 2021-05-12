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

    // int fd1=fileno(popen("./c1","r"));
    // int fd2=fileno(popen("./c2","r"));
    // int fd3=fileno(popen("./c3","r"));

   const char *fifo1="xyz";
    mkfifo(fifo1,0666 | O_CREAT);
    const char *fifo2="ayz";
    mkfifo(fifo2,0666 | O_CREAT);
    const char *fifo3="xbz";
    mkfifo(fifo3,0666 | O_CREAT);

    int fd1=open(fifo1,O_RDWR);
    int fd2=open(fifo2,O_RDWR);
    int fd3=open(fifo3,O_RDWR);

    fd_set rset;
    FD_ZERO(&rset);
    int n=max(fd1,max(fd2,fd3))+1;
    int fd[3]={fd1,fd2,fd3};
      FD_SET(fd1,&rset);
        FD_SET(fd2,&rset);
        FD_SET(fd3,&rset);
    while(1){
         
        int nready=select(n,&rset,NULL,NULL,NULL);
        for(int i=0;i<3;i++){
            if(FD_ISSET(fd[i],&rset)){
                char buff[1024];
                read(fd[i],buff,1024);
                cout<<"Message from client"<<i+1<<" : "<<buff<<endl;
                cout.flush();
            }
            else FD_SET(fd[i],&rset);
        }
    }


}