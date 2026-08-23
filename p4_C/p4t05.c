#include <stdlib.h> /* atoi() */
#include <stdio.h>

int get_and_zero(int* var) 
{
    int input = *var;
    *var = 0;
    return input;
}

int main(int argc, char **argv)
{
    if (!argc) return -1;
    int value = atoi(argv[1]); /* argv[1] is pointer on initial char */
    printf("%d\n", get_and_zero(&value)); /* "3" */
    printf("%d\n", value);                /* "0" */
    return 0;
}


