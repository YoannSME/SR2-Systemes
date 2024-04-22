#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <sys/types.h>
int x = 3;

int main(int argc,char** argv){
   switch(fork()){
    case -1:
    exit(1);
    case 0:
    x+=1;
    printf("%d\n",x);
    exit(2);

    default:
    printf("pere %d\n",x);
    break;
   }

   return 0;
}