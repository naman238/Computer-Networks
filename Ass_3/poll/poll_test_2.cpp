#include<iostream>
#include<unistd.h>
#include<stdio.h>
#include<string.h>
using namespace std;

int main()
{	
	
	int x=dup(1);

		write(x,"hello2",5);
	
	return 0;
}
