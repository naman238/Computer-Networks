#include<stdio.h>  
#include<unistd.h>  
#include<fcntl.h> 
#include<stdlib.h> 
#include <semaphore.h>
const char* name1="s1";
const char* name2="s2";
void p2()
{
	sem_t *s1=sem_open(name1,O_EXCL);
	sem_t *s2=sem_open(name2,O_EXCL);
	sem_wait(s1);

	char c;
	while(read(0,&c,1)==1)
	{
		if(c=='\n'){
		   printf("->p2\n");
		   sem_post(s2);
		   sem_wait(s1);
		}
	  
	  else{
		printf("%c",c);
	  }
	}
	sem_post(s2);
}
int main()
{
	
	p2();
	return 0;
}

