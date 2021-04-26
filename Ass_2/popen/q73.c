#include <stdio.h>
#include <unistd.h>
#include <string.h> 
int main()
{
    printf("p3");
    FILE* fd2=popen("./q74","r");
	char buf[1024];
	read(fileno(fd2),buf,1024);
	printf("From pipe4: %s",buf);
	pclose(fd2);
    return 0;
}
