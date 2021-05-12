#include<time.h>
#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<sys/select.h>
#include<pthread.h>
#include<signal.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/shm.h>
#include<unistd.h>
#include<sys/un.h>
#include<netinet/ip.h>
#include<arpa/inet.h>
#include<errno.h>
#include<netinet/if_ether.h>
#include<net/ethernet.h>
#include<netinet/ether.h>
#include<netinet/udp.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include <sys/un.h>


#define CONTROLLEN CMSG_LEN(sizeof(int))
static struct cmsghdr *cmptr = NULL;
int rfd;
char * path1 ="./p2";
struct info{
    int pid;
    int fd;
};
int nsfd;
int shmid;
struct info* ptr;
int usfd;
int recv_fd(int usfd) {

    struct iovec iov[1];
    char buf[2];
    int newfd;

    struct msghdr msg;

    iov[0].iov_base = buf;
    iov[0].iov_len = sizeof(buf);
    msg.msg_iov = iov;
    msg.msg_iovlen = 1;
    msg.msg_name = NULL;
    msg.msg_namelen = 0;

    cmptr = malloc(CONTROLLEN);
    msg.msg_control = cmptr;
    msg.msg_controllen = CONTROLLEN;

    if(recvmsg(usfd, &msg, 0) < 0) {
        perror("recvmsg");
        return -1;
    }

    newfd = *(int *)CMSG_DATA(cmptr);

    return newfd;
}
void* func(void*args)
{
     
     char buf[100];
    while(1)
    {
        if(recv(nsfd,&buf,sizeof(buf),0)>=0)
        {
            printf("%s\n",buf);
        }
    }
}
void handler(int signo)
{
    char buf[100];
     nsfd=recv_fd(usfd);
    printf("%d\n",nsfd);
    fflush(stdout);
    struct sockaddr_in cliadr;
    socklen_t clilen=sizeof(cliadr);
    while(recv(nsfd,buf,sizeof(buf),0)==-1);
        printf("hello");
    printf("%s\n",buf);
    fflush(stdout);
        if(buf[0]=='1')
        {
            printf("handling itslef\n");
            fflush(stdout);
            while(1)
            {
                if(recv(nsfd,buf,sizeof(buf),0)>=0)
                {
                    printf("%s\n",buf);
                }
            }
        }
        else if(buf[0]=='2')
        {
            printf("thread created\n");
            fflush(stdout);
            pthread_t t1;
            pthread_create(&t1,NULL,(void*)&func,NULL);
            pthread_join(t1,NULL);
        }
        else if(buf[0]=='3'){
            printf("forking\n");
            fflush(stdout);
            int c=fork();
            if(c==0)
            {
                int x=dup(0);
                dup2(nsfd,0);
                execv("./S11",NULL);
                dup2(4,0);
            }
            else{
                close(nsfd);
            }
            
        }
    
    
}
int main()
{
    struct sockaddr_un addr;
	char buf[100];
	int rc;

	if((usfd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
		perror("socket error");
		exit(1);
	}
	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	strcpy(addr.sun_path, path1);
	if(connect(usfd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
		perror("connect error");
		exit(1);
	}
    struct sockaddr_in cliadr;
    socklen_t clilen=sizeof(cliadr);
    key_t k=ftok(".",45);
    shmid=shmget(k,sizeof(struct info),0666);
    ptr=(struct info*)shmat(shmid,NULL,0);
    ptr->pid=getpid();
    signal(SIGUSR1,handler);
    pause();
    return 0;
}