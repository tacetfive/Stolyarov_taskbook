#include <stdio.h>
#include <stdlib.h>

struct number {
    long value;
    int  occur; // number of occurrences
    struct number *next;
};

struct number *list();
void count_list(struct number *list_ptr);
int max_occurance(const struct number *);
void print_out(const struct number *, int);

int main(int argc, char **argv)
{
    struct number *list_ptr = list();
    count_list(list_ptr); // count occurances of numbers and delete duplicates
    print_out(list_ptr, max_occurance(list_ptr));
    return 0;
}

struct number *list() // forming linked list
{
    struct number *first = NULL, *last = NULL, *tmp;
    long n;
    while(scanf("%ld", &n) != EOF) { //insert new number in the end of list
        tmp = malloc(sizeof(struct number));
        tmp->value = n;
        tmp->next = NULL;
        if(last)
            last->next = tmp;
        else    
            first = tmp;
        last = tmp;
        }
    return first;
}

int max_occurance(const struct number *list_ptr)
{
    int max = 0;
    const struct number *tmp = list_ptr;
    while(tmp) {
        max = (max < tmp->occur) ? tmp->occur : max;
        tmp = tmp->next;
    }
    return max;
}

void print_out(const struct number *list_ptr, int max_occurance)
{
    const struct number *tmp = list_ptr;
    while(tmp) {
        if (tmp->occur == max_occurance)
            printf("number %ld occurs %d times\n", tmp->value, tmp->occur);
        tmp = tmp->next;
    }
}

void count_list(struct number *list_ptr)
{
    struct number *tmp, *tmp_cmp1, *tmp_cmp2;
    for(tmp = list_ptr; tmp; tmp = tmp->next) {
        tmp->occur = 1;
        tmp_cmp1 = tmp->next;
        tmp_cmp2 = tmp; // catching up pointer
        while(tmp_cmp1) {
            if (tmp_cmp1->value == tmp->value) {
                tmp->occur += 1;
                tmp_cmp2->next = tmp_cmp1->next;
                free(tmp_cmp1);
                tmp_cmp1 = tmp_cmp2->next;
            }
            else { 
                tmp_cmp1 = tmp_cmp1->next;
                tmp_cmp2 = tmp_cmp2->next;
            }
        }
    }
}

// Count without deleting elements
/*
void count_list(struct number *list_ptr)
{
    struct number *tmp, *tmp_cmp;
    for(tmp = list_ptr; tmp; tmp = tmp->next) {
        tmp->occur = 1;
        for(tmp_cmp = tmp->next; tmp_cmp; tmp_cmp = tmp_cmp->next) {
            if (tmp_cmp->value == tmp->value) {
                tmp->occur += 1;
            }
        }
    }
}
*/
