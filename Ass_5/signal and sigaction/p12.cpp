#include <stdio.h> 
#include <unistd.h> //for fork system call
#include <sys/types.h>
#include <stdlib.h> //for exit system call
#include <sys/wait.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <fcntl.h>
#include <pthread.h> 
#include <semaphore.h> 
#include <string.h>
#include <sys/poll.h>
#include <sys/time.h>
#include <assert.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>
#include <unistd.h>
#include <signal.h>
#include <iostream>
using namespace std;

#define FILLED 0 
#define Ready 1 
#define NotReady -1 

#define SIGHUP  1   /* Hangup the process */ 
#define SIGINT  2   /* Interrupt the process */ 
#define SIGQUIT 3   /* Quit the process */ 
#define SIGILL  4   /* Illegal instruction. */ 
#define SIGTRAP 5   /* Trace trap. */ 
#define SIGABRT 6   /* Abort. */

struct sigaction act;
		
void handler(int sig, siginfo_t *siginfo, void *context){
	printf("Sending pid: %ld, UID: %ld\n",(long) siginfo->si_pid, (long) siginfo->si_uid);
}

int main(){
	int pid = getpid();
	printf("PID of P2: %d\n",pid);
	act.sa_sigaction = &handler;
	act.sa_flags = SA_SIGINFO;

	//signal(SIGINT , SIG_DFL);
	int k = sigaction(SIGUSR1 , &act , NULL);

	if(k < 0){
		printf("Gadbad\n");
		return 1;
	}
	else{
		//sleep(1);
		//signal(SIGINT , SIG_DFL);
		printf("Yes ! received.\n");
	}
	
	sleep(5);
	return 0;
}