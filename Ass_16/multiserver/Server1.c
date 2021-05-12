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
int nsfd[255],ind=0;
fd_set rset; 
char reply[100];   
void* func(void* arg)
{
	int fd = *(int*)arg,sz;
	char buffer[50];
	while(1)
	{
		if((sz = recv(fd,buffer,50,0))<0)
			perror("Could not read");
		else
		{
			if(sz==0)
				pthread_exit(0);
			buffer[sz] = '\0';
			for(int i=0;i<sz;i++)
			{
				if(buffer[i]>='a'&&buffer[i]<='z')
					buffer[i] = buffer[i]-'a'+'A';
			}
			if(send(fd,buffer,sz,0)<0)
				perror("Could not send");
		}
	}
}

int main()
{
    FD_ZERO(&rset);
    int sfd=socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in adr,rawaddr;
    adr.sin_family=AF_INET;
    adr.sin_addr.s_addr=inet_addr("127.0.0.1");
    adr.sin_port=htons(9354);
    int opt1=1;
    setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR|SO_REUSEPORT,&opt1,sizeof(opt1));
    if(bind(sfd,(struct sockaddr*)&adr,sizeof(adr))<0)
    {
        perror("Could not bind1");
    }
    else if(listen(sfd,10)<0)
    {
        perror("Could not listen");
    }
    else
    {
        FD_SET(sfd,&rset);
    }

    int rsfd=socket(AF_INET,SOCK_RAW,253);
    int opt=1;
    setsockopt(rsfd, IPPROTO_IP, SO_BROADCAST, &opt, sizeof(opt));
    rawaddr.sin_family = AF_INET;
	rawaddr.sin_addr.s_addr = inet_addr("127.0.0.2");
    if(bind(rsfd,(struct sockaddr*)&rawaddr,sizeof(rawaddr))<0)
	{
		perror("Could not bind");
	}
	else
	{
		FD_SET(rsfd,&rset);
	}
    char buffer[100];
    int r;
    struct sockaddr_in cl_addr;
    int len=sizeof(cl_addr);
	while(1)
	{
        r = select(FD_SETSIZE+1,&rset,NULL,NULL,NULL);
        if(r>0)
        {
            if(FD_ISSET(rsfd,&rset))
			{
				if(recvfrom(rsfd,buffer,100,0,(struct sockaddr*)&cl_addr,&len)<0)
				{
					perror("Could not receive");
				}
                struct iphdr *ip;
				ip = (struct iphdr*)buffer;char ad[INET_ADDRSTRLEN];
				printf("Remote IP: %s\n",inet_ntop(AF_INET,&ip->saddr,ad,INET_ADDRSTRLEN));
                sprintf(reply,"Port of Server1-%d",9354);
                if(sendto(rsfd,reply,strlen(reply)+1,0,(struct sockaddr*)&cl_addr,sizeof(cl_addr))<0)
				{
					perror("Could not send");
				}
            }
				FD_SET(rsfd,&rset);
            if(FD_ISSET(sfd,&rset))
            {
                nsfd[ind]=accept(sfd,NULL,NULL);
                pthread_t t;
                pthread_create(&t,NULL,func,&nsfd[ind]);
                ind++;
            }
			    FD_SET(sfd,&rset);

        }
    }
    return 0;

}