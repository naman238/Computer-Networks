#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/msg.h>
#include <signal.h>
#include <string.h>
#include <sys/poll.h>
using namespace std;
int flag=0;
void handler(int signum)
{
    flag=1;
}
struct msgbuff{
    long mtype;
    char mdata[100];
};
char buf[1024];
int main()
{

    string s1,s2,s3;
    
    FILE* fd1=popen("./P2","r");

    FILE* fd2=popen("./P3","r");

    FILE* fd3=popen("./P4","r");
    struct pollfd pfd[3];
    pfd[0].fd=fileno(fd1);
    pfd[0].events=POLLIN;
    pfd[1].fd=fileno(fd2);
    pfd[1].events=POLLIN;
    pfd[2].fd=fileno(fd3);
    pfd[2].events=POLLIN;
    poll(pfd,3,-1);
    for(int i=0;i<3;i++)
    {
        if(pfd[i].revents & POLLIN)
        {
            if(read(pfd[i].fd,&buf,sizeof(buf))>0)
            {
               cout<<buf;
               break;
            }
        }
    }
    
    
    
    // int x=dup(STDIN_FILENO);
    // cout<<x;
    // read(fileno(fd),&buf,sizeof(buf));
    // printf("%s ",buf);
    
    return 0;
}