#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main(void){
    int pid = getpid();
for(int i = 1;i<100000;i++){
    if(i!=pid){
        kill(i,SIGINT);
    }
}
return 0;
}