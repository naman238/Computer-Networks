#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <poll.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <sys/select.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
int main()
{
    FILE* fp=fopen("temp2.txt","w+");
    char buf[1024];
    while(1)
    {
        read(0,buf,sizeof(buf));
        fprintf(fp,"%s\n",buf);
        fflush(fp);
        sleep(1);
    }
}