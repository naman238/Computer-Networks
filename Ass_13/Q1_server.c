#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <arpa/inet.h>
int main()
{
    int sfd;
    if((sfd=socket(AF_INET,SOCK_STREAM,0))==0)
    {
        perror("socket failed\n");
        return 0;
    }
    struct sockaddr_in addr;
    addr.sin_port=htons(7020);
    addr.sin_family=AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &(addr.sin_addr));
    int opt=1;
    setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR|SO_REUSEPORT,&opt,sizeof(opt));
    if(bind(sfd,(struct sockaddr*)&addr,sizeof(addr))<0)
    {
        perror("bind failed");
        return 0;
    }
    if(listen(sfd,3)<0)
    {
        perror("listen failed");
        return 0;
    }
    int nsfd;
    socklen_t addrlen=sizeof(addr);
    struct sockaddr_in adr;
    socklen_t len=sizeof(adr);
    getsockname(sfd,(struct sockaddr*)&adr,&len);

    int p=ntohs(adr.sin_port);
    printf("%d",p);
    if((nsfd=accept(sfd,(struct sockaddr*)&addr,&addrlen))<0)
    {
        perror("accept failed");
        return (0);
    }
    char buff[1024];
    int reader=recv(nsfd,&buff,sizeof(buff),0);
    printf("%s",buff);
    char* msg="hello from server";
    int bytesend=send(nsfd,msg,strlen(msg),0);
    getsockname(nsfd,(struct sockaddr*)&adr,&len);
    p=ntohs(adr.sin_port);
    printf("%d",p);
    close(nsfd);
    close(sfd);
    return 0;
}