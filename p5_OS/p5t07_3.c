#include <unistd.h>
#include "p5t07_3.h"

enum { BUFFER_SIZE = sizeof(struct table) << 10 };

enum cmd { CMD_ADD, CMD_QUERY, CMD_LIST, CMD_UNKNOWN };

enum cmd get_command_code(const char *command)
{
    if (!strcmp("add", command)) return CMD_ADD;
    else if (!strcmp("query", command)) return CMD_QUERY;
    else if (!strcmp("list", command)) return CMD_LIST;
    else return CMD_UNKNOWN;
}

struct table *find_by_key(int fd, const char *key_to_find, uint32_t *idx_db)
{
    int record_size = sizeof(struct table);
    struct table *table_elem = malloc(record_size);
    struct table *seek_elem = malloc(record_size);
    uint8_t buf[BUFFER_SIZE];
    int idx_buf = 0;
    ssize_t red_bytes;
    uint32_t init_db_cap;
    lseek(fd, 0, SEEK_SET);
    read(fd, &init_db_cap, sizeof(init_db_cap));
    /* initialize table record */
    table_elem->hash_value = djb2_hf(key_to_find);
    table_elem->counter = 0;
    strcpy(table_elem->key, key_to_find);
    /* search one in hash table */
    *idx_db = 4 + ( table_elem->hash_value % init_db_cap ) * record_size;
    if ( *idx_db >= lseek(fd, 0, SEEK_END) ) { /* if out of file */
        free( seek_elem ); 
        return table_elem;
    }
    lseek(fd, *idx_db, SEEK_SET);
    while ( (red_bytes = read(fd, buf, BUFFER_SIZE)) ) {
        if ( red_bytes == -1 ) {
            perror("read error");
            exit(1);
        }
        for ( idx_buf = 0; idx_buf < red_bytes; idx_buf += record_size ) {
            memcpy(seek_elem, &buf[idx_buf], record_size);
            if ( is_record_empty( seek_elem ) ) {
                free( seek_elem ); 
                return table_elem;
            }
            if ( rec_cmp( table_elem, seek_elem ) == 1 ) {
                table_elem->counter = seek_elem->counter;
                free(seek_elem);
                return table_elem;
            }
            (*idx_db) += record_size;
        }
    }
    free(seek_elem);
    return table_elem;
}

void increase_counter(int fd, const char *key) 
{
    int record_size = sizeof(struct table);
    uint32_t idx_db;
    struct table *table_elem = find_by_key(fd, key, &idx_db);
    ++(table_elem->counter);
    lseek(fd, idx_db, SEEK_SET);
    write(fd, table_elem, record_size);
    free(table_elem);
    return;
}

void query(int fd, char *key)
{
    uint32_t i;
    struct table *table_elem = find_by_key(fd, key, &i);
    printf("%d\n", table_elem->counter);
    free(table_elem);
}

void print_table(int fd) 
{
    int records_counter = 1;
    int record_size = sizeof(struct table);
    uint8_t buf[BUFFER_SIZE];
    int red_bytes, idx_b;
    char key[KEY_LENGHT];
    uint32_t hash, counter;
    int hash_l = sizeof(hash), count_l = sizeof(counter);
    lseek(fd, 4, SEEK_SET);
    printf("%-9s %-15s %-11s %s\n", "rec.no", "hash", "counter", "key");
    while ( (red_bytes = read(fd, buf, BUFFER_SIZE)) ) {
        for ( idx_b = 0; idx_b < red_bytes; idx_b += record_size ) {
            if( !is_record_empty_bytes( &buf[idx_b]) ) {
                memcpy( &hash, &buf[idx_b], hash_l );
                memcpy( &counter, &buf[idx_b + hash_l], count_l );
                memcpy( key, &buf[idx_b + hash_l + count_l], KEY_LENGHT );
                printf("%-9u %-15u %-11u %s\n", records_counter, hash, counter, key);
                ++records_counter;
            }
        }
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
    printf("Execution time: %.3f ms\n", 
           ((double)(time_end - time_start) / CLOCKS_PER_SEC * 1000 ) );
    return 0;
}
