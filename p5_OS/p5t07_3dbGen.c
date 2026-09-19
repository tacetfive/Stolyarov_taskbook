#include <math.h>  /* sqrt() */
#include "p5t07_3.h"

/* enum { LINES_IN_FILE = 500 }; */
 enum { LINES_IN_FILE = 466434 };
/*#define WORDS_FILE_NAME "test_files/words.txt"*/
#define WORDS_FILE_NAME "test_files/db_400k"

int is_prime(uint32_t num)
{
    uint32_t p_i;
    double sq_num = sqrt(num);
    if ( num < 2 ) return 0;
    if ( num <= 3 ) return 1;
    if ( !(num & 1) || num % 3 == 0 ) return 0;
    for ( p_i = 5; p_i < sq_num; p_i += 6 ) {
        if ( num % p_i == 0 || num % ( p_i + 2 ) == 0 )
            return 0;
    }
    return 1;
}

uint32_t closest_prime(uint32_t num)
{
    uint32_t candidate;
    if (num <= 2) return 2;
    if (num == 3) return num;
    candidate = (num & 1) ? num : num + 1;
    while ( !is_prime(candidate) ) {
        candidate += 2;
    }
    return candidate;
}

char **create_db_words(int records_quantity)
{
    char **out = malloc(records_quantity * sizeof(*out));
    FILE *f = fopen( WORDS_FILE_NAME, "r" );
    int i;
    for ( i = 0; i < records_quantity; ++i ) {
        out[i] = calloc(KEY_LENGHT, 1);
        fseek(f, (4+i*64), SEEK_SET);
        fread(out[i], 1, KEY_LENGHT, f);
    } 
    for ( i = 0; i < records_quantity; ++i ) { // empty string check 
        if ( is_string_empty( out[i] ) )
            fprintf(stderr, "Error: record %d in file is empty.\n", i);
    }
    printf("Keys created.\n");
    return out;
}

uint32_t collision_resolution(uint32_t idx_db, FILE *db_f, uint32_t *db_cap_b,
                         const struct table_rec *curr_rec, int *counter)
{
    int record_size = sizeof(struct table_rec);
    struct table_rec *seek_rec = malloc(record_size);
    while (1) {
        if ( idx_db >= *db_cap_b ) { /* out of file? */
            (*db_cap_b) = idx_db + record_size;
            free(seek_rec); /* \ cannot use fseek to reach file size because */
            return idx_db;       /* fseek is bufferized and file may not written. */
        }
        fseek(db_f, idx_db, SEEK_SET);
        if ( (fread(seek_rec, 1, record_size, db_f)) != record_size ) {
            fprintf(stderr, "Read database error at %d position\n", idx_db);
            exit(1);
        }
        if ( !is_record_empty(seek_rec) ) {
            if ( rec_cmp( seek_rec, curr_rec ) == 1 ) {
                fprintf(stderr, "Fatal: two records are identical %s, %s\n",
                            seek_rec->key, curr_rec->key);
                exit(1);
            }
            (*counter)++;
            idx_db += record_size;
        }
        else {
            free(seek_rec);
            return idx_db;
        }
    }
}

int write_database( char **keys, int records_quantity, char *db_file )
{
    FILE *db_f = fopen(db_file, "w+");
    int rec_size = sizeof(struct table_rec);
    int md_size = sizeof(struct table_md);
    struct table_rec *curr_rec = malloc(rec_size);
    struct table_md *tmd = malloc(md_size);
    uint32_t db_cap_b = md_size;
    uint32_t idx_keys = 0;
    uint32_t idx_db; /* byte number points to the record start */
    int col_counter = 0;
    /* write metadata */
    tmd->init_db_cap = closest_prime(records_quantity * 2);
    fwrite(tmd, md_size, 1, db_f);
    for ( idx_keys = 0; idx_keys < records_quantity; ++idx_keys ) {
        /* pack record data */
        curr_rec->hash_value = djb2_hf(keys[idx_keys]);
        curr_rec->counter = rand() % 300;
        memcpy(curr_rec->key, keys[idx_keys], KEY_LENGHT);
        /*                  */
        idx_db = md_size + (curr_rec->hash_value % tmd->init_db_cap) * rec_size;
        idx_db = collision_resolution(idx_db, db_f, &db_cap_b, 
                                        curr_rec, &col_counter);
        fseek(db_f, idx_db, SEEK_SET);
        fwrite(curr_rec, 1, rec_size, db_f);
        free(keys[idx_keys]);
    }
    printf("Database with %d records wrote. %d collisions happened.\n",
                records_quantity, col_counter);
    fclose(db_f); 
    free(curr_rec); free(tmd); 
    free(keys);
    return 0;
}

int main(int argc, char **argv) 
{
    uint32_t records_quantity;
    char *output_file = NULL;
    char **keys;
    clock_t time_start = clock();
    clock_t time_end;
    if ( argc < 3 ) {
        fprintf(stderr, "Incorrect arguments\n");
        return -1;
    }
    output_file = argv[1];
    records_quantity = strtol(argv[2], NULL, 10);
    keys = create_db_words(records_quantity);
    write_database(keys, records_quantity, output_file);
    time_end = clock();
    printf("Execution time: %.3f ms\n", 
           ((double)(time_end - time_start) / CLOCKS_PER_SEC * 1000 ) );
    return 0;
}

/*
int *generate_unique_numbers( int records_quantity )
{
    int numbers[LINES_IN_FILE];
    int *out = malloc(records_quantity * sizeof(*out));
    int i, j, tmp;
    for ( i = 0; i < LINES_IN_FILE; ++i )
        numbers[i] = i + 1;
    srand(time(NULL));
    for ( i = LINES_IN_FILE - 1; i > 0; --i ) { // Fisher-Yates shuffle 
        j = rand() % ( i + 1 ); // 0 <= j <= i  
        tmp = numbers[i];
        numbers[i] = numbers[j];
        numbers[j] = tmp;
    }
    for ( i = 0; i < records_quantity; ++i)
        out[i] = numbers[i];
    return out;
}

char **create_db_words(int records_quantity)
{
    char **out = malloc(records_quantity * sizeof(*out));
    int *line_numbers = generate_unique_numbers( records_quantity);
    FILE *f = fopen( WORDS_FILE_NAME, "r" );
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;
    int i;
    int current_line = 1;
    for ( i = 0; i < records_quantity; ++i ) {
        out[i] = calloc(KEY_LENGHT, 1);
        fseek(f, 0, SEEK_SET);
        current_line = 1;
        while ((nread = getline(&line, &len, f)) != -1) {
            if (current_line == line_numbers[i]) {
                memcpy(out[i], line, (nread - 1) );
                break;
            }
            ++current_line;
        }
    } 
    for ( i = 0; i < records_quantity; ++i ) { // empty string check 
        if ( is_string_empty( out[i] ) )
            fprintf(stderr, "Error: line %d in file is empty.\n", line_numbers[i]);
    }
    printf("Keys created.\n");
    return out;
}
*/

       /*     printf("col!!!   %10d %12ld %19s %19s\n", *idx_db_ptr, ftell(db_f), 
                                                curr_rec->key, seek_rec->key);*/
