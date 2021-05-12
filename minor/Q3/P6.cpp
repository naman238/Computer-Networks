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
    sem_t *s1=sem_open(sem1,O_EXCL);
    string str;
    sem_wait(s1);
    FILE* fd=popen("./P1","r");
    int x=dup(STDIN_FILENO);
    dup2(fileno(fd),STDIN_FILENO);
    cin>>str;
    dup2(STDIN_FILENO,6);
    cout<<str;
    sem_unlink(sem1);
    return 0;
}