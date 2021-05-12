#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <arpa/inet.h>
#include <poll.h>
int main()
{
    char buff[1024];
    int reader=recv(STDIN_FILENO,&buff,sizeof(buff),0);
    printf("%s\n",buff);
    char* msg="hello from server1";
    int bytesend=send(STDIN_FILENO,msg,strlen(msg),0);
    return 0;
}