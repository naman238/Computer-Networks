/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _IDL_H_RPCGEN
#define _IDL_H_RPCGEN

#include <rpc/rpc.h>


#ifdef __cplusplus
extern "C" {
#endif


struct values {
	float num1;
	float num2;
	char operation;
};
typedef struct values values;

#define COMPUTE 456123789
#define COMPUTE_VERS 6

#if defined(__STDC__) || defined(__cplusplus)
#define MUL 1
extern  float * mul_6(values *, CLIENT *);
extern  float * mul_6_svc(values *, struct svc_req *);
extern int compute_6_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define MUL 1
extern  float * mul_6();
extern  float * mul_6_svc();
extern int compute_6_freeresult ();
#endif /* K&R C */

/* the xdr functions */

#if defined(__STDC__) || defined(__cplusplus)
extern  bool_t xdr_values (XDR *, values*);

#else /* K&R C */
extern bool_t xdr_values ();

#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_IDL_H_RPCGEN */
