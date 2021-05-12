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
int main()
{
    int sfd=socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in adr;
    adr.sin_family=AF_INET;
    adr.sin_port=htons(5000);
    adr.sin_addr.s_addr=inet_addr("127.0.0.1");
    socklen_t l=sizeof(adr);
    bind(sfd,(struct sockaddr*)&adr,l);
    listen(sfd,5);
    while(1)
    {
        int nsfd=accept(sfd,(struct sockaddr*)&adr,&l);
        if(nsfd>=0)
        {
            printf("accpted\n");
            fflush(stdout);
            char buf[1024];
            while(1)
            {
                fgets(buf,sizeof(buf),stdin);
                printf("%s\n",buf);
			    fflush(stdout);
                write(nsfd,buf,sizeof(buf));
                if(buf[0]=='q')
                    break;
                
            }
        }
    }
}