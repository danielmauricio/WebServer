#include <stdio.h>
#include <stdlib.h>

int main(int argc,char **argv){
    int a = atoi(argv[1]);
    int b = atoi(argv[2]);

    printf("\nEl resultado de multiplicar %d y %d es %d\n",a,b,a*b);

    return 0;
}
