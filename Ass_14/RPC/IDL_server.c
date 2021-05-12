#include "IDL.h"
#include <stdio.h>


float *
mul_6_svc(values *argp, struct svc_req *rqstp)
{
	static float  result;
	result = argp->num1 * argp->num2;
	return &result;
}

