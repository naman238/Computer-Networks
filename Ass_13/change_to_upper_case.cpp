#include <iostream>
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
#include <ctype.h>
using namespace std;

int main()
{
	char  buff[1024];int sz;
	while(1)
	{
		scanf("%s",buff);
		
		for(int i=0;i<strlen(buff);i++)
		{
			if(buff[i]>='a'&& buff[i]<='z')
			{
                buff[i]=toupper(buff[i]);
			}
		}
		cout<<buff<<"\n";
		fflush(stdout);
	}
	return 0;
}