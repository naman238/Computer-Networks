#include <string.h>
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
struct triplet
{
	int a,b,c;
};

union ops
{
	struct triplet t;
	char buffer[50];
};

struct Packet
{
	int ind;
	union ops Opt;
};

void encrypt(char* s)
{
	for(int i=0;i<strlen(s);i++)
	{
		s[i] = s[i]-'a'+'1';
	}
}

int main(int argc, char const *argv[])
{
	struct Packet buff;
	while(1)
	{
		if(read(0,&buff,sizeof(struct Packet))<0)
		{
			perror("Could not read");
			exit(0);
		}
		if(strcmp(buff.Opt.buffer,"bye")==0)
			break;
		encrypt(buff.Opt.buffer);
		write(1,&buff,sizeof(struct Packet));
	}
	return 0;
}