#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#define MONTANT_MAX 120

void erreur(const char *msg, int intexit)
{
    perror(msg);
    exit(intexit);
}

int main(int argc, char **argv)
{

    srand(time(NULL));

    if (argc != 2)
    {
        fprintf(stderr, "Usage[%s] <nbFils>", argv[0]);
        exit(1);
    }

    int nbFils = atoi(argv[1]);
    int tubeSend[nbFils][2];
    int tubeReceive[nbFils][2];

    for (int i = 0; i < nbFils; i++)
    {
        if (pipe(tubeSend[i]) == -1 || pipe(tubeReceive[i]) == -1)
        {
            erreur("pipe", 2);
        }
    }
    // écris dans le 1 read dans le 0
    for (int i = 0; i < nbFils; i++)
    {
        switch (fork())
        {
        case -1:
            erreur("fork ", i + 10);

        case 0:
            close(tubeReceive[i][0]);
            char buffer[64];
            if (read(tubeSend[i][0], buffer, 64) == -1)
            {
                erreur("read fils", i + 20);
            }

            printf("Fournisseur %d : j'ai reçu la demande (%s)\n", i, buffer);
            double nb = rand() % (MONTANT_MAX + i);
            printf("Fournisseur %d : montant = %f\n", i, nb);
            snprintf(buffer, 64, "%f", nb);
            if (write(tubeReceive[i][1], buffer, strlen(buffer) + 1) == -1)
            {
                erreur("write fils", 4);
            }
            close(tubeReceive[i][1]);
            
            if (read(tubeSend[i][0], buffer, 64) > 0)
            {
                printf("Fournisseur %d : j'ai obtenu le marche avec %lf\n", i, nb);
            }
            else
            {
                printf("Fournisseur %d : j'ai perdu le marche pour %lf\n", i, nb);
            }
            close(tubeSend[i][0]);
            exit(0);

        default:
            close(tubeSend[i][0]);
            char *reference = "AB5";

            if (write(tubeSend[i][1], reference, strlen(reference) + 1) == -1)
            {
                perror("write père");
                exit(3);
            }

            printf("Client : J'envoie demande au fournisseur %d\n", i);
           

            break;
        }
    }
    double min = MONTANT_MAX + 1;
    int fournisseur;

    char *endptr;
    for (int i = 0; i < nbFils; i++)
    {
        close(tubeReceive[i][1]);
        char buffer[64];
        if (read(tubeReceive[i][0], buffer, 64) == -1)
        {
            erreur("read père", 5);
        }
        double nb = strtod(buffer, &endptr);
        printf("Client : Je reçois %lf du fournisseur %d\n", nb, i);
        if (nb < min)
        {
            min = nb;
            fournisseur = i;
        }
        else{
            close(tubeSend[i][1]);
        }
        close(tubeReceive[i][0]);
    }

    printf("Client : Je choisis le fournisseur %d pour un montant de %lf\n", fournisseur, min);
    char buffer[64];

    if (write(tubeSend[fournisseur][1], buffer, strlen(buffer) + 1) == -1)
    {
        erreur("write pour fin", 6);
    }
    close(tubeSend[fournisseur][1]);

    return 0;
}