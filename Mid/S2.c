#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <arpa/inet.h>
#include <sys/msg.h> // for msg queue
#include <sys/poll.h>
struct msgbuf{
    long mtype;
    char mdata[100];
};
int main()
{
    int key=ftok(".",65);
    int msgid=msgget(key,0666);
    char buf[100];
    struct msgbuf m;
    while(1)
    {
        if(recv(0,buf,100,0)>=0)
        {
            strcpy(m.mdata,buf);
            m.mtype=getpid();
            msgsnd(msgid,&m,sizeof(m),0);
        }
    }
}