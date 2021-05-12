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
#include <linux/if_packet.h>
#include <net/ethernet.h> 
int main()
{
    int rsfd=socket(AF_INET,SOCK_RAW,253);
    int opt=1,sz;
    setsockopt(rsfd, htons(ETH_P_IP), SO_BROADCAST|IP_HDRINCL|IP_OPTIONS, &opt, sizeof(int));
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
     struct iphdr *ip;
            ip = (struct iphdr*)buffer;
            //
            ip->protocol=253;
            ip = (struct iphdr*)buffer;
            printf("%d",ip->protocol);
	strcpy(buffer,"C1");
    if(sendto(rsfd,buffer,strlen(buffer)+1,0,(struct sockaddr*)&rawaddr,sizeof(rawaddr))<0)
	{
		perror("Could not send");
	}
	else
	{
        printf("details sent to server\n");
        char buf[100];
        int f=0;
        if(recvfrom(rsfd,buffer,100,0,NULL,NULL)<0){
				perror("Could not read");
		}
        else
        {
            struct iphdr *ip;
            ip = (struct iphdr*)buffer;
            strcpy(buf,buffer+(ip->ihl*4));
            if(strcmp(buf,"C1")==0)
            {
                f=1;
                strcpy(buffer,"Yes");
                if(sendto(rsfd,buffer,strlen(buffer)+1,0,(struct sockaddr*)&rawaddr,sizeof(rawaddr))<0)
                {
                    perror("Could not send");
                    exit(0);
                }
            }
        }
		
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
    

}