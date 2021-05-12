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
struct msgbuff msg;
int main()
{
    key_t key=ftok(".",64);
    int msgid=msgget(key,0666|IPC_CREAT);
    struct msqid_ds buff;
    msgctl(msgid,IPC_STAT,&buff);
    printf("before revcieving\n");
    printf("number of messages in queue: %d\n",buff.msg_qnum);
    printf("pid of last messgae: %d\n",buff.msg_lrpid);
    printf("max number of bytes allowed on queue: %d\n",buff.msg_qbytes);
    printf("pid of last msg send:%d\n",buff.msg_lspid);
    
    int x=msgrcv(msgid,&msg,sizeof(msg),1,IPC_NOWAIT);
    msgctl(msgid,IPC_STAT,&buff);
    printf("after revcieving\n");

   printf("before revcieving\n");
    printf("number of messages in queue: %d\n",buff.msg_qnum);
    printf("pid of last messgae: %d\n",buff.msg_lrpid);
    printf("max number of bytes allowed on queue: %d\n",buff.msg_qbytes);
    printf("pid of last msg send:%d\n",buff.msg_lspid);

    return 0;
}