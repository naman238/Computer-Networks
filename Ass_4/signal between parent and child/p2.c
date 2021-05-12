#include <stdio.h>
#include <signal.h>
#include <unistd.h>
void p_c()
{
    printf("parent to child\n");
    signal(SIGUSR1,SIG_DFL);
}
void c_p()
{
    printf("child to parent\n");
    signal(SIGUSR2,SIG_DFL);
}
int main()
{
    signal(SIGUSR1,p_c);
    signal(SIGUSR2,c_p);
    int c= fork();
    if(c>0)
    {
        kill(c,SIGUSR1);
        wait(NULL);
    }
    else
    {
        kill(getppid(),SIGUSR2);
    }
    return 0;
}