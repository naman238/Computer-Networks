#include<stdio.h>
#include<poll.h>
#include<iostream>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>
using namespace std;

#define TIMEOUT 5
int main(){
	int rfd=open("text.txt",O_RDONLY);
	int rfd2=86;
	struct pollfd pfd[2];
	int x=dup(0);
	dup2(rfd2,0);
	char buf[100];
	pfd[0].fd=0;
	pfd[0].events=POLLIN;
	pfd[0].revents=0;
	pfd[1].fd=rfd;
	pfd[1].events = 0;
	pfd[1].events = POLLIN;
	//pfd[1].revents=0;
	
	if(poll(pfd,2,TIMEOUT*1000)==0){
		cout<<"time elapsed\n";
		return 0;
	}
	//pfd[0].revents=POLLIN;
	if(pfd[1].revents & POLLIN){
		char buf1[100];
		read(pfd[1].fd,buf1,100);
		cout<<buf1<<" read from file \n";
	}
	
	if(pfd[0].revents & POLLIN){
		dup2(x,0);
		scanf("%s",buf);
		cout<<buf<<" read from file \n";
	}
}