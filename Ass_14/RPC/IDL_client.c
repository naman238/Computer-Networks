#include "IDL.h"
#include <stdio.h>


float
compute_6(char *host,float a,float b,char op)
{
	CLIENT *clnt;
	float  *res1;
	values  mul_6_arg;


if(op=='*'){
	mul_6_arg.num1=a;
	mul_6_arg.num2=b;
	mul_6_arg.operation=op;

	clnt = clnt_create (host, COMPUTE, COMPUTE_VERS, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}

	res1 = mul_6(&mul_6_arg, clnt);
	if (res1 == (float *) NULL) {
		clnt_perror (clnt, "call failed");
	}

	clnt_destroy (clnt);

	return (*res1);	
}

}


int
main (int argc, char *argv[])
{
	char *host;

	float number1,number2;
	char oper;
    	printf("Enter the 2 numbers for multiplication operation:\n");
    	scanf("%f",&number1);
    	scanf("%f",&number2);
	scanf("%s",&oper);

	host = argv[1];
	printf("Answer= %f\n",compute_6 (host,number1,number2,oper));
        exit(0);

}