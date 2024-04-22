#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
int nbL;
int nbSig;
pid_t pidPere;
int nbV1 = 0;
int nbV2 = 0;
void traiterSig(int sig)
{
    if (sig == SIGUSR1)
    {
        nbV1 += nbSig;
        printf("Pere (%d) - Capteur 0 : nombre de vehicules = %d\n", pidPere, nbV1);
    }
    if (sig == SIGUSR2)
    {
        nbV2 += nbSig;
        printf("Pere (%d) - Capteur 1: nombre de vehicules = %d\n", pidPere, nbV2);
    }
}
int main(int argc, char **argv)
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage[%s] <NbVehicules> <NbCompté>\n", argv[0]);
        exit(1);
    }
    nbL = atoi(argv[1]);   // nbVehicules
    nbSig = atoi(argv[2]); // au bout de nbSig vehicules , envoyer signal
    pidPere = getpid();

    struct sigaction action;
    action.sa_handler = traiterSig;
    action.sa_flags = 0;
    sigemptyset(&action.sa_mask);
    if (sigaction(SIGUSR1, &action, NULL) == -1)
    {
        perror("sigaction USR1\n");
        exit(2);
    }
    if (sigaction(SIGUSR2, &action, NULL) == -1)
    {
        perror("sigaction USR2\n");
        exit(3);
    }

    for (int i = 0; i < 2; i++)
    {
        switch (fork())
        {
        case -1:
            perror("fork");
            exit(i + 10);
        case 0:
            pid_t pidFils = getpid();
            int nbVehicules = 0;
            char buffer[64];
            while (nbVehicules != nbL)
            {
                if (read(STDIN_FILENO, buffer, 64) > 0)
                {
                    nbVehicules++;
                    printf("\tCapteur %d (%d) : %d vehicules de plus => %d\n", i, pidFils, 1, nbVehicules);
                    
                }
                if ((nbVehicules % nbSig) == 0)
                {
                    if (i == 0)
                    {
                        kill(pidPere, SIGUSR1);
                    }
                    if (i == 1)
                    {
                        kill(pidPere, SIGUSR2);
                    }
                }
            }
            printf("\tCapteur %d (%d) :  Termine\n", i, pidFils);
            exit(EXIT_SUCCESS);

        default:
            break;
        }
    }
    int cr;
    while (wait(&cr) !=-1 || errno != ECHILD)
    {
    }
    printf("Père %d se termine\n", pidPere);
    return 0;
}