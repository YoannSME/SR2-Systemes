#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
time_t t;

int main(int argc,char** argv){
    if(argc!= 3){
        fprintf(stderr,"Usage[%s] <NB fois> <NB secondes\n",argv[0]);
        exit(1);
    }

    int nbFois = atoi(argv[1]);
    int nbSec = atoi(argv[2]);
    pid_t pidPere = getpid();
    for(int i = 0;i<nbFois;i++){
        printf("Mon numero est %d, il est %s",pidPere,(time(&t), ctime(&t)));
        sleep(nbSec);
    }
    return 0;
}