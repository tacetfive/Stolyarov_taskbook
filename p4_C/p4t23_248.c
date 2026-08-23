#include <stdio.h>
#include <stdlib.h>

int string_count(char *filename);

int main(int argc, char **argv)
{
    char *filename = argv[1];
    printf("%d\n", string_count(filename));
    return 0;
}

int string_count(char *filename)
{
    int c, counter;
    FILE *f = fopen(filename, "r");
    if (!f) {
        perror(filename); // send message to stderr
        exit(1);
        }
    while((c = fgetc(f)) != EOF) {
        if (c == '\n') {
            counter++;
        }
    }
    fclose(f);
    return counter;
}
