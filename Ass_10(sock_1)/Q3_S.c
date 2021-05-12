#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <arpa/inet.h>
#include <poll.h>
#include <sys/poll.h>
#include <pthread.h>
int nsfd1,nsfd2,nsfd3;
  int sfd1,sfd2,sfd3;
void* func1(void*args)
{
   while(1)
   {
    char buff[1024];
    int reader=recv(nsfd1,&buff,sizeof(buff),0);
    if(reader==0)
    {
        close(nsfd1);
        break;
    }
    printf("%s\n",buff);
    char* msg="hello from server1";
    int bytesend=send(nsfd1,msg,strlen(msg),0);
   }
}
void* func2(void*args)
{
   while(1)
   {
    char buff[1024];
    int reader=recv(nsfd2,&buff,sizeof(buff),0);
    if(reader==0)
    {
        close(nsfd2);
        break;
    }
    printf("%s\n",buff);
    char* msg="hello from server1";
    int bytesend=send(nsfd2,msg,strlen(msg),0);
   }
}
void* func3(void*args)
{
   while(1)
   {
    char buff[1024];
    int reader=recv(nsfd3,&buff,sizeof(buff),0);
    if(reader==0)
    {
        close(nsfd3);
        break;
    }
    printf("%s\n",buff);
    char* msg="hello from server1";
    int bytesend=send(nsfd3,msg,strlen(msg),0);
   }
}
int main()
{
  
    if((sfd1=socket(AF_INET,SOCK_STREAM,0))==0)
    {
        perror("socket 1 failed\n");
        return 0;
    }
    int opt1=1,opt2=1,opt3=1;
    if (setsockopt(sfd1, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
												&opt1, sizeof(opt1))) 
	{ 
		perror("setsockopt"); 
		exit(EXIT_FAILURE); 
	} 
    if((sfd2=socket(AF_INET,SOCK_STREAM,0))==0)
    {
        perror("socket 2 failed\n");
        return 0;
    }
    if (setsockopt(sfd2, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
												&opt2, sizeof(opt2))) 
	{ 
		perror("setsockopt"); 
		exit(EXIT_FAILURE); 
	} 
    if((sfd3=socket(AF_INET,SOCK_STREAM,0))==0)
    {
        perror("socket 3 failed\n");
        return 0;
    }
    if (setsockopt(sfd3, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
												&opt3, sizeof(opt3))) 
	{ 
		perror("setsockopt"); 
		exit(EXIT_FAILURE); 
	} 
    struct sockaddr_in addr1,addr2,addr3;
    addr1.sin_port=htons(7001);
    addr1.sin_family=AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &(addr1.sin_addr));
    addr2.sin_port=htons(8001);
    addr2.sin_family=AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &(addr2.sin_addr));
    addr3.sin_port=htons(9001);
    addr3.sin_family=AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &(addr3.sin_addr));
    if(bind(sfd1,(struct sockaddr*)&addr1,sizeof(addr1))<0)
    {
        perror("bind 1 failed");
        return 0;
    }
    if(bind(sfd2,(struct sockaddr*)&addr2,sizeof(addr2))<0)
    {
        perror("bind 2 failed");
        return 0;
    }
    if(bind(sfd3,(struct sockaddr*)&addr3,sizeof(addr3))<0)
    {
        perror("bind 3 failed");
        return 0;
    }
    if(listen(sfd1,3)<0)
    {
        perror("listen 1 failed");
        return 0;
    }
    if(listen(sfd2,3)<0)
    {
        perror("listen 2 failed");
        return 0;
    }
    if(listen(sfd3,3)<0)
    {
        perror("listen 3 failed");
        return 0;
    }
    
    socklen_t addrlen1=sizeof(addr1);
    socklen_t addrlen2=sizeof(addr2);
    socklen_t addrlen3=sizeof(addr3);
    struct pollfd pfd[3];
    pfd[0].fd=sfd1;
    pfd[0].events=0;
    pfd[0].events|=POLLIN;
    pfd[1].fd=sfd2;
    pfd[1].events=0;
    pfd[1].events|=POLLIN;
    pfd[2].fd=sfd3;
    pfd[2].events=0;
    pfd[2].events|=POLLIN;
    
    while(1)
    {
        poll(pfd,3,-1);
        int check[3]={0};
        for(int i=0;i<3;i++)
        {
 
            if(pfd[i].revents&POLLIN)
            {
               
                if(i==0)
                {
                    if((nsfd1=accept(sfd1,(struct sockaddr*)&addr1,&addrlen1))<0)
                    {
                        perror("accept 1 failed");
                        return 0;
                    }
                    
                    check[i]=1;
                   
                }
                else if(i==1)
                {
                    if((nsfd2=accept(sfd2,(struct sockaddr*)&addr2,&addrlen2))<0)
                    {
                        perror("accept 2 failed");
                        return 0;
                    }
                    check[i]=1;
                    
                    
                }
                else if(i==2)
                {
                    if((nsfd3=accept(sfd3,(struct sockaddr*)&addr3,&addrlen3))<0)
                    {
                        perror("accept 3 failed");
                        return 0;
                    }
                    check[i]=1;
                   
                }
            }
        }
        if(check[0])
        {
            //  int c=fork();
            //         if(c==0)
            //         {
            //             close(sfd1);
            //             int x=dup(STDIN_FILENO);
            //             dup2(nsfd1,STDIN_FILENO);
            //             execv("./q3s1",NULL);
            //             dup2(6,STDIN_FILENO);
            //             return 0;
            //         }
            //         else
            //         {
            //             close(nsfd1);
            //         }
            pthread_t t2;
            pthread_create(&t2,NULL,&func1,0);
          
            
        } 
        if(check[1])
        {
            // int c=fork();
            //         if(c==0)
            //         {
            //             close(sfd3);
            //              int x=dup(2);
            //             dup2(nsfd3,2);
            //             execv("./q3s2",NULL);
            //             dup2(8,2);
            //             return 0;
            //         }
            //         else
            //         {
            //             close(nsfd2);
            //         }
            
            pthread_t t;
            pthread_create(&t,NULL,&func2,0);
          
           
            

        }
        if(check[2])
        {
            // int c=fork();
            //         if(c==0)
            //         {
            //             close(sfd3);
            //              int x=dup(2);
            //             dup2(nsfd3,2);
            //             execv("./q3s3",NULL);
            //             dup2(8,2);
            //             return 0;
            //         }
            //         else
            //         {
            //             close(nsfd3);
            //         }
            pthread_t t1;
            pthread_create(&t1,NULL,&func3,0);
            
            

        }
    }
    return 0;
}