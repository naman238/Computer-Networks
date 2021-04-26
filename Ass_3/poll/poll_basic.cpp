#include<stdio.h>
#include<poll.h>
#include<iostream>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>
#define SIZE 1024
using namespace std;

#define TIMEOUT -1
struct pollfd pfd[3];
int main()
{
	FILE* rfd_1=popen("./pt1","r");
	FILE* rfd_2=popen("./pt2","r");
	FILE* rfd_3=popen("./pt3","r");
	
	
	
 	
 	int t=4;
	while(t--)
	{
		pfd[0].fd=fileno(rfd_1);
	pfd[0].events=0;
	pfd[0].events|=POLLIN;
	
	pfd[1].fd=fileno(rfd_2);
	pfd[1].events=0;
	pfd[1].events|=POLLIN;
	
	pfd[2].fd=fileno(rfd_3);
	pfd[2].events=0;
	pfd[2].events|=POLLIN;
	
	poll(pfd,3,TIMEOUT);
		printf("%d %d %d",pfd[0].revents,pfd[1].revents,pfd[2].revents);
		char msg[SIZE];
		int c;
		if(pfd[0].revents & POLLIN && (c=read(pfd[0].fd,msg,SIZE))>0)
		{
	
			printf("%s --->Read from P1\n",msg);
		}
		sleep(2);
		if(pfd[1].revents & POLLIN && (c=read(pfd[1].fd,msg,SIZE))>0)
		{
			
		
			printf("%s --->Read from P2\n",msg);
		}
		sleep(2);
		if(pfd[2].revents & POLLIN && (c=read(pfd[2].fd,msg,SIZE))>0)
		{
			
			printf("%s --->Read from P3\n",msg);
		}
		sleep(2);
	}
	return 0;
}
