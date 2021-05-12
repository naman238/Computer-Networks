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
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main(int argc, char const *argv[])
{
	char addr1[INET_ADDRSTRLEN];
	const char *adr;
	struct sockaddr_in addr,cl_addr;
	int sfd = socket(AF_INET,SOCK_DGRAM,0);
	int broadcast = 1;
    if(setsockopt(sfd,SOL_SOCKET,SO_BROADCAST,&broadcast,sizeof(broadcast)) < 0)
    {
        perror("Error in setting broadcast");
        return 0;
    }
    else
    	printf("Done!..\n");
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(atoi(argv[1]));
	if(bind(sfd,(struct sockaddr*)&addr,sizeof(addr))<0)
		perror("Could not bind");
	char buffer[1024];int len = sizeof(cl_addr);
	while(1)
	{
		fgets(buffer,1024,stdin);
		sendto(sfd,&buffer,1024,0,(struct sockaddr*)&addr,sizeof(addr));
		int sz = recvfrom(sfd,buffer,1024,0,(struct sockaddr*)&cl_addr,&len);
		buffer[sz] = '\0';
		printf("Reading: %s\n",buffer);
	}
	return 0;
}