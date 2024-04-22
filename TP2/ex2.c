#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
time_t t;
int filsCourant;
int nbVF;
int tube[][2];
int nbSec;

void traiterSignal(int sig)
{
    if(sig == SIGINT){
        exit(0);
    }
    if(sig == SIGALRM){
    printf("Capteur %d (%d) valeur %d ecrite dans tube %d à %s", filsCourant, getpid(), nbVF,filsCourant, (time(&t), ctime(&t)));
    write(tube[filsCourant][1], &nbVF, sizeof(int));
    alarm(nbSec);
    }
}
int main(int argc, char **argv)
{
    if (argc != 4)
    {
        fprintf(stderr, "Usage[%s] <nbFils> <nbVéhiculesMax> <nbSec>\n", argv[0]);
        exit(1);
    }

     int nbFils = atoi(argv[1]);
    int nbMax = atoi(argv[2]);
    nbSec = atoi(argv[3]);

    struct sigaction action;
    action.sa_handler = traiterSignal;
    action.sa_flags = 0;
    sigemptyset(&action.sa_mask);
    if (sigaction(SIGALRM, &action, NULL) == -1)
    {
        perror("sigaction");
        exit(88);
    }
    if (sigaction(SIGINT, &action, NULL) == -1)
    {
        perror("sigaction");
        exit(88);
    }

   

    for (int i = 0; i < nbFils; i++)
    {
        if (pipe(tube[i]) == -1)
        {
            perror("pipe");
            exit(i + 10);
        }
    }

    for (int i = 0; i < nbFils; i++)
    {
        switch (fork())
        {
        case -1:
            perror("fork");
            exit(20 + i);
        case 0:
            alarm(nbSec);
            
            nbVF = 0;
            filsCourant = i;

            while (nbVF < nbMax)
            {
                char buffer[64];
                if (read(STDIN_FILENO, buffer, sizeof(buffer)) > 0)
                {
                    nbVF += strlen(buffer) - 1;
                }
            }
            close(tube[i][1]);
            
            int sortie;
            (read(tube[i][0],&sortie,sizeof(int)));
            if(sortie==2){
                printf("Capteur %d (%d) : Termine\n", i, getpid());
                exit(0);
            }


        default:
            break;
        }
    }
    int nbFilsFini = 0;
    int fin = 2;
    pid_t pere = getpid();
    while(nbFilsFini<nbFils){
        for(int i = 0;i<nbFils;i++){
            int msgLu;
            if(read(tube[i][0],&msgLu,sizeof(int))>0 && msgLu>0){
    
                printf("Père (%d) - Capteur %d : nombre de véhicules : %d\n",pere,i,msgLu);
                if(msgLu>=nbMax){
                    printf("Fils %d fini\n",i);
                    nbFilsFini++;
                    write(tube[i][1],&fin,sizeof(int));
                    close(tube[i][0]);
                    close(tube[i][1]);
                }
            }

        }

    }

    return 0;
}