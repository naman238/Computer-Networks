#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <arpa/inet.h>
int main()
{
    int csfd;
    if((csfd=socket(AF_INET,SOCK_STREAM,0))==0)
    {
        perror("socket failed\n");
        return 0;
    }
    struct sockaddr_in addr;
    
        addr.sin_port=htons(7001);
    addr.sin_family=AF_INET;

    inet_pton(AF_INET, "127.0.0.1", &(addr.sin_addr));
   
    
    if(connect(csfd,(struct sockaddr*)&addr,(socklen_t)sizeof(addr))<0)
    {
        perror("connet failed");
        return (0);
    }
    while(1)
    {
    char msg[1024];
    scanf("%s",msg);
   
    int bytesend=send(csfd,msg,sizeof(msg),0);
    char buff[1024];
    int reader=recv(csfd,&buff,1024,0);
    printf("%s",buff);}
    close(csfd);
    return 0;
}