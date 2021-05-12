#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h> 
#include <string.h> 
#include <arpa/inet.h>
int main()
{
    int sfd=socket(AF_INET,SOCK_DGRAM,0);
    struct sockaddr_in adr;
    adr.sin_port=htons(7005);
    adr.sin_family=AF_INET;
    adr.sin_addr.s_addr=inet_addr("127.0.0.1");
    socklen_t x=sizeof(adr);
    char buf[100];
    strcpy(buf,"hi");
    sendto(sfd,buf,100,0,(struct sockaddr*)&adr,x);
    struct sockaddr_in cliadr;
    socklen_t cliadrlen=sizeof(cliadr);
    recvfrom(sfd,&buf,100,0,(struct sockaddr*)&cliadr,&cliadrlen);
    printf("%s\n",buf);
    return 0;
}