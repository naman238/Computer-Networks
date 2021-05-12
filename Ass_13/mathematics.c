#include <stdio.h>
#include <stdlib.h>
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
#define operation(a,b,c) ((c==1)?(a+b):((c==2)?(a-b):((c==3)?(a*b):(a/b))))

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

int main()
{
	struct Packet buff;
	while(1)
	{
		if(read(0,&buff,sizeof(struct Packet))<0)
		{
			perror("Reading error");
			exit(0);
		}
		if(buff.Opt.t.c=='@')
			break;
		int res = operation(buff.Opt.t.a,buff.Opt.t.b,buff.Opt.t.c);
		buff.ind = res;
		write(1,&buff,sizeof(struct Packet));
	}
}