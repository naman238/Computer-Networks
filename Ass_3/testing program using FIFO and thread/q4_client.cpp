#include <iostream>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <string.h>
using namespace std;

const char* name = "Client";

void* reader(void* args)
{
   	 int rfd = open(name,O_RDONLY);
   	 char c,msg[1024];
   	 int j=0;
   	 while(read(rfd,&c,1))
   	 {
   	 	msg[j++]=c;
   	 }
   	 msg[j]='\0';
   	 cout<<msg<<"\n";
   	 close(rfd);
}

void* writer(void* args)
{
  	 char c,msg[1024],buff[1024];
  	 int j=0;
  	 int fd=open("code.cpp",O_RDONLY); 
     
     read(fd,msg,1024);
  	 buff[0] = '\0';
  	 strcat(buff,name);
  	 strcat(buff,":");
  	 strcat(buff,msg);
     strcat(buff,"$\n");
     //mkfifo("server",0666);
  	 int wfd = open("tester",O_WRONLY);
  	 write(wfd,&buff,1024);
     close(fd);
     close(wfd);
}

int main()
{
	cout<<name<<" Client started!\n";
	mkfifo(name,0666);



	pthread_t write_th,read_th;
	pthread_create(&write_th,NULL,writer,NULL);
	pthread_create(&read_th,NULL,reader,NULL);

	pthread_join(write_th,NULL);
	pthread_join(read_th,NULL);
}