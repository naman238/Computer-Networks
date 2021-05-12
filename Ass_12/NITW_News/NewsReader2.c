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
const char*name="fifo1";
struct msgbuff{
    long mtype;
    char mdata[1024];
};
void sem_wait(int semid)
{
	struct sembuf sb;
	sb.sem_num=0;
	sb.sem_op=-1;
	sb.sem_flg=0;
	if((semop(semid,&sb,1))==-1)
	{
		perror("\nFailed to acquire semaphore.");
		exit(0);
	}
}

void sem_signal(int semid)
{
	struct sembuf sb;
	sb.sem_num=0;
	sb.sem_op=1;
	sb.sem_flg=0;
	if((semop(semid,&sb,1))==-1)
	{
		perror("\nFailed to release semaphore.");
		exit(0);
	}
}
struct info{
    int no_of_tc;
    int pid;
};
int shmid;
struct info* ptr;
int fd;
int livetele(int port)
{
	int sfd=socket(AF_INET,SOCK_STREAM,0);
	kill(ptr->pid,SIGUSR1);
	struct sockaddr_in adr;
	adr.sin_addr.s_addr=inet_addr("127.0.0.1");
	adr.sin_family=AF_INET;
	adr.sin_port=htons(port);
	socklen_t l=sizeof(adr);
	int x=connect(sfd,(struct sockaddr*)&adr,l);
	if(x==-1)perror("connect");
	printf("%d\n",port);
	fflush(stdout);
	char buf[1024];
	while(1)
	{
		int sz=recv(sfd,&buf,sizeof(buf),0);
		if(sz>=0)
		{
			buf[sz]='\0';
			printf("%s\n",buf);
			fflush(stdout);
			if(buf[0]=='q')break;
			write(fd,buf,sizeof(buf));
		}
	}
	return 0;
	
}
int isnum(char *buf)
{
	int n=strlen(buf);
	for(int i=0;i<n;i++)
	{
		if(buf[i]<'0' || buf[i]>'9')return 0;
	}
	return 1;
}
int main()
{
    int state=1;
	key_t h=ftok(".",state++);	// value of state should on every program where this semaphore is used
	int sem_id;
	if((sem_id=semget(h,1,0666|IPC_CREAT))==-1)
 	{
		printf("error in creation semaphore\n");
		exit(0);
	}
	
	int semaphore_value=0;

	if((semctl(sem_id,0,SETVAL,semaphore_value))==-1)
	{
		printf("error to set value\n");
	}

    int state1=2;
	key_t h1=ftok(".",state1++);	// value of state should on every program where this semaphore is used
	int sem_id1;
	if((sem_id1=semget(h1,1,0666|IPC_CREAT))==-1)
 	{
		printf("error in creation semaphore\n");
		exit(0);
	}
	
	int semaphore_value1=0;

	if((semctl(sem_id1,0,SETVAL,semaphore_value1))==-1)
	{
		printf("error to set value\n");
	}

    key_t k=ftok(".",23);
    int msgid=msgget(k,0666);

	shmid=shmget(k,sizeof(struct info),IPC_CREAT|0666);
    ptr=(struct info*)shmat(shmid,NULL,0);

    struct msgbuff m;
    mkfifo(name,O_CREAT|0666);
    fd=open(name,O_RDWR);
    char buf[1024];
    while(1)
    {
		sem_wait(sem_id1);
        if(msgrcv(msgid,&m,sizeof(m),10,0)!=-1)
        {
			if(isnum(m.mdata))
			{
				printf("livetele started\n");
				fflush(stdout);
				livetele(atoi(m.mdata));
				printf("livetele completed\n");
				fflush(stdout);
			}
			else
			{
				strcpy(buf,m.mdata);
				write(fd,buf,sizeof(buf));
				printf("sent");
				fflush(stdout);
			}
        }
        sem_signal(sem_id);
        
    }
    return 0;
}