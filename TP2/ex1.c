#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
time_t t;
int nbSec;
int nbFois;

#define SIZE 256
int tube[2];
int nb = 0;

void traiterSignal(int sig)
{
    
        char msgEnvoye[256];
       
        time(&t);
        char *current_time = ctime(&t);
        snprintf(msgEnvoye, sizeof(msgEnvoye), "Je suis ton pere ...%d envoye a %s", nb, current_time);

        nb++;
        write(tube[1], msgEnvoye, strlen(msgEnvoye) + 1);
        alarm(nbSec);
    
}

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage[%s] <NbFils> <nbSecondes>\n", argv[0]);
        exit(1);
    }

    struct sigaction action;
    action.sa_handler = traiterSignal;
    action.sa_flags = 0;
    sigemptyset(&action.sa_mask);
    if (sigaction(SIGALRM, &action, NULL) == -1)
    {
        perror("SIGACTION");
        exit(8);
    }

    nbSec = atoi(argv[1]);
    nbFois = atoi(argv[2]);
    pid_t pidFils;
    if (pipe(tube) == -1)
    {
        perror("pipe");
        exit(2);
    }
    switch (pidFils = fork())
    {
    case -1:
        perror("fork");
        exit(3);

    case 0:
        char buffer[SIZE];
        int cpt = 0;
        close(tube[1]);
        while (cpt < nbFois)
        {
            if (read(tube[0], buffer, SIZE) > 0)
            {
                printf("Fils - Recu de mon pere : %s\n", buffer);
                cpt++;
            }
        }
        close(tube[0]);
        time(&t);
        char* tpsFils = ctime(&t);
        printf("Fils - je me termine à %s\n",tpsFils);
        exit(0);

    default:
        close(tube[0]);
        alarm(nbSec);

       while(nb<nbFois){
        for(int i=0;i<300000000;i++);
        printf("\tPère - je travaille entre les envois\n");
       }
        printf("\tPère je me termine en dernier\n");
        close(tube[1]);
        exit(0);
    }

    return 0;
}