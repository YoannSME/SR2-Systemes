#define _POSIC_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


void erreur(const char* msg,int nb){
    perror(msg);
    exit(nb);
}

int main(int argc,char** argv){
    if(argc != 3){
        fprintf(stderr,"Usage [%s] <NF> <NM>",argv[0]);
        exit(1);
    }
    int nbFils = atoi(argv[1]);
    


    pid_t pidPere = getpid();
    for(int i = 0;i<nbFils;i++){
        switch(fork()){
            case -1:
            erreur("fork",10+i);
            
            case 0:
                for(int j = 0;j<atoi(argv[2]);j++){
                    printf("Activite rang %d : identifiant : %d\n",i,getpid());
                }
                exit(i);
            default:
                break;
        }
    }
    pid_t pidFilsTermine;
    int cr;
    while((pidFilsTermine = wait(&cr))!=-1){
        printf("Valeur retournée par le fils %d = %d\n",pidFilsTermine,WEXITSTATUS(cr));
    }
    printf("père %d fini\n",pidPere);
    return 0;

}


