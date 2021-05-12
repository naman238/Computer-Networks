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
struct msgbuff
{
    long mtype;
    char mtext[100];
};
int main()
{
    key_t key=ftok(".",64);
    int msgid=msgget(key,0666|IPC_CREAT);
    int x=getpid();
    struct msgbuff msg;
    msg.mtype=x;
    msgsnd(msgid,&msg,sizeof(msg),0);
    key_t key4=ftok(".",65);
    int msgid4=msgget(key4,0666|IPC_CREAT);
    struct msgbuff common;
    while(1)    
        {
            if(msgrcv(msgid4,&common,sizeof(common),x,IPC_NOWAIT)!=-1)
            {printf("%s",common.mtext);break;}
        }
}