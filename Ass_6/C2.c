#include <stdio.h> 
#include <unistd.h> //for fork system call
#include <sys/types.h>
#include <stdlib.h> //for exit system call
#include <sys/wait.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <fcntl.h>
#include <pthread.h> 
#include <semaphore.h> 
#include <string.h>
#include <sys/poll.h>
#include <sys/time.h>
#include <assert.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

char* c2 = "c2";

int main()
{    
    
	while(1)
    {
        char c;
        scanf("%c",&c);
        
        int fd2 = open(c2,O_RDWR);
        write(fd2,"OK_C2",1024);
        
        close(fd2);
    }
    
}