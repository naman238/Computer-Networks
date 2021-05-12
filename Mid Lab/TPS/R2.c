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
char * path1 ="./p02";
char * path2 ="./p12";
char * path3 ="./p23";
struct info{
    int busy[3];
};
struct score{
    int marks[3][3];
};
int shmid,shmid1;
struct info* ptr;
struct score* ptr1;

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
int main()
{
    key_t k=ftok(".",50);
    shmid=shmget(k,sizeof(struct info),IPC_CREAT|0666);
    ptr=(struct info*)shmat(shmid,NULL,0);

    key_t k1=ftok(".",55);
    shmid1=shmget(k1,sizeof(struct score),IPC_CREAT|0666);
    ptr1=(struct score*)shmat(shmid1,NULL,0);
struct sockaddr_un adr1,adr2,adr3,adr4,adr5;
    int usfd1;
    int usfd2=socket(AF_UNIX,SOCK_STREAM,0);
    int usfd3=socket(AF_UNIX,SOCK_STREAM,0);
    if((usfd1 = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
		perror("socket error");
		exit(1);
	}
	memset(&adr1, 0, sizeof(adr1));
	adr1.sun_family = AF_UNIX;
	strcpy(adr1.sun_path, path1);
	if(connect(usfd1, (struct sockaddr*)&adr1, sizeof(adr1)) == -1) {
		perror("connect error");
		exit(1);
	}

    if((usfd2= socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
		perror("socket error");
		exit(1);
	}
	memset(&adr2, 0, sizeof(adr2));
	adr2.sun_family = AF_UNIX;
	strcpy(adr2.sun_path, path2);

	if(connect(usfd2, (struct sockaddr*)&adr2, sizeof(adr2)) == -1) {
		perror("connect error");
		exit(1);
	}
    
   
    adr3.sun_family=AF_UNIX;
    strcpy(adr3.sun_path,path3);
    unlink(path3);
   
       socklen_t l3=sizeof(adr3); 
       
   bind(usfd3,(struct sockaddr*)&adr3,l3);
    listen(usfd3,3);
    struct sockaddr cliadr3;
    socklen_t len3=sizeof(cliadr3);
    int nufd3;
    while(1){
        
         nufd3=accept(usfd3,(struct sockaddr*)&cliadr3,&len3);
        if(nufd3!=-1)break;
    }
              
   

   
    fd_set rset;
    FD_ZERO(&rset);
    int mxfd=max(usfd1,max(usfd2,nufd3))+1;

    while(1)
    {
        FD_SET(usfd1,&rset);
        FD_SET(usfd2,&rset);
        FD_SET(nufd3,&rset);
        int r=select(mxfd,&rset,NULL,NULL,NULL);
        if(r>0)
        {
            ptr->busy[1]=0;
            if(FD_ISSET(usfd1,&rset))
            {
                int rfd=recv_fd(usfd1);
                printf("aptitude test going on\n");
                int m=rand()%11;
                int ind=rfd%3;
                ptr1->marks[1][ind]=m;
                if(ptr->busy[0]==0 && ptr1->marks[0][ind]==-1)
                {
                    send_fd(usfd2,rfd);
                    ptr->busy[0]=1;

                }
                else
                {
                    send_fd(nufd3,rfd);
                    ptr->busy[2]=1;

                }
            }
            if(FD_ISSET(usfd2,&rset))
            {
                printf("recived from 1\n");
                int rfd=recv_fd(usfd2);
                printf("aptitude test going on\n");
                int m=rand()%11;
                int ind=rfd%3;
                ptr1->marks[1][ind]=m;
                if(ptr->busy[2]==0 && ptr1->marks[2][ind]==-1)
                {
                    printf("sent to 3\n");
                    send_fd(nufd3,rfd);
                    ptr->busy[2]=1;
                }
            }
            if(FD_ISSET(nufd3,&rset))
            {
                int rfd=recv_fd(nufd3);
                printf("aptitude test going on\n");
                int m=rand()%11;
                int ind=rfd%3;
                ptr1->marks[1][ind]=m;
                if(ptr->busy[0]==0 && ptr1->marks[0][ind]==-1)
                {
                    send_fd(usfd2,rfd);
                    ptr->busy[0]=1;
                }
            }

        }
    }

}