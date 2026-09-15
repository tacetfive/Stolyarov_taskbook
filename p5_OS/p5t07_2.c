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
enum { BUFFER_SIZE = 1048576 };
#endif

enum cmd { CMD_ADD, CMD_QUERY, CMD_LIST, CMD_UNKNOWN };

struct table {
    uint32_t counter;
    char key[KEY_LENGHT];
    int start; /* a byte number where entry starts in input file.
                  If there's no such key in database, equals -1 */
};

struct discovered_indices {
    int size; /* number of active objects inside the vector */ 
    int capacity; /* total elements vector can hold */
    int *arr;
};

void double_arr_size(struct discovered_indices *d)
{
    int new_capacity = d->capacity << 1;
    int element_size = sizeof(d->arr[0]);
    int *new_arr = malloc( new_capacity * element_size );
    memcpy(new_arr, d->arr, d->capacity * element_size);
    d->capacity = new_capacity;
    free(d->arr);
    d->arr = new_arr;
}

void insert_discovered_idx(struct discovered_indices *d, int target_idx)
{
    int left = 0;
    int right = d->size;
    int middle;
    int i;
    while ( left < right ) { /* BS */
        middle = (left + right) /2;
        if ( d->arr[middle] == target_idx ) {
            printf("Error: the idx is already in array\n");
            return;
        }
        else if ( d->arr[middle] < target_idx )
            left = middle + 1;
        else
            right = middle;
    } /* now arr[left] contains closest bigger to target */
    if ( d->size == d->capacity )
        double_arr_size(d);
    for (i = d->size; i > left; --i)
        d->arr[i] = d->arr[i-1];
    d->arr[left] = target_idx;
    ++d->size;
    return;
}

int is_idx_discovered(struct discovered_indices *d, int target_idx)
{
    int left = 0;
    int right = d->size;
    int middle;
    while ( left < right ) { /* BS */
        middle = (left + right) /2;
        if ( d->arr[middle] == target_idx )
            return 1;
        else if ( d->arr[middle] < target_idx )
            left = middle + 1;
        else
            right = middle;
    }
    return 0;
}

/* edge case: d->size == 0 */
void append_db(int fd_db2, int fd_out)
{
    char buf[BUFFER_SIZE];
    ssize_t red_bytes;
    lseek(fd_db2, 0, SEEK_SET);
    lseek(fd_out, 0, SEEK_END); 
    while ( (red_bytes = read(fd_db2, buf, BUFFER_SIZE)) )
        write(fd_out, buf, red_bytes);
}

/* append to output file records that not in db1 but only in db2 */
void append_remaining_records(int fd_db2, int fd_out, 
                              const struct discovered_indices *d)
{
    char buf_db2[BUFFER_SIZE];
    char buf_out[BUFFER_SIZE];
    int idx_buf_db2 = 0;
    int idx_buf_out = 0;
    ssize_t red_bytes;
    uint32_t idx_db2 = 0;
    int d_idx; /* d->arr index */
    int record_size = KEY_LENGHT + sizeof(uint32_t);
    /* load db2 file in buffer and analyze it there */
    while ( ( red_bytes = read(fd_db2, buf_db2, BUFFER_SIZE) ) ) {
        for (idx_buf_db2 = 0; idx_buf_db2 <= red_bytes; 
             idx_buf_db2 += record_size) {
            while ( idx_db2 != d->arr[d_idx] ) {
                memcpy(&buf_out[idx_buf_out], 
                       &buf_db2[idx_buf_db2], record_size);
                idx_buf_out += record_size;
                idx_buf_db2 += record_size;
                if ( idx_buf_out == BUFFER_SIZE || idx_buf_db2 >= red_bytes ) {
                    write(fd_out, buf_out, idx_buf_out);
                    idx_buf_out = 0;
                }
            }
            ++d_idx;    
            idx_db2 += record_size;
            }
    }
}

