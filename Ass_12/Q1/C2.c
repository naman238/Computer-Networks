// UDP client program 
#include <arpa/inet.h> 
#include <errno.h> 
#include <netinet/in.h> 
#include <signal.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <strings.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <sys/select.h>
#include <sys/un.h>
#define PORT 6034
#define MAXLINE 1024 
int main() 
{ 
	int sockfd; 
	char buffer[MAXLINE]; 
	char* message = "Hello Server"; 
	struct sockaddr_in servaddr; 

	int n, len; 
	// Creating socket file descriptor 
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) { 
		printf("socket creation failed"); 
		exit(0); 
	} 

	memset(&servaddr, 0, sizeof(servaddr)); 

	// Filling server information 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_port = htons(PORT); 
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
	// send hello message to server 
    char c;
    while(1)
    {
         char buffer[1024];
        fgets(buffer,sizeof(buffer),stdin);
	    sendto(sockfd, (const char*)buffer, strlen(buffer), 
		0, (const struct sockaddr*)&servaddr, 
		sizeof(servaddr)); 
    }
	 
	close(sockfd); 
	return 0; 
} 
