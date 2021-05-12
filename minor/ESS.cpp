#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/msg.h>
#include <signal.h>
#include <string.h>
#include <sys/poll.h>
using namespace std;
key_t key;
int msgid;
struct msgbuff
{
    long mtype;
    char mtext[100];
};
struct msgbuff msg;
int main()
{
    key=ftok(".",65);
    msgid=msgget(key,0666);
    while(1)
    {
        if(msgrcv(msgid,&msg,sizeof(msg),0,0)!=-1)
        {
            cout<<"C"<<msg.mtype<<": "<<msg.mtext<<endl;
        }
    }
}