void find_by_key(int fd, const char *key_to_find, struct table *table_elem,
                 struct discovered_indices *d) 
{
    char buf[BUFFER_SIZE];
    int idx_buf = 0;
    uint32_t idx_db2 = 0;
    int counter_size = sizeof(uint32_t);
    int record_size = counter_size + KEY_LENGHT;
    int buffers_counter = 0;
    ssize_t red_bytes;
    table_elem->counter = 0;
    strcpy(table_elem->key, key_to_find);
    lseek(fd, 0, SEEK_SET);
    while ( (red_bytes = read(fd, buf, BUFFER_SIZE)) ) { /* read file by 4096 bytes */
        if ( red_bytes == -1 ) {
            perror("read error");
            return;
        }
        for ( idx_buf = counter_size; idx_buf < red_bytes; 
              idx_buf += record_size ) {
            idx_db2 = buffers_counter * BUFFER_SIZE + idx_buf - counter_size;
            if (is_idx_discovered(d, idx_db2)) continue;
            if (!strcmp(&buf[idx_buf], key_to_find)) {
                table_elem->start = idx_db2;
                memcpy( &(table_elem->counter), &buf[idx_buf - counter_size], 
                        counter_size);
                return;
            }
        }
        ++buffers_counter;
    }
    table_elem->start = -1; /* if nothing find, "start" == -1. */
}

void combine_db(int fd_db1, int fd_db2, int fd_out)
{
    char buf_db1[BUFFER_SIZE];
    char buf_out[BUFFER_SIZE];
    int red_bytes;
    int idx_buf_db1 = 0;
    int idx_buf_out = 0;
    struct table *db2_record = malloc(sizeof(struct table));
    uint32_t current_record_counter;
    struct discovered_indices *d = malloc(sizeof(struct discovered_indices));
    d->size = 0; d->capacity = 5;
    d->arr = malloc(d->capacity * sizeof(*(d->arr)));
    int counter_size = sizeof(db2_record->counter);
    int record_size = KEY_LENGHT + counter_size;
    d->size = 0;
/* combine db1 & db2 matches */
    while ( ( red_bytes = read(fd_db1, buf_db1, BUFFER_SIZE) ) ) {
        for ( idx_buf_db1 = counter_size; idx_buf_db1 < red_bytes; 
              idx_buf_db1 += record_size ) {
            find_by_key(fd_db2, &buf_db1[idx_buf_db1], db2_record, d);
            if ( db2_record->start == -1) { /* no record in db2 */
                memcpy(&buf_out[idx_buf_out], 
                       &buf_db1[idx_buf_db1 - counter_size], record_size);
                idx_buf_out += record_size;
                if ( idx_buf_out >= BUFFER_SIZE || 
                    (idx_buf_db1 + record_size) > red_bytes ) {
                    write(fd_out, buf_out, idx_buf_out);
                    idx_buf_out = 0;
                }
                continue;
            }
            insert_discovered_idx(d, db2_record->start);
            memcpy(&current_record_counter, 
                   &buf_db1[idx_buf_db1 - counter_size], counter_size);
            db2_record->counter += current_record_counter;
            memcpy(&buf_out[idx_buf_out], db2_record, record_size);
            idx_buf_out += record_size;
            if ( idx_buf_out >= BUFFER_SIZE || 
                (idx_buf_db1 + record_size) > red_bytes ) {
                write(fd_out, buf_out, idx_buf_out);
                idx_buf_out = 0;
            }
        }
    }
/* combine db2 remainder (BUT: if all records from db2 matched, exit) */
    if ( d->size == 0 ) {
        append_db(fd_db2, fd_out);
        free(d->arr); free(d);
        return;
    }
    append_remaining_records(fd_db2, fd_out, d);
    free(d->arr); free(d);
}

int main(int argc, char **argv) 
{
    clock_t time_start = clock();
    clock_t time_end;
    int fd_db1;
    int fd_db2;
    int fd_out;
    char *filename_db1;
    char *filename_db2;
    char *filename_out;
    if ( argc < 4 ) {
        fprintf(stderr, "Incorrect parameters");
        return 1;
    }
    filename_db1 = argv[1];
    filename_db2 = argv[2];
    filename_out = argv[3];
    fd_db1 = open(filename_db1, O_RDONLY);
    fd_db2 = open(filename_db2, O_RDONLY);
    fd_out = open(filename_out, O_WRONLY);
    if ( fd_db1 == -1 || fd_db2 == -1 || fd_out == -1 ) {
        fprintf(stderr, "Error while open file. ");
        perror("");
        return 1;
    }
    combine_db(fd_db1, fd_db2, fd_out);
    close(fd_db1);
    close(fd_db2);
    close(fd_out);
    time_end = clock();
    printf("Execution time: %.2f ms\n", 
           ((double)(time_end - time_start) / CLOCKS_PER_SEC * 1000 ) );
    return 0;
}
