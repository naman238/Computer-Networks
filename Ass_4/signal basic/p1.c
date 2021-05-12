#include <stdio.h>
#include <unistd.h>
#include <signal.h>
void handler()
{
    printf("no\n");
    signal(SIGINT,SIG_DFL);
}
int main()
{
    signal(SIGINT,handler);
    
    while(1);
    return 0;
}