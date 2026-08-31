
#include <stdio.h>
#include <unistd.h>

int main()
{
    char buf[1024];
    int  red_bytes;
    while( (red_bytes = read(0, buf, 1024)) ) {
        write(1, buf, red_bytes);
    }
    return 0;
}








