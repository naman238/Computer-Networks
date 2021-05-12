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
char * path ="./p1";
char * path1 ="./p2";
struct info{
    int pid;
    int fd;
};
int shmid;
struct info* ptr;
int send_fd(int usfd, int fd_to_send) {
    
    struct iovec iov[1];
    struct msghdr msg;
    
    char buf[2] = {0, 0};

    iov[0].iov_base = buf;
    iov[0].iov_len = 2;
    msg.msg_iov = iov;
    msg.msg_iovlen = 1;
    msg.msg_name = NULL;
    msg.msg_namelen = 0;
    
    cmptr = malloc(CONTROLLEN);
    cmptr->cmsg_level = SOL_SOCKET;
    cmptr->cmsg_type = SCM_RIGHTS;
    cmptr->cmsg_len = CONTROLLEN;
    msg.msg_control = cmptr;
    msg.msg_controllen = CONTROLLEN;
    *(int *)CMSG_DATA(cmptr) = fd_to_send;

    if(sendmsg(usfd, &msg, 0) < 0) {
        perror("sendmsg");
        return -1;
    }

    return 0;
}
int max(int x, int y) 
{ 
    if (x > y) 
        return x; 
    else
        return y; 
} 
int main()
{
    key_t k=ftok(".",45);
    shmid=shmget(k,sizeof(struct info),IPC_CREAT|0666);
    ptr=(struct info*)shmat(shmid,NULL,0);
    int sfd1=socket(AF_INET,SOCK_STREAM,0);
    int sfd2=socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in adr1,adr2,adr3;
    adr1.sin_family=AF_INET;
    adr2.sin_family=AF_INET;
    adr1.sin_port=htons(8030);
    adr2.sin_port=htons(6020);
    adr1.sin_addr.s_addr=inet_addr("127.0.0.1");
    adr2.sin_addr.s_addr=inet_addr("127.0.0.1");
    socklen_t adrlen1=sizeof(adr1);
    socklen_t adrlen2=sizeof(adr2);
    bind(sfd1,(struct sockaddr*)&adr1,adrlen1);
    bind(sfd2,(struct sockaddr*)&adr2,adrlen2);
    listen(sfd1,3);
    listen(sfd2,3);

    fd_set rset;
    FD_ZERO(&rset);
    int mxfd=max(sfd1,sfd2)+1;

    int usfd=socket(AF_UNIX,SOCK_STREAM,0);
    struct sockaddr_un adr;
    adr.sun_family=AF_UNIX;
    strcpy(adr.sun_path,path);
    unlink(path);
    socklen_t l=sizeof(adr);
    bind(usfd,(struct sockaddr*)&adr,l);
    listen(usfd,3);
    struct sockaddr cliadr;
    socklen_t len=sizeof(cliadr);
    int nufd = accept(usfd, (struct sockaddr*)&cliadr, &len);
    int usfd1=socket(AF_UNIX,SOCK_STREAM,0);
    struct sockaddr_un adr4;
    adr4.sun_family=AF_UNIX;
    strcpy(adr4.sun_path,path1);
    unlink(path1);
    socklen_t l1=sizeof(adr4);
    bind(usfd1,(struct sockaddr*)&adr4,l1);
    listen(usfd1,3);
    struct sockaddr cliadr1;
    socklen_t len1=sizeof(cliadr1);
    int nufd1 = accept(usfd1, (struct sockaddr*)&cliadr1, &len1);
    int pid=ptr->pid;
    while(1)
    {
        FD_SET(sfd1,&rset);
        FD_SET(sfd2,&rset);
        int r=select(mxfd,&rset,NULL,NULL,NULL);
        if(r>0)
        {
            if(FD_ISSET(sfd1,&rset))
            {
                char buf[100];
                int nsfd=accept(sfd1,(struct sockaddr*)&adr1,&adrlen1);
                ptr->fd=nsfd;
                printf("%d\n",nsfd);
                printf("signal sent\n");
                fflush(stdout);
                send_fd(nufd1,nsfd);
                kill(pid,SIGUSR1);
                
            }
            if(FD_ISSET(sfd2,&rset))
            {
                int nsfd=accept(sfd2,(struct sockaddr*)&adr2,&adrlen2);
                send_fd(nufd,nsfd);
            }
        }

    }

}