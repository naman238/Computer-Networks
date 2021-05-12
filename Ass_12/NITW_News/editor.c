#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <poll.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <sys/select.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
int max(int a,int b)
{
    if(a>b)return a;
    else return b;
}
struct msgbuff{
    long mtype;
    char mdata[1024];
};
struct info{
    int no_of_tc;
    int pid;
};
int shmid;
struct info* ptr;
void handler(int signo)
{
    ptr->no_of_tc++;
    printf("no of telecasts-> %d\n",ptr->no_of_tc);
    fflush(stdout);
}
int main()
{
    signal(SIGUSR1,handler);
    key_t k=ftok(".",23);
    int msgid=msgget(k,IPC_CREAT|0666);
    shmid=shmget(k,sizeof(struct info),IPC_CREAT|0666);
    ptr=(struct info*)shmat(shmid,NULL,0);
    ptr->pid=getpid();
    ptr->no_of_tc=0;
    FILE* fp[3];
    fp[0]=popen("./d","w");
    fp[1]=popen("./r1","r");
    fp[2]=popen("./r2","r");
    int fd[2];
    fd[0]=fileno(fp[1]);
    fd[1]=fileno(fp[2]);
    int mx=max(fd[0],fd[1])+1;
    fd_set rset;
    FD_ZERO(&rset);
    while(1)
    {
        FD_SET(fd[0],&rset);
        FD_SET(fd[1],&rset);
        int r=select(mx,&rset,NULL,NULL,NULL);
        if(r>0)
        {
            for(int i=0;i<2;i++)
            {
                if(FD_ISSET(fd[i],&rset))
                {
                    char buf[1024];
                    read(fd[i],buf,sizeof(buf));
                    if(strlen(buf)>=2 && buf[0]=='\\' && buf[1]=='d')
                    {
                        write(fileno(fp[0]),buf+2,sizeof(buf+2));
                    }
                    else
                    {
                     
                       struct msgbuff m;
                       strcpy(m.mdata,buf);
                       m.mtype=10;
                       int x=msgsnd(msgid,&m,sizeof(m),0);
                        
                    }
                }
            }
        }

    }
}