#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdio.h> 
#include<fcntl.h> 
#include<stdlib.h> 
#include <semaphore.h>
#include<sys/wait.h>
#include <string.h>
  
const char* sem1= "s1";
const char* sem2 = "s2"; 

void p2(){

    sem_t *s1=sem_open(sem1,O_EXCL);
    sem_t *s2=sem_open(sem2,O_EXCL);

    key_t key1 = ftok("X",65); 
  
    int shmid1 = shmget(key1,1024,0666|IPC_CREAT); 
  
    char *str1 = (char*) shmat(shmid1,(void*)0,0); 
    
    
    key_t key2 = ftok("Y",65); 
  
    
    int shmid2 = shmget(key1,1024,0666|IPC_CREAT); 
  
    
    char* str2 = (char*) shmat(shmid2,(void*)0,0); 
    char* offset="x";
    sem_wait(s1);
    int t=5;
    while(t--){
        printf("p2 -> reading %s  from X\n" , str1);
  		strcpy(str2 , strcat(str1 , offset));
  		printf("p2-> writing %s in Y\n", str2 );
  		sem_post(s2);
       	sem_wait(s1);
    }
    
    shmdt(str1); 
    shmdt(str2); 
}

int main(){
    p2();
    return 0;

}
