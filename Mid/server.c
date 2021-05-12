#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <arpa/inet.h>
#include <sys/msg.h> // for msg queue
#include <sys/poll.h>
#include <wait.h>
struct msgbuf{
    long mtype;
    char mdata[100];
};
int main()
{
    int key=ftok(".",65);
    int msgid=msgget(key,0666|IPC_CREAT);
    int sfd1,sfd2;
    if((sfd1=socket(AF_INET,SOCK_STREAM,0))==0)
    {
        perror("socket failed");
        return 0;
    }
    if((sfd2=socket(AF_INET,SOCK_STREAM,0))==0)
    {
        perror("socket failed");
        return 0;
    }
    struct sockaddr_in addr1,addr2;
    addr1.sin_family=AF_INET;
    addr2.sin_family=AF_INET;
    addr1.sin_port=htons(7000);
    addr2.sin_port=htons(8000);
    inet_pton(AF_INET,"127.0.0.1",&(addr1.sin_addr));
    inet_pton(AF_INET,"127.0.0.1",&(addr2.sin_addr));
    if(bind(sfd1,(struct sockaddr*)&addr1,sizeof(addr1))<0)
    {
        perror("bind 1 failed");
        return 0;
    }
    if(listen(sfd1,3)<0)
    {
        perror("listen 1failed");
        return 0;
    }
    if(bind(sfd2,(struct sockaddr*)&addr2,sizeof(addr2))<0)
    {
        perror("bind 2 failed");
        return 0;
    }
    if(listen(sfd2,3)<0)
    {
        perror("listen 2failed");
        return 0;
    }
    struct pollfd pfd[2];
    pfd[0].fd=sfd1;
    pfd[0].events=POLLIN;
    pfd[1].fd=sfd2;
    pfd[1].events=POLLIN;
    pfd[0].revents=0;
    pfd[1].revents=0;
    int c=0;
    int nsfd1,nsfd2;
    int addrlen1 = sizeof(addr1); 
    int addrlen2 = sizeof(addr2); 
    int f=0;int d=0;
    while(1)
    {
        poll(pfd,2,-1);
        for(int i=0;i<2;i++)
        {
            if(pfd[i].revents & POLLIN){
                if(i==0){

                    if ((nsfd1 = accept(sfd1, (struct sockaddr *)&addr1, (socklen_t *)&addrlen1))<0) 
                    { 
                        perror("accept"); 
                        exit(EXIT_FAILURE); 
                    } 
                    else{
                    int c=fork();
                    if(c==0){
                        close(sfd1);
                        int x=dup(STDIN_FILENO);
                        dup2(nsfd1,0);
                        execv("./S1",NULL);
                        exit(1);      
                    }
                    else{
                        f++;
                        close(nsfd1);
                        if(f==2)d=1;
                    }
                    }

                }
            
            else if(i==1){

                    if ((nsfd2 = accept(sfd2, (struct sockaddr *)&addr2, (socklen_t *)&addrlen2))<0) 
                    { 
                        perror("accept"); 
                        exit(EXIT_FAILURE); 
                    } 
                    else{
                    int c=fork();
                    if(c==0)
                    {
                        close(sfd2);
                        int x=dup(STDIN_FILENO);
                        dup2(nsfd2,0);
                        execv("./S2",NULL);
                        exit(1);      
                    }
                    else
                    {
                        f++;
                        close(nsfd2);
                        if(f==2)d=1;
                    }
                    }

                }
            }
        }
        if(d==1)break;
        

    }
    struct msgbuf m;
    while(1)
    {
        
        
        if(msgrcv(msgid,&m,sizeof(m),0,0)!=-1)
        {
            printf("%ld:%s\n",m.mtype,m.mdata);
        }
    }
       
    
    
}