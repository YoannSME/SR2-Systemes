#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

pid_t p;
void g(int p){
    printf("%d a reçu %d\n",getpid(),p);
}

int main(void){
    struct sigaction act;
    act.sa_handler = g;
    sigaction(SIGINT,&act,NULL);
    switch(p = fork()){
        case -1:
        exit(1);
        case 0:while(1);
        printf("%d fils fini\n",getpid());
        default:break;
    }
    wait(NULL);
    printf("%d père fini\n",getpid());
    return 0;
}