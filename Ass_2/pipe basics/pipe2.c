#include <stdio.h>
#include <unistd.h>
#include <string.h>
int main()
{
    int a[2],b[2];
    pipe(a);
    pipe(b);
    int c=fork();
    if(c>0)
    {
        close(a[0]);
        close(b[1]);
        char buf1[128];char buf2[128];
        int t=5;
        while(t--)
        {
        fgets(buf1,128,stdin);
        write(a[1],buf1,128);
        
        read(b[0],buf2,128);
        printf("pipe b reads %s\n",buf2);}
    }
    else
    {
        close(b[0]);
        close(a[1]);    
        char buf1[128];
        int t=5;
        while(t--)
        {
        read(a[0],buf1,128);
        printf("pipe a reads %s\n",buf1);
        buf1[0]='X';
        write(b[1],buf1,128);}
    }
    return 0;
}