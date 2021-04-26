#include <stdio.h>
#include <unistd.h>
#include <string.h> 
int main()
{
    printf("p2");
    FILE* fd1=popen("./q73","r");
	char buf[1024];
	read(fileno(fd1),buf,1024);
	printf("From pipe3: %s",buf);
	pclose(fd1);
    return 0;
}
