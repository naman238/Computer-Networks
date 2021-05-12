#include<stdio.h>	//For standard things
#include<stdlib.h>	//malloc
#include<string.h>	//memset
#include<netinet/ip_icmp.h>	//Provides declarations for icmp header
#include<netinet/udp.h>	//Provides declarations for udp header
#include<netinet/tcp.h>	//Provides declarations for tcp header
#include<netinet/ip.h>	//Provides declarations for ip header
#include<sys/socket.h>
#include<arpa/inet.h>
#include <sys/select.h>
#include <unistd.h>
int max(int a,int b)
{
    if(a>b)return a;
    return b;
}
int main()
{
    int sfd[3];
    int port=7070;
    struct sockaddr_in adr[3];
    fd_set rset;
    FD_ZERO(&rset);
    int mxfd=0;
    for(int i=0;i<3;i++)
    {
        sfd[i]=socket(AF_INET,SOCK_STREAM,0);
        adr[i].sin_family=AF_INET;
        adr[i].sin_port=htons(port+i);
        adr[i].sin_addr.s_addr=inet_addr("127.0.0.1");
        bind(sfd[i],(struct sockaddr*)&adr[i],sizeof(adr[i]));
        listen(sfd[i],3);
        FD_SET(sfd[i],&rset);
        mxfd=max(mxfd,sfd[i]);
    }
    mxfd++;
    struct sockaddr_in adr1;
    int len=sizeof(adr1);
    char buf[1024];
    while(1)
	{
		for(int i=0;i<3;i++)
        {
            FD_SET(sfd[i],&rset);
        }
        int r=select(mxfd,&rset,NULL,NULL,NULL);
        if(r>0)
        {
            for(int i=0;i<3;i++)
            {
                if(FD_ISSET(sfd[i],&rset))
                {
                    int nsfd=accept(sfd[i],(struct sockaddr*)&adr,(socklen_t*)&len);
                    int z=read(sfd[i],buf,sizeof(buf));
                    printf("recieved\n");
                }
            }
        }
        
        
	}
    return 0;
}