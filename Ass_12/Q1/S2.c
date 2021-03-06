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

//char *socket_path = "./socket";
char *path = "./path1";

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
int max(int x, int y) 
{ 
    if (x > y) 
        return x; 
    else
        return y; 
} 

int main(int argc, char *argv[]) {
	struct sockaddr_un addr;
	char buf[100];
	int usfd,rc;

	if((usfd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
		perror("socket error");
		exit(1);
	}
	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	strcpy(addr.sun_path, path);
	if(connect(usfd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
		perror("connect error");
		exit(1);
	}
    int ind=-1;
    int arr[100];
    fd_set rset;
    int mx=-1;
    mx=max(mx,usfd)+1;
    while(1)
    {
        FD_SET(usfd,&rset);
        
        
        for(int i=0;i<=ind;i++)
            FD_SET(arr[i],&rset);
        //mx=max(mx,rfd)+1;
        int n=select(mx,&rset,NULL,NULL,NULL);
        if(n>0)
        {
            if(FD_ISSET(usfd,&rset))
            {
                int rfd;
                if((rfd = recv_fd(usfd)) < 0) {
                    perror("recv_fd");
                }
                else {arr[++ind]=rfd;mx=max(mx,rfd)+1;continue;}
            }
            for(int i=0;i<=ind;i++)
            {
                if(FD_ISSET(arr[i],&rset))
                {
                    char buff[1024];
                    int x=read(arr[i], buff,1024);
                    printf("%s\n",buff);
                }
            }
        }
    }
    
	return 0;
}