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
#include <netinet/ip.h>
#include <pthread.h>
int main()
{
    int rsfd=socket(AF_INET,SOCK_RAW,253);
    int opt=1,sz;
    setsockopt(rsfd, IPPROTO_IP, SO_BROADCAST, &opt, sizeof(int));
    struct sockaddr_in rawaddr,cl_addr;
    rawaddr.sin_family=AF_INET;
    rawaddr.sin_addr.s_addr=inet_addr("127.0.0.2");
    cl_addr.sin_family=AF_INET;
    cl_addr.sin_addr.s_addr=inet_addr("127.0.0.3");

    if(bind(rsfd,(struct sockaddr*)&cl_addr,sizeof(cl_addr))<0)
	perror("Could not bind");
	else
	printf("Success..\n");

    char buffer[100];
	strcpy(buffer,"?");
	if(sendto(rsfd,buffer,strlen(buffer)+1,0,(struct sockaddr*)&rawaddr,sizeof(rawaddr))<0)
	{
		perror("Could not send");
	}
	else
	{
        printf("enquiry sent to server\n");

        for(int i=0;i<2;i++)
		{
			if(recvfrom(rsfd,buffer,100,0,NULL,NULL)<0){
				perror("Could not read");
			}
			else
			{
				struct iphdr *ip;
				ip = (struct iphdr*)buffer;
				printf("Reading: %s\n",buffer+(ip->ihl*4));
			}
		}
        while(1)
        {
            printf("Enter the port no you want to connect to\n");
            int portno;
            scanf("%d",&portno);while(getchar()!='\n');
            int sfd = socket(AF_INET,SOCK_STREAM,0);
            struct sockaddr_in addr;
            addr.sin_family = AF_INET;
            addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
            addr.sin_port = htons(portno);
            if(connect(sfd,(struct sockaddr*)&addr,sizeof(addr))<0)
            {
                perror("Could not connect");
            }
            else
            {
                while(1)
                {
                    printf("Enter a string\n");
                    scanf("%[^\n]s",buffer);
                    if(strcmp(buffer,"quit")==0)break;
                    while(getchar()!='\n');
                    send(sfd,buffer,strlen(buffer),0);
                    sz = recv(sfd,buffer,100,0);
                    buffer[sz] = '\0';
                    printf("Reading: %s\n",buffer);
                }
            }
        }
	}
	return 0;
}