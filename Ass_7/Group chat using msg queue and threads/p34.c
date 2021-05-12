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
#include <mqueue.h>

#define MSG_EXCEPT 020000
struct msgbuff
{
    long mtype;
    char mtext[100];
};
key_t key;
int msgid;
int x;
key_t key4;
int msgid4;
struct msgbuff msg,msg1;
void* reader(void*args)
{
    
    while(1)
    {
        if(msgrcv(msgid4,&msg,sizeof(msg),x,IPC_NOWAIT)!=-1)
        {
            printf("%s\n",msg.mtext);
        }
    }
    
}
void* writer(void* args)
{
    char buff[100];
     
    while(1)
    {
        fgets(buff,100,stdin);
       
        msg1.mtype=x;
        strcpy(msg1.mtext,buff);
        msgsnd(msgid,&msg1,sizeof(msg1),0);
    }
}
int main()
{
    key=ftok(".",64);
    msgid=msgget(key,0666|IPC_CREAT);
    x=getpid();
    
    msg1.mtype=x;
    msgsnd(msgid,&msg1,sizeof(msg1),0);
    key4=ftok(".",65);
    msgid4=msgget(key4,0666|IPC_CREAT);
    

    pthread_t t1,t2;
    pthread_create(&t1,NULL,reader,NULL);
    pthread_create(&t1,NULL,writer,NULL);
    pthread_join(t1,NULL);
    pthread_join(t2,NULL);
    return 0;
}