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
#include <sys/stat.h>

char* c1="c1";
char* c2="c2";
char* sc1="sc1";
char* sc2="sc2";
char* a="a";
char* b="b";

char buff_c1[1024],buff_c2[1024];
int children[4];

int* shmptr;
int* kill_call_shmptr;

key_t k_shmptr;
key_t k_kill_call;

int shm_spid;
int shm_kill_call;

int pid_s;
struct pollfd pfd[2];
pthread_t t1[0];
sigset_t set;

void handler(int sig);
void handler_sc1(int sig);
void handler_sc2(int sig);
void handler_a(int sig);
void handler_b(int sig);
void *polling(void* args);

void handler(int sig)
{
	if(sig==SIGUSR1)
	{
		printf("G1 is resumed  and G2 is stopped\n");
		printf("Sending signal to Group1\n");
		int kills=*kill_call_shmptr;
		*kill_call_shmptr=kills+4;

		killpg(children[3],SIGSTOP);
		killpg(children[0],SIGCONT);
		killpg(children[0],SIGUSR1);
		int fd_sc1=open(sc1,O_WRONLY);
		write(fd_sc1,"message from S to SC1\n",sizeof "message from S to SC1\n");
		
		int fd_sc2=open(sc2,O_WRONLY);
		write(fd_sc2,"message from S to SC2\n",sizeof "message from S to SC2\n");
	}
	else
	{
		printf("G1 is stopped and G2 is resumed\n");
		printf("Sending signal to Group2\n");
		int kills=*kill_call_shmptr;
		*kill_call_shmptr=kills+4;
		killpg(children[0],SIGSTOP);
		killpg(children[3],SIGCONT);
		killpg(children[3],SIGUSR1);
		int fd_a=open(a,O_WRONLY);
		write(fd_a,"message from S to A\n",sizeof "message from S to A\n");
		
		int fd_b=open(b,O_WRONLY);
		write(fd_b,"message from S to B\n",sizeof "message from S to B\n");
	}
	
}

void handler_sc1(int sig)
{
	printf("[handler_sc1]: signal recieved.\n");
	char buff[1024];
	int fd_sc1=open(sc1,O_RDONLY);
	read(fd_sc1,buff,1024);
	close(fd_sc1);

	printf("[handler_sc1]: Message read from S = %s\n",buff);
	
}

void handler_sc2(int sig)
{
	printf("[handler_sc2]: siganl received.\n");
	char buff[1024];
	int fd_sc2=open(sc2,O_RDONLY);
	read(fd_sc2,buff,1024);
	close(fd_sc2);

	printf("[handler_sc2]: Message read from S = %s\n",buff);
}

void handler_a(int sig)
{
	printf("[handler_a]: signal received.\n");
	char buff[1024];
	int fd_a=open(a,O_RDONLY);
	read(fd_a,buff,1024);
	close(fd_a);

	printf("[handler_a]: Message read from S = %s\n",buff);
	
}

void handler_b(int sig)
{
	printf("[handler_b]: signal received.\n");
	char buff[1024];
	int fd_b=open(b,O_RDONLY);
	read(fd_b,buff,1024);
	close(fd_b);

	printf("[handler_b]: Message read from S = %s\n",buff);
}

void *polling(void* args)
{
	printf("polling\n");
	pthread_sigmask(SIG_BLOCK,&set,NULL);

	
	int c1_pid=open(c1,O_RDWR|O_NONBLOCK);
	int c2_pid=open(c2,O_RDWR|O_NONBLOCK);
	pfd[0].fd=c1_pid;
	pfd[1].fd=c2_pid;

	pfd[0].events=POLLIN;
	pfd[1].events=POLLIN;

	while(1)
	{
		int ret=poll(pfd,2,3000);
		if(ret!=0)
		{
			if(ret==-1)
			{	
				perror("Error in polling\n");
				exit(1);
			}
			char buff[1024];
			for(int i=0;i<2;i++)
			{
				if(pfd[i].revents & POLLIN)
				{
					if(read(pfd[i].fd,buff_c1,1024) > 0)
					{
						printf("[POLL]Read buffer:  %s\n",buff_c1);
					}
				}
			}
		}
	}
}

int main()
{	
	signal(SIGUSR1,handler);
	signal(SIGUSR2,handler);

	k_shmptr=ftok(".",'s');
	shm_spid=shmget(k_shmptr,sizeof(int),IPC_CREAT|0666);
	shmptr=(int*)shmat(shm_spid,NULL,0);
	
	*shmptr=0;
	
	k_kill_call=ftok(".",'c');
	shm_kill_call=shmget(k_kill_call,sizeof(int),IPC_CREAT|0666);
	kill_call_shmptr=(int*)shmat(shm_kill_call,NULL,0);
	
	 pid_s=getpid();
	*shmptr=pid_s;
	printf("Send pid to D: %d\n",pid_s);

	
	*kill_call_shmptr=0;

	mkfifo(c1,0666|O_CREAT);
	mkfifo(c2,0666|O_CREAT);

	mkfifo(sc1,O_CREAT|0666);
	mkfifo(sc2,O_CREAT|0666);
	mkfifo(a,O_CREAT|0666);
	mkfifo(b,O_CREAT|0666);
   
	int pid=fork();
	if(pid==0)
	{
		signal(SIGUSR1,handler_sc1);
		printf("Child S`1 Started\n");

		while(1)
		{
			printf("Child S`1 running\n");
			sleep(2);
		}
	}
	else
	{
		int sc1_pid=pid;
		children[0]=sc1_pid;
		pid=fork();
		if(pid==0)
		{
			signal(SIGUSR1,handler_sc2);
			printf("Child S`2 Started\n");

			while(1)
			{
				printf("Child S`2 running\n");
				sleep(2);
			}
		}
		else
		{
			int sc2_pid=pid;
			children[1]=sc2_pid;
			pid=fork();
			if(pid==0)
			{
				signal(SIGUSR1,handler_a);
				printf("Child A Started\n");

				while(1)
				{
					printf("Child A running\n");
					sleep(2);
				}
			}
			else
			{
				int a_pid=pid;
				children[2]=a_pid;
				pid=fork();
				if(pid==0)
				{
					signal(SIGUSR1,handler_b);
					printf("Child B Started\n");

					while(1)
					{
						printf("Child B running\n");
						sleep(2);
					}
				}
				else
				{
				    int b_pid=pid;
					children[3]=b_pid;
					sleep(2);
					setpgid(children[3],children[3]); 
					setpgid(children[2],children[3]); 

					setpgid(children[0],children[0]); 
					setpgid(children[1],children[0]); 

					killpg(children[0],SIGSTOP);
					killpg(children[3],SIGSTOP);
					printf("G1 and G2 are stopped\n");
					sigemptyset(&set);
					sigaddset(&set,SIGUSR1);
					sigaddset(&set,SIGUSR2);

					
					pthread_create(&t1[0],NULL,polling,NULL);
					pthread_join(t1[0],NULL);
				}
			}
		}
	}
	return 0;	
}