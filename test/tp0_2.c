#define _POSIX_C_SOURCE 200809L
#define PRODUCTION 60
#define fabrication(n) (n + 1) * 6
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

time_t t;
int nbProduction;

void erreur(const char *msg, int intexit)
{
    perror(msg);
    exit(intexit);
}

int main(int argc, char **argv)
{
    if (argc < 4)
    {
        fprintf(stderr, "Usage[%s] <NF> <NB> <NS>\n", argv[0]);
        exit(1);
    }

    int nbFils = atoi(argv[1]);
    int nbFois = atoi(argv[2]);
    int sec = atoi(argv[3]);
    pid_t pidCourant;
    char buffer[PRODUCTION];
    

    int tube[2];
    if (pipe(tube) == -1)
    {
        erreur("pipe ", 2);
    }

    for (int i = 0; i < nbFils; i++)
    {
        switch (pidCourant = fork())
        {
        case -1:
            erreur("fork", i + 10);
        case 0:
            close(tube[1]);
            int nbLus,nb_box = 0;
            while((nbLus = read(tube[0], buffer, fabrication(i)))==fabrication(i)){
            nbProduction -= fabrication(i);
            nb_box++;

            printf("Emballage %d : nouvelle boite de %d produite\n", i, fabrication(i));
            }
            printf("Emballage %d j'ai produit %d boites de %d chocolats\n", i, nb_box, fabrication(i));
            close(tube[0]);
            exit(0);
        default:
            break;
        }
    }
    
    close(tube[0]);
    for (int i = 0; i < nbFois; i++)
    {
        int nbProduction = PRODUCTION;
        printf("Usine : je produis %d chocolats |%s", PRODUCTION, (time(&t), ctime(&t)));
        write(tube[1], buffer, nbProduction);
        
        
        sleep(sec);
    }
    sleep(1);
    close(tube[1]);

    return 0;
}