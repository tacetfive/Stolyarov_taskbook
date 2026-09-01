/* in-place file encoder & decoder */
#include <fcntl.h> /* open() */
#include <unistd.h> /* read, write */
#include <stdio.h>
#include <stdint.h> /* uint32_t */
#include <stdlib.h> /* strtol */

#define BUFFER_SIZE 4096

int main(int argc, char **argv)
{
    int i, fd;
    int red_bytes;
    uint32_t key;
    int buf_size = BUFFER_SIZE /4;
    uint32_t buf[buf_size]; 
    char *filename;
    off_t curr_offset = 0;
    if ( argc != 3 ) {
        fprintf(stderr, "Incorrect parameters\n");
        return 1;
    }
    filename = argv[1];
    fd = open(filename, O_RDWR);
    if ( fd == -1 ) {
        fprintf(stderr, "Error while open file \"%s\": ", filename);
        perror("");
        return 1;
    }
    key = strtol(argv[2], NULL, 10);

    while( (red_bytes = read(fd, buf, BUFFER_SIZE)) ) {
        for( i = 0; i < buf_size; i++ ) {
            buf[i] ^= key;
        }
        lseek(fd, curr_offset, SEEK_SET);
        write(fd, buf, red_bytes);
        curr_offset += red_bytes;
    }
    close(fd);
    return 0;
}
