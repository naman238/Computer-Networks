#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdio.h> 
#include<fcntl.h> 
#include<stdlib.h> 
#include <semaphore.h>
#include<sys/wait.h>
#include<string.h>

  
const char* sem1= "s1";
const char* sem2 = "s2"; 

void p1(){

    sem_t *s1=sem_open(sem1,O_EXCL);
    sem_t *s2=sem_open(sem2,O_EXCL);

    key_t key1 = ftok("X",65); 
  
    int shmid1 = shmget(key1,1024,0666|IPC_CREAT); 
  
    char *str1 = (char*) shmat(shmid1,(void*)0,0); 
    
    
    key_t key2 = ftok("Y",65);    
    int shmid2 = shmget(key2,1024,0666|IPC_CREAT); 
    char* str2 = (char*) shmat(shmid2,(void*)0,0); 
    char *offset="a";
    int t=5;
    strcpy(str1,"a");
    while(t--)
    {   
        printf("p1-> writing %s in X\n", str1 );
        sem_post(s1);
		sem_wait(s2);
  		printf("p1 -> reading %s from Y\n" , str2);
  		strcpy(str2 , strcat(str1 , offset));
    }
    sem_post(s1);
    shmdt(str1); 
    shmdt(str2); 
    shmctl(shmid1,IPC_RMID,NULL);
    shmctl(shmid2,IPC_RMID,NULL);

}
  
int main() 
{ 
    
    sem_t *s1 = sem_open(sem1,O_CREAT|O_EXCL,0666,0);
    sem_t *s2 = sem_open(sem2,O_CREAT|O_EXCL,0666,0);
    p1();
    sem_unlink(sem1);
    sem_unlink(sem2); 
    return 0; 
}