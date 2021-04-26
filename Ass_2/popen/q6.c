#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

int main()
{
	FILE* fd=popen("./q6_p2","r");//read from q6_p2
	char buf[1024];
	
	read(fileno(fd),buf,1024);//fileno helps in typecasting
	printf("From pipe: %s",buf);
	
	pclose(fd);
	return 0;
}
