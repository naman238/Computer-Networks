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
char * path1 ="./p01";
char * path2 ="./p02";
char * path3 ="./p03";
struct info{
    int busy[3];
};
struct score{
    int marks[3][3];
};
int shmid,shmid1;
struct info* ptr;
struct score* ptr1;
int nufd1,nufd2,nufd3;
int max(int x, int y) 
{ 
    if (x > y) 
        return x; 
    else
        return y; 
} 

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
void* func(void*args)
{
    int* fdptr=(int*)args;
    int fd=*fdptr;
    if(ptr->busy[0]==0)
    {
        send_fd(nufd1,fd);
        ptr->busy[0]=1;
    }
    else if(ptr->busy[1]==0)
    {
        send_fd(nufd2,fd);
        ptr->busy[1]=1;

    }
    else if(ptr->busy[2]==0)
    {
        send_fd(nufd3,fd);
        ptr->busy[2]=1;

    }
}
int main()
{
    key_t k=ftok(".",50);
    shmid=shmget(k,sizeof(struct info),IPC_CREAT|0666);
    ptr=(struct info*)shmat(shmid,NULL,0);
    for(int i=0;i<3;i++)
    ptr->busy[i]=0;
     key_t k1=ftok(".",55);
    shmid1=shmget(k1,sizeof(struct score),IPC_CREAT|0666);
    ptr1=(struct score*)shmat(shmid1,NULL,0);

    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++)
        {
            ptr1->marks[i][j]=-1;
        }
    }
    /////

    int usfd1=socket(AF_UNIX,SOCK_STREAM,0);
    int usfd2=socket(AF_UNIX,SOCK_STREAM,0);
    int usfd3=socket(AF_UNIX,SOCK_STREAM,0);
    struct sockaddr_un adr1,adr2,adr3;
    adr1.sun_family=AF_UNIX;
    strcpy(adr1.sun_path,path1);
    unlink(path1);
    adr2.sun_family=AF_UNIX;
    strcpy(adr2.sun_path,path2);
    unlink(path2);
    adr3.sun_family=AF_UNIX;
    strcpy(adr3.sun_path,path3);
    unlink(path3);

    socklen_t l1=sizeof(adr1);
    bind(usfd1,(struct sockaddr*)&adr1,l1);
    listen(usfd1,3);
    socklen_t l2=sizeof(adr2);
    bind(usfd2,(struct sockaddr*)&adr2,l2);
    listen(usfd2,3);
    socklen_t l3=sizeof(adr3);
    bind(usfd3,(struct sockaddr*)&adr3,l3);
    listen(usfd3,3);
    printf("hello\n");
    fflush(stdout);
    struct sockaddr_un cliadr1;
    socklen_t len1=sizeof(cliadr1);
    
    
    struct sockaddr_un cliadr2;
    socklen_t len2=sizeof(cliadr2);
   // nufd2 = accept(usfd2, (struct sockaddr*)&cliadr2, &len2);
   

    struct sockaddr_un cliadr3;
    socklen_t len3=sizeof(cliadr3);
    //nufd3 = accept(usfd3, (struct sockaddr*)&cliadr3, &len3);
    
    fd_set rset;
    FD_ZERO(&rset);
    int mxfd=max(usfd1,max(usfd2,usfd3))+1;
    int cnt=0;
    while(cnt<3)
    {
        FD_SET(usfd1,&rset);
        FD_SET(usfd2,&rset);
        FD_SET(usfd3,&rset);
        int r=select(mxfd,&rset,NULL,NULL,NULL);
        if(r>0)
        {
            if(FD_ISSET(usfd1,&rset))
            {
                nufd1 = accept(usfd1, (struct sockaddr*)&cliadr1, &len1);
                cnt++;
            }

            if(FD_ISSET(usfd2,&rset))
            {
                nufd2 = accept(usfd2, (struct sockaddr*)&cliadr2, &len2);
                cnt++;
            }

            if(FD_ISSET(usfd3,&rset))
            {
                nufd3 = accept(usfd3, (struct sockaddr*)&cliadr3, &len3);
                cnt++;
            }
        }
    }

    /////////////////////////////////////////
    printf("success\n");
    fflush(stdout);
    int sfd=socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in adr;
    adr.sin_family=AF_INET;
    adr.sin_port=htons(8033);
    adr.sin_addr.s_addr=inet_addr("127.0.0.1");
    socklen_t adrlen=sizeof(adr);
    bind(sfd,(struct sockaddr*)&adr,adrlen);
    listen(sfd,3);
    ////
    int nsfd;
    while(1)
    {
        nsfd=accept(sfd,(struct sockaddr*)&adr,&adrlen);
        if(nsfd!=-1)
        {
            printf("thread opened\n");
            fflush(stdout);
            pthread_t t;
            pthread_create(&t,NULL,(void*)func,(void*)&nsfd);
            pthread_join(t,NULL);
        }
    }
    return 0;
}