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
struct score{
    int marks[3][3];
};
int shmid1;
struct score* ptr1;
int main()
{
    key_t k1=ftok(".",55);
    shmid1=shmget(k1,sizeof(struct score),IPC_CREAT|0666);
    ptr1=(struct score*)shmat(shmid1,NULL,0);

    int cnt=0;
    while(cnt<3)
    {
        for(int i=0;i<3;i++)
        {
            int f=0;
            int sum=0;
            for(int j=0;j<3;j++)
            {
                if(ptr1->marks[i][j]==-1)f=1;
                else sum+=ptr1->marks[i][j];
            }
            if(f==0)
            {
                cnt++;
                printf("score of client%d is %d\n",i+1,sum/3);
            }
        }
    }

}