#include <iostream>
#include <stdio.h> 
#include <unistd.h> //for fork system call
#include <sys/types.h>
#include <stdlib.h> //for exit system call
#include <sys/wait.h>
#include <sys/ipc.h> 
#include <fcntl.h>
#include <pthread.h> 
#include <semaphore.h> 
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/msg.h> // for msg queue
#include <string.h>
using namespace std;
const char* sem1="i1";
const char* sem2="i";
key_t key;
int msgid;
int pid1;
int pid2;

struct msgbuff
{
    long mtype;
    char mtext[100];
    
};
struct msgbuff msg;
struct msgbuff msg1;
int x;
void* reader(void* agrs)
{
    while(1)
    {
        int y=msgrcv(msgid,&msg1,sizeof(msg1),0,0);
        if(y==-1)continue;
        if(msg1.mtype==1)
        {
            msg.mtype=1;
            msgsnd(msgid,&msg,sizeof(msg),0);
        }
        else
        {
            cout<<msg1.mtext<<endl;
        }
    }
}
void* writer(void* args)
{
    char buff[100];
    while(1)
    {
        fgets(buff,100,stdin);
        strcpy(msg.mtext,buff);
        msg.mtype=1;
        msgsnd(msgid,&msg,sizeof(msg),0);
        msg.mtype=1;
        msgsnd(msgid,&msg,sizeof(msg),0);
    }
}
int main()
{
    
    
    key=ftok(".",65);
     msgid=msgget(key,0666|IPC_CREAT);
     while(msgrcv(msgid,&msg,sizeof(msg),0,IPC_NOWAIT)!=-1);
      x=getpid();
    msg.mtype=1;
    msgsnd(msgid,&msg,sizeof(msg),IPC_NOWAIT);
    msg.mtype=1;
    msgsnd(msgid,&msg,sizeof(msg),IPC_NOWAIT);
    // cout<<"pid send";
    
    pid1=-1;
    
    while(1)
    {
        int y=msgrcv(msgid,&msg1,sizeof(msg1),0,IPC_NOWAIT);
        if(y==-1)continue;
        else{
            if(msg1.mtype==1)
            {
                msg.mtype=1;
                msgsnd(msgid,&msg,sizeof(msg),IPC_NOWAIT);
            }
            else if(pid1!=-1 && msg1.mtype==pid1)
            {
                 msg.mtype=pid1;
                msgsnd(msgid,&msg,sizeof(msg),IPC_NOWAIT);
            }
            else{
                if(pid1==-1)
                {
                    pid1=msg1.mtype;
                }
                else{
                    pid2=msg1.mtype;
                    
                    break;
                }
            }
        }
        //printf("%d hi\n",pid1);
    }
    sem_t* d1=sem_open(sem1,O_CREAT,0666,0);
    sem_t* d=sem_open(sem2,0);
    sem_post(d);
    sem_wait(d1);
    pthread_t t1,t2;
    pthread_create(&t1,NULL,&reader,NULL);
    pthread_create(&t1,NULL,&writer,NULL);
    pthread_join(t1,NULL);
    pthread_join(t2,NULL);
    sem_unlink(sem1);
    sem_unlink(sem2);
    
    
    
    return 0;
}