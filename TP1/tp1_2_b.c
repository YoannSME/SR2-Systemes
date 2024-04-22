#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int nbFois;
int nbSec;
char * exec;
void traiterSignal(int sig) {
    printf("Ctrl-C/SIGINT reçu par le processus de n° %d\n", getpid());
    printf("Et je vais devenir l'executable %s pour afficher %d fois toutes les %d secondes\n", exec,nbFois, nbSec);
    

    char nbFoisStr[10], nbSecStr[10];
    sprintf(nbFoisStr, "%d", nbFois);
    sprintf(nbSecStr, "%d", nbSec);
    execlp(exec, exec, nbFoisStr, nbSecStr, NULL);
    perror("execlp");
    exit(EXIT_FAILURE);
}

int main(int argc, char** argv) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <NbFois> <NbSec> <Executable>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    nbFois = atoi(argv[1]);
    nbSec = atoi(argv[2]);
    exec = argv[3];
    struct sigaction action;
    action.sa_handler = traiterSignal;
    action.sa_flags = 0;
    sigemptyset(&action.sa_mask);
    sigaction(SIGINT, &action, NULL);
    pid_t pid = getpid();
    while (1) {
        printf(" processus : %d\n", pid);
        sleep(1);
    }
    return 0;
}
