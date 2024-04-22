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
}

int main(void){
    struct sigaction action;
    action.sa_handler = traiterSignal;
    action.sa_flags = 0;
    sigemptyset(&action.sa_mask);
    sigaction(SIGINT,&action,NULL);
    pid_t pid = getpid();
    switch(fork()){
        case -1:
        perror("fork");
        exit(1);

        case 0:
        int pidFils = getpid();
        while(1){
            printf("Je suis le processus %d\n",pidFils);
            sleep(1);
        }
        printf("fils\n");
        exit(0);

        default:break;
    }
    wait(NULL);
    printf("père %d fini\n",pid);

    return 0;
}