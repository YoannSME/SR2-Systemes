#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <wait.h>
pid_t p;
void g(int p){
    printf("%d a recu %d\n",getpid(),p);
}

int main(void){
    printf("pid père = %d\n",getpid());
    struct sigaction act;
    act.sa_handler = g;
    sigaction(SIGINT,&act,NULL);
    switch(p = fork()){
        case -1:
        exit(1);
        case 0:while(1);
        default:break;
    }
    wait(NULL);
    return 0;

}