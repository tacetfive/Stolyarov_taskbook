#include <fcntl.h> /* open() */
#include <unistd.h> /* read, write */
#include <stdio.h>
#include <stdint.h> /* uint32_t */
#include <string.h> /* strcmp */
#include <stdlib.h> /* malloc */
#include <time.h>

#define KEY_LENGHT  60

#ifndef SMALL_BUF
#define SMALL_BUF 0
#endif

#if SMALL_BUF
enum { BUFFER_SIZE = 64 };
#else
enum { BUFFER_SIZE = 4096 };
#endif
 /* #define BUFFER_SIZE 4096 */

enum cmd { CMD_ADD, CMD_QUERY, CMD_LIST, CMD_UNKNOWN };

struct table {
    uint32_t counter;
    char key[60];
    int start; /* a byte number where entry starts in input file.
                  If there's no such key in database, equals -1 */
};

enum cmd get_command_code(const char *command)
{
    if (!strcmp("add", command)) return CMD_ADD;
    else if (!strcmp("query", command)) return CMD_QUERY;
    else if (!strcmp("list", command)) return CMD_LIST;
    else return CMD_UNKNOWN;
}

struct table *find_by_key(int fd, const char *key_to_find) 
{
    char buf[BUFFER_SIZE];
    int idx_buf = 0;
    int counter_size = sizeof(uint32_t);
    int record_size = counter_size + KEY_LENGHT;
    struct table *table_elem = malloc(sizeof(struct table));
    int buffers_counter = 0;
    ssize_t red_bytes;
    table_elem->counter = 0;
    strcpy(table_elem->key, key_to_find);
    while ( (red_bytes = read(fd, buf, BUFFER_SIZE)) ) { /* read file by 4096 bytes */
        if ( red_bytes == -1 )
            perror("read error");
        for ( idx_buf = counter_size; idx_buf < red_bytes; 
              idx_buf += record_size ) {
            if (!strcmp(&buf[idx_buf], key_to_find)) {
                table_elem->start = buffers_counter * BUFFER_SIZE + 
                                    idx_buf - counter_size;
                memcpy( &(table_elem->counter), &buf[idx_buf - counter_size], 
                        counter_size);
                return table_elem;
            }
        }
        ++buffers_counter;
    }
    table_elem->start = -1;
    return table_elem;
}

void increase_counter(int fd, const char *key) 
{
    int counter_size = sizeof(uint32_t);
    int record_size = counter_size + KEY_LENGHT;
    struct table *table_elem = find_by_key(fd, key);
    if ( table_elem->start == -1 ) {
        ++(table_elem->counter);
        lseek(fd, 0, SEEK_END);
        write(fd, table_elem, record_size);
        return;
    }
    ++(table_elem->counter);
    lseek(fd, table_elem->start, SEEK_SET);
    write(fd, &(table_elem->counter), counter_size );
    free(table_elem);
}

void query(int fd, char *key)
{
    struct table *table_elem = find_by_key(fd, key);
    printf("%d\n", table_elem->counter);
    free(table_elem);
}

void print_table(int fd) 
{
    int idx = 0;
    int idx_mod = 0;
    int red_bytes;
    int records_counter = 0;
    int counter_size = sizeof(uint32_t);
    int record_size = counter_size + KEY_LENGHT;
    char buf[BUFFER_SIZE];
    /* buf_modified is an array of pointers to table record: */
    struct table *buf_modified[BUFFER_SIZE / record_size];
    while ( (red_bytes = read(fd, buf, BUFFER_SIZE)) ) {
        idx_mod = 0;
        for ( idx = 0; idx < red_bytes; idx += record_size ) {
            buf_modified[idx_mod] = malloc( sizeof(struct table) );
            memcpy( buf_modified[idx_mod], &buf[idx], record_size );
            ++idx_mod;
        }
        records_counter = idx_mod; /* how many records actually red */
        for ( idx_mod = 0; idx_mod < records_counter ; ++idx_mod )
            printf("%12d    [ %s ]\n", buf_modified[idx_mod]->counter, 
                                       buf_modified[idx_mod]->key);
        /* free dynamic memory */
        for ( idx_mod = 0; idx_mod < records_counter ; ++idx_mod )
            free(buf_modified[idx_mod]);
    }
}

int main(int argc, char **argv) 
{
    clock_t time_start = clock();
    clock_t time_end;
    enum cmd command_code;
    int fd;
    char *filename;
    char key[KEY_LENGHT] = { '\0' }; 
    /* I think, initialize command line arguments is more safely,
     * this avoided segmentation fault problems. */
    char command[12] = { '\0' };
    filename = argv[1];
    if ( argc > 2 ) 
        strcpy(command, argv[2]);
    if ( argc > 3 && strlen(argv[3]) < KEY_LENGHT )
        strcpy(key, argv[3]);
    fd = open(filename, O_RDWR);
    if ( fd == -1 ) {
        fprintf(stderr, "Error while open file \"%s\": ", filename);
        perror("");
        return 1;
    }
    command_code = get_command_code(command);
    switch (command_code) {
        case CMD_ADD:
            increase_counter(fd, key);
            break;
        case CMD_QUERY:
            query(fd, key);
            break;
        case CMD_LIST:
            print_table(fd);
            break;
        default:
            printf("nothing happened\n");
    }
    close(fd);
    time_end = clock();
    printf("Execution time: %.2f ms\n", 
           ((double)(time_end - time_start) / CLOCKS_PER_SEC * 1000 ) );
    return 0;
}
