#include <fcntl.h> /* open() */
#include <unistd.h> /* read, write */
#include <stdio.h>
#include <stdint.h> /* uint32_t */
#include <stdlib.h> /* strtol */

#define BUFFER_SIZE 4096
#define KEY_LENGHT  60

struct table {
    uint32_t counter;
    char *key[60];
    int start; /* a byte number where entry starts in input file */
};

struct table *find_by_key(int fd, char *key_to_find) {
    int table_elem_size = sizeof(struct table);
    struct table *table_elem = malloc(table_elem_size);
    uint8_t buf[BUFFER_SIZE];
    int idx_buf = 0;
    int idx_key = 0;
    int counter_size = sizeof(uint32_t);
    int entry_num = 0;
    int buffers_counter = 0;
    ssize_t red_bytes;
    table_elem->counter = 0;
    memcpy(table_elem->key, key_to_find, KEY_LENGHT);
    while (red_bytes = read(fd, buf, BUFFER_SIZE)) { /* read file by 4096 bytes */
        if ( red_bytes == -1 )
            perror("read error");
        for ( idx_buf = counter_size; idx_buf < red_bytes; ) {
            if ( (buf[idx_buf] == '\0') && (key_to_find[idx_key] == '\0') )
                break; /* both strings ended up */
            /* if char is not match, shift idx_buf to next entry: */
            if (buf[idx_buf] != key_to_find[idx_key]) { 
                idx_buf = ((++entry_num) * table_elem_size) + counter_size;
                idx_key = 0;
                continue;
            }
            ++idx_key;
            ++idx_buf;
        }
        ++buffers_counter;
    }
    memcpy(&(table_elem->counter), buf[entry_num * table_elem_size], counter_size);
    table_elem->start = buffers_counter * BUFFER_SIZE + 
                        entry_num * table_elem_size;
    return table_elem;
}


void increase_counter(int fd, struct table *request) {
    

void print_table(int fd) {
    while ( red_bytes = read()


int main(int argc, char **argv)
{
    int i, fd;
    int red_bytes;
    uint32_t key;
    int buf_size = BUFFER_SIZE /4;
    uint32_t buf[buf_size]; 
    char *filename;
    char *key_to_find[KEY_LENGHT];
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
