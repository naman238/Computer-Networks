#include<stdio.h>  
#include<unistd.h>  
#include<fcntl.h> 
#include<stdlib.h> 
#include <semaphore.h>
const char* name1="s1";
const char* name2="s2";

void p1(int fd)
{
	sem_t *s1=sem_open(name1,O_EXCL);
	sem_t *s2=sem_open(name2,O_EXCL);
	char buf;
	while(read(fd,&buf,1)==1){

	  if(buf=='\n'){
	   printf("->p1\n");
	   sem_post(s1);
	   sem_wait(s2);
	   
	  }
	  
	  else{
	    printf("%c",buf);
	  }

	}
	
}

int main()
{
	sem_t *s1 = sem_open(name1,O_CREAT|O_EXCL,0666,0);
    sem_t *s2 = sem_open(name2,O_CREAT|O_EXCL,0666,0);
	int fd;
	fd=open("mytext.txt",O_RDONLY);
	dup2(fd,0);
	int c=fork();
	if(c>0)
	{
		p1(fd);			
	}
	else
	{
		execv("./Q2",NULL);
	}
	 sem_unlink(name1);
	 sem_unlink(name2);
	return 0;
}
