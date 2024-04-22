#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

void traiterSignal(int sig){
    printf("Ctrl-C/SIGINT reçu par le processus de n° %d\n",getpid());
    printf("Et je vais devenir l'executable")
}

int main(void){
    struct sigaction action;
    action.sa_handler = traiterSignal;
    action.sa_flags = 0;
    sigemptyset(&action.sa_mask);
    sigaction(SIGINT,&action,NULL);
    pid_t pid = getpid();
    while(1){
        printf("%d\n",pid);
        sleep(1);
    }

    return 0;
}