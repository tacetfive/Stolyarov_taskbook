#include <fcntl.h> /* open() */
#include <stdio.h>
#include <stdlib.h> /* free(), rand() */
#include <stdint.h> /* uint32_t */
#include <string.h> /* strcmp */
#include <time.h>
#include <ctype.h> /* isspace() */

enum { KEY_LENGHT = 60 };

struct table {
    uint32_t hash_value;
    uint32_t counter;
    char key[KEY_LENGHT];
};

uint32_t djb2_hf(const char *str)
{
    uint32_t h = 5381;
    char ch;
    while ( ( ch = *str++ ) )
        h = ( ( h << 5) + h ) + ch;
    return h;
}

int is_string_empty(const char *str)
{
    if ( !str ) /* NULL check */
        return 1;
    while ( *str ) {
        if ( !isspace(*str) )
            return 0;
        ++str;
    }
    return 1;
}

int is_record_empty(const struct table *r)
{
    int record_size = sizeof(struct table);
    const uint8_t *byte_check = (const uint8_t *)r;
    int i;
    for ( i = 0; i < record_size; ++i ) {
        if ( byte_check[i] )
            return 0;
    }
    return 1;
}

int is_record_empty_bytes(const uint8_t *r)
{
    int record_size = sizeof(struct table);
    int i;
    for ( i = 0; i < record_size; ++i ) {
        if ( r[i] )
            return 0;
    }
    return 1;
}

int rec_cmp(const struct table *a, const struct table *b)
{
    if ( a->hash_value == b->hash_value ) {
        printf("Hashes are identical for %s, %s\n", a->key, b->key);
        if ( !strcmp(a->key, b->key) ) {
            if ( a->counter == b->counter )
                return 0; /* fully equal */
            return 1; /* keys are equal */
        }
        return 2; /* hashes are equal */
    }
    return 3; /* none is equal */
}

