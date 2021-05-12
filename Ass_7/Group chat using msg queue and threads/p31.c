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
int cnt;
key_t key4;
int msgid4;
     
struct msgbuff msg;
struct msgbuff msg1;
void handler(int sig)
{
    msgctl(msgid,IPC_RMID,NULL);
    msgctl(msgid4,IPC_RMID,NULL);
    signal(SIGINT,SIG_DFL);
    raise(SIGINT);
}
int main()
{  
    signal(SIGINT,handler);
    key=ftok(".",64);
    msgid=msgget(key,0666|IPC_CREAT);
   
    key4=ftok(".",65);
    msgid4=msgget(key4,0666|IPC_CREAT);
    cnt=0;
    int pr_id[100];
    while(1)
    { 
        if(msgrcv(msgid,&msg1,sizeof(msg1),0,IPC_NOWAIT)!=-1)
        {
            pr_id[cnt++]=msg1.mtype;
            //printf("hello");
        }
        else break;
    }
    printf("%d",cnt);
    fflush(stdout);
    while(1)
    {
        if(msgrcv(msgid,&msg1,sizeof(msg1),0,IPC_NOWAIT)!=-1)
        {
            int temp=msg1.mtype;
            for(int i=0;i<cnt;i++)
            {
                if(pr_id[i]!=temp)
                {
                    printf("%d ",pr_id[i]);
                    fflush(stdout);
                    msg.mtype=pr_id[i];
                    strcpy(msg.mtext,msg1.mtext);
                    msgsnd(msgid4,&msg,sizeof(msg),0);
                }
            }
        }
    }  
    return 0;
}