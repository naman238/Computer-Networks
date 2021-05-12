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

#define SIZE 10
struct msgbuff
{
    long mtype;
    char mtext[100];
};

int main()
{
    key_t key=ftok("mq",65);
    int msgid=msgget(key,0666|IPC_CREAT);

    struct msgbuff msg[5];
    msg[0].mtype=1;
    strcpy(msg[0].mtext,"CSE");
    msg[1].mtype=1;
    strcpy(msg[1].mtext,"CODE");
    msg[2].mtype=2;
    strcpy(msg[2].mtext,"NITW");
    msg[3].mtype=2;
    strcpy(msg[3].mtext,"CN");
    msg[4].mtype=3;
    strcpy(msg[4].mtext,"ORACLE");
    for(int i=0;i<5;i++)
    {
        msgsnd(msgid,&msg[i],sizeof(msg[i]),0);
    }
    int t=1;
    int cnt=5;
     struct msgbuff m;
    while(cnt>0)
    { 
        if(msgrcv(msgid,&m,sizeof(m),t,IPC_NOWAIT)==-1)
        {
            t++;
            //perror("error");
        }
        else
        {
            cnt--;
            printf("%s\n",m.mtext);
        }
    }
    msgctl(msgid, IPC_RMID, NULL);
    return 0;
}