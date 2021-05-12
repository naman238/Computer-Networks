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
int main()
{
    int sfd=socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in adr;
    adr.sin_family=AF_INET;
    adr.sin_port=htons(8033);
    adr.sin_addr.s_addr=inet_addr("127.0.0.1");
    connect(sfd,(struct sockaddr*)&adr,(socklen_t)(sizeof(adr)));
    printf("giving test\n");
}