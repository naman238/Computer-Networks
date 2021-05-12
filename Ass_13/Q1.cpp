#include <iostream>
#include <string>
using namespace std;
int main()
{
    string output = "pidof ";
    output+="./Q2";
    char line[ 15 ];
    FILE *cmd = popen( output.c_str( ), "r");
    fgets( line, 15, cmd );
    pid_t pid = strtoul( line, NULL, 10 );
    pclose(cmd);
    cout<<pid;
}