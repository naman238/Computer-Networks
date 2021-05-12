#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/msg.h>
#include <signal.h>
#include <string.h>
#include <sys/poll.h>
#include <sys/types.h>
#include <sys/stat.h>
using namespace std;
key_t key;
int msgid;
key_t key1;
int cmsgid;
int x;
struct msgbuff
{
    long mtype;
    char mtext[100];
};
struct msgbuff msg;
struct msgbuff msg1;

void handler(int signo)
{
    char buff[100];
    int f=0;
    while(1)
    {
        if(f==1)
        {
            cout<<"Want Ess service?\n";
            int z;
            cin>>z;
            if(z==0)continue;
            else{
                 f=0;
                 
                msg.mtype=1;
                msgsnd(cmsgid,&msg,sizeof(&msg),0);
               
               
            }
        }
        
        if(msgrcv(cmsgid,&msg1,sizeof(msg1),0,IPC_NOWAIT)!=-1)
        {
            if(msg1.mtype==1)
            {
                msg1.mtype=1;
                msgsnd(cmsgid,&msg1,sizeof(msg1),0);
                continue;
            }
            int pid=msg1.mtype;
            kill(pid,SIGUSR1);
            f=1;
            continue;
        }
        fgets(buff,100,stdin);
        strcpy(msg.mtext,buff);
        msg.mtype=1;
        msgsnd(msgid,&msg,sizeof(msg),0);
        sleep(1);
    }
  
  
}
int main()
{
    signal(SIGUSR1,handler);
    key=ftok(".",65);
    msgid=msgget(key,0666|IPC_CREAT);
    key1=ftok(".",64);
    cmsgid=msgget(key1,0666|IPC_CREAT);
    x=getpid();
    raise(SIGUSR1);
}