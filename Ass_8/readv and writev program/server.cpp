#include <iostream>
#include <stdio.h> 
#include <unistd.h> //for fork system call
#include <sys/types.h>
#include <stdlib.h> //for exit system call
#include <sys/wait.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 
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
#include <sys/uio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
using namespace std;
const char* name1="clientToServer";
const char* name2="serverToClient";
int fd1;
int fd2;
char wbuff[100];
char rbuff[100];
struct iovec rvec[1];
struct iovec wvec[1];
void* reader(void* args)
{
    while(1)
    {
        rvec[0].iov_base=rbuff;
        rvec[0].iov_len=sizeof(rbuff);
      
        int x=readv(fd1,rvec,1);
        if(x==-1)
        {
            perror("error while reading");
        }
        else printf("%s\n",rbuff);
    }
}
void* writer(void* args)
{
    string str;
    while(1)
    {
        getline(cin,str);
        strcpy(wbuff,str.c_str());
        wvec[0].iov_base=wbuff;
        wvec[0].iov_len=sizeof(wbuff);
        
        int x=writev(fd2,wvec,1);
        if(x==-1)
        {
            perror("error while writing");
        }
    }
}
int main()
{
    mkfifo(name1,0666);
    fd1=open(name1,O_RDONLY);
    mkfifo(name2,0666);
    fd2=open(name2,O_WRONLY);
    pthread_t t1,t2;
    pthread_create(&t1,NULL,&reader,NULL);
    pthread_create(&t1,NULL,&writer,NULL);
    pthread_join(t1,NULL);
    pthread_join(t2,NULL);

   
    return 0;
}