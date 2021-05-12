#include<time.h>
#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<sys/select.h>
#include<pthread.h>
#include<signal.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/shm.h>
#include<unistd.h>
#include<sys/un.h>
#include<netinet/ip.h>
#include<arpa/inet.h>
#include<errno.h>
#include<netinet/if_ether.h>
#include<net/ethernet.h>
#include<netinet/ether.h>
#include<netinet/udp.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include <sys/un.h>
#include <sys/wait.h>
int main()
{
    int c=fork();
    if(c==0)
    {
        sleep(2);
       
    }
    else
    {
        int c1=fork();
        if(c1==0)
        {
            sleep(5);
      
        }
        else
        {
            int st=1;
            waitpid(c,&st,0);
            if(WIFEXITED(st))printf("exited child 1 %d\n",WIFEXITED(st));
            waitpid(c1,&st,0);
            if(WIFEXITED(st))printf("exited child 2 %d\n",WIFEXITED(st));
        }
    }
}