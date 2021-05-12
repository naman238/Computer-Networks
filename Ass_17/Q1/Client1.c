#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <poll.h>
#include <netinet/tcp.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <signal.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/ipc.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stddef.h>
#include <fcntl.h>
#include <netinet/ether.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netinet/ip_icmp.h>
#include <unistd.h>

int port = 7070;
int main(){
    int rsfd = socket(AF_INET, SOCK_STREAM,0);
    
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(port);
    saddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(rsfd,(struct sockaddr*)&saddr,sizeof(saddr));
    char buf[1024];
    strcpy(buf,"hello1");
    write(rsfd,buf,sizeof(buf));
    struct sockaddr_in cdr;
    socklen_t x=sizeof(cdr);
    getsockname(rsfd,(struct sockaddr*)&cdr,&x);
    printf("local port %d\n",ntohs(cdr.sin_port));
    
}