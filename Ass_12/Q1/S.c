#include <arpa/inet.h> 
#include <errno.h> 
#include <netinet/in.h> 
#include <signal.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <strings.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <sys/select.h>
#include <sys/un.h>

#define CONTROLLEN CMSG_LEN(sizeof(int))
static struct cmsghdr *cmptr = NULL;
char *path = "./path1";

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
    int sfd1,sfd2,sfd3;
    sfd1=socket(AF_INET,SOCK_STREAM,0);
    sfd2=socket(AF_INET,SOCK_DGRAM,0);
    sfd3=socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in adr1,adr2,adr3;
    adr1.sin_family=AF_INET;
    adr2.sin_family=AF_INET;
    adr3.sin_family=AF_INET;
    adr1.sin_port=htons(6003);
    adr2.sin_port=htons(6034);
    adr3.sin_port=htons(6040);
    adr1.sin_addr.s_addr=inet_addr("127.0.0.1");
    adr2.sin_addr.s_addr=inet_addr("127.0.0.1");
    adr3.sin_addr.s_addr=inet_addr("127.0.0.1");
    socklen_t adrlen1=sizeof(adr1);
    socklen_t adrlen2=sizeof(adr2);
    socklen_t adrlen3=sizeof(adr3);

    bind(sfd1,(struct sockaddr*)&adr1,adrlen1);
    bind(sfd2,(struct sockaddr*)&adr2,adrlen2);
    bind(sfd3,(struct sockaddr*)&adr3,adrlen3);

    listen(sfd1,3);
    listen(sfd3,3);

    fd_set rset;
    FD_ZERO(&rset);
    int mxfd=max(sfd1,max(sfd2,sfd3))+1;
    

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

    while(1)
    {
        FD_SET(sfd1,&rset);
        FD_SET(sfd2,&rset);
        FD_SET(sfd3,&rset);

        int r=select(mxfd,&rset,NULL,NULL,NULL);
        if(r>0)
        {
            if(FD_ISSET(sfd2,&rset))
            {
                struct sockaddr_in cliadr;
                socklen_t clilen=sizeof(cliadr);
                char buf[1024];
                recvfrom(sfd2,&buf,sizeof(buf),0,(struct sockaddr*)&cliadr,&clilen);
                printf("%s\n",buf);
            }
            else
            {
                int nsfd;
                if(FD_ISSET(sfd3,&rset))
                    nsfd=accept(sfd3,(struct sockaddr*)&adr3,&adrlen3);
                else if(FD_ISSET(sfd1,&rset))
                    nsfd=accept(sfd1,(struct sockaddr*)&adr1,&adrlen1);
                
                send_fd(nufd,nsfd);

            }
        }

    }



}