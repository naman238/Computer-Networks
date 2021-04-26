#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main()
{
    int pp[2];
    pipe(pp);
    char buf[128];
    char buf2[128];
    fgets(buf,128,stdin);
    write(pp[1],buf,128);
    read(pp[0],buf2,128);
    printf("%s",buf2);
    return 0;
}