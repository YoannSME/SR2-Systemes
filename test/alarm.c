#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
time_t t;
int cpt = 0;

void timeout(int sig){
    cpt++;
    printf("Processus %d a reçu %d\n",getpid(),sig);
}

int main(void){
    printf("Pid père = %d\n",getpid());
    struct sigaction action;
    action.sa_handler = timeout;
    sigaction(SIGALRM,&action,NULL);
    switch(fork()){
        case -1:exit(1);
        case 0:
        printf("pid fils = %d\n",getpid());
        alarm(1);
       
       
        default:break;
    }
    sleep(2);
    return 0;
}