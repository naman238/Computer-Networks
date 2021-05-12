#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/msg.h>
#include <signal.h>
#include <semaphore.h>
using namespace std;
char * sem1 ="s3";
int main()
{
    sem_t *s1 = sem_open(sem1,O_CREAT|O_EXCL,0666,0);
    char x;
    
        //cout<<"want to quit?";
        cin>>x;
        if(x=='y')
        {
            sem_post(s1);
            return 0;
        }
        else{
           string s1;
        FILE* fd=popen("./P1","r");
        int x=dup(STDIN_FILENO);
        dup2(fileno(fd),STDIN_FILENO);
        cin>>s1;
        dup2(STDIN_FILENO,6);
        cout<<s1; 
        }
    
    sem_unlink(sem1);
